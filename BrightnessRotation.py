import requests
import signal
import webbrowser
import json
import time
from PIL import Image, ImageStat
from io import BytesIO

IP = "192.168.100.40"
PARAMS = {'sid' : 'Drive'}
run = True

def signal_handler(sig, frame):
    global run
    print('U pressed Ctrl-C')
    run = False



# DEFINE ALL URL's

def set_vel(vel):
    OMNIDRIVE_URL = "http://" + IP + "/data/omnidrive"
    r = requests.post(url=OMNIDRIVE_URL, params=PARAMS, json=vel)
    if r.status_code != requests.codes.ok:
        raise RuntimeError("Error: post to %s with params %s failed", OMNIDRIVE_URL, PARAMS)

def bumper():
    BUMPERURL = "http://" + IP + "/data/bumper"
    r = requests.get(url=BUMPERURL)
    if r.status_code == requests.codes.ok:
        data = r.json()
        return data["value"]
    else:
        raise RuntimeError("Error: post to %s with params %s failed", BUMPERURL, PARAMS)


def distances():
    DISTANCE_URL = "http://" + IP + "/data/distancesensorarray"
    r = requests.get(url=DISTANCE_URL, params=PARAMS)
    if r.status_code == requests.codes.ok:
        data = r.json()
        return data


def Camera():
    CAMURL = "http://" + IP + "/cam0"
    r = requests.get(url=CAMURL)
    while r.status_code != requests.codes.ok:
        r = requests.get(url=CAMURL)
    img = Image.open(BytesIO(r.content))
    return img


def BrightnessCalc(img):
   lum = img.convert('L')
   stat = ImageStat.Stat(lum)
   return stat.mean[0]


def GetImageAndBrightness():
    vec_to_rotate_left = [0,0, 0.5]
    vec_to_rotate_right = [0,0, -0.5]
    
    image = Camera()
    width, height = image.size
    left_image = image.crop((0,0,width/2,height))

    right_image = image.crop((width/2,0,width,height))


    Left_Brightness = BrightnessCalc(left_image)
    print("Left Image Brightness " + str(Left_Brightness))

    Right_Brightness = BrightnessCalc(right_image)
    print("Right Image Brighness " + str(Right_Brightness))

    if Left_Brightness > Right_Brightness and Left_Brightness - Right_Brightness > 15:
        print("Left is brighter then Right")
        set_vel(vec_to_rotate_left) and print("Rotate Left")

    if Right_Brightness > Left_Brightness and Right_Brightness - Left_Brightness > 15:
        print("Right is brighter then Left")
        set_vel(vec_to_rotate_right) and print("Rotate Right")

def main():
    signal.signal(signal.SIGINT, signal_handler)

    try:
        while False == bumper() and True == run:
            GetImageAndBrightness()
            time.sleep(0.3)

            
    except Exception as e:
        print(e)
        return 1
    return 0

if __name__ == "__main__":
    main()
