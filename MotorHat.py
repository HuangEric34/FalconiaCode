import time
import board
from adafruit_motorkit import MotorKit

# Below initialises the variable kit to be our I2C Connected Adafruit Motor HAT. If stacking Multiple
# Adafruit HATs we can then explain in code exactly which I2C address we want focused on here.
kit = MotorKit(i2c = board.I2C())

# top speed clockwise
kit.motor1.throttle = 1.0

# 4 second delay
time.sleep (4)

# half speed clockwise
kit.motor1.throttle = .5

time.sleep (4)

# one third speed counter-clockwise
kit.motor1.throttle = - 0.3333

time.sleep (4)

# does not de-energize the motor, stays put
kit.motor1.throttle = 0

time.sleep (4)

# de-energizes motor, free spin
kit.motor1.throttle = None
