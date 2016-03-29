var Promise = require('bluebird');
var co = require('co');
var koa = require('koa');
var inquirer = require('inquirer');
var serialPort = Promise.promisifyAll(require('serialport'));
var localtunnel = Promise.promisifyAll(require('localtunnel'));

var app = koa();
var SerialPort = serialPort.SerialPort;
var msgCount = 0;
var varTypes = ['CHAR', 'INT', 'FLOAT']; //from Wifiduino.h

console.alert = (msg) => console.log(`\u2770 ALERT \u2771: ${msg}`);
console.error = (msg) => console.log(`\u2770 ERROR \u2771: ${msg}`);
console.recieved = (msg) => console.log(`\u2770 RECIEVED \u2771: ${msg}`);

co(function* () {
  var ports = yield serialPort.listAsync();

  if (!ports.length) {
    console.log('No valid ports found, exiting');
    process.exit(1);
  }

  var answers = yield new Promise(resolve => {
    inquirer.prompt(
      [{
        type: 'list',
        name: 'usbPort',
        message: 'Which serial port is the arduino connected to?',
        choices: ports.map(port => port.comName)
      },{
        type: 'list',
        name: 'baud',
        message: 'What baud rate should serial begin at?',
        choices: ['115200', '57600', '38400', '28800', '19200', '14400', '9600', '4800', '2400', '1200', '600', '300']
      },{
        type: 'input',
        name: 'httpPort',
        message: 'What port should the server be hosted on? (between 1024 and 49151)',
        validate: value => ( 1024 <= value && value <= 49151 )
      },{
        type: 'confirm',
        name: 'doTunnel',
        message: 'Make server acessible remotely using localtunnel?',
        default: false
      }
    ], input => resolve(input));
  });

  if (answers.doTunnel) {
    var tunnel = localtunnel(answers.httpPort, (err, tunnel) => console.alert(`localtunnel URL is ${tunnel.url}`));
    tunnel.on('close', () => console.alert('localtunnel has closed'));
  }

  answers.serialPort = new SerialPort(answers.usbPort, {
    baudrate: answers.baud,
    parser: serialPort.parsers.readline('\n'),
    disconnectedCallback: () => {console.alert('Arduino Disconnected, Exiting...'); process.exit(1);}
  }, true);

  answers.serialPort.on('open', () => {
      console.alert('Arduino Connected');
  });

  function getData() {
    return new Promise((resolve, reject) => {
      answers.serialPort.on('data', (data) => {
        resolve(data);
      });
      answers.serialPort.on('error', (e) => {
        reject(e);
      });
    });
  }

  app.use(function *(){
    var location = this.path.split('/');
    if (location[1] === 'variable') {
      if (location[2] === 'get') {
        answers.serialPort.write(`[${msgCount++},${location[3]},variable]`);
      } else if (location[2] === 'set') {
        answers.serialPort.write(`[${msgCount++},${location[3]},variable,${location[4]}]`);
      } else {
        this.status = 404;
      }
    } else if (location[1] === 'function') {
      this.status = 418; //change when function calls added to c code
    } else {
      this.status = 404; //should be either variable or function
    }
    var result = yield getData();
    var resultJSON = JSON.parse(result);
    if (resultJSON) {
      this.body = {
        'msgNumber': result[0],
        'name': result[1],
        'value': result[2],
        'varType': varTypes[result[3]]
      }
      console.alert(`User successfully used ${this.path} and recieved data ${this.body}`);
    } else {
      console.error(`User attempted to use nonexistent var or funct (${this.path})`);
      this.body = { 'Error': 'Variable or function not found, make sure you\'ve linked it in your .cpp file' };
    }
  });

  app.listen(answers.httpPort);

}).catch(console.error);
