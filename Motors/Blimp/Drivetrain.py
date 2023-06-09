import RPi.GPIO as gpio
import time

class Drivetrain:
    """Drivetrain contains raw commands for the interface to use."""
    
    def __init__(self):
        LM1 = 6
        LM2 = 26
        RM1 = 
        RM2 = 
        # CM1 = 
        # CM2 = 

        # Sets up the GPIO pins 
        gpio.setmode(gpio.BCM)
        gpio.setwarnings(False)
        gpio.setup(LM1, gpio.OUT)
        gpio.setup(LM2, gpio.OUT)
        gpio.setup(RM1, gpio.OUT)
        gpio.setup(RM2, gpio.OUT)
        # gpio.setup(CM1, gpio.OUT)
        # gpio.setup(CM2, gpio.OUT)

    def forward(sec):
        gpio.output(LM1, False)
        gpio.output(LM2, True)
        gpio.output(RM1, True)
        gpio.output(RM2, False)
        time.sleep(sec)

    def back(sec):
        gpio.output(LM1, True)
        gpio.output(LM2, False)
        gpio.output(RM1, False)
        gpio.output(RM2, True)
        time.sleep(sec)

    def left(sec):
        gpio.output(LM1, True)
        gpio.output(LM2, False)
        gpio.output(RM1, True)
        gpio.output(RM2, False)
        time.sleep(sec)

    def right(sec):
        gpio.output(LM1, False)
        gpio.output(LM2, True)
        gpio.output(RM1, False)
        gpio.output(RM2, True)
        time.sleep(sec)

    # def up(sec):
    #     gpio.output(CM1, False)
    #     gpio.output(CM2, True)

    # def down(sec):
    #     gpio.output(CM1, True)
    #     gpio.output(CM2, False)

gpio.cleanup()
