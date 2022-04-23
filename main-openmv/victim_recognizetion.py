import sensor, image, time,pyb
from pyb import Pin ,UART,LED, Timer



led = Pin('P7',Pin.OUT_PP,Pin.PULL_NONE)
start = Pin('P3',Pin.IN,Pin.PULL_NONE)
lighton = Pin('P6',Pin.IN,Pin.PULL_NONE)
status = Pin('P8',Pin.OUT_PP,Pin.PULL_NONE)

lowbit = Pin('P4',Pin.OUT_PP,Pin.PULL_NONE)
highbit = Pin('P5',Pin.OUT_PP,Pin.PULL_NONE)

status.off()


red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

tim = Timer(4, freq=20000)

#led.value(False)
while(start.value() == False):
    if lighton.value() == True:
        ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=50)
        #led.value(not led.value())
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
thr = [(0, 26, -128, 127, -128, 127),(31, 41, -40, -19, 7, 26),(75, 84, -1, 15, 59, 84),(39, 57, 16, 78, 9, 65)]#code 1,2,4,8/Black Green,Yellow,Red
#llo,lhi,alo,ahi,blo,bhi
min_degree = 0
max_degree = 179
edge_cut_width = 0
result = 'N'
blob_diagonal_sq = 6000 # <- proper size for existing algorithm
status.on()

clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8)
    blobcode = 0
    for blob in img.find_blobs(thr, pixels_threshold=500, area_threshold=500,merge = True):
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
            for c in img.find_circles(roi = blob.rect(), threshold = 2000, x_margin = 10, y_margin = 20, r_margin = 100,
            r_min = circle_r, r_max = circle_r + 10, r_step = 2):
                img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))
                circlecount += 1
            if linecount == 0 and circlecount == 0:
                result = 'U'
            elif circlecount >= 2 and linecount <= 1:
                result = 'S'
            elif linecount >= 2 and linecount <=3 and circlecount >= 1:
                result = 'U'
            elif linecount >= 3:
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
        img.draw_rectangle(blob.rect())
        #for k in range(2):

    if result == 'H' or result == 'S':
        lowbit.value(True)
        highbit.value(True)
    elif result == 'Y' or result == 'R':
        lowbit.value(False)
        highbit.value(True)
    elif result == 'G' or result == 'U':
        lowbit.value(True)
        highbit.value(False)
    else:
        lowbit.value(False)
        highbit.value(False)

    print(result)
    #print(blob_diagonal_sq) #wrote for setting values
    print(clock.fps())
    result = 'N'
