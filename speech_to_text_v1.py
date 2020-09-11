import speech_recognition as sr
import pyaudio
import threading

r = sr.Recognizer()

def SpeechToText(): 
    with sr.Microphone() as source:
        try:
            audio_data = r.record(source, duration=3)
            print("Recognizing...")
            text = r.recognize_google(audio_data)
            print(text)
        except:
            print("ERROR")


def main ():
    while True:
        threads = list()
        for i in range(1):


if True:
    main()