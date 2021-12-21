/*
 *ESP32 Bluetooth  
 *Author : gudang robot
 *20 Desember 2021
*/
#include <EEPROM.h>
#include "BluetoothSerial.h"
#define RXD2 16
#define TXD2 17

//Simpan data serial pada eeprom, njagani jika alat mati
void EEPROM_put(char add, String data);
String EEPROM_get(char add);
String msgeeprom;

// Check if Bluetooth configs are enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

// GPIO where LED is connected to
const int ledPin =  2;

// Handle received and sent messages
String message = "";
String message_1 = "";
char incomingChar;
int flag = 0;

unsigned long previousMillis = 0;  
const long interval = 5000;         

void setup() {
  EEPROM.begin(512);
  pinMode(ledPin, OUTPUT);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.begin(115200);
  // Bluetooth device name
  SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();
  // Send temperature readings
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    SerialBT.println("Hello Im esp32");
    if(flag == 0)
    {
      Serial.println(message_1); 
      Serial2.println(message_1);
      message_1 = msgeeprom;
      flag = 1;
    }
    // Jika mengaktifkan eeprom maka data akan dikirim terus ke stm 5 detik sekali, njagani alat mati
    String savedata;
    savedata = EEPROM_get(10);
    Serial.println(savedata);
    Serial2.println(savedata);
  }
  // Read received messages (LED control command)
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else
    {
      message = "";
    }
    Serial.write(incomingChar); 
  }
  
  // Check received message and control output accordingly
  if (message.length()>=10){
    digitalWrite(ledPin, HIGH);
    Serial.println(message.length());
    Serial.println("Data berhasil masuk - 1");
    message_1 = message;
    msgeeprom = message_1;
    EEPROM_put(10, msgeeprom);
    msgeeprom = "";
    flag = 0;
  }
  else if (message.length()<20&&message.length()>1){
    digitalWrite(ledPin, LOW);
    Serial.println(message.length());
    Serial.println("Data berhasil masuk - 2");
    message_1 = message;
    flag = 0;
  }
  delay(20);
}

void EEPROM_put(char add, String data)
{
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++)
  {
    EEPROM.write(add + i, data[i]);
  }
  EEPROM.write(add + _size, '\0'); //Add termination null character for String Data
  EEPROM.commit();
}
String EEPROM_get(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500) //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}
