import socket 
import time 
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
import pandas as pd
import datetime
from matplotlib import style

style.use("dark_background")

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect(("192.168.0.20", 80))

data = pd.DataFrame(columns = {"Time", "Cold Humidity", "Cold Temp", "Hot Humidity", "Hot Temp"})

cold_humid = -1
cold_temp = -1
hot_humid = -1
hot_temp = -1


fig = plt.figure(num="Bart Monitor")
axHum = fig.add_subplot(211)
axTemp = fig.add_subplot(212)

def getNewData():
    global data
    global cold_humid
    global cold_temp
    global hot_humid
    global hot_temp

    curData = sock.recv(1024)
    curData = curData.decode("utf-8")
    print(curData)
    curData = curData.split(",")
    #print(curData)

    cold_humid = curData[1]
    cold_temp = curData[3]
    hot_humid = curData[5]
    hot_temp = curData[7]

    time = datetime.datetime.now()

    newData = pd.DataFrame({"Time": [str(time.hour) + ":" + str(time.minute) + ":" + str(time.second)], "Cold Humidity" : [cold_humid], "Cold Temp" : [cold_temp], "Hot Humidity" : [hot_humid], "Hot Temp" : [hot_temp]})

    print(newData)

    if (len(data.index) >= 500):
        data = data.drop([0])
        data = data.reset_index(drop = True)
    
    data = data.append(newData, ignore_index = True)
    data["Cold Humidity"] = data["Cold Humidity"].astype(float)
    data["Hot Humidity"] = data["Hot Humidity"].astype(float)
    data["Cold Temp"] = data["Cold Temp"].astype(float)
    data["Hot Temp"] = data["Hot Temp"].astype(float)

    return data

def animate(i):
    global data
    global cold_humid
    global cold_temp
    global hot_humid
    global hot_temp
    
    data = getNewData()

    axHum.clear()
    axHum.tick_params(axis='x', rotation=90)
    axHum.xaxis.set_visible(False)
    axHum.set_ylabel("Humidity")
    #axHum.yaxis.set_label_position("right")
    #axHum.yaxis.tick_right()
    axHum.plot(data["Time"].to_numpy(), data["Cold Humidity"].to_numpy(), "b", label = str(cold_humid) + "%")
    axHum.plot(data["Time"].to_numpy(), data["Hot Humidity"].to_numpy(), "r", label = str(hot_humid) + "%")
    axHum.legend(loc="upper left")

    axTemp.clear()
    axTemp.tick_params(axis='x', rotation=90)
    axTemp.xaxis.set_major_locator(plt.MaxNLocator(10))
    axTemp.set_ylabel("Temperature")
    #axTemp.yaxis.set_label_position("right")
    #axTemp.yaxis.tick_right()
    axTemp.plot(data["Time"].to_numpy(), data["Cold Temp"].to_numpy(), "b", label = str(cold_temp) + chr(176) + "f")
    axTemp.plot(data["Time"].to_numpy(), data["Hot Temp"].to_numpy(), "r", label = str(hot_temp) + chr(176) + "f")
    axTemp.legend(loc="upper left")

ani = animation.FuncAnimation(fig, animate, interval=2000)
plt.show()
