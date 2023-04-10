# FS2011 Pro
# Remote control for tests
#
# (C) 2022 Gissio
#
# License: MIT
#
# Uses pyswd to contro lthe firmware remotely. Modify memory addresses acordingly.

import keyboard
import time
import swd

dev = swd.Swd()

def set_generator(value):
    global dev

    if value:
        dev.write_mem8(0x200001d0, [255, 14, 0, 0])
    else:
        dev.write_mem8(0x200001d0, [0, 0, 0, 0])
   
def set_key(key, state):
    global dev

    dev.write_mem8(0x200004fc + key, [state])

def show_cnt():
    global dev

    value = dev.read_mem8(0x40001024, 4)
    print("Count: " + ' '.join(['%02x' % d for d in value]))
    value = dev.read_mem8(0x4000102c, 4)
    print("ARR: " + ' '.join(['%02x' % d for d in value]))

def on_key(event):
    state = (event.event_type == keyboard.KEY_DOWN)
    if event.name == "space":
        set_key(0, state)
    elif event.name == "flecha arriba":
        set_key(1, state)
    elif event.name == "flecha abajo":
        set_key(2, state)
    elif event.name == "flecha derecha":
        set_key(3, state)
    elif event.name == "flecha izquierda":
        set_key(4, state)
    elif event.name == "1":
        set_generator(0)
    elif event.name == "2":
        set_generator(1)
    elif event.name == "3":
        show_cnt()
    print(event.name + ": " + event.event_type)

keyboard.hook(on_key)

while True:
    time.sleep(1)
