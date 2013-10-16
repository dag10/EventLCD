EventLCD
========

A project using an Arduino Ethernet and LCD character display to show upcoming events in a location from a shared Google calendar.

How to build
--------

### Arduino sketch

I'm using [ino](http://inotool.org) to build the arduino code.

If you use ino, you must delete the Robot_Control library from your Arduino IDE installation ([explanation here](https://github.com/amperka/ino/issues/85)).

To use ino, navigate to the arduino directory and run `ino build` then `ino upload`.

If you build using the Arduino IDE, make sure to add the libraries in the lib folder.

### Web application

Go to the website directory and run setup.sh to install the virtualenv with flask and its extensions.

Then copy app.yaml.example to app.yaml and insert your Google API token and Google calendar ID.

To run the app, execute `./run.py`.
