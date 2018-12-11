#include <Adafruit_NeoPixel.h>


#define PIN 6
char receivedChar;
boolean effectActive = false;
int r = 1;
int rCount = 0;
int hashCount = 0;
String hashString = "";
String effect = "";
int redValue;
int greenValue;
int blueValue;
int speedValue;
String id = "s255255255001";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);


void setup() {

  Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if(Serial.available() > 0){
    r = Serial.read();
    if(r == 36){
      rCount += 1;
    }
    if(rCount > 1){
      //Serial.println(id);
      setByteData(id);
      createEffects();
      effectActive = true;
      rCount = 0;
      hashCount = 0;
      id = "";
    }
    if(r != 36){
        id += ConvertToString(String(r));
    }
    
    
  
  }else if(effectActive){
    createEffects();  
  }
  
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
  //theaterChaseRainbow(50);
}
void createEffects(){
    if(effect == "r"){
      rainbowCycle(1);
    } 
    else if(effect == "s"){
       colorSolid(strip.Color(redValue, greenValue, blueValue), 50);
    }
    else if(effect == "c"){
       colorWipe(strip.Color(redValue, greenValue, blueValue), 50);  
    }else if(effect == "t"){
       theaterChase(strip.Color(redValue, greenValue, blueValue), 50);
    }
}

void setByteData(String s){
  effect = s[0];
  Serial.println("effect: " + effect);  

  redValue = s.substring(1,4).toInt();
  Serial.println("red: " + String(redValue));
  greenValue = s.substring(4,7).toInt();
  Serial.println("green: " + String(greenValue));

  blueValue = s.substring(7,10).toInt();
  Serial.println("blue: " + String(blueValue));
  speedValue = s.substring(10,13).toInt();
  Serial.println("speed: " + String(speedValue));
}

String ConvertToString(String s){
    int i = s.toInt();    
    char c = char(i);
    s = String(c);
    return s;
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colorSolid(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
