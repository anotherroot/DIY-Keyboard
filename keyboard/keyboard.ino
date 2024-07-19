#include <Keyboard.h> 
#include <Wire.h>

#define PACK(kbd,row,column) (kbd*100+row*10+column)
#define UNPACK(packed) Loc(packed/100,(packed%100)/10,packed%10)
#define HOLD_TIME 30

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
		Serial.print(rows[x]); Serial.println(" as input");
		pinMode(rows[x], INPUT);
	}

	for (int x=0; x<columnCount; x++) {
		Serial.print(columns[x]); Serial.println(" as input-pullup");
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
    :layer(layer),mode(ModeLayerMod){
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
    case KEY_LEFT_CTRL: { Serial.print("Ctrl"); } break;
    case KEY_LEFT_SHIFT: { Serial.print("Shift"); } break;
    case KEY_LEFT_ALT: { Serial.print("Alt"); } break;
    case KEY_LEFT_GUI: { Serial.print("Win"); } break;
    case KEY_RIGHT_CTRL: { Serial.print("Ctrl"); } break;
    case KEY_RIGHT_SHIFT: { Serial.print("Shift"); } break;
    case KEY_RIGHT_ALT: { Serial.print("Alt"); } break;
    case KEY_RIGHT_GUI: { Serial.print("Win"); } break;
    case 'q': { Serial.print("q"); } break;
    case 'w': { Serial.print("w"); } break;
    case 'e': { Serial.print("e"); } break;
    case 'r': { Serial.print("r"); } break;
    case 't': { Serial.print("t"); } break;
    case 'y': { Serial.print("y"); } break;
    case 'u': { Serial.print("u"); } break;
    case 'i': { Serial.print("i"); } break;
    case 'o': { Serial.print("o"); } break;
    case 'p': { Serial.print("p"); } break;
    case 'a': { Serial.print("a"); } break;
    case 's': { Serial.print("s"); } break;
    case 'd': { Serial.print("d"); } break;
    case 'f': { Serial.print("f"); } break;
    case 'g': { Serial.print("g"); } break;
    case 'h': { Serial.print("h"); } break;
    case 'j': { Serial.print("j"); } break;
    case 'k': { Serial.print("k"); } break;
    case 'l': { Serial.print("l"); } break;
    case 'z': { Serial.print("z"); } break;
    case 'x': { Serial.print("x"); } break;
    case 'c': { Serial.print("c"); } break;
    case 'v': { Serial.print("v"); } break;
    case 'b': { Serial.print("b"); } break;
    case 'n': { Serial.print("n"); } break;
    case 'm': { Serial.print("m"); } break;
    case ' ': { Serial.print("Space"); } break;
    case KEY_RETURN: { Serial.print("Enter"); } break;
    case KEY_TAB: { Serial.print("Tab"); } break;
    case KEY_ESC: { Serial.print("Esc"); } break;
    default: { Serial.print("None"); } break;
  }
}

void printLayer(byte layer){
  switch(layer){
    case Layer0: { Serial.print("Layer0"); } break;
    case Layer1: { Serial.print("Layer1"); } break;
    case Layer2:{ Serial.print("Layer2"); } break;
    case Layer3: { Serial.print("Layer3"); } break;
    default: { Serial.print("Unknown Layer"); } break;
  }
}

Key keyboard[CountLayer][2][rowCount][columnCount] = {
  {
    {
      {K('t'),       K('r'),      K(KEY_TAB),   K(KEY_ESC,M(KEY_RIGHT_CTRL)),    K('i')},
      {K(Layer2),       K('g'),      K('e'),     K('w'),      K('q')},
      {K(' '),   K('f'),      K('d'),     K('s'),      K('a')},
      {K(KEY_LEFT_CTRL),    K('i'),      K('i'),     K('i'),      K(KEY_RIGHT_GUI)},
      {K(KEY_LEFT_ALT),     K(KEY_LEFT_SHIFT),  K('i'),     K('i'),      K('i')},
    },
    {
      {K('y'),     K('u'),      K(KEY_TAB),   K(KEY_ESC),    K('p')},
      {K(Layer1),     K('h'),      K('i'),     K('o'),      K(';')},
      {K(KEY_RETURN), K('j'),      K('k'),     K('l'),      K('a')},
      {K(KEY_RIGHT_CTRL),  K(),      K(),     K(),      K(KEY_RIGHT_GUI)},
      {K(KEY_RIGHT_ALT),   K(KEY_RIGHT_SHIFT),  K(),     K(),      K()},
    },
  },
  {
    {
      {K(),       K(),      K(),   K(),    K()},
      {K(Layer0),       K(),      K(),     K(),      K()},
      {K(),   K(),      K(),     K(),      K()},
      {K(),    K(),      K(),     K(),      K()},
      {K(),     K(),  K(),     K(),      K()},
    },
    {
      {K(),     K(),      K(),   K(),    K()},
      {K( Layer2),     K(),      K(),     K(),      K()},
      {K(), K(),      K(),     K(),      K()},
      {K(),  K(),      K(),     K(),      K()},
      {K(),   K(),  K(),     K(),      K()},
    },
  },
  {
    {
      {K(),       K(),      K(),   K(),    K()},
      {K(Layer1),       K(),      K(),     K(),      K()},
      {K(),   K(),      K(),     K(),      K()},
      {K(),    K(),      K(),     K(),      K()},
      {K(),     K(),  K(),     K(),      K()},
    },
    {
      {K(),     K(),      K(),   K(),    K()},
      {K(Layer0),     K(),      K(),     K(),      K()},
      {K(), K(),      K(),     K(),      K()},
      {K(),  K(),      K(),     K(),      K()},
      {K(),   K(),  K(),     K(),      K()},
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
  Serial.println("ResetQueue");
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
byte prev_layer = Layer0;


void pressKey(byte packedLoc, bool hold){
  Loc loc = UNPACK(packedLoc);
  Serial.print("Press -> packed:");
  Serial.print(packedLoc);
  Serial.print(", hold:");
  Serial.print(hold);
  Serial.print(", Loc.kbd:");
  Serial.print(loc.kbd);
  Serial.print(", Loc.row:");
  Serial.print(loc.row);
  Serial.print(", Loc.column:");
  Serial.print(loc.column);

  Key *key = &keyboard[current_layer][loc.kbd][loc.row][loc.column];
  
  Serial.print(key->mode);
  Serial.print(": ");

  if(key->mode == ModeNone){
    Serial.print("None");
  }
  else if(key->mode == ModeKey){
    printKeycode(key->keycode);

    Keyboard.press(key->keycode);
  }
  else if(key->mode == ModeKeyMod){
    printKeycode(key->keycode);
    Serial.print(", hold: ");
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
    Serial.print(", hold: ");
    printLayer(key->layer.layer);

    if(hold){
      prev_layer = current_layer;
      current_layer = key->layer.layer;
    }else{
      Keyboard.press(key->keycode);
    }
  }
  else if(key->mode == ModeLayer){
    printLayer(key->layer.layer);

    current_layer = key->layer.layer;
  }
  else if(key->mode == ModeLayerMod){
    printLayer(key->layer.layer);
    Serial.print(", hold: ");
    printKeycode(key->mod.keycode);

    if(hold){
      Keyboard.press(key->mod.keycode);
    }else{
      prev_layer = current_layer;
      current_layer = key->layer.layer;
    }
  }
  Serial.println();
}
void releaseKey(byte packedLoc, bool hold){
  Loc loc = UNPACK(packedLoc);
  Serial.print("Release -> packed:");
  Serial.print(packedLoc);
  Serial.print(", hold:");
  Serial.print(hold);
  Serial.print(", Loc.kbd:");
  Serial.print(loc.kbd);
  Serial.print(", Loc.row:");
  Serial.print(loc.row);
  Serial.print(", Loc.column:");
  Serial.print(loc.column);

  Key *key = &keyboard[current_layer][loc.kbd][loc.row][loc.column];
  
  Serial.print(key->mode);
  Serial.print(": ");

  if(key->mode == ModeNone){
    Serial.print("None");
  }
  else if(key->mode == ModeKey){
    printKeycode(key->keycode);

    Keyboard.release(key->keycode);
  }
  else if(key->mode == ModeKeyMod){
    printKeycode(key->keycode);
    Serial.print(", hold: ");
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
    Serial.print(", hold: ");
    printLayer(key->layer.layer);

    if(hold){
      current_layer = prev_layer;
    }else{
      Keyboard.release(key->keycode);
    }
  }
  else if(key->mode == ModeLayer){
    printLayer(key->layer.layer);
  }
  else if(key->mode == ModeLayerMod){
    printLayer(key->layer.layer);
    Serial.print(", hold: ");
    printKeycode(key->mod.keycode);
    if(hold){
      Keyboard.release(key->mod.keycode);
    }
  }
  Serial.println();
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

  bool has_hold = key->mode == ModeKeyMod || key->mode == ModeKeyLayer || key->mode == ModeLayerMod;
  
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
    Serial.print("Press ");
    Serial.print(packedLoc);
    Serial.println("");

    unsigned int qi = addToPressQueue(packedLoc);
    keyp->press(0,qi);
  }
  else{
    Serial.print("Release \"");
    Serial.print(packedLoc);
    Serial.println("\"");

    // if already processed by pressQueue release key
    if(keyp->qindex < queue_index){
      // how can i know if this was a hold?
      Key* key = &keyboard[current_layer][loc.kbd][loc.row][loc.column];
      bool has_hold = key->mode == ModeKeyMod || key->mode == ModeKeyLayer || key->mode == ModeLayerMod;
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
