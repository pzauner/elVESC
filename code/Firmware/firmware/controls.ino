void buttons() {

  if (digitalRead(LICHT_IN)==0) {
    lastDebounceTime = millis();
    if (licht == 0) {
      licht = 1;
    } else {
      licht = 0;
    }
  }

  if (licht == 1) {
    digitalWrite(LICHT_OUT, HIGH);
    Serial.println("Licht ist an");
  } else {
    digitalWrite(LICHT_OUT, LOW);
  }


  // Menu Button
  if (digitalRead(MENU)==0) {
      stick = "menu";
      menuUpdate();
      delay(200);

  }

  // Select / View
  else if (digitalRead(SELECT)==0) {
      stick = "select";
      menuUpdate();
      delay(200);
  }

  // Option / Confirm Button
  else if (digitalRead(OPTION)==0) {
      stick = "option";
      menuUpdate();
      delay(200);
  }
}

void menuUpdate() {
    if (stick == "select") {
      switch (c) {
        case 0:
          if (d != 3) {
            d++;
            refreshUI();
          } else {
            d = 1;
            refreshUI();
          }
          break;
      
        case 1:
          if (f != 3) {
          f++;
          refreshUI();
          } else {
            f = 1;
            refreshUI();
          }
        break;

        case 2:
          if (v != 3) {
            v++;
            refreshUI();
          } else {
            v = 1;
            refreshUI();
          }
        break;
          
      }
    }

    if (stick == "option") {
      switch (c) {
        case 0:
          // TODO -> Delay einbauen
          km = 0;
          wh = 0;
          refreshUI();
          break;
      
        case 1:
          //Bestätigung eigentlich nicht nötig
          break;

        case 2:
          if (v == 2) {
          logging = true;
          v = 1;
          refreshUI();
          }
          if (v == 3) {
            logging = false;
            v = 1;
            refreshUI();
          }
          break;
      }
    }


    if (stick == "menu") {

      switch (c) {
        case 0:
          c++;
          refreshUI();
          break;
        
        case 1:
          c++;
          refreshUI();
          break;
        
        case 2:
          v = 1;
          c = 0;
          refreshUI();
          break;
        /*
        case 3:
          c = 0;
          refreshUI();
          break;  */        
      }
    }


}