import sys, serial, subprocess, time, keyboard 



# keyboard layout

#types
KEY = 0
MOD = 1
DICT = 2
TEXT = 3
CMOD = 4
SHIFT = 5
MOUSE_MOVE = 6
MOUSE_SCROLL = 7


val = 0
def next_val():
    global val
    prev = val
    val+=1
    return prev

L_PINKY_1 = next_val()
L_PINKY_2 = next_val()
L_PINKY_3 = next_val()

L_RING_1 = next_val()
L_RING_2 = next_val()
L_RING_3 = next_val()
L_RING_4 = next_val()

L_MIDDLE_1 = next_val()
L_MIDDLE_2 = next_val()
L_MIDDLE_3 = next_val()
L_MIDDLE_4 = next_val()

L_POINT_1 = next_val()
L_POINT_2 = next_val()
L_POINT_3 = next_val()
L_POINT_4 = next_val()

L_POINT_5 = next_val()
L_POINT_6 = next_val()

L_THUMB_1 = next_val()
L_THUMB_2 = next_val()
L_THUMB_3 = next_val()
L_THUMB_4 = next_val()

R_PINKY_1 = next_val()
R_PINKY_2 = next_val()
R_PINKY_3 = next_val()

R_RING_1 = next_val()
R_RING_2 = next_val()
R_RING_3 = next_val()
R_RING_4 = next_val()

R_MIDDLE_1 = next_val()
R_MIDDLE_2 = next_val()
R_MIDDLE_3 = next_val()
R_MIDDLE_4 = next_val()

R_POINT_1 = next_val()
R_POINT_2 = next_val()
R_POINT_3 = next_val()
R_POINT_4 = next_val()

R_POINT_5 = next_val()
R_POINT_6 = next_val()

R_THUMB_1 = next_val()
R_THUMB_2 = next_val()
R_THUMB_3 = next_val()
R_THUMB_4 = next_val()


layout = [
    
        [
        [None,      L_RING_4,   L_MIDDLE_4,     L_POINT_4,      L_POINT_6,],
        [L_PINKY_3, L_RING_3,   L_MIDDLE_3,     L_POINT_3,      L_POINT_5,],
        [L_PINKY_2, L_RING_2,   L_MIDDLE_2,     L_POINT_2,      L_THUMB_1,], # thumb TOP
        [L_PINKY_1, L_RING_1,   L_MIDDLE_1,     L_POINT_1,      L_THUMB_4,], # thumb BOTTOM
        [None,      None,       None,           L_THUMB_2,      L_THUMB_3,], # thumb LEFT, RIGHT
        ],


        # right
        [
        [R_POINT_6,     R_POINT_4,      R_MIDDLE_4,     R_RING_4,  None,],
        [R_POINT_5,     R_POINT_3,      R_MIDDLE_3,     R_RING_3,  R_PINKY_3,],
        [R_THUMB_1,     R_POINT_2,      R_MIDDLE_2,     R_RING_2,  R_PINKY_2,], # thumb TOP
        [R_THUMB_4,     R_POINT_1,      R_MIDDLE_1,     R_RING_1,  R_PINKY_1,], # thumb BOTTOM
        [R_THUMB_3,     R_THUMB_2,      None,           None,       None], # thumb LEFT, RIGHT
        ],
        ]


HOLD = 8
LATER = None
RETURN = 9
# TODO: add HOLD
left_dict = [DICT,{
                                        L_RING_4:[KEY,"7"],     L_MIDDLE_4:[KEY,"8"],           L_POINT_4:[KEY,"9"],    L_POINT_6:None,
            L_PINKY_3:[KEY,"escape"],   L_RING_3:[KEY,"4"],     L_MIDDLE_3:[KEY,"5"],           L_POINT_3:[KEY,"6"],    L_POINT_5:None,
            L_PINKY_2:LATER,            L_RING_2:[KEY,"1"],     L_MIDDLE_2:[KEY,"2"],           L_POINT_2:[KEY,"3"],    L_THUMB_1:LATER,
            L_PINKY_1:None,             L_RING_1:[KEY,"0"],     L_MIDDLE_1:[KEY,"_"],           L_POINT_1:[KEY,"+"],    L_THUMB_4:[MOD,"shift"],
                                                                                        L_THUMB_2:[KEY,"backspace"],    L_THUMB_3:[MOD,"ctrl"],

           R_POINT_6:None,              R_POINT_4:None,      R_MIDDLE_4:None,        R_RING_4:None,  
           R_POINT_5:None,              R_POINT_3:[KEY,"h"],        R_MIDDLE_3:[KEY,":"],   R_RING_3:[SHIFT,"#"],   R_PINKY_3:[KEY,"enter"],
           R_THUMB_1:LATER,             R_POINT_2:[TEXT,"nk"],      R_MIDDLE_2:[KEY,"/"],   R_RING_2:[KEY,"j"],     R_PINKY_2:LATER,
            R_THUMB_4:[MOD,"shift"],     R_POINT_1:[TEXT,"ck"],      R_MIDDLE_1:[KEY,26],    R_RING_1:[KEY,27],      R_PINKY_1:None,
            R_THUMB_3:[MOD,"ctrl"],  R_THUMB_2:[KEY,"space"],  

            }]
right_dict = [DICT,{
                                        L_RING_4:None,          L_MIDDLE_4:[TEXT,"gh"],        L_POINT_4:[TEXT,"ph"],  L_POINT_6:None,
            L_PINKY_3:[KEY,"escape"],   L_RING_3:[KEY,"q"],     L_MIDDLE_3:[KEY,"z"],   L_POINT_3:[KEY,"p"],    L_POINT_5:None,
            L_PINKY_2:LATER,            L_RING_2:[KEY,"x"],     L_MIDDLE_2:[KEY,"g"],   L_POINT_2:[KEY,"c"],    L_THUMB_1:LATER,
            L_PINKY_1:None,             L_RING_1:[SHIFT,"("],   L_MIDDLE_1:[SHIFT,")"], L_POINT_1:[TEXT,"ch"],  L_THUMB_4:[MOD,"shift"],
                                                                        L_THUMB_2:[KEY,"backspace"],    L_THUMB_3:[MOD,"ctrl"],

            R_POINT_6:None,             R_POINT_4:None,                 R_MIDDLE_4:None,            R_RING_4:None,  
            R_POINT_5:None,             R_POINT_3:[SHIFT,"*"],         R_MIDDLE_3:[SHIFT,"&"],      R_RING_3:[SHIFT,"@"], R_PINKY_3:[KEY,"enter"],
            R_THUMB_1:LATER,    R_POINT_2:[SHIFT,"!"],      R_MIDDLE_2:[KEY,"|"],    R_RING_2:[KEY,"`"],          R_PINKY_2:LATER,
            R_THUMB_4:[MOD,"shift"],     R_POINT_1:[SHIFT,"%"],    R_MIDDLE_1:[SHIFT,"^"],            R_RING_1:[SHIFT,"$"],          R_PINKY_1:None,
            R_THUMB_3:[MOD,"windows"],  R_THUMB_2:[KEY,"space"],  

            }]

func_dict_right = [DICT,{
                               L_RING_4:None,           L_MIDDLE_4:None,            L_POINT_4:None,             L_POINT_6:None,
            L_PINKY_3:[KEY,"escape"],    L_RING_3:[KEY,"left"],   L_MIDDLE_3:[KEY,"up"],      L_POINT_3:[KEY,"right"],    L_POINT_5:None,
            L_PINKY_2:LATER,    L_RING_2:[KEY,"home"],   L_MIDDLE_2:[KEY,"down"],    L_POINT_2:[KEY,"page up"],  L_THUMB_1:LATER,
            L_PINKY_1:None,    L_RING_1:[KEY,"end"],    L_MIDDLE_1:None,            L_POINT_1:[KEY,"page down"],L_THUMB_4:[MOD,"shift"],
                                                                      L_THUMB_2:[KEY,"backspace"],    L_THUMB_3:[MOD,"ctrl"],

            R_POINT_6:None,             R_POINT_4:[KEY,"F10"],         R_MIDDLE_4:[KEY,"F11"],    R_RING_4:[KEY,"F12"],  
            R_POINT_5:None,             R_POINT_3:[KEY,"F7"],         R_MIDDLE_3:[KEY,"F8"],    R_RING_3:[KEY,"F9"],   R_PINKY_3:[KEY,"enter"],
            R_THUMB_1:LATER,    R_POINT_2:[KEY,"F4"],         R_MIDDLE_2:[KEY,"F5"],    R_RING_2:[KEY,"F6"],                    R_PINKY_2:LATER,
            R_THUMB_4:[MOD,"shift"],     R_POINT_1:[KEY,"F1"],         R_MIDDLE_1:[KEY,"F2"],    R_RING_1:[KEY,"F3"],            R_PINKY_1:None,
            R_THUMB_3:[MOD,"ctrl"],  R_THUMB_2:[KEY,"space"],  

            }]
func_dict_right[1][R_THUMB_1] = left_dict
func_dict_right[1][R_PINKY_2] = left_dict
func_dict_right[1][R_POINT_5] = right_dict

func_dict_right[1][L_THUMB_1] = right_dict
func_dict_right[1][L_PINKY_2] = right_dict
func_dict_right[1][L_POINT_5] = left_dict
func_dict_right[1][R_POINT_6] = func_dict_right


left_dict[1][R_THUMB_1] = left_dict
left_dict[1][R_PINKY_2] = left_dict
left_dict[1][R_POINT_5] = right_dict

left_dict[1][L_THUMB_1] = right_dict
left_dict[1][L_PINKY_2] = right_dict
left_dict[1][L_POINT_5] = left_dict


right_dict[1][R_THUMB_1] = left_dict
right_dict[1][R_PINKY_2] = left_dict
right_dict[1][R_POINT_5] = right_dict

right_dict[1][L_THUMB_1] = right_dict
right_dict[1][L_PINKY_2] = right_dict
right_dict[1][L_POINT_5] = left_dict


key_dict = [DICT,{

                                     L_RING_4:[MOD,"windows"],   L_MIDDLE_4:[MOD,"alt"],           L_POINT_4:[KEY,"tab"],         L_POINT_6:None,
        L_PINKY_3:[KEY,"escape"],    L_RING_3:[KEY,"w"],         L_MIDDLE_3:[KEY,"f"],      L_POINT_3:[KEY,"t"],    L_POINT_5:left_dict,
        L_PINKY_2:right_dict,        L_RING_2:[KEY,"r"],         L_MIDDLE_2:[KEY,"s"],      L_POINT_2:[KEY,"d"],    L_THUMB_1:right_dict,
        L_PINKY_1:[KEY,"a"],         L_RING_1:[KEY,"'"],       L_MIDDLE_1:[KEY,"b"],    L_POINT_1:[KEY,"v"],    L_THUMB_4:[MOD,"shift"],
                                                                                            L_THUMB_2:[KEY,"backspace"],         L_THUMB_3:[MOD,"ctrl"],

        R_POINT_6:func_dict_right,             R_POINT_4:[KEY,"k"],         R_MIDDLE_4:[MOD,"alt"],        R_RING_4:[MOD,"windows"],  
        R_POINT_5:right_dict,             R_POINT_3:[KEY,"l"],    R_MIDDLE_3:[KEY,"u"],   R_RING_3:[KEY,"y"],       R_PINKY_3:[KEY,"enter"],
        R_THUMB_1:left_dict,            R_POINT_2:[KEY,"n"],    R_MIDDLE_2:[KEY,"e"],   R_RING_2:[KEY,"i"],         R_PINKY_2:left_dict,
        R_THUMB_4:[MOD,"shift"],     R_POINT_1:[KEY,"m"],    R_MIDDLE_1:[KEY,","],        R_RING_1:[KEY,"."],        R_PINKY_1:[KEY,"o"],
        R_THUMB_3:[MOD,"ctrl"],  R_THUMB_2:[KEY,"space"],  

        }]




import time
prefix = []
# current time
prefix_time = time.time()
prefix_hold = False
 

PREFIX_DELAY = 2
default_dict = key_dict;

alt_active = False


def process_input(line:str):
    global active_layer, prefix, prefix_time, prefix_hold, default_dict, alt_active, func_dict_right, right_dict, left_dict, key_dict
    # print(line.strip(), end=" ")
    # seperate key index and key action (press,release,repeat) from line
    side,key_row, key_col, key_action = line.split(":")

    key = layout[int(side)][int(key_row)][int(key_col)]
    # print("->",key, ": ", key_dict[key])
    if len(prefix) != 0 and prefix_hold == False:
        # if time.time() - prefix_time > PREFIX_DELAY:
        #     prefix = []
        pass


    prefix.append(key)
     
    dict = default_dict
    for k in prefix:
        dict = dict[1][k]

    
    if key_action.strip() == "p":
        # print(prefix,"->",end=" ")
        if dict is None:
            prefix = []
            prefix_hold = False
            # print("None")
            return

        if dict == HOLD:
            prefix_hold = True
            prefix = prefix[:-1]
            # print("HOLD")
            return

        # check if dict is a dictionary
        if dict[0] == DICT:
            if len(prefix)>1:
                if prefix[-2] == prefix[-1]:
                    # print("HOLD", end=" ")
                    default_dict = dict
                    prefix = []

            # print("DICT")
            return

        prefix = prefix[:-1]
        prefix_time = time.time()

        if dict[0] == KEY:

            # print(dict[1])
            if prefix_hold == False:
                prefix = []
            if alt_active:
                keyboard.release('alt')
                if dict[1]=="l": 
                    keyboard.press('left')
                elif dict[1]=="i": 
                    keyboard.press('right')
                elif dict[1]=="n": 
                    keyboard.press('down')
                elif dict[1]=="e": 
                    keyboard.press('up')
                else:
                    keyboard.press('alt')
                    keyboard.press(dict[1])
            else:
                if alt_active:
                    keyboard.press('alt')
                keyboard.press(dict[1])
        elif dict[0] == TEXT:
            # print(dict[1])
            if prefix_hold == False:
                prefix = []
            keyboard.write(dict[1])

        elif dict[0] == SHIFT:

            # print("SHIFT+",dict[1])
            if prefix_hold == False:
                prefix = []
            keyboard.press_and_release("shift+"+dict[1])

        elif dict[0] == MOD:
            # print(dict[1])
            keyboard.press(dict[1])
            if key == L_MIDDLE_4 or key == R_MIDDLE_4:
                alt_active = True


    else:
        prefix = prefix[:-1]
        if dict == HOLD:
            # print("HOLD")
            return

        if dict is None:
            pass
            # print("")
        elif key == R_MIDDLE_4 or key == L_MIDDLE_4:
            alt_active = False
            keyboard.release('up+down+left+right')
        elif dict[0] == DICT:
            # print("STOP HOLD")
            default_dict = key_dict
        # elif key==L_RING_4 or key == R_RING_4 or key == L_MIDDLE_4 or key == R_MIDDLE_4:
        #     keyboard.release(key_dict[1][key])
        elif dict[0]==KEY:
            if alt_active:
                if dict[1]=="l": 
                    keyboard.release('left')
                if dict[1]=="i": 
                    keyboard.release('right')
                if dict[1]=="n": 
                    keyboard.release('down')
                if dict[1]=="e": 
                    keyboard.release('up')
            # print("")
        else:
            pass
            # print("")
        l = [ 
                key_dict[1][key], 
                left_dict[1][key], 
                right_dict[1][key],
                func_dict_right[1][key]]


        for d in l:
            if d is None:
                continue
            if d[0] == KEY:
                # print('release',d[1])
                keyboard.release(d[1])
            if d[0] == MOD:
                # print('release',d[1])
                keyboard.release(d[1])









#main
if __name__ == '__main__':
    while True: 

        try:


            dev = sys.argv[1]
            baud = int(sys.argv[2]) if len(sys.argv) > 2 else 9600
            if dev == "-p":
                proc = subprocess.Popen("find /dev/serial/by-id | grep 'a86_USB2' | sed '1q'", shell=True, stdout=subprocess.PIPE)
                dev = proc.communicate()[0].decode('utf-8')[:-1]
            ser = serial.Serial(dev, baud)
            while True:
                process_input(ser.readline().decode('utf-8'))

        except:
            print("Exception accured.. starting again in 1s")
            # sleep for 1 s
            time.sleep(1)


            




