# Tango Sentio
Tango Sentio is a 3D Scanner Glove using the Bosch BNO055 Absolute Orientation 9-DOF sensor.

## The Name
From Latin tango: touching, and sentio: seeing.  No we don't speak Latin but we have google and it sounded cool.

## The Device
The device straps to your middle finger

### Base Hardware
 - Sensor:  [Adafruit 9-DOF Absolute Orientation - BNO055](https://www.adafruit.com/product/2472)
 - Controller:  [Adafruit Feather M0 Express](https://www.adafruit.com/product/3403)
### Tutorials
  - Sensor:  https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor
  - Controller:  https://learn.adafruit.com/adafruit-feather-m0-express-designed-for-circuit-python-circuitpython

### Buttons
 - Button 1 is on the left and advances the state from RUNNING to TRACKING and RECORDING.
 - Button 2 is on the right and sets the state to RUNNING effectively canceling anything you were doing.


## The Software

### Client Program
The client runs on the micro controller to handle button events and communicate
the sensor data via serial (115200).  

### Server Program
The "server" is a python script that runs on your computer and reads the incoming
data from the client.

## Operating the device

1. When the device first starts up it is CALIBRATING.  Go through the calibration
steps below to move to RUNNING.
2. Move your hand to a known X=0,Y=0,Z=0 location and push button 1 to move to TRACKING.
3. Move your hand to where you would like your scan path to start and push button 1 to move to RECORDING.
4. When you are reached the end of your scan path , push button 1 to move back to RUNNING.

### Run States
 - CALIBRATING: Red
 - RUNNING: Green
 - TRACKING: White
 - RECORDING: Blue

### Calibration
The sensor outputs calibration data for the gyroscope, accelerometer,
magnetometer, and the system as a whole in a range of 0 to 3.  When the device
starts up the LED will be red.  Watch the serial port for the calibration data,
and move the device through the calibration steps until all values are 3.  The
LED will turn green when this is complete.

#### Steps
- **Gyroscope:** Leave flat on the table for a couple seconds.
- **Accelerometer:** Rotate on one axis in 45˚ intervals.
- **Magnetometer:** Hold vertically up, and then down.

For a more elaborate explanation of calibration you can watch the
[Bosch Training Video] (https://www.youtube.com/watch?v=Bw0WuAyGsnY). The
configuration is in NDoF_FMC_On mode so the important part is at the end of the
video.

## Tips, Tricks and Tools

### Watch Serial Console
 ```screen /dev/tty.usbmodem* 115200```
