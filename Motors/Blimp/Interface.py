from Drivetrain import Drivetrain

class Interface:
    """Interface class for Drivetrain.  This will by running on the Raspberry Pi (and use ssh to remote control)."""
    def __init__(self, drivetrain):
        self.drivetrain = drivetrain
        self.instruction_log = []

    def input(self):
        """Get input from user."""
        inp = input("Enter command: ")

        commands = {
            "forward": self.drivetrain.forward,
            "backward": self.drivetrain.backward,
            "left": self.drivetrain.left,
            "right": self.drivetrain.right,
            "stop": self.drivetrain.stop,
            "exit": exit
        }

        for key in commands:
            # check if speed is specified
            if key in inp:
                speed = inp.split(" ")[1]
                commands[key](speed)
                self.instruction_log.append(f"{key} {speed}")
                break
        else:
            commands[inp]()
            self.instruction_log.append(inp)

    def run(self):
        """Run the interface."""
        while True:
            self.input()

if __name__ == "__main__":
    interface = Interface(Drivetrain())
    interface.run()
