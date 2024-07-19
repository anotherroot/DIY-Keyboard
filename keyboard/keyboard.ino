#include <Keyboard.h> 
#include <Wire.h>

byte rows[] = { 8, 7, 6, 5, 4};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// byte columns[] = {9,10,16,14,15};
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
        processKeyEvent(0, rowIndex, colIndex, state);
      }
      matrix[rowIndex][colIndex] = state;
			pinMode(rowCol, INPUT);
		}
		// disable the column
		pinMode(curCol, INPUT);
	}
}

void loop()
{
  readMatrix();
  delay(10);
}


// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  int row = Wire.read();
  int column = Wire.read();
  bool pressed = !Wire.read();
  processKeyEvent(1,row,column,pressed);
}

const byte Layer0 = 0;
const byte Layer1 = 1;
const byte Layer2 = 2;
const byte CountLayer = 4;

const byte ModeNone=0;
const byte ModeKey=1;
const byte ModeKeyMod=2;
const byte ModeMod=3;
const byte ModeKeyLayer=4;
const byte ModeLayer=5;
const byte ModeLayerMod=6;

struct Layer {
  char* name;
  byte layer;
  Layer(){
  }
  Layer(char name[6], int layer):name(name),layer(layer){
  }
};

struct Mod {
  char* name;
  byte mod;
  Mod(){
  }
  Mod(char name[6],short mod):name(name),mod(mod){
  }
};

struct Key{
  byte mode;
  char* name;
  byte keycode;
  Mod mod;
  Layer layer;
  Key():name("None"),mode(ModeNone){
  }
  Key(char name[6], byte keyCode)
    :name(name),keycode(keycode),mode(ModeKey){
  }
  Key(char name[6], byte keyCode, Mod mod)
    :name(name),keycode(keycode),mod(mod),mode(ModeKeyMod){
  }
  Key(char name[6], Mod mod)
    :name(name),mod(mod),mode(ModeMod){
  }
  Key(char name[6], byte keyCode, Layer layer)
    :name(name),keycode(keycode),layer(layer),mode(ModeKeyLayer){
  }
  Key(char name[6], Layer layer)
    :name(name),layer(layer),mode(ModeLayer){
  }
  Key(char name[6], Layer layer, Mod mod)
    :name(name),layer(layer),mode(ModeLayerMod){
  }
};

typedef Key K;
typedef Mod M;
typedef Layer L;

#define K_CTRL "Ctrl",KEY_LEFT_CTRL
#define K_SHIFT "Shift",KEY_LEFT_SHIFT
#define K_ALT "Alt",KEY_LEFT_ALT
#define K_WIN "Win",KEY_LEFT_GUI

#define M_CTRL Mod("Ctrl",KEY_LEFT_CTRL)
#define M_SHIFT Mod("Shift",KEY_LEFT_SHIFT)
#define M_ALT Mod("Alt",KEY_LEFT_ALT)
#define M_WIN Mod("Win",KEY_LEFT_GUI)

#define K_q "q",'q'
#define K_w "w",'w'
#define K_e "e",'e'
#define K_r "r",'r'
#define K_t "t",'t'
#define K_y "y",'y'
#define K_u "u",'u'
#define K_i "i",'i'
#define K_o "o",'o'
#define K_p "p",'p'
#define K_a "a",'a'
#define K_s "s",'s'
#define K_d "d",'d'
#define K_f "f",'f'
#define K_g "g",'g'
#define K_h "h",'h'
#define K_j "j",'j'
#define K_k "k",'k'
#define K_l "l",'l'
#define K_z "z",'z'
#define K_x "x",'x'
#define K_c "c",'c'
#define K_v "v",'v'
#define K_b "b",'b'
#define K_n "n",'n'
#define K_m "m",'m'

#define K_space "Space",' '
#define K_enter "Enter",KEY_RETURN
#define K_tab "Tab",KEY_TAB
#define K_esc "Esc",KEY_ESC

Key keyboard[CountLayer][2][rowCount][columnCount] = {
  {
    {
      {K(K_t),       K(K_r),      K(K_tab),   K(K_esc,M_CTRL),    K(K_i)},
      {K("L2", Layer2),       K(K_g),      K(K_e),     K(K_w),      K(K_q)},
      {K(K_space),   K(K_f),      K(K_d),     K(K_s),      K(K_a)},
      {K(K_CTRL),    K(K_i),      K(K_i),     K(K_i),      K(K_WIN)},
      {K(K_ALT),     K(K_SHIFT),  K(K_i),     K(K_i),      K(K_i)},
    },
    {
      {K(K_t),     K(K_r),      K(K_tab),   K(K_esc),    K(K_i)},
      {K("L1", Layer1),     K(K_g),      K(K_e),     K(K_w),      K(K_q)},
      {K(K_enter), K(K_f),      K(K_d),     K(K_s),      K(K_a)},
      {K(K_CTRL),  K(K_i),      K(K_i),     K(K_i),      K(K_WIN)},
      {K(K_ALT),   K(K_SHIFT),  K(K_i),     K(K_i),      K(K_i)},
    },
  },
  {
    {
      {K(),       K(),      K(),   K(),    K()},
      {K("L0", Layer0),       K(),      K(),     K(),      K()},
      {K(),   K(),      K(),     K(),      K()},
      {K(),    K(),      K(),     K(),      K()},
      {K(),     K(),  K(),     K(),      K()},
    },
    {
      {K(),     K(),      K(),   K(),    K()},
      {K("L2", Layer2),     K(),      K(),     K(),      K()},
      {K(), K(),      K(),     K(),      K()},
      {K(),  K(),      K(),     K(),      K()},
      {K(),   K(),  K(),     K(),      K()},
    },
  },
  {
    {
      {K(),       K(),      K(),   K(),    K()},
      {K("L1", Layer1),       K(),      K(),     K(),      K()},
      {K(),   K(),      K(),     K(),      K()},
      {K(),    K(),      K(),     K(),      K()},
      {K(),     K(),  K(),     K(),      K()},
    },
    {
      {K(),     K(),      K(),   K(),    K()},
      {K("L0", Layer0),     K(),      K(),     K(),      K()},
      {K(), K(),      K(),     K(),      K()},
      {K(),  K(),      K(),     K(),      K()},
      {K(),   K(),  K(),     K(),      K()},
    },
  }
};

int layer = Layer0;


void processKeyEvent(int kbd,int row, int column,bool pressed){
  Serial.print("kbd:");
  Serial.print(kbd);
  Serial.print(", row:");
  Serial.print(row);
  Serial.print(", column:");
  Serial.print(column);
  if(pressed){
    Serial.print(", p -> ");
  }
  else{
    Serial.print(", r -> ");
  }

  Key *key = &keyboard[layer][kbd][row][column];

  if(key->mode == ModeNone){
    Serial.print(key->name);
  }
  else if(key->mode == ModeKey){
    Serial.print(key->name);
  }
  else if(key->mode == ModeKeyMod){
    Serial.print(key->name);
    Serial.print(", hold: ");
    Serial.print(key->mod.name);
  }
  else if(key->mode == ModeMod){
    Serial.print(key->mod.name);
  }
  else if(key->mode == ModeKeyLayer){
    Serial.print(key->name);
    Serial.print(", hold: ");
    Serial.print(key->layer.name);
  }
  else if(key->mode == ModeLayer){
    Serial.print(key->layer.name);
  }
  else if(key->mode == ModeLayerMod){
    Serial.print(key->layer.name);
    Serial.print(", hold: ");
    Serial.print(key->mod.name);
  }

  Serial.println();          // new line for readability

  // if(pressed){
  //   Keyboard.press(keyboard[kbd][row][column]);
  // }
  // else{
  //   Keyboard.release(keyboard[kbd][row][column]);
  // }

}
