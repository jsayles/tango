import time
from core import led, button1, button2, sensor, pixel

LOOPS_PER_CYCLE = 10

cycle_number = 0
loop_number = 0
start_ts = 0
recording_ts = 0
last_loop_ts = 0


def start():
    print("Starting....")
    pixel.blue()
    start_ts = time.monotonic()
    cycle_number = 0
    loop_number = 0
    last_loop_ts = 0
    recording_ts = 0


def record():
    # print('Temperature: {} degrees C'.format(sensor.temperature))
    # print('Accelerometer (m/s^2): {}'.format(sensor.accelerometer))
    # print('Magnetometer (microteslas): {}'.format(sensor.magnetometer))
    # print('Gyroscope (deg/sec): {}'.format(sensor.gyroscope))
    # print('Euler angle: {}'.format(sensor.euler))
    # print('Quaternion: {}'.format(sensor.quaternion))
    # print('Gravity (m/s^2): {}'.format(sensor.gravity))
    print('Linear acceleration (m/s^2): {}'.format(sensor.linear_acceleration))


def stop():
    print("Stopping...")
    pixel.blue()
    end_ts = time.monotonic()
    print("Total Time: %s seconds" % str(end_ts - start_ts))
    recording_ts = 0


start()
try:
    while True:
        cycle_ts = time.monotonic()
        cycle_number = cycle_number + 1

        # Main loop lobic
        if cycle_ts - last_loop_ts >= 1 / LOOPS_PER_CYCLE:
            loop_number = loop_number + 1
            #print("Cycle: %d, Loop#: %d" % (cycle_number, loop_number))

            if recording_ts > 0:
                led.toggle()
                record()

            last_loop_ts = cycle_ts

        # Button1 press
        if button1.pressed():
            # print("Button1 Pressed!")
            if recording_ts == 0:
                # Start recording
                recording_ts = time.monotonic()
                pixel.green()
            elif time.monotonic() - recording_ts > 1:
                # Stop recording
                time.sleep(2)
                stop()
                recording_ts = 0

        # Button2 press
        if button2.pressed():
            # print("Button2 Pressed!")
            stop()
            time.sleep(1)
            start()
except KeyboardInterrupt:
    stop()

# Turn the lights off we are going home
led.on()
pixel.off()
