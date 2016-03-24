# Wifiduino
bindings for Arduino boards similar to those for Particle boards

Wifiduino attempts to copy the functionality and syntax of Particle bindings such that code made for use with Wifiduino can be used on a Particle and vice versa simply by replacing all instances of Wifiduino with Particle or vice versa.

# Installation and Setup
  1. Clone the repo
  * install the Wifiduino library using the Arduino IDE
  * include the library and use the bindings in the .ino file
  * cd into the Wifiduino/node folder and npm start
  * send data to and from the Arduino using <a href="#Usage">POST requests</a>

# Usage


# Specifics

The specific details of how Wifiduino works are covered as follows:

## Data transmission:
Data is sent back and forth using serial events. Laptop to Arduino is sent like this: ```<Wifiduino:A:B>C</Wifiduino>```, where in position A, the request number is sent, in position B either a D for data or R for request for data is sent, and in position C either the variable name if requesting data or the data in JSON if a request for data is sent.

For example, if a user sends a GET request to get a variable value 'test' from the Arduino, the laptop's node instance could send ```<Wifiduino:0:R>test</Wifiduino>```

## Language Specifics
### Javascript
~~this['foo'] is used to get the value of a variable from node to pass to the Arduino~~ All of the js code is pretty standard and simple for the most part; nothing too wonky happens.

### C
The following structures and functions are used for C (mostly based on simplicity and dev time needed to implement: I'm on a bit of a time crunch for at least the next month)
 * varDict: a linked list with the calling name of all of the variables and their locations and types, pointer to the next node in the linked list
 * functDict: the same thing as varDict but for functions, so it has the calling name of the function, the address of the function
 * Wifiduino.variable("foo", &foo, INT), checks to see if the variable calling name "foo" is in the varDict and changes the address and data type if it is, otherwise, it adds a new entry with calling name "foo", address &foo, and return type INT, in this case
 * Wifiduino.function("bar", bar), same thing as .variable, except in this case with a function calling name and address

Note: the "calling name" is not necessarily the name of the variable, but the name that is associated with the address of a variable and is sent in a request to get the value for the variable, so it's really it's name in every sense but its name, if that makes sense.
