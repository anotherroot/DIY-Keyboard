import sys, serial, subprocess, time, pyautogui, threading, keyboard, mouse




# keyboard layout
layout = [];

#types
KEY = 0
FUNC = 2
TEXT = 3
CMOD = 4
SHIFT = 5
MOUSE_MOVE = 6
MOUSE_SCROLL = 7

# layer 0
layout.append([
    # left
        [
        [None,      [CMOD,2],   [CMOD,0],   [KEY,"p"],          [KEY,"g"],],
        [[KEY,"q"], [KEY,"w"],  [KEY,"f"],  [KEY,"t"],          [KEY,"b"],],
        [[KEY,"a"], [KEY,"r"],  [KEY,"s"],  [KEY,"d"],          [KEY,"escape"],], # thumb TOP
        [[KEY,"z"], [KEY,"x"],  [KEY,"c"],  [KEY,"v"],          [KEY,"shift"],], # thumb BOTTOM
        [None,      None,       None,       [KEY,"backspace"],  [KEY,"alt"],], # thumb LEFT, RIGHT
        ],


        # right
        [
        [[KEY,"j"],     [KEY,"h"],      [CMOD,1],   [CMOD,3],   None,],
        [[KEY,"k"],     [KEY,"l"],      [KEY,"u"],  [KEY,"y"],  [KEY,";"],],
        [[KEY,"enter"], [KEY,"n"],      [KEY,"e"],  [KEY,"i"],  [KEY,"o"],], # thumb TOP
        [[KEY,"ctrl"],  [KEY,"m"],      [KEY,","],  [KEY,"."],  [KEY,"/"],], # thumb BOTTOM
        [[KEY,"tab"],   [KEY,"space"],  None,       None,       None], # thumb LEFT, RIGHT
        ],
    ])

# layer 1: code and numpad
layout.append([

    # left: 0 keys left
        [
        [None,       [CMOD,2],  [CMOD,0],   [KEY,"*"],          [KEY,"_"],],
        [[None],  [KEY,"7"], [KEY,"8"],  [KEY,"9"],          [KEY,"+"],],
        [[KEY,"/"],  [KEY,"4"], [KEY,"5"],  [KEY,"6"],          [KEY,"escape"],], # thumb TOP
        [[KEY,"0"],  [KEY,"1"], [KEY,"2"],  [KEY,"3"],          [KEY,"shift"],], # thumb BOTTOM
        [None,       None,      None,       [KEY,"backspace"],  [KEY,"alt"],], # thumb LEFT, RIGHT
        ],

    # right: 1 keys left
        [
        [[KEY,"`"],         [KEY,"^"],      [CMOD, 1],       [CMOD, 3],       None,],
        [[SHIFT,"%"],       [SHIFT,"&"],    [SHIFT,"("],    [SHIFT,")"],    [None],],
        [[KEY,"enter"],     [KEY,"|"],      [KEY,26],      [KEY,27],      [KEY,"'"],], # thumb TOP
        [[KEY,"ctrl"],      [SHIFT,"!"],    [KEY,"@"],      [KEY,"#"],      [KEY,"$"],], # thumb BOTTOM
        [[KEY,"tab"],       [KEY,"space"],  None,           None,           None], # thumb LEFT, RIGHT
        ],

    ])

# layer 2: navigation (dwm,browser,pdf)
layout.append([
    # left: 2 keys left
        [
        [None,      [CMOD,2],       [CMOD,0],       [KEY,"t"],          [None],],
        [[KEY,"q"],  [KEY,"7"],   [KEY,"8"],     [KEY,"9"],     [None],],
        [[KEY,"s"],  [KEY,"4"],    [KEY,"5"],   [KEY,"6"],     [KEY,"windows+tab"],], # thumb TOP
        [[KEY,"z"],  [KEY,"1"],      [KEY,"2"],       [KEY,"3"],          [KEY,"shift"],], # thumb BOTTOM
        [None,      None,           None,           [KEY,"alt+shift"], [KEY,"alt"],], # thumb LEFT, RIGHT
        ],

        # right: 4 keys left
        [
        [[KEY, "o"],              [KEY,"space"],              [CMOD,1],          [CMOD,3],          None,],
        [[KEY, "r"],              [KEY,"c"],              [KEY,"w"],          [KEY,"page up"],    [None],],
        [[KEY,"windows+enter"], [KEY,"left"],           [KEY,"up"],         [KEY,"right"],      [KEY,"home"],], # thumb TOP
        [[KEY,"ctrl"],          [KEY,"b"],              [KEY,"down"],       [KEY,"page down"],  [KEY,"end"],], # thumb BOTTOM
        [[KEY,"windows"],       [KEY,"windows+ctrl"],   None,               None,               None], # thumb LEFT, RIGHT
        ],
    ])

# layer 3: mouse and special keys
layout.append([
    # left
        [
        [None,      [CMOD,2],   [CMOD,0],   [KEY,"F10"],          [KEY,"F12"],],
        [[None], [KEY,"F7"],  [KEY,"F8"],  [KEY,"F9"],          [KEY,"F11"],],
        [[None], [KEY,"F4"],  [KEY,"F5"],  [KEY,"F6"],          [KEY,"mute"],], # thumb TOP
        [[None], [KEY,"F1"],  [KEY,"F2"],  [KEY,"F3"],     [KEY,"shift"],], # thumb BOTTOM
        [None,      None,       None,       [KEY,"alt+shift"],  [KEY,"alt"],], # thumb LEFT, RIGHT
        ],

        # right mouse
        [
        [[CMOD,1],         [None],              [CMOD,1],   [CMOD,3],   None,],
        [[CMOD,3],         [None],    [None],  [None],  [None],],
        [[KEY,"enter"],     [None],              [None],  [None],  [None],], # thumb TOP
        [[KEY,"ctrl"],      [None],              [None],  [None],  [None],], # thumb BOTTOM
        [[KEY,"windows"],   [KEY,"windows+ctrl"],   None,       None,       None], # thumb LEFT, RIGHT
        ],

    ])

# - letters
# - navigation (mouse, dwm, movement)
# - code and numpad
# - other weird commands

active_layer = 0




def process_input(line:str):
    global active_layer
    print(line.strip(), end=" ")
    # seperate key index and key action (press,release,repeat) from line
    side,key_row, key_col, key_action = line.split(":")

    key = layout[active_layer][int(side)][int(key_row)][int(key_col)]
    print("->",key)
    if key is None:
        return
    if key[0] is None:
        return
    
    if key_action.strip() == "p":
        if key[0] == KEY:
            keyboard.press(key[1])
        if key[0] == CMOD:
            active_layer = key[1]
        if key[0] == SHIFT:
            keyboard.press_and_release("shift+"+key[1])
    else:
        if key[0] == KEY:
            keyboard.release(key[1])





#main
if __name__ == '__main__':
    while True: 

        # try:


        dev = sys.argv[1]
        baud = int(sys.argv[2]) if len(sys.argv) > 2 else 9600
        if dev == "-p":
            proc = subprocess.Popen("find /dev/serial/by-id | grep 'Arduino_Uno' | sed '1q'", shell=True, stdout=subprocess.PIPE)
            dev = proc.communicate()[0].decode('utf-8')[:-1]
        ser = serial.Serial(dev, baud)
        while True:
            process_input(ser.readline().decode('utf-8'))

        # except:
        #     print("Exception accured.. starting again in 1s")
        #     # sleep for 1 s
        #     time.sleep(1)


            




