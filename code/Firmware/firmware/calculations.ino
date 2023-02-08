
void getTelemetry() {
  
  if ( UART.getVescValues() ) {

    rpm = (UART.data.rpm);
    voltage = (UART.data.inpVoltage);
    current = (UART.data.avgInputCurrent);
    motorcurrent = (UART.data.avgMotorCurrent);
    tempVesc = (UART.data.tempMosfet); 
 
    power = voltage*current;
    amphours = (UART.data.ampHours);
    tach = (UART.data.tachometerAbs)/42;                   // The '42' is the number of motor poles multiplied by 3
    velocity = rpm*3.142*(60/1000)*0.6096;          // Motor RPM x Pi x (seconds in a minute / meters in a mile) x Wheel diameter
    batpercentage = ((voltage-9)/3)*100;
    //batpercentage = ((voltage-39)/13)*100;               // ((Battery voltage - minimum voltage) / number of cells) x 100
        

    powerfiltered = Filter1.updateEstimate(power);
    rpmfiltered = Filter1.updateEstimate(rpm);


    wh = wh - wattHours;
    km = km - distance;
    odo = odo - km;
    
    distance = tach*3.142*(1/1000)*0.6096;          // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter 
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