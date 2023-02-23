import time
import board
from adafruit_motorkit import MotorKit

kit = MotorKit(i2c = board.I2C())
  
kit.motor3.throttle = 0.5
time.sleep(2)

kit.motor3.throttle = 0
time.sleep(10)
  
kit.motor3.throttle = -0.5
time.sleep(2)
  
kit.motor3.throttle = 0
time.sleep(3)
