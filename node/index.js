var Promise = require('bluebird');
var co = require('co');
var koa = require('koa');
var inquirer = require('inquirer');
var serialPort = Promise.promisifyAll(require('serialport'));
var localtunnel = Promise.promisifyAll(require('localtunnel'));

var app = koa();
var SerialPort = serialPort.SerialPort;
var i = 0;

console.alert = (msg) => console.log(`\u2770 ALERT \u2771: ${msg}`);
console.error = (msg) => console.log(`\u2770 ERROR \u2771: ${msg}`);

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
      }], input => resolve(input));
  });

  console.log(answers);

  if (answers.doTunnel) {
    var tunnel = localtunnel(answers.port, (err, tunnel) => console.alert(`localtunnel URL is ${tunnel.url}`));
    tunnel.on('close', () => console.alert('localtunnel has closed'));
  }

  answers.serialPort = new SerialPort(answers.usbPort, {
      baudrate: answers.baud,
      parser: serialPort.parsers.raw,
      disconnectedCallback: () => console.alert('Arduino Disconnected')
  }, true);

  answers.serialPort.on('open', () => {
      console.alert('Arduino Connected')
  });

}).catch(console.error);
