import time
import requests
from bs4 import BeautifulSoup
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
ser.flushInput()
testString = "s 255 255 255 1"
oldString = ""
def reformatString(s):
    stringlist = s.split()
    effect = stringlist[0]
    redValue = formatValues(stringlist[1])
    greenValue = formatValues(stringlist[2])
    blueValue = formatValues(stringlist[3])
    speedValue = formatValues(stringlist[4])

    s = "$" + effect + redValue + greenValue + blueValue + speedValue + "$"
    return s

def formatValues(s):
    f = float(s)
    num = int(f)
    
    if(num < 10):
        s = "00" + str(num)
    elif(num < 100):
        s = "0" + str(num)
    elif(num < 0):
        s = "000"
    else:
        s = str(num)
    return s
def sendCode(s):
    ser.write(reformatString(s).encode())
    print(reformatString(s))

        
while True:
    try:
        page = requests.get("http://pacific-reef-86986.herokuapp.com")
        soup = BeautifulSoup(page.content, 'html.parser')
        orginalString = soup.find('h1', {'id':'text'}).text
        newString = orginalString[15:]
        if(oldString != newString):
            oldString = newString
            sendCode(newString)
     
        
    except:
        sendCode(testString)
        print("exeption in check site")
        
    
