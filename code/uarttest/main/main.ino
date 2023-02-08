/*
  Name:    getVescValues.ino
  Created: 19-08-2018
  Author:  SolidGeek
  Description:  This example is made using a Arduino Micro (Atmega32u4) that has a HardwareSerial port (Serial1) seperated from the Serial port. 
                A Arduino Nano or Uno that only has one Serial port will not be able to display the data returned.
*/

// Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VescUart.h>
#include <SimpleKalmanFilter.h>


// Variablen
#define RXD2 16
#define TXD2 17


int rpm;
float voltage;
float current;
int power;
float amphour;
float tach;
float distance;
float velocity;
float watthour;
float batpercentage;


/* Setup */
VescUart UART;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4); //GPIO Pins standardmäßig 21 und 22


// Werte glätten
SimpleKalmanFilter Filter1(2, 2, 0.01);

// Zeichen fürs Display

byte ph[] = {
  B11100,
  B10100,
  B11100,
  B10000,
  B10101,
  B00111,
  B00101,
  B00101
};

byte mi[] = {
  B10001,
  B11011,
  B10101,
  B10001,
  B00100,
  B00000,
  B00100,
  B00110
};

byte km[] = {
  B01000,
  B01010,
  B01100,
  B01010,
  B10001,
  B11011,
  B10101,
  B10001
};

byte ah[] = {
  B00100,
  B01010,
  B01110,
  B01010,
  B00000,
  B01010,
  B01110,
  B01010
};
byte percent[] = {
  B11001,
  B11001,
  B00010,
  B00100,
  B01000,
  B10011,
  B10011,
  B00000
};

byte m[] = {
  B10001,
  B11011,
  B10101,
  B10001,
  B10001,
  B00000,
  B00000,
  B00000
};

byte k[] = {
  B01000,
  B01010,
  B01100,
  B01010,
  B01010,
  B00000,
  B00000,
  B00000
};

byte bat[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111
};

byte sprk[] = {
  B00000,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B00100,
  B00010
};

void setup() {

  /* Setup Serial port to display data */
  Serial.begin(115200);
  Serial.println("gebootet.");

  /* Setup UART port - 0 USB bzw. Standardseriell; 1 standardmäßig SD-Karte; 2 Standard-UART Port via GPIO 16 und 17 für RX und TX */
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  
  // Warten bis seriell gestartet
  while (!Serial) {;}

  /* Define which ports to use as UART */
  UART.setSerialPort(&Serial2);
  
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(9,0);
  lcd.print("Elmoto HR-2");          // Startup screen (can't be more than 10 characters)
  lcd.createChar(5, bat);
  lcd.setCursor(1,1);
  lcd.write(5);
  lcd.createChar(6, bat);
  lcd.setCursor(14,1);
  lcd.write(6);
  lcd.createChar(7, sprk);
  lcd.setCursor(1,0);
  lcd.write(7);
  lcd.createChar(8, sprk);
  lcd.setCursor(14,0);
  lcd.write(8);

  delay(2000);
  lcd.clear();


}

void loop() {
  
  /** Call the function getVescValues() to acquire data from VESC */
  if ( UART.getVescValues() ) {


    rpm = (UART.data.rpm);
    voltage = (UART.data.inpVoltage);
    current = (UART.data.avgInputCurrent);
    power = voltage*current;
    amphour = (UART.data.ampHours);
    watthour = amphour*voltage;
    tach = (UART.data.tachometerAbs)/42;                   // The '42' is the number of motor poles multiplied by 3
    distance = tach*3.142*(1/1000)*0.6096*(16/185);          // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter x (motor pulley / wheelpulley)
    velocity = rpm*3.142*(60/1000)*0.6096*(16/185);          // Motor RPM x Pi x (seconds in a minute / meters in a mile) x Wheel diameter x (motor pulley / wheelpulley)
    batpercentage = ((voltage-39)/13)*100;               // ((Battery voltage - minimum voltage) / number of cells) x 100

    float powerfiltered = Filter1.updateEstimate(power);
    float rpmfiltered = Filter1.updateEstimate(rpm);

    Serial.println(UART.data.rpm);
    Serial.println(UART.data.inpVoltage);
    Serial.println(UART.data.ampHours);
    Serial.println(UART.data.tachometerAbs);
    
    if(velocity < 10){
      lcd.setCursor(1,0);
      lcd.print(" ");
      lcd.print(velocity, 0);
      }
      else{
      lcd.setCursor(1,0);
      lcd.print(velocity, 0);
      }

    lcd.createChar(4, k);          // Change 'm' to 'k' for kilometers
    lcd.setCursor(3,0);
    lcd.write(4);
    lcd.createChar(0, ph);
    lcd.setCursor(4,0);
    lcd.write(0);

    lcd.setCursor(7,0);
    lcd.print(voltage,0);
    lcd.setCursor(9,0);
    lcd.print("V");
    
    lcd.setCursor(11,0);
    lcd.print(distance, 2);
    lcd.createChar(1, km);          // Change 'mi' to 'km' for kilometers
    lcd.setCursor(15,0);
    lcd.write(1);

    if(powerfiltered < 10){
      lcd.setCursor(0,1);
      lcd.print("   ");
      lcd.print(powerfiltered, 0);
    }
    if(powerfiltered > 10 && powerfiltered < 100){
      lcd.setCursor(0,1);
      lcd.print("  ");
      lcd.print(powerfiltered, 0);
    }
    if(powerfiltered > 100 && powerfiltered < 1000){
      lcd.setCursor(0,1);
      lcd.print(" ");
      lcd.print(powerfiltered, 0);
    }
    if(powerfiltered > 1000){
      lcd.setCursor(0,1);
      lcd.print(powerfiltered, 0);
    }
      lcd.setCursor(4,1);
      lcd.print("W");
    
    if(batpercentage < 100){
      lcd.setCursor(7,1);
      lcd.print(batpercentage,0);
    }
    else{
      lcd.setCursor(6,1);
      lcd.print(batpercentage,0);
    }
    lcd.createChar(3, percent);
    lcd.setCursor(9,1);
    lcd.write(3);
  
    lcd.setCursor(11,1);
    lcd.print(amphour, 2);
    lcd.createChar(2, ah);
    lcd.setCursor(15,1);
    lcd.write(2);
    
    lcd.println();
  }
  else
  {
    lcd.println("Nicht angeschlossen");
  }

  delay(50);
}