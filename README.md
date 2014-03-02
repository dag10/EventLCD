EventLCD
========

A project using an Arduino Ethernet and LCD character display to show upcoming events in a location from a shared Google calendar.

Details
-------

### Hardware

The hardware consists of:
 - Arduino Ethernet w/ a POE (Power Over Ethernet) module [Amazon Link](http://www.amazon.com/gp/product/B005D22FR6/ref=oh_details_o03_s00_i00)
 - 20x4 Amerlight LCD Module with HD44780 controller [Amazon Link](http://www.amazon.com/gp/product/B00D7Z2BWU/ref=oh_details_o09_s00_i01)
 - Custom PCB for connecting the Arduino with the LCD module (see /pcb)
   - 74LS595 Shift register
   - 10K Trim potentiometer
   - 16-pin header row
 - Generic project enclosure
 - POE Injector

### Software

 - Flask-powered web server that pulls the events from Google Calendar and renders a 20x4 character HTTP response containing what the LCD should display. (see /website)
   - Configurable items: Google API token, Google Calendar ID
 - C++ Arduino code. (see /arduino)
   - Configurable items: MAC address

Hardware Instructions
--------

1. Drill out your enclosure to expose the LCD screen and Arduino's ethernet port.
2. Solder shift register and trimpot onto custom PCB.
3. Solder wires to pads on custom PCB.
4. Solder custom PCB onto back of LCD module with header pins.
5. Attach (perhaps with hot glue) the LCD module and Arduino into the enclosure.
6. Plug those wires onto the Arduino's pins. Look at the Arduino code for pin mapping.
7. Install the software (see below), and you're done! Just plug it into powered ethernet.

Software Instructions
--------

### Arduino sketch

I'm using [ino](http://inotool.org) to build the arduino code. If you use ino, you must delete the Robot_Control library from your Arduino IDE installation ([explanation here](https://github.com/amperka/ino/issues/85)).

To use ino, navigate to the arduino directory and run `ino build` then `ino upload`. If you build using the Arduino IDE, make sure to add the libraries in the lib folder.

### Web application

Go to the website directory and run setup.sh to install the virtualenv with flask and its extensions. Then copy app.yaml.example to app.yaml and insert your Google API token and Google calendar ID.

To run the app, execute `./run.py`.
