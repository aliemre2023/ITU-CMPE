# https://noobc0re.itch.io/sound-of-the-machine
'''
This code is a bot for a game, the link to which is given above.
'''


import numpy as np
import soundcard as sc
import pyautogui
import time
import threading

speakers = sc.all_speakers()
default_speaker = sc.default_speaker()
mics = sc.all_microphones()
default_mic = sc.default_microphone()


def enter():
    pyautogui.press("enter")

def move(key, second):
    pyautogui.keyDown(key)
    time.sleep(second)
    pyautogui.keyUp(key)

def measure_amplitude(way, duration=1, samplerate=48000, numframes=1024):
    move_thread = threading.Thread(target=move, args=(way, duration))
    move_thread.start()

    amplitudes = []
    with default_mic.recorder(samplerate=samplerate) as mic:
        start_time = time.time()
        while time.time() - start_time < duration:
            data = mic.record(numframes=numframes)
            amplitude = np.abs(data).max()
            #print(f"Amplitude {way}: {amplitude}")
            amplitudes.append([amplitude, time.time() - start_time])
            
    return amplitudes

def ensure():
    keys = ["w","a","s","d"]
    wait = 0.1
    i = 0
    while(i < 10):
        key = keys[i % len(keys)]
        pyautogui.keyDown(key)
        time.sleep(wait)
        pyautogui.keyUp(key)

        if i%2: wait += 0.1
        i += 1

def main():
    prob_time = 1.5
    time.sleep(1)
    enter()
    start = time.time()
    try_count = 0
    while(time.time() - start < 60):
        try_count += 1
        print(f"---------{try_count}---------")

        move_thread = threading.Thread(target=move, args=("s", prob_time))
        move_thread.start()
        move("a", prob_time)

        print("Vertical amplitude calculating...")
        v = measure_amplitude("w", prob_time)
        print("Horizontal amplitude calculating...")
        h = measure_amplitude("d", prob_time)

        max_v = max(v, key=lambda x: x[0])
        max_h = max(h, key=lambda x: x[0])
        print(f"Max v amplitude: {max_v[0]:.4} at time {max_v[1]:.4}")
        print(f"Max h amplitude: {max_h[0]:.4} at time {max_h[1]:.4}")

        print("Catching...")
        move_thread = threading.Thread(target=move, args=("s", prob_time-max_v[1]))
        move_thread.start()
        move("a", prob_time-max_h[1])
        ensure()

        start += 1

main()