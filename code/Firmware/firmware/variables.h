
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