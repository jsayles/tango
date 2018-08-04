This client implementation is built on Circuit Python 2.3.1 but due to the fact
that the sensor lib does not expose calibration data and that circuitpython does
not yet support interupts (for the buttons), I reluctantly switched to arduino.
I'm leaving this here in hopes I can switch back to python when it's ready. 

The original idea here is that the micro controller would be able to both read
the sensor data and calculate location although when I switched to the arduino
client I decided to make the client as simple as possible and put most of the
processing in the server code.

--JLS
