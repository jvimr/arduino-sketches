

//nacteni tepoty z DTH11 a zobrazeni na LCD shieldu
//nacteni akt. casu z 192.168.0.42:13 a jeho zobrazeni


//  puvodni priklad:
//    FILE: dht_test.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.07
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// Released to the public domain
//
#define PROG_VERSION "1.0.2"
#define PROG_NAME "LCD_TEMP_HUM"
#include <dht.h>

dht DHT;

//#define IS_MEGA
#define IS_UNO
#ifdef IS_UNO
//UNO: 2-green-gnd, 1-white-data, 0-red-vcc (+5)
//dht 11 je zapojen na port 12 (data) a 13 (napajeni, proto je nastaven na high)
#define DHT11_PIN 3
#define DHT_GND_PIN 0
#define DHT_PLUS_PIN 0

#endif

#ifdef IS_MEGA
//MEGA: GND-green, 13-white-data, 12-red-vcc (+5)
//dht 11 je zapojen na port 12 (data) a 13 (napajeni, proto je nastaven na high)
#define DHT11_PIN 13
#define DHT_GND_PIN 0
#define DHT_PLUS_PIN 12

#endif

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidCrystal lcd(8,9,4,5,6,7); 
//LiquidCrystal lcd(8,9,2,3,4,5); 
//minimalisticke zapojeni lcd data jsou jen na konektorech 4-9, t.j. spotrebujeme pouze 2 kabely po 4 zilach
LiquidCrystal lcd(8,9,4,5,6,7); 

#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x1E, 0xF2 };
IPAddress ip(192,168,0,17);
EthernetClient client;
IPAddress server(192,168,0,42);
char time[] = "00:00:00\0";
char err[] = "ConnErr\0";

void setup()
{
  lcd.begin(16, 2);
  //vypiseme jmeno programu, abysme vedeli co je v arduinu nahrany ve chvili kdy ho zapneme ;-)
  lcd.print(PROG_NAME);
  lcd.setCursor(0, 1);
  lcd.print("v.:");
  lcd.print(PROG_VERSION);
  
 
  

  pinMode(DHT11_PIN, INPUT); 
  //slouzi jako napajeni DHT11 - abych moh pouzit spolecnej konektor
  if(DHT_PLUS_PIN != 0){
    pinMode(DHT_PLUS_PIN, OUTPUT); 
    digitalWrite(DHT_PLUS_PIN, HIGH);
  }
  if(DHT_GND_PIN != 0){
    pinMode(DHT_GND_PIN, OUTPUT); 
  
    digitalWrite(DHT_GND_PIN, LOW);
  }
  Serial.begin(115200);
  Serial.println(PROG_NAME);
  //lcd.print("DhtLib.v.:");

 // lcd.print(DHT_LIB_VERSION);

  //vypiseme jmeno programu, abysme vedeli co je v arduinu nahrany ve chvili kdy ho zapneme ;-)
 // lcd.print(PROG_NAME);
 // lcd.setCursor(0, 1);
  //lcd.print("v.:");
  //lcd.print(PROG_VERSION);
  delay(1000);
  
  Serial.println("DHCP....");
  
 // if (Ethernet.begin(mac) == 0) {
  // Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  //}
  
  
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP:");
  lcd.print(Ethernet.localIP());
  delay(500);

  Serial.println(Ethernet.localIP());
  Serial.print("Temp. sensor @");
  Serial.println(DHT11_PIN);
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  //lcd.print("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}



void getTime(){
  
  
 
  if(client.connect( server, 13) && client.connected()){
    
    delay(40);
    
    Serial.print("IP: connected to ");
    Serial.println(server);
    
    Serial.print("IP: available: ");
    Serial.println(client.available());
    
    int i = 0;
    while( i < 8 ){
      time[i] = client.read();
      i++;
    }
    
    
  }else{
    Serial.println("IP: unable to connect");
    int i = 0;
    while( i < 8 ){
      time[i] = err[i];
      i++;
    }

  }
  
  client.stop();
  
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
		Serial.print("temp OK,\n"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		lcd.setCursor(0, 0);
                lcd.print("Checksum error"); 
                 Serial.print("temp: Checksum error");
                 ok = 0;
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		lcd.setCursor(0, 0);
               lcd.print("Time out error");
                Serial.print("Temp: Time out error\n"); 
               ok = 0;
		break;
    default: 
		lcd.setCursor(0, 0);
                 lcd.print("Unknown error"); 
                 Serial.print("Temp: Unknown error: ");
                 Serial.print(chk);
                 Serial.print("\n");
                 ok = 0;
		break;
  }
  
 // DISPLAY DATA
 getTime();
 lcd.clear();
 lcd.setCursor(0, 0);

if(ok == 1){
  lcd.print(DHT.temperature,1);
  lcd.print(" *C"); 
}

  
  lcd.print(" ");
  lcd.print(time);
if(ok == 1){

  lcd.setCursor(0, 1);
  lcd.print(DHT.humidity,1);
 lcd.print(" %");
  }

}
//
// END OF FILE
//

