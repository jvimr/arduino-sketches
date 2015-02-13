

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
#define PROG_VERSION "1.0.3"
#define PROG_NAME "LCD_TEMP_HUM"
#include <OneWire.h>

//dht DHT;

#define IS_MEGA
//#define IS_UNO
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
#define DHT11_PIN 12
#define DHT_GND_PIN 0
#define DHT_PLUS_PIN 13

#endif

OneWire  ds(DHT11_PIN); 

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

float getTemp(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return -99.0;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -99.0;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return -99.0;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  return celsius;
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


float temp = -99.0;

void loop()
{
//   delay(1000);
 
  // READ DATA
  float tmp = getTemp();
  if(tmp != -99.0){
    temp = tmp;
    
  }
 
 
  int ok = 1;
  
  
  
 // DISPLAY DATA
 getTime();
 lcd.clear();
 lcd.setCursor(0, 0);
  

  
  lcd.print(" ");
  lcd.print(time);



  if(temp != -99.0){
    lcd.setCursor(0, 1);
    lcd.print(temp,1);
    lcd.print(" *C"); 
  }

}
//
// END OF FILE
//

