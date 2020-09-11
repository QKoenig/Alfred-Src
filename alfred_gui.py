import tkinter
import time
import socket

root = tkinter.Tk()

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect(("192.168.0.21", 80))

def execDoorLock():
    sock.send(str.encode("LOCK\n"))

doorBtn = tkinter.Button(root, command = execDoorLock, text = "DOOR LOCK") 

doorBtn.pack()
root.mainloop()
