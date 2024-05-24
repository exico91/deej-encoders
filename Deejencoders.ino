#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
// Set number of encoders
const int NUM_SLIDERS = 5;
// Set RGB PIXELS if present
#define NUMPIXELS 5
// and the pin
#define RGB_PIN 21

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
int curr_encoder[NUM_SLIDERS] = {512,512,512,512,512};
int prev_encoder[NUM_SLIDERS] = {512,512,512,512,512};
int volume[NUM_SLIDERS];
bool mute[NUM_SLIDERS] = {false, false, false, false, false};

//Set the encoders pins

Encoder knobMaster(10,9);
Encoder knobDiscord(6,7);
Encoder knobChrome(16,14);
Encoder knobGames(3,4);
Encoder knobMusic(18,19);

//Set the encoders buttons pins

const int Master_button = 8;
const int Discord_button = 5;
const int Chrome_button = 15;
const int Games_button = 2;
const int Music_button = 20;

// volume steps

const int increment[NUM_SLIDERS] = {4,4,2,1,4};

// starting volume

int startingVolume[NUM_SLIDERS] = {90,80,20,20,20};

// dont touch, other variables

bool masterMute, discordMute, chromeMute, gamingMute, musicMute;

const int min_volume = 0;
const int max_volume = 1023;
const int buttons_pins[NUM_SLIDERS] = {Master_button, Discord_button, Chrome_button, Games_button, Music_button};


void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);

pinMode(Master_button, INPUT_PULLUP);
pinMode(Discord_button, INPUT_PULLUP);
pinMode(Chrome_button, INPUT_PULLUP);
pinMode(Games_button, INPUT_PULLUP);
pinMode(Music_button, INPUT_PULLUP);

// set the initial position of the knobs

volume[0] = max_volume / 100 * startingVolume[0];
volume[1] = max_volume / 100 * startingVolume[1];
volume[2] = max_volume / 100 * startingVolume[2];
volume[3] = max_volume / 100 * startingVolume[3];
volume[4] = max_volume / 100 * startingVolume[4];


knobDiscord.write(512);
knobMaster.write(512);
knobMusic.write(512);
knobChrome.write(512);
knobGames.write(512);

pixels.begin();
pixels.clear(); 


}

void loop() {
  // put your main code here, to run repeatedly:

curr_encoder[0] = knobMaster.read();
curr_encoder[1] = knobDiscord.read();
curr_encoder[2] = knobChrome.read();
curr_encoder[3] = knobGames.read();
curr_encoder[4] = knobMusic.read();




for (int i = 0; i < NUM_SLIDERS; i++) {

// get encoders movement and set the volume
volume[i] = CheckEncoders(i);
//check if volume is within boundaries
if (volume[i] >= max_volume) {
  volume[i] = max_volume;
} else if (volume[i] <= min_volume) {
  volume[i] = min_volume;
}

// Check if mute buttons are pressed
CheckButtons(i);

}


// Build the serial string and send it
String builtString = String("");

for (int i = 0; i < NUM_SLIDERS; i++) {
  String actualvolume = String(volume[i]);
  if (mute[i] == true) {
    actualvolume = String(0);
  }
  builtString = builtString + String(actualvolume);
  if (i < NUM_SLIDERS - 1) {
    builtString += String("|");
  }
  actualvolume = "0";
  
}

Serial.println(builtString);


//delay(200);
}

int CheckEncoders(int knob_index) {
  if (curr_encoder[knob_index] > prev_encoder[knob_index]) {
    prev_encoder[knob_index] = curr_encoder[knob_index];
    return volume[knob_index] + increment[knob_index];
  }
  else if (curr_encoder[knob_index] < prev_encoder[knob_index]) {
    prev_encoder[knob_index] = curr_encoder[knob_index];
    return volume[knob_index] - increment[knob_index];
  } else {
    return volume[knob_index];
  }
  
}

void CheckButtons(int knob_index) {
  if (digitalRead(buttons_pins[knob_index]) == LOW && mute[knob_index] == false) {
    mute[knob_index] = true;
    delay(100);
  }
  else if (digitalRead(buttons_pins[knob_index]) == LOW && mute[knob_index] == true) {
    mute[knob_index] = false;
    delay(100);
  }
}

