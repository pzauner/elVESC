//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ezButton.h>
#include <VescUart.h>
#include <SimpleKalmanFilter.h>
#include <ESP32Time.h>

// Variablen für Display Berechnungen:
#define RXD2 16
#define TXD2 17

#define VRX_PIN  36 // ESP32 pin GIOP36 (ADC0) connected to VRX pin
#define VRY_PIN  39 // ESP32 pin GIOP39 (ADC0) connected to VRY pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define TCAADDR 0x70

#define DISPLAYADDR 1
#define BASICADDR 2
#define POWERMETERADDR 3
#define TACHOMETERADDR 4

#define LICHT_IN 32
int licht = 0; 
#define BREMSLICHT_IN 33
int bremse = 0;
#define HUPE_IN 27
int hupe = 0;

#define LICHT_OUT 19
#define RUECKLICHT_OUT 23
#define BREMSLICHT_OUT 18
#define HUPE_OUT 5

#define SELECT 1
#define VIEW 1
#define OPTION 0
#define OK 0
#define MENU 3
#define MENU 3


int valueX = 0 ; // to store the X-axis value
int valueY = 0 ; // to store the Y-axis value

// Variablen: TODO -> Speichern und versuchen zu lesen, ansonsten Standardwert nehmen 

int d = 1; // Dashboardauswahl
int f = 1; // Fahrmodiauswahl
int v = 1; // Loggingmenü (verbose)
int c = 0; // Spaltenauswahl der UI (column)

long lastDebounceTime = 0;
long debounceDelay = 100;

bool logging = false; //
String stick; // Joystickposition oder Button -> Delay beachten
float km = 0; // Tageskm
float odo = 6700; // Gesamtkilometer // Todo -> Read & Write mit Poweroff
float whkm = 15; // in Wh/km
float wh = 200; // Gesamtverbrauch seit Reset

float sport = 10; // Werte für den Motor
float sanft = 5; // Werte für den Motor


// Vom VESC:
int rpm;
float voltage;
float current;
float motorcurrent;
float tempVesc;
int power;
float amphours;
float wattHours;
float tach;
float distance;
float velocity;
float batpercentage;
float powerfiltered;
float rpmfiltered;


const unsigned char basicUI [] PROGMEM = {
	0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 
	0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 
	0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0xff, 0x00, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xff, 0x80, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc3, 0xff, 0xe0, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x03, 0xff, 0xf0, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x07, 0xff, 0xf8, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0xff, 0xfc, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x67, 0xff, 0xfc, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe7, 0xff, 0xfe, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x8f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x0f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x0f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0xcf, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xcf, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x6f, 0xff, 0xff, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0xe7, 0xff, 0xfe, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x87, 0xff, 0xfc, 
	0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0xff, 0xfc, 
	0x40, 0x01, 0x01, 0xfe, 0x3f, 0xc7, 0xf8, 0x3f, 0xc7, 0xf8, 0xff, 0x00, 0x78, 0x07, 0xff, 0xf8, 
	0x40, 0x01, 0x02, 0x01, 0x40, 0x28, 0x04, 0x40, 0x28, 0x05, 0x00, 0x81, 0xe0, 0x33, 0xff, 0xf0, 
	0x40, 0x01, 0x02, 0x01, 0x40, 0x28, 0x04, 0x40, 0x28, 0x05, 0x00, 0x81, 0x80, 0xf3, 0xff, 0xe0, 
	0x40, 0x01, 0x02, 0x01, 0x40, 0x28, 0x04, 0x40, 0x28, 0x05, 0x00, 0x80, 0x03, 0xc3, 0xff, 0x80, 
	0x40, 0x01, 0x02, 0x01, 0x40, 0x28, 0x04, 0x40, 0x28, 0x05, 0x00, 0x80, 0x0f, 0x01, 0xff, 0x00, 
	0x40, 0x01, 0x02, 0x01, 0x40, 0x28, 0x04, 0x40, 0x28, 0x05, 0x00, 0x80, 0x3c, 0x01, 0xfc, 0x00, 
	0x20, 0x02, 0x01, 0xfe, 0x3f, 0xc7, 0xf8, 0x3f, 0xc7, 0xf8, 0xff, 0x00, 0xf0, 0x00, 0xe0, 0x00, 
	0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00
};

const unsigned char powermeterUI [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3e, 0xfd, 0xfa, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7e, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbe, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x42, 0x85, 0x0a, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 0x42, 0x85, 0x0a, 0x14, 0x28, 0x50, 0xa1, 
	0x3e, 0xfd, 0xfa, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7e, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbe, 
	0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char tachometerUI [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x1d, 0xe0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x33, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x33, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x33, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x33, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x33, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char dashboard [] PROGMEM = {
	// 'Dashboard, 128x32px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xec, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x92, 0x00, 
	0x00, 0x00, 0x00, 0x49, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x92, 0x00, 
	0x00, 0x00, 0x00, 0x55, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x92, 0x00, 
	0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x92, 0x00, 
	0x00, 0x00, 0x00, 0x22, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0xd3, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x41, 0x60, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x78, 0x38, 
	0x00, 0x00, 0x00, 0x41, 0x20, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x44, 0x44, 
	0x00, 0x00, 0x00, 0x41, 0x2c, 0x09, 0x27, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x42, 0x82, 
	0x00, 0x00, 0x00, 0x41, 0x32, 0x11, 0x2c, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x42, 0x82, 
	0x00, 0x00, 0x00, 0x49, 0x22, 0x11, 0xc4, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x42, 0x82, 
	0x00, 0x00, 0x00, 0x55, 0x22, 0x21, 0x44, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x42, 0x82, 
	0x00, 0x00, 0x00, 0x22, 0x22, 0x21, 0x24, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x44, 0x44, 
	0x00, 0x00, 0x00, 0x22, 0x77, 0x41, 0x2e, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x78, 0x38, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0x6a, 0xaa, 0xaa, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x6a, 0xaa, 0xaa, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc
};




Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Adafruit_SSD1306 basic(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Adafruit_SSD1306 powermeter(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Adafruit_SSD1306 tachometer(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

VescUart UART;
SimpleKalmanFilter Filter1(2, 2, 0.01);
ESP32Time rtc(3600);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200, SERIAL_8N1);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  while (!Serial) {;}

  rtc.setTime(10, 59, 17, 4, 2, 2023); 

  tcaselect(DISPLAYADDR);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("Display allocation failed"));
  for(;;); // Don't proceed, loop forever
  }

  display.setTextSize(2);
  display.setTextColor(WHITE);
  String startuptext = "Elmoto HR-2";
  display.setCursor(20, 16);
  display.print(startuptext);
  display.display();

  tcaselect(BASICADDR);
  if (!basic.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("Display allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  delay(50);
  basic.clearDisplay();
  basic.drawBitmap(0, 0, basicUI, 128, 32, WHITE);
  basic.display();

  tcaselect(POWERMETERADDR);
  if (!powermeter.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("Display allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  delay(50);
  powermeter.clearDisplay();
  powermeter.drawBitmap(0, 0, powermeterUI, 128, 32, WHITE);
  powermeter.display();

  tcaselect(TACHOMETERADDR); 
  if (!tachometer.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("Display allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  delay(50);
  tachometer.clearDisplay();
  tachometer.drawBitmap(0, 0, tachometerUI, 128, 32, WHITE);
  tachometer.display();

  UART.setSerialPort(&Serial2);


  pinMode(LICHT_IN, INPUT_PULLUP);
  pinMode(BREMSLICHT_IN, INPUT_PULLUP);
  pinMode(HUPE_IN, INPUT_PULLUP);

  pinMode(SELECT, INPUT_PULLUP);
  pinMode(OPTION, INPUT_PULLUP);
  pinMode(MENU, INPUT_PULLUP);

  pinMode(LICHT_OUT, OUTPUT);
  pinMode(BREMSLICHT_OUT, OUTPUT);
  pinMode(HUPE_OUT, OUTPUT);
  

  delay(1000); // Pause for 1 seconds

  // Clear the buffer
  refreshUI();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100); // this speeds up the simulation
  
  buttons();
  Serial.println("Test0");


  getTelemetry();
  refreshDisplay();

  //refreshUI();


  //Serial.println(stick);
  //Serial.println(change);

  /*
  Serial.print("Spalte: ");
  Serial.print(c);
  Serial.println();
  Serial.print("Dashboard: ");
  Serial.print(d);
  Serial.println();
  Serial.print("Fahrmodus: ");
  Serial.print(f);
  Serial.println();
  Serial.print("Loggingmenü: ");
  Serial.print(v);
  Serial.println();
*/  


}




// Wenn man es für große Menüs realisieren will: Dann wohl am besten mit Arrays. Min und Max werden in bspw. Zeile 0 für entsprechende Spalte hinterlegt
// Bei Hoch / Runter / Links / Rechts wird dann überprüft, ob man an der Grenze ist, ansonsten entsprechend ein Cursor erhöht / verringert








