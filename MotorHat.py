# Below imports all necessary packages to make this Python Script run
import time
import board
from adafruit_motorkit import MotorKit

# Below initialises the variable kit to be our I2C Connected Adafruit Motor HAT. If stacking Multiple
# Adafruit HATs we can then explain in code exactly which I2C address we want focused on here.
kit = MotorKit(i2c = board.I2C())

# Below will provide the DC Motor maximum current available. This allows it to rotate at top speed clockwise
kit.motor1.throttle = 1.0

# This will create a 4 second break until the next command. Note that the above command continues to have effect
# as the motor will continue to spin at top speed
time.sleep (4)

# Below will provide the DC Motor half maximum current available. This allows it to rotate at half speed clockwise
kit.motor1.throttle = .5

# This will create a 4 second break until the next command. Note that the above command continues to have same effect
time.sleep (4)

# Below will provide the DC Motor Third maximum current available. This allows it to rotate at third speed anti-clockwise
# Note the negative symbol which allows it to spin in the other direction.
kit.motor1.throttle = - 0.3333

# This will create a 4 second break until the next command. Note that the above command continues to have same effect
time.sleep (4)

# Below will energise the DC Motor however it will not rotate. This is not the same as None which would de-energise the motor.
# Thus, with the below active, the DC motor would not be free spinning
kit.motor1.throttle = 0

# This will create a 4 second break until the next command. Note that the above command continues to have same effect
time.sleep (4)

# Below will not energise the DC Motor allowing it to freely spin.
kit.motor1.throttle = None