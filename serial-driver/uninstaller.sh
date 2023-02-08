#!/bin/bash

if [ -d /System/Library/Extensions/SiLabsUSBDriver.kext ]; then
sudo kextunload /System/Library/Extensions/SiLabsUSBDriver.kext
sudo rm -rf /System/Library/Extensions/SiLabsUSBDriver.kext
fi

if [ -d /System/Library/Extensions/SiLabsUSBDriver64.kext ]; then
sudo kextunload /System/Library/Extensions/SiLabsUSBDriver64.kext
sudo rm -rf /System/Library/Extensions/SiLabsUSBDriver64.kext
fi

if [ -d /Library/Extensions/SiLabsUSBDriverYos.kext ]; then
sudo kextunload /Library/Extensions/SiLabsUSBDriverYos.kext
sudo rm -rf /Library/Extensions/SiLabsUSBDriverYos.kext
fi

if [ -d /Library/Extensions/SiLabsUSBDriver.kext ]; then
sudo kextunload /Library/Extensions/SiLabsUSBDriver.kext
sudo rm -rf /Library/Extensions/SiLabsUSBDriver.kext
fi

if [ -d /Applications/CP210xVCPDriver.app/Contents/Library/SystemExtensions/com.silabs.cp210x.dext ]; then
/Applications/CP210xVCPDriver.app/Contents/MacOS/CP210xVCPDriver uninstall
sudo rm -rf /Applications/CP210xVCPDriver.app
fi