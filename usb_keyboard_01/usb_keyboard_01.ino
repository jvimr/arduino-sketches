/* Arduino USB Keyboard HID demo
 * Volume+/Volume-/Mute keys
 */
 
uint8_t buf[8] = { 
  0 }; 	/* Keyboard report buffer */
 
//digital pin 
#define PIN_BTN_ENTER 12
#define PIN_BTN_ESC 10
#define PIN_BTN_MENU 11

// analog pins
#define PIN_UP_DN 0
#define PIN_LR 1
 
int state = 1;
 
void setup() 
{
  Serial.begin(9600);
  pinMode(PIN_BTN_ENTER, INPUT);
  pinMode(PIN_BTN_ESC, INPUT);
  pinMode(PIN_BTN_MENU, INPUT);  

  // enable internal pull-ups
  digitalWrite(PIN_BTN_ENTER, 1); 
  digitalWrite(PIN_BTN_ESC, 1); 
  digitalWrite(PIN_BTN_MENU, 1);   

 
  delay(200);
}
 
void loop() 
{
  state = digitalRead(PIN_BTN_ENTER);
  if (state != 1) {
    sendKey(0, 0x28);
  } 
  
  state = digitalRead(PIN_BTN_ESC);
  if (state != 1) {
    sendKey(0, 0x2A);
  } 
  state = digitalRead(PIN_BTN_MENU);
  if (state != 1) {
    sendKey(0, 0x06);//C = context menu
  }   
 
  state = analogRead(PIN_UP_DN);
  if(state < 100){
     sendKey(0, 0x52);//upArrow
  }
  if(state > 950){
     sendKey(0, 0x51);//downArrow
  }

  state = analogRead(PIN_LR);
  if(state < 100){
     sendKey(0, 0x50);//leftArrow
  }
  if(state > 950){
     sendKey(0, 0x4F);//rightArrow
  }


}

void sendKey(int code1, int code2){
  buf[0] = code1;
  buf[2] = code2;	  
  Serial.write(buf, 8);	// Send keypress
  releaseKey();
}
 
void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);	// Release key  
   delay(400);
}
