# vcgencmd get_camera - command to see if camera is there, for troubleshooting

from picamera import PiCamera
camera = PiCamera()
camera.resolution = (800, 480)
camera.hflip = True
camera.start_preview(alpha = 255, fullscreen = True, window = (1000,100,1*800,2*480))
