# elVESC
 everything you need to replace the controller of the elmoto (hr-2) with a vesc


## Structure
    UI/
    ├─ dashboard.png
    ├─ powermeter.png
    ├─ tachometer.png
    code/
    ├─ Firmware/
    ├─ uarttest/
    ├─ lcdtest/
    ├─ serialtester/
    ├─ i2scanner/
    vesc-related/
    ├─ firmware/
    ├─ manual/
    ├─ tool/
    libraries/
    serial-driver/
    pcb/
    ├─ schematics/
    ├─ partlist.md/


The UI-Folder provides the .png files for the screens as a backup. The actual images are stored in bytes via image2cpp

"code" contains all the things you could flash to the esp32 used. The main project is in the directory "Firmware". All others are used for some testing. I did not write most or any of the code in these test-programs.

Inside the folder "vesc-related" are backups of the flashed firmware of my vesc provided by the manufacturer, the vesc-tools as backup - which of course are not pushed to GitHub as the license doesn't allow it and the manual.

"libraries" includes the arduino libraries used for this project and "serial-driver" includes a backup of the USB to serial driver used on the ESP32 dev board.

Inside "pcb" you'll find the schematic to print the PCB for the display and the controller. If you want any first contact me as I should have - once printed - some leftover.

It includes a partlist as well.

## Helpful links
VESC:
available values of the used library - https://github.com/SolidGeek/VescUart/blob/d95a1bae5295dcb1e9e793d3a0b9dbffe6676d6b/src/VescUart.cpp
(rows 197-214)

other VESC - UART Projekte:
https://github.com/RollingGecko/ArduBoardControler
https://github.com/RollingGecko/VescUartControl

offizielle VESC Firmware:
https://github.com/vedderb/bldc



similar project yet simpler:
https://github.com/lucanatorvs/VESC_6_in_elmoto


