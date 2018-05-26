import time
from core import led, button1, button2, sensor, pixel

LOOPS_PER_CYCLE = 10


class Scan:

    def __init__(self):
        self.init_ts = time.monotonic()
        self.start_ts = 0
        self.end_ts = 0
        self.data = []

    def start(self):
        print("Starting....")
        self.start_ts = time.monotonic()

    def recording(self):
        return self.start_ts > 0

    def record(self, sensor):
        # print('Temperature: {} degrees C'.format(sensor.temperature))
        # print('Accelerometer (m/s^2): {}'.format(sensor.accelerometer))
        # print('Magnetometer (microteslas): {}'.format(sensor.magnetometer))
        # print('Gyroscope (deg/sec): {}'.format(sensor.gyroscope))
        # print('Euler angle: {}'.format(sensor.euler))
        # print('Quaternion: {}'.format(sensor.quaternion))
        # print('Gravity (m/s^2): {}'.format(sensor.gravity))
        reading = sensor.linear_acceleration
        # print('Linear acceleration (m/s^2): {}'.format(reading))
        row = [time.monotonic() - self.init_ts]
        row.extend(reading)
        print(row)

    def stop(self):
        print("Stopping...")
        self.end_ts = time.monotonic()
        print("Total Time: %s seconds" % str(self.end_ts - self.start_ts))
        self.start_ts = 0

cycle_number = 0
loop_number = 0
last_loop_ts = 0
scan = None
pixel.blue()

try:
    while True:
        cycle_number = cycle_number + 1
        cycle_ts = time.monotonic()

        # Main loop lobic
        if cycle_ts - last_loop_ts >= 1 / LOOPS_PER_CYCLE:
            loop_number = loop_number + 1
            #print("Cycle: %d, Loop#: %d" % (cycle_number, loop_number))

            if scan and scan.recording():
                led.toggle()
                scan.record(sensor)

            last_loop_ts = cycle_ts

        # Button1 press
        if button1.pressed():
            # print("Button1 Pressed!")
            if scan:
                if scan.recording():
                    # Stop recording
                    scan.stop()
                    pixel.blue()
                else:
                    # Start recording
                    scan.start()
                    pixel.green()
            time.sleep(1)

        # Button2 press
        if button2.pressed():
            scan = Scan()
            time.sleep(1)
            pixel.yellow()

except KeyboardInterrupt:
    stop()

# Turn the lights off we are going home
led.on()
pixel.off()
