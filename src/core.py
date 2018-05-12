import board
from busio import I2C
from adafruit_bno055 import BNO055
from digitalio import DigitalInOut, Direction, Pull
from neopixel_write import neopixel_write


class Pixel:

    def __init__(self, pin, brightness):
        self.pin = DigitalInOut(pin)
        self.pin.direction = Direction.OUTPUT
        self.brightness = brightness

    def set(self, r, g, b):
        color = bytearray([g, r, b])
        neopixel_write(self.pin, color)

    def off(self):
        self.set(0, 0, 0)

    def red(self):
        self.set(self.brightness, 0, 0)

    def yellow(self):
        self.set(self.brightness, int(self.brightness * .8), 0)

    def green(self):
        self.set(0, self.brightness, 0)

    def blue(self):
        self.set(0, 0, self.brightness)


class LED:

    def __init__(self, pin):
        self.pin = DigitalInOut(pin)
        self.pin.direction = Direction.OUTPUT

    def on(self):
        self.pin.value = True

    def off(self):
        self.pin.value = False

    def toggle(self):
        self.pin.value = not self.pin.value


led = LED(board.D13)

button1 = DigitalInOut(board.D5)
button1.direction = Direction.INPUT
button1.pull = Pull.UP

button2 = DigitalInOut(board.D0)
button2.direction = Direction.INPUT
button2.pull = Pull.UP

i2c = I2C(board.SCL, board.SDA)
sensor = BNO055(i2c)

pixel = Pixel(board.NEOPIXEL, 42)
