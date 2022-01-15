# Untitled - By: hajim - 土 1 15 2022

import sensor, image, time, pyb

start = Pin('P0',Pin.IN,Pin.PULL_UP)
lighton = Pin('P1',Pin.IN,Pin.PULL_UP)
status = Pin('P7',Pin.OUT_PP,Pin.PULL_NONE)
from pyb import Pin ,UART,LED

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

clock = time.clock()

state = False;

while(True):
    if(start.value())
    clock.tick()
    img = sensor.snapshot()
    print(clock.fps())
