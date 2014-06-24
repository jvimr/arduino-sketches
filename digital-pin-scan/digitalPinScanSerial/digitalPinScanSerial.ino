/*
  scanuje digit. porty a vypisuje zmeny
  pozor- nezapojene piny maji nezname hodnoty.....
 */


const int ledPin =  13;      // the number of the LED pin
const int FIRST_INPUT_PIN = 2;
const int LAST_INPUT_PIN = 12;
int i =FIRST_INPUT_PIN;
int index;
int buttonState = 0;         // variable for reading the pushbutton status

int last_val[LAST_INPUT_PIN-FIRST_INPUT_PIN+1]; 



// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);   
  
  Serial.println("setup()");
  
  for( index = FIRST_INPUT_PIN ; index <= LAST_INPUT_PIN ; index++ ){
    
    i = index - FIRST_INPUT_PIN;
    pinMode(i, INPUT);
    
    last_val[i] = digitalRead(i);
    print_val(0, last_val[i], index);
  }
  
  
  Serial.println("setup done");

}

// the loop routine runs over and over again forever:
void loop() {
  
  
  for( index = FIRST_INPUT_PIN ; index <= LAST_INPUT_PIN ; index++ ){
    
    i = index - FIRST_INPUT_PIN;
     buttonState = digitalRead(i);
     
     if(buttonState != last_val[i]){
       print_val(last_val[i], buttonState, index);
       last_val[i] = buttonState;
       digitalWrite(ledPin, buttonState); 
     }
  }
  // read the input pin:


  delay(10);        // delay in between reads for stability
}


void print_val(int previous, int current, int index){
        Serial.print("D");
        Serial.print(index);
        Serial.print(" ");
        Serial.print(previous);
        Serial.print(" => ");
        Serial.println(current);
}



