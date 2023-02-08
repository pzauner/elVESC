#!/bin/bash

set -x

echo "********" >> ~/Desktop/info.txt
echo "Result of ioclasscount:" >> ~/Desktop/info.txt
ioclasscount | grep silabs >> ~/Desktop/info.txt
echo "********" >> ~/Desktop/info.txt
echo "Result of kextstat:" >> ~/Desktop/info.txt
kextstat | grep silabs >> ~/Desktop/info.txt
echo "********" >> ~/Desktop/info.txt
ls -la /dev/cu.* >> ~/Desktop/info.txt
echo "********" >> ~/Desktop/info.txt
ioreg -lbfp IOUSB >> ~/Desktop/info.txt
echo "********" >> ~/Desktop/info.txt
echo "Version of Silabs driver in file system:" >> ~/Desktop/info.txt
grep -A 1 CFBundleVersion /Library/Extensions/SiLabsUSBDriver.kext/Contents/Info.plist >> ~/Desktop/info.txt
