/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

#include <SD.h>

Sd2Card card;



// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;
 boolean sdOk= false;
 int analogPin = 2;
int digitalPin  = 3;
 
 File dataFile;
void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


 // Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  pinMode(digitalPin, INPUT);
  
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    sdOk = false;
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
   sdOk = true;
  }
  
  // see if the card is present and can be initialized:
  if (sdOk == true && !SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    sdOk = false;
    return;
  }
  sdOk = true;
  Serial.println("card initialized.");
  
  dataFile = getFile();
  delay(1000);
  
}

String getDataString()
{
  // make a string for assembling the data to log:
  String dataString = String( millis(), DEC)  + ",";

  // read three sensors and append to the string:
  //for (int analogPin = 0; analogPin < 5; analogPin++) {
    
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    dataString += ",";
    dataString += String(digitalRead(digitalPin));
    

  //}
       //dataString += "\n";  
       return dataString;
}

int fIndex = 0;
char *fn = {"                   "};
String filename = "data.csv";
File getFile(){
  File dataFile;
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  filename.toCharArray(fn, 12);
  dataFile = SD.open(fn, FILE_WRITE);
 
  if(!dataFile){
   Serial.println("error opening " + filename);
  }else{
   Serial.println("file " + filename + " opened ok"); 
   dataFile.seek(0);
  }
  return dataFile;
 
}




void loop()
{
 
  
  if(!sdOk || !dataFile){
    return;
  }
  // make a string for assembling the data to log:
  String dataString = "";
 
  

  // if the file is available, write to it:
  if (dataFile) {
    
      dataString = getDataString();
      dataFile.println(dataString);
      dataFile.flush();
    // print to the serial port too:
      Serial.println(filename + "=>" + dataString);
      
  }  else{
  // if the file isn't open, pop up an error:
    //String fn = String(filename);
    Serial.println("error opening " + filename);
    fIndex +=1;
  }
}









