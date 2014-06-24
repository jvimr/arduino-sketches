

//nacteni tepoty z DTH11 a zobrazeni na LCD shieldu


//  puvodni priklad:
//    FILE: dht_test.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.07
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// Released to the public domain
//
#define PROG_VERSION "1.0.0"
#define PROG_NAME "LCD_TEMP_HUM"
#include <dht.h>

dht DHT;

//dht 11 je zapojen na port 12 (data) a 13 (napajeni, proto je nastaven na high)
#define DHT11_PIN 12
#define DHT_GND_PIN 0
#define DHT_PLUS_PIN 13

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidCrystal lcd(8,9,4,5,6,7); 
//LiquidCrystal lcd(8,9,2,3,4,5); 
//minimalisticke zapojeni lcd data jsou jen na konektorech 4-9, t.j. spotrebujeme pouze 2 kabely po 4 zilach
LiquidCrystal lcd(8,9,4,5,6,7); 

void setup()
{
  lcd.begin(16, 2);

  pinMode(DHT11_PIN, INPUT); 
  //slouzi jako napajeni DHT11 - abych moh pouzit spolecnej konektor
  pinMode(DHT_PLUS_PIN, OUTPUT); 
  digitalWrite(DHT_PLUS_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  //lcd.print("DhtLib.v.:");

 // lcd.print(DHT_LIB_VERSION);

  //vypiseme jmeno programu, abysme vedeli co je v arduinu nahrany ve chvili kdy ho zapneme ;-)
  lcd.print(PROG_NAME);
  lcd.setCursor(0, 1);
  lcd.print("v.:");
  lcd.print(PROG_VERSION);
  delay(1000);

  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  //lcd.print("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
   delay(1000);
 
  // READ DATA
 
  int chk = DHT.read11(DHT11_PIN);
  int ok = 1;
  switch (chk)
  {
    case DHTLIB_OK:  
		//Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		lcd.setCursor(0, 0);
                 lcd.print("Checksum error"); 
                 ok = 0;
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		lcd.setCursor(0, 0);
               lcd.print("Time out error"); 
               ok = 0;
		break;
    default: 
		lcd.setCursor(0, 0);
                 lcd.print("Unknown error"); 
                 ok = 0;
		break;
  }
  if(ok == 1){
 // DISPLAY DATA
 lcd.clear();
 lcd.setCursor(0, 0);

  lcd.print(DHT.temperature,1);
  lcd.print(" *C"); 


  lcd.setCursor(0, 1);
  lcd.print(DHT.humidity,1);
 lcd.print(" %");
  }

}
//
// END OF FILE
//

