import sensor, image, time,pyb
from pyb import Pin ,UART,LED, Timer
from ulab import numpy as np



led = Pin('P7',Pin.OUT_PP,Pin.PULL_NONE)
start = Pin('P3',Pin.IN,Pin.PULL_NONE)
lighton = Pin('P6',Pin.IN,Pin.PULL_NONE)
status = Pin('P8',Pin.OUT_PP,Pin.PULL_NONE)

status.off()

uart = UART(3,9600)

#red_led   = LED(1)
#green_led = LED(2)
#blue_led  = LED(3)

tim = Timer(4, freq=20000)

led.value(False)
while(start.value() == False):
    if lighton.value() == True:
        ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=50)
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
thr = [(0, 40, -15, 15, -15, 15),(0, 92, -80, -11, 15, 62),(45, 90, -20, 50, 40, 90),(33, 60, 40, 80, 40, 80)]#code 1,2,4,8/Black Green,Yellow,Red
#llo,lhi,alo,ahi,blo,bhi
min_degree = 0
max_degree = 179
result = 'N'
edge_cut_width = 0
blob_diagonal_sq = 6000 # <- proper size for existing algorithm
status.on()

#clock = time.clock()
while(True):
    #divhist = []
    #divstati = []
    #clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8)
    blobcode = 0
    #//
    for blob in img.find_blobs(thr, pixels_threshold=500, area_threshold=500,merge = True):
        #linecoor = [0,0]
        #for i in range(0,len(linecoor)):    #drawing tricolore-like lines
            #linecoor[i] = blob.x() + (blob.w()*(i+1))/3
            #divhist.append(img.get_histogram(roi = [blob.x(),blob.y(),int(linecoor[i]),blob.h()]))
            #img.draw_line(int(linecoor[i]),blob.y(),int(linecoor[i]),blob.y() + blob.h())
            #divstati.append(divhist[i].get_statistics())
            #print(divstati[i].l_mean())

        #print(blob.code())
        blobcode = blob.code()
        blob_diagonal_sq = blob.w()*blob.w()+blob.h()*blob.h()
        blob_cutted = blob.x()<=edge_cut_width or blob.x()+blob.w()>=img.width()-edge_cut_width\
         or blob.y()<=edge_cut_width or blob.y()+blob.h()>=img.height()-edge_cut_width
        if blob_cutted:
            result =='N'

        elif blobcode==1:#black
            linecount = 0
            for l in img.find_lines(roi = blob.rect() ,threshold = 1200, theta_margin = 50, rho_margin = 20):#直線検知しきい値 あまりいじる必要なし
                if (min_degree <= l.theta()) and (l.theta() <= max_degree):
                    img.draw_line(l.line(), color = (255, 0, 0))
                    linecount += 1
            circlecount = 0
            circle_r=int(blob_diagonal_sq*0.00167) #dividing by 600
            #circle_r = 10
            for c in img.find_circles(roi = blob.rect(), threshold = 2200, x_margin = 10, y_margin = 20, r_margin = 100,
            r_min = circle_r, r_max = circle_r + 10, r_step = 2):
                img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))
                circlecount += 1
            if linecount == 0 and circlecount == 0:
                result = 'U'
            if circlecount >= 2 and linecount <= 1:
                result = 'S'
            elif linecount == 2 and circlecount >= 1:
                result = 'U'
            elif linecount >=3 and circlecount== 0:
                result = 'H'
            else:
                result = 'N'
        elif blobcode==2:#green
            result = 'G'
        elif blobcode==4:#Yellow
            result = 'Y'
        elif blobcode==6:
            result = 'Y'
        elif blobcode==8:#Red
            result = 'R'
        else:
            result = 'N'
        if(blob_cutted):
            img.draw_rectangle(blob.rect(), color = (0,0,255))
        else:
            img.draw_rectangle(blob.rect())
        #for k in range(2):

    #print(result)
    #print(blob_diagonal_sq) #wrote for setting values
    #print(clock.fps())
    #uart.writechar(0x47)//
    uart.write(result)
    #result = 'N'
