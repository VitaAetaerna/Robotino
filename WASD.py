import requests
import signal
import webbrowser
import json
import time
from PIL import Image, ImageStat
from io import BytesIO
import keyboard

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

def OdometryGet():
    ODOURL = "http://" + IP + "/data/odometry"
    r = requests.get(url=ODOURL)
    if r.status_code == requests.codes.ok:
        data = r.json()
        return [round(data[0], 1), round(data[1], 1)]
    else:                
        raise RuntimeError("Error: post to %s with params %s failed", ODOURL, PARAMS)


def InputKeyboard():
    while keyboard.is_pressed('w'):
        set_vel([2, 0, 0])

    while keyboard.is_pressed('a'):
        set_vel([0, 2, 0])

    while keyboard.is_pressed('s'):
        set_vel([-2, 0, 0])

    while keyboard.is_pressed('d'):
        set_vel([0, -2, 0])

    while keyboard.is_pressed('e'):
        set_vel([0.2, 2, 0])

    while keyboard.is_pressed('r'):
        set_vel([0.2, -2, 0])

    while keyboard.is_pressed('f'):
        set_vel([-2, 2, 0])

    while keyboard.is_pressed('g'):
        set_vel([-2, -2, 0])


    while keyboard.is_pressed('y'):
        set_vel([0,0,3])

    while keyboard.is_pressed('x'):
        set_vel([0,0,-3])


    set_vel([0, 0, 0])


def main():
    signal.signal(signal.SIGINT, signal_handler)

    try:
        while False == bumper() and True == run:
            
            InputKeyboard()
            time.sleep(0.3)

            
    except Exception as e:
        print(e)
        return 1
    return 0

if __name__ == "__main__":
    main()
