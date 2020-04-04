import time
import serial
import pygame
import keyboard

#===========================JOYSTICK CONFIG====================================
ROLL_AXIS = 0
PITCH_AXIS = 1
THROTTLE_AXIS = 2
YAW_AXIS = 3

DEADZONE_ROLL_AXIS = .125
DEADZONE_PITCH_AXIS = .125
DEADZONE_THROTTLE_AXIS = 0
DEADZONE_YAW_AXIS = .3

#==============================================================================

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0).init()

arduino = serial.Serial("COM4", 115200)
time.sleep(1)

def getThrottle():
    return pygame.joystick.Joystick(0).get_axis(THROTTLE_AXIS)
def getZ():
    return pygame.joystick.Joystick(0).get_axis(YAW_AXIS)
def getDeadzoneFromAxis(axis):
    if axis == 0: return DEADZONE_ROLL_AXIS
    elif axis == 1: return DEADZONE_PITCH_AXIS
    elif axis == 2: return DEADZONE_THROTTLE_AXIS
    elif axis == 3: return DEADZONE_YAW_AXIS
    else: return 0

while True:
    if keyboard.is_pressed("ctrl") and keyboard.is_pressed("c"): break
    
    pygame.event.get()
    pygame.event.pump()
    
    while arduino.in_waiting == 0: pass
    arduino.reset_input_buffer()
    for i in range(0, 4):
        raw = pygame.joystick.Joystick(0).get_axis(i)

        if raw > 1 or raw < -1:
            if raw > 1: raw = 1
            else: raw = -1
        if abs(raw) < getDeadzoneFromAxis(i): raw = 0

        axis_value = int((raw + 1) * 256 / 2)

        toSend = axis_value

        arduino.write(bytes([toSend]))