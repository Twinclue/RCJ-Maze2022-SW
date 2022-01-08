import sensor, image, time,pyb
from pyb import Pin ,UART,LED
from ulab import numpy as np



led = Pin('P3',Pin.OUT_PP,Pin.PULL_NONE)
start = Pin('P0',Pin.IN,Pin.PULL_UP)
lighton = Pin('P1',Pin.IN,Pin.PULL_UP)
status = Pin('P7',Pin.OUT_PP,Pin.PULL_NONE)

status.off()
uart = UART(3,9600)

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

led.value(False)
while(start.value() == False):
    if lighton.value() == True:
        led.value(not led.value())
        time.sleep_ms(500)




sensor.reset()
sensor.set_pixformat(sensor.RGB565) # or RGB565.
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_brightness(2)
sensor.set_contrast(3)
sensor.set_saturation(3)
thr = [(0, 30, -128, 127, -128, 127),(70,90,-90,-65,60,80),(60,80,-30,15,60,80),(45,55,70,85,65,75)]#code 1,2,4,8/Black Green,Yellow,Red
#llo,lhi,alo,ahi,blo,bhi
min_degree = 0
max_degree = 179
result = ''
status.on()
clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8)
    blobcode = 0
    for blob in img.find_blobs(thr, pixels_threshold=5, area_threshold=5,merge = True):
        #print(blob.code())
        blobcode = blob.code()
        if blobcode==1:#black
            linecount = 0
            for l in img.find_lines(roi = blob.rect() ,threshold = 1000, theta_margin = 50, rho_margin = 15):#直線検知しきい値 あまりいじる必要なし
                if (min_degree <= l.theta()) and (l.theta() <= max_degree):
                    img.draw_line(l.line(), color = (255, 0, 0))
                    img.draw_rectangle(blob.rect())
                    linecount += 1
            if linecount == 0:
                result = 'N'
            elif linecount == 1:
                result = 'S'
            elif linecount == 2:
                result = 'U'
            elif linecount == 3:
                result = 'H'
            else:
                result = 'U'
        elif blobcode==2:#green
            result = 'G'
        elif blobcode==4:#Yellow
            result = 'Y'
        elif blobcode==8:#Red
            result = 'R'
        elif blobcode==0:
            result = 'N'
    print(result)
    uart.write(result)
