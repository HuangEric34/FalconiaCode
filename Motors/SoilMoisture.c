import time
import board
from adafruit_motorkit import MotorKit

kit = MotorKit(i2c = board.I2C())
  
kit.motor3.throttle = 1.0

time.sleep(4)


kit.motor3.throttle = 0
