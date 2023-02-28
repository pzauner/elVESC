
void getTelemetry() {
  
  if ( UART.getVescValues() ) {

    rpm = (UART.data.rpm);
    voltage = (UART.data.inpVoltage);
    current = (UART.data.avgInputCurrent);
    motorcurrent = (UART.data.avgMotorCurrent);
    tempVesc = (UART.data.tempMosfet); 
 
    power = voltage*current;
    amphours = (UART.data.ampHours);
    batpercentage = ((voltage-10)/3)*100;
    //batpercentage = ((voltage-39)/13)*100;               // ((Battery voltage - minimum voltage) / number of cells) x 100
        

    powerfiltered = Filter1.updateEstimate(power);
    rpmfiltered = Filter1.updateEstimate(rpm);

    if (rpmfiltered < 0) {
      rpmfiltered = rpmfiltered * -1;
    }
    velocity = rpmfiltered*3.142*60/1000*0.6096;          // Motor RPM x Pi x seconds in a minute / meters in a km x Wheel diameter

    wh = wh - wattHours;
    km = km - distance;
    odo = odo - km;
    
    distance = rpmfiltered*3.142*1/1000*0.6096;          // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter 
    wattHours = (UART.data.wattHours);

    wh = wh + wattHours;
    km = km + distance;
    odo = odo + km;

    if (km >= 0) { // Prinzipiell nicht notwendig bisher - aber bevor es irgendwann zum Problem wird..
    
      whkm = wh / km; 
    
    }
    Serial.println("Telemetrie funktioniert");

    Serial.println("getTelemetry");

    
  }  else {
    Serial.print("Vesc nicht verbunden");
  }
  
}