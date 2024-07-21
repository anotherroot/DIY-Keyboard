#include <Keyboard.h> 
#include <Wire.h>

#define DEBUG

#ifdef DEBUG
#define LOG(x) Serial.print(x)
#define LOGLN(x) Serial.println(x)
#else
#define LOG(x) 
#define LOGLN(x) 
#endif

#define PACK(kbd,row,column) (kbd*100+row*10+column)
#define UNPACK(packed) Loc(packed/100,(packed%100)/10,packed%10)
#define HOLD_TIME 17

byte rows[] = { 8, 7, 6, 5, 4};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

byte columns[] = { 15, 14, 16, 10, 9, };
const int columnCount = sizeof(columns)/sizeof(columns[0]);

byte matrix[columnCount][rowCount];

void setup()
{
  Wire.begin(9);               // join I2C bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);          // start serial for output

  for(int x=0; x<rowCount; x++) {
		LOG(rows[x]); LOGLN(" as input");
		pinMode(rows[x], INPUT);
	}

	for (int x=0; x<columnCount; x++) {
		LOG(columns[x]); LOGLN(" as input-pullup");
		pinMode(columns[x], INPUT_PULLUP);
	}
}


void readMatrix() {
	// iterate the columns
	for (int colIndex=0; colIndex < columnCount; colIndex++) {
		// col: set to output to low
		byte curCol = columns[colIndex];
		pinMode(curCol, OUTPUT);
		digitalWrite(curCol, LOW);

		// row: interate through the rows
		for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
			byte rowCol = rows[rowIndex];
			pinMode(rowCol, INPUT_PULLUP);
      byte old_state = matrix[rowIndex][colIndex];
      byte state = !digitalRead(rowCol);
      if(old_state^state){
        processKeyEvent(PACK(0, rowIndex, colIndex), state);
      }
      matrix[rowIndex][colIndex] = state;
			pinMode(rowCol, INPUT);
		}
		// disable the column
		pinMode(curCol, INPUT);
	}
}



// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  int row = Wire.read();
  int column = Wire.read();
  bool pressed = !Wire.read();
  processKeyEvent(PACK(1,row,column),pressed);
}

const byte Layer0 = 0;
const byte Layer1 = 1;
const byte Layer2 = 2;
const byte Layer3 = 3;
const byte CountLayer = 4;

const byte ModeNone=0;
const byte ModeKey=1;
const byte ModeKeyMod=2;
const byte ModeMod=3;
const byte ModeKeyLayer=4;
const byte ModeLayer=5;
const byte ModeLayerMod=6;
const byte ModeLayerLayer=7;

struct Layer {
  byte layer;
  Layer(){
  }
  Layer( int layer):layer(layer){
  }
};

struct Mod {
  byte keycode;
  Mod(){
  }
  Mod(short keycode):keycode(keycode){
  }
};

struct Key{
  byte mode;
  byte keycode;
  Mod mod;
  Layer layer;
  Layer layer_hold;
  Key():mode(ModeNone){
  }
  Key( byte keycode)
    :keycode(keycode),mode(ModeKey){
  }
  Key( byte keycode, Mod mod)
    :keycode(keycode),mod(mod),mode(ModeKeyMod){
  }
  Key( Mod mod)
    :mod(mod),mode(ModeMod){
  }
  Key( byte keycode, Layer layer)
    :keycode(keycode),layer(layer),mode(ModeKeyLayer){
  }
  Key( Layer layer)
    :layer(layer),mode(ModeLayer){
  }
  Key( Layer layer, Mod mod)
    :layer(layer),mod(mod),mode(ModeLayerMod){
  }
  Key( Layer layer, Layer layer_hold)
    :layer(layer),layer_hold(layer_hold),mode(ModeLayerLayer){
  }
};
struct Loc{
  byte kbd;
  byte row;
  byte column;
  Loc(){
  }
  Loc(byte kbd, byte row, byte column):kbd(kbd),row(row),column(column){
  
  }
};

typedef Key K;
typedef Mod M;
typedef Layer L;



void printKeycode(byte keycode){
  switch(keycode){
    case KEY_LEFT_CTRL: { LOG("Ctrl"); } break;
    case KEY_LEFT_SHIFT: { LOG("Shift"); } break;
    case KEY_LEFT_ALT: { LOG("Alt"); } break;
    case KEY_LEFT_GUI: { LOG("Win"); } break;
    case KEY_RIGHT_CTRL: { LOG("Ctrl"); } break;
    case KEY_RIGHT_SHIFT: { LOG("Shift"); } break;
    case KEY_RIGHT_ALT: { LOG("Alt"); } break;
    case KEY_RIGHT_GUI: { LOG("Win"); } break;
    case 'q': { LOG("q"); } break;
    case 'w': { LOG("w"); } break;
    case 'e': { LOG("e"); } break;
    case 'r': { LOG("r"); } break;
    case 't': { LOG("t"); } break;
    case 'y': { LOG("y"); } break;
    case 'u': { LOG("u"); } break;
    case 'i': { LOG("i"); } break;
    case 'o': { LOG("o"); } break;
    case 'p': { LOG("p"); } break;
    case 'a': { LOG("a"); } break;
    case 's': { LOG("s"); } break;
    case 'd': { LOG("d"); } break;
    case 'f': { LOG("f"); } break;
    case 'g': { LOG("g"); } break;
    case 'h': { LOG("h"); } break;
    case 'j': { LOG("j"); } break;
    case 'k': { LOG("k"); } break;
    case 'l': { LOG("l"); } break;
    case 'z': { LOG("z"); } break;
    case 'x': { LOG("x"); } break;
    case 'c': { LOG("c"); } break;
    case 'v': { LOG("v"); } break;
    case 'b': { LOG("b"); } break;
    case 'n': { LOG("n"); } break;
    case 'm': { LOG("m"); } break;
    case ' ': { LOG("Space"); } break;
    case KEY_RETURN: { LOG("Enter"); } break;
    case KEY_TAB: { LOG("Tab"); } break;
    case KEY_ESC: { LOG("Esc"); } break;
    case KEY_BACKSPACE: { LOG("Backspace"); } break;
    case KEY_DELETE: { LOG("Delete"); } break;
    case KEY_HOME: { LOG("Home"); } break;
    case KEY_END: { LOG("End"); } break;
    case KEY_PAGE_UP: { LOG("Page Up"); } break;
    case KEY_PAGE_DOWN: { LOG("Page Down"); } break;
    case KEY_UP_ARROW: { LOG("Up"); } break;
    case KEY_DOWN_ARROW: { LOG("Down"); } break;
    case KEY_LEFT_ARROW: { LOG("Left"); } break;
    case KEY_RIGHT_ARROW: { LOG("Right"); } break;
    case KEY_F1: { LOG("F1"); } break;
    case KEY_F2: { LOG("F2"); } break;
    case KEY_F3: { LOG("F3"); } break;
    case KEY_F4: { LOG("F4"); } break;
    case KEY_F5: { LOG("F5"); } break;
    case KEY_F6: { LOG("F6"); } break;
    case KEY_F7: { LOG("F7"); } break;
    case KEY_F8: { LOG("F8"); } break;
    case KEY_F9: { LOG("F9"); } break;
    case KEY_F10: { LOG("F10"); } break;
    case KEY_F11: { LOG("F11"); } break;
    case KEY_F12: { LOG("F12"); } break;
    default: { LOG("None"); } break;
  }
}

void printLayer(byte layer){
  switch(layer){
    case Layer0: { LOG("Layer0"); } break;
    case Layer1: { LOG("Layer1"); } break;
    case Layer2:{ LOG("Layer2"); } break;
    case Layer3: { LOG("Layer3"); } break;
    default: { LOG("Unknown Layer"); } break;
  }
}

Key keyboard[CountLayer][2][rowCount][columnCount] = {
  {
    {// normal characters
      {K('t'),       K('e'),      K('w'),   K('q'),    K(0)},
      {K(' '),       K('d'),      K('s'),     K('a'),      K(KEY_TAB)},
      {K(' ',L(Layer1)),   K('g'),      K('f'),     K('r'),      K(KEY_ESC,L(Layer2))},
      {K('.',M(KEY_LEFT_GUI)),    K('v'),      K('c'),     K('x'),      K('z')},
      {K('/',M(KEY_LEFT_SHIFT)),     K(',',M(KEY_LEFT_ALT)),  K(0),     K(0),      K(0)},
    },
    {
      {K('y'),     K('u'),      K('\''),   K(';'),    K(0)},
      {K(KEY_RETURN),     K('h'),      K('i'),     K('o'),      K('p')},
      {K(KEY_RETURN,L(Layer1)), K('j'),      K('k'),     K('l'),      K(L(Layer1),L(Layer2))},
      {K('[',M(KEY_RIGHT_CTRL)),  K('b'),      K('n'),     K('m'),      K(L(Layer0))},
      {K('\\',M(KEY_RIGHT_ALT)),   K(']',M(KEY_RIGHT_SHIFT)),  K(0),     K(0),      K(0)},
    },
  },
  { // 
    {
      {K('='),       K('3'),      K('2'),   K('1'),    K(0)},
      {K(),       K('6'),      K('5'),     K('4'),      K('`')},
      {K(' ',L(Layer1)),   K('9'),      K('8'),     K('7'),      K()},
      {K('.',M(KEY_LEFT_GUI)),    K('+'),      K('0'),     K('-'),      K()},
      {K('/',M(KEY_LEFT_SHIFT)),     K(',',M(KEY_LEFT_ALT)),  K(0),     K(0),      K(0)},
    },
    {
      {K('_'),       K('!'),      K('~'),   K(),    K(0)},
      {K(),       K('$'),      K('@'),     K('#'),      K()},
      {K(KEY_RETURN,L(Layer1)),    K('&'),      K('%'),     K('^'),      K(L(Layer2))},
      {K('[',M(KEY_RIGHT_CTRL)),    K('*'),      K('('),     K(')'),      K(L(Layer0))},
      {K('\\',M(KEY_RIGHT_ALT)),   K(']',M(KEY_RIGHT_SHIFT)),  K(0),     K(0),      K(0)},
    },
  },
  {
    {
      {K(),       K(KEY_F3),      K(KEY_F2),   K(KEY_F1),    K(0)},
      {K(),       K(KEY_F6),      K(KEY_F5),     K(KEY_F4),      K()},
      {K(KEY_BACKSPACE),   K(KEY_F9),      K(KEY_F8),     K(KEY_F7),      K()},
      {K(KEY_END),    K(KEY_F12),      K(KEY_F11),     K(KEY_F10),      K()},
      {K(KEY_HOME),     K(),  K(0),     K(0),      K(0)},
    },
    {
      {K(),       K(),      K(),   K(),    K(0)},
      {K(),       K(KEY_LEFT_ARROW),      K(KEY_UP_ARROW),     K(KEY_RIGHT_ARROW),      K()},
      {K(KEY_DELETE),   K(),      K(KEY_DOWN_ARROW),     K(),      K(L(Layer2))},
      {K(KEY_PAGE_DOWN),    K(),      K(),     K(),      K(L(Layer0))},
      {K(KEY_PAGE_UP),     K(),  K(0),     K(0),      K(0)},
    },
  }
};

int layer = Layer0;

// TODO: make sure the scenario where the queue overrides insetlf pysically impossible (if you press a hold key and mash the others very fast)
#define QUEUE_SIZE 100 
unsigned int queue_index = 1; 
unsigned int queue_last = 1; //TODO: you can probably reduce this to a short (you would have to presse 64 thousand keys without a single break to break it). maybe even byte
byte press_queue[QUEUE_SIZE];

unsigned int addToPressQueue(byte packedLoc){
  press_queue[queue_last%QUEUE_SIZE] = packedLoc;

  return queue_last++;
}
inline void resetQueue(){
  queue_index = 1;
  queue_last = 1;
  LOGLN("ResetQueue");
}

struct PressedKey{
  byte timePressed; // in x*10ms
  byte data;
  unsigned int qindex;
#define DATA_LAYER_SPACE 3
#define DATA_PRESSED_SPACE 4

  PressedKey():data(0),timePressed(0){

  }

  byte getLayer(){
    return data & ((2<<DATA_LAYER_SPACE) - 1);
  }
  bool isPressed(){
    return data & 1<<DATA_PRESSED_SPACE;
  }
  void setData(byte layer,bool pressed){
    data = layer & ((2<<DATA_LAYER_SPACE) - 1);
    data += pressed ? 1<<DATA_PRESSED_SPACE: 0;
  }

  void press(byte layer,  unsigned int qi){
    setData(layer,true);
    qindex = qi;
  }
  byte release(){

    byte layer = getLayer();
    setData(0,0);
    timePressed = 0;
    qindex = 0;
    return layer;
  }
  void addTime(unsigned long delta){

    unsigned long d = delta/10;

    if(d+timePressed>=255){
      timePressed = 255;
    }
    else{
      timePressed += d;
    }
  }
};

typedef PressedKey PK;

PressedKey pressedKeys[2][rowCount][columnCount] = {
    {
      {PK(),       PK(),      PK(),   PK(),    PK()},
      {PK(),       PK(),      PK(),     PK(),      PK()},
      {PK(),   PK(),      PK(),     PK(),      PK()},
      {PK(),    PK(),      PK(),     PK(),      PK()},
      {PK(),     PK(),  PK(),     PK(),      PK()},
    },
    {
      {PK(),     PK(),      PK(),   PK(),    PK()},
      {PK(),     PK(),      PK(),     PK(),      PK()},
      {PK(), PK(),      PK(),     PK(),      PK()},
      {PK(),  PK(),      PK(),     PK(),      PK()},
      {PK(),   PK(),  PK(),     PK(),      PK()},
    },
};

byte current_layer = Layer0;

byte layerOfPressedKeys[2][rowCount][columnCount] = {
    {
      {Layer0,       Layer0,      Layer0,   Layer0,    Layer0},
      {Layer0,       Layer0,      Layer0,     Layer0,      Layer0},
      {Layer0,   Layer0,      Layer0,     Layer0,      Layer0},
      {Layer0,    Layer0,      Layer0,     Layer0,      Layer0},
      {Layer0,     Layer0,  Layer0,     Layer0,      Layer0},
    },
    {
      {Layer0,     Layer0,      Layer0,   Layer0,    Layer0},
      {Layer0,     Layer0,      Layer0,     Layer0,      Layer0},
      {Layer0, Layer0,      Layer0,     Layer0,      Layer0},
      {Layer0,  Layer0,      Layer0,     Layer0,      Layer0},
      {Layer0,   Layer0,  Layer0,     Layer0,      Layer0},
    },
};

void pressKey(byte packedLoc, bool hold){
  Loc loc = UNPACK(packedLoc);
  LOG("Press -> packed:");
  LOG(packedLoc);
  LOG(", hold:");
  LOG(hold);
  LOG(", Loc.kbd:");
  LOG(loc.kbd);
  LOG(", Loc.row:");
  LOG(loc.row);
  LOG(", Loc.column:");
  LOG(loc.column);

  Key *key = &keyboard[current_layer][loc.kbd][loc.row][loc.column];
  
  LOG(key->mode);
  LOG(": ");

  layerOfPressedKeys[loc.kbd][loc.row][loc.column] = current_layer;

  if(key->mode == ModeNone){
    LOG("None");
  }
  else if(key->mode == ModeKey){
    printKeycode(key->keycode);

    Keyboard.press(key->keycode);
  }
  else if(key->mode == ModeKeyMod){
    printKeycode(key->keycode);
    LOG(", hold: ");
    printKeycode(key->mod.keycode);

    if(hold){
      Keyboard.press(key->mod.keycode);
    }else{
      Keyboard.press(key->keycode);
    }
  }
  else if(key->mode == ModeMod){
    printKeycode(key->mod.keycode);

    Keyboard.press(key->mod.keycode);
  }
  else if(key->mode == ModeKeyLayer){
    printKeycode(key->keycode);
    LOG(", hold: ");
    printLayer(key->layer.layer);

    if(hold){
      current_layer = key->layer.layer;
      LOG(", layer: ");
      printLayer(current_layer);
    }else{
      Keyboard.press(key->keycode);
    }
  }
  else if(key->mode == ModeLayer){
    printLayer(key->layer.layer);

    current_layer = key->layer.layer;
    LOG(", layer: ");
    printLayer(current_layer);
  }
  else if(key->mode == ModeLayerMod){
    printLayer(key->layer.layer);
    LOG(", hold: ");
    printKeycode(key->mod.keycode);

    if(hold){
      Keyboard.press(key->mod.keycode);
    }else{
      current_layer = key->layer.layer;
      LOG(", layer: ");
      printLayer(current_layer);
    }
  }
  else if(key->mode == ModeLayerLayer){
    printLayer(key->layer.layer);
    LOG(", hold: ");
    printLayer(key->layer_hold.layer);

    if(hold){
      current_layer = key->layer_hold.layer;
      LOG(", layer: ");
      printLayer(current_layer);
    }else{
      current_layer = key->layer.layer;
      LOG(", layer: ");
      printLayer(current_layer);
    }
  }
  LOGLN();
}
void releaseKey(byte packedLoc, bool hold){
  Loc loc = UNPACK(packedLoc);
  LOG("Release -> packed:");
  LOG(packedLoc);
  LOG(", hold:");
  LOG(hold);
  LOG(", Loc.kbd:");
  LOG(loc.kbd);
  LOG(", Loc.row:");
  LOG(loc.row);
  LOG(", Loc.column:");
  LOG(loc.column);

  byte key_layer = layerOfPressedKeys[loc.kbd][loc.row][loc.column];
  Key *key = &keyboard[key_layer][loc.kbd][loc.row][loc.column];
  
  LOG(key->mode);
  LOG(": ");

  if(key->mode == ModeNone){
    LOG("None");
  }
  else if(key->mode == ModeKey){
    printKeycode(key->keycode);

    Keyboard.release(key->keycode);
  }
  else if(key->mode == ModeKeyMod){
    printKeycode(key->keycode);
    LOG(", hold: ");
    printKeycode(key->mod.keycode);

    if(hold){
      Keyboard.release(key->mod.keycode);
    }else{
      Keyboard.release(key->keycode);
    }
  }
  else if(key->mode == ModeMod){
    printKeycode(key->mod.keycode);

    Keyboard.release(key->mod.keycode);
  }
  else if(key->mode == ModeKeyLayer){
    printKeycode(key->keycode);
    LOG(", hold: ");
    printLayer(key->layer.layer);

    if(hold){
      current_layer = key_layer;
      LOG(", layer: ");
      printLayer(current_layer);

    }else{
      Keyboard.release(key->keycode);
    }
  }
  else if(key->mode == ModeLayer){
    printLayer(key->layer.layer);
  }
  else if(key->mode == ModeLayerMod){
    printLayer(key->layer.layer);
    LOG(", hold: ");
    printKeycode(key->mod.keycode);
    if(hold){
      Keyboard.release(key->mod.keycode);
    }
  }
  else if(key->mode == ModeLayerLayer){
    printLayer(key->layer.layer);
    LOG(", hold: ");
    printLayer(key->layer_hold.layer);

    if(hold){
      current_layer = key_layer;
      LOG(", layer: ");
      printLayer(current_layer);
    }
  }
  LOGLN();
}

inline bool processPressCurrent(){
  byte i = queue_index % QUEUE_SIZE;
  byte packedLoc = press_queue[i];
  Loc loc = UNPACK(packedLoc);
  PressedKey* pkey = &pressedKeys[loc.kbd][loc.row][loc.column];
  Key* key = &keyboard[current_layer][loc.kbd][loc.row][loc.column];
  if(pkey->qindex!=queue_index){
    // already released
    pressKey(packedLoc, false);
    releaseKey(packedLoc, false);
    return true;
  }

  bool has_hold = key->mode == ModeKeyMod || key->mode == ModeKeyLayer || key->mode == ModeLayerMod || key->mode == ModeLayerLayer;
  
  if(has_hold){
    if(pkey->timePressed>=HOLD_TIME){
      pressKey(packedLoc,true);
      return true;
    }
  }
  else{
    pressKey(packedLoc,false);
    return true;
  }

  return false;
}

void processPressQueue(){
  while(queue_index<queue_last){ //TODO: see if adding a small delay would be better (if the Keybaord breaks if i just spam?)
    if(!processPressCurrent()){
      break;
    }
    queue_index++;
  }
}






void processKeyEvent(byte packedLoc,bool pressed){

  Loc loc = UNPACK(packedLoc);

  PressedKey* keyp = &pressedKeys[loc.kbd][loc.row][loc.column];

  if(pressed){
    LOG("Press ");
    LOG(packedLoc);
    LOGLN("");

    unsigned int qi = addToPressQueue(packedLoc);
    keyp->press(0,qi);
  }
  else{
    LOG("Release \"");
    LOG(packedLoc);
    LOGLN("\"");

    // if already processed by pressQueue release key
    if(keyp->qindex < queue_index){
      // how can i know if this was a hold?
      byte key_layer = layerOfPressedKeys[loc.kbd][loc.row][loc.column];
      Key* key = &keyboard[key_layer][loc.kbd][loc.row][loc.column];
      bool has_hold = key->mode == ModeKeyMod || key->mode == ModeKeyLayer || key->mode == ModeLayerMod || key->mode == ModeLayerLayer;
      releaseKey(packedLoc, has_hold);
    }
    keyp->release();
  }
}

#define DELAY 10


unsigned long last_millis=0;
void updatePressed(){

  unsigned long new_millis = millis();
  unsigned long delta = new_millis - last_millis;
  last_millis = new_millis;

  bool is_pressed = false;
  for(int i=0;i<2;++i){
    for(int j=0;j<rowCount;++j){
      for(int k=0;k<columnCount;++k){
        PressedKey *pkey = &pressedKeys[i][j][k];
        if(pkey->isPressed()){
          pkey->addTime(delta);
          is_pressed = true;
        }
      }
    }
  }

  if(!is_pressed&&queue_last>1&&queue_index==queue_last){
    resetQueue();
  }
}

void loop()
{
  readMatrix();
  delay(DELAY);
  updatePressed();
  processPressQueue();

}
