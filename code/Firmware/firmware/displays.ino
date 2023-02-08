void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void refreshDisplay() {

int16_t x1;
int16_t y1;
uint16_t width;
uint16_t height;

String displaykm;
String displayodo;
String displaywhkm;
String displaywh;
String displayrpm;
String displayvelocity;
String displayvoltage;
String displaycurrent;
String displaymotorcurrent;
String displaytempVesc;
String displaypower;
String displaybatpercentage;

String displayclock;
String displayhour;
String displayminute;
int second;

float powerfiltered;
float rpmfiltered;

tcaselect(BASICADDR);
basic.setTextSize(3);
basic.setTextColor(WHITE,BLACK);

if (rtc.getHour(true) < 10) {
  displayhour = "0" + String(rtc.getHour(true));
} else {
  displayhour = String(rtc.getHour(true));
}

if (rtc.getMinute() < 10) {
  displayminute = "0" + String(rtc.getMinute());
} else {
  displayminute = String(rtc.getMinute());
}
displayclock = displayhour + displayminute;

basic.getTextBounds(displayclock, 0, 0, &x1, &y1, &width, &height);
basic.setCursor(20 + ((72 - width)/2), 0);
basic.print(displayhour);
basic.setCursor(58 + ((72 - width)/2), 0);
basic.print(displayminute);

if (rtc.getMillis() < 501) {
  basic.fillRect(54,6,3,3, WHITE);
  basic.fillRect(54,16,3,3, WHITE);
} else {
  basic.fillRect(54,6,3,3, BLACK);
  basic.fillRect(54,16,3,3, BLACK);
}

second = rtc.getSecond();

second = second / 10;

if (second < 3) {
  for (int n = 0; n < second; n++) {
    basic.fillRect(23 + (second*11),25, 8,5, WHITE);
  }
  for (int n = second + 1; n < 3; n++) {
    basic.fillRect(23 + (n*11),25, 8,5, BLACK);
  }
  for (int n = 0; n < 3; n++) {
    basic.fillRect(58 + (n*11),25, 8,5, BLACK);
  }
} else {
  for (int n = 0; n < 3; n++) {
    basic.fillRect(23 + (n*11),25, 8,5, WHITE);
  }
  for (int n = 0; n < (second - 2); n++) {
    basic.fillRect(58 + (n*11),25, 8,5, WHITE);
  }
}


basic.display();




tcaselect(DISPLAYADDR);
  if (c == 0 && d == 1) {

    // Textkoordinaten: 7, 3 Wh; 7, 15 Wh/km; 67, 3 km; 67, 15 ODO

    //Serial.println(displaykm);
    //Serial.println(odokm);
    displaykm = String(km, 2);
    displayodo = String(odo, 0);
    displaywhkm = String(whkm, 0);
    displaywh = String(wh, 0);

    display.setTextSize(2);
    display.setTextColor(WHITE,BLACK);

    display.getTextBounds(displaykm, 0, 0, &x1, &y1, &width, &height);
    display.setTextWrap(false);
    display.setCursor(104 - width, 0);
    display.print(displaykm);

    display.getTextBounds(displayodo, 0, 0, &x1, &y1, &width, &height);
    display.setCursor(104 - width, 17);
    display.print(displayodo);

    display.getTextBounds(displaywh, 0, 0, &x1, &y1, &width, &height);
    display.setCursor(23 - width, 0);
    display.print(displaywh);

    display.getTextBounds(displaywhkm, 0, 0, &x1, &y1, &width, &height);
    display.setCursor(23 - width, 17);
    display.print(displaywhkm);

    display.display();

  }

  if (c == 0 && d == 2) {

    // Textkoordinaten: 7, 3 Wh; 7, 15 Wh/km; 67, 3 km; 67, 15 ODO

    //Serial.println(displaykm);
    //Serial.println(odokm);
    Serial.println(powerfiltered);
    Serial.println(motorcurrent);
    Serial.println(powerfiltered);
    Serial.println(batpercentage);



    displayrpm = String(rpmfiltered, 0);
    displayvelocity = String(velocity, 0);
    displaypower = String(powerfiltered, 0);
    displayvoltage = String(voltage, 0); 
    displaycurrent = String(current, 0);
    displaymotorcurrent = String(motorcurrent, 0);
    displaytempVesc = String(tempVesc, 0);
    displaybatpercentage = String(batpercentage, 0);

    display.setTextColor(WHITE,BLACK);

    display.setTextWrap(false);
    display.setCursor(0,0);

    display.print(displayrpm);
    display.print("");

    display.print(displayvelocity);
    display.print("km/h ");
    display.print(displaypower);
    display.print("W  ");

    display.print(displayvoltage);
    display.print("V");
    display.print(displaycurrent);
    display.print("A");
    display.print(displaymotorcurrent);
    display.print("A");
    display.println();

    display.print("   ");
    display.print(displaytempVesc);
    display.print("°C ");
    display.print(displaybatpercentage);
    display.print("%       ");

    display.display();

  }

}

void refreshUI() {

tcaselect(DISPLAYADDR);
display.clearDisplay();

  switch (c) {
    // Dashboard
    case 0:
      switch (d) {
        case 1:    
          display.drawBitmap(0, 0, dashboard, 128, 32, WHITE);
          display.display();
          break;
        case 2:
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          //display.println("Historie");
          display.display();
          break;
        case 3:
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("Anderes");
          display.display();
          break;
      }
      break;

    // Fahrmodus
    case 1:
      switch (f) {
        case 1:    
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("SPORT");
          display.display();
          //UART.setCurrent(sport);
          break;
        case 2:
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("SANFT");
          display.display();
          //UART.setCurrent(sanft);
          break;
        case 3:
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("SPORT & Feldabschwächung");
          display.display();
          //UART.setCurrent(sport);

          break;
      }
      break;
    
    // Autologging
    case 2:
      switch (v) {
        case 1:        
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("Logging");
          display.print("aktueller Status: ");
          if (logging == true) {
            display.print("EIN");
          } else display.print("AUS");
          display.display();
          break;
        case 2:
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("Logging aktivieren?");
          display.display();
          break;
        case 3:
          display.setTextColor(WHITE);
          display.setTextWrap(false);
          display.setCursor(0,0);
          display.println("Logging deaktivieren?");
          display.display();
          break;
          
      }
      break;


  }


}