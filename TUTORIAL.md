Prerequisites:
* Arduino Pro Micro 5V 16Mhz (8Mhz version would get damaged without additional plugin. Take a look at product description for "Supported under Arduino IDE v1.0.2" which means that the device may not work without reflashing. Pick those with v1.05 support instead. I personally recommend [this seller](http://www.ebay.co.uk/usr/scooterboy101), gives outstanding support and has good knowledge about these devices.)
* bluetooth module HC-06
* 2K Ohm resistor
* 4.7K Ohm resistor (tested and working with 5.1k too)
* USB adapter
* solid core wire
* soldering equipment
* hot glue gun
* heatshrink


**Hardware setup:**  
1. Make sure to have all prerequisites. 
2. Desolder and remove pins from the bluetooth module. It may be difficult to desolder them all at once, I used pliers to destroy the black plastic which holds them together and removed them separately. 
3. Hot glue the Arduino and bluetooth boads together (chips pointing outwards) 
4. Cut a 6cm piece of wire, cut 1 "leg" of each resistor (2k and 4.7k) and create voltage divider by soldering all 3 together. 
5. Cut wire into 3x6cm pieces, strip the rubber from their ends and solder them accordingly to the pattern presented on the images. 
6. Connect the device to PC, it should be [blinking](https://youtu.be/ZgmhzojPXA4)

![](http://i.imgur.com/rwbGNvQ.jpg)

![](http://i.imgur.com/GhOuCLn.png)

![](http://i.imgur.com/mnR4kfK.jpg)


**Software (arduino):**  
1. Download [Arduino IDE](https://www.arduino.cc/en/main/software) and [Arduino sketch](https://github.com/michalmonday/supremeDuck/blob/master/source/supremeDuck.ino).
2. Open arduino IDE. Click on Tools -> Board -> **Arduino Leonardo (Important - incorrect setting of the board may require [reflashing](http://forum.arduino.cc/index.php?topic=376079.0) the arduino or performing a tricky reset.** Click on Tools -> Port -> COM X. Paste [this sketch](http://pastebin.com/cSpApBy9) and upload it to the board.
2. Open Tools -> Serial Monitor, the new window will popup, select "No line ending" and "9600 baud" at the bottom corner of it. Then input the following commands into the input box located at the top:
AT+NAMEyourname
AT+PIN1234
Note that the pin has to consist of 4 digits. If everything goes right you should get "OKsetname" and "OKsetpin" responses.
3. Upload the main sketch.

![](http://i.imgur.com/EKH4JhM.png)

![](http://i.imgur.com/AFmjLLG.png)



**Software (app):**  
1.Download MIT project to your PC. Then download [MIT App Inventor 2 Companion](https://play.google.com/store/apps/details?id=edu.mit.appinventor.aicompanion3&hl=en_GB) to your android.  
2. Register at MIT app inventor website, log into it and import the [MIT project](https://github.com/michalmonday/supremeDuck/blob/master/source/supremeDuck.aia) (Connect -> Import project .aia). I recommend to watch [this tutorial](https://www.youtube.com/watch?v=o-YVvxYiSuk) which explains how to create and manage applications made using this tool.  
3. Click on Build -> App (provide QR code for .apk). The QR code will appear, it has to be scanned with a smartphone. Open your MIT AI2 Companion app on android and press scan QR code which will should appear on the screen of your PC. (This step isn't necessary to test and develop the application, MIT provides comfortable quick update system which automatically lets the user to preview application while changing its components.)  
4. Complete the installation process and the project should be functional.

![](http://i.imgur.com/O2RVH0X.png)

![](http://i.imgur.com/Hz8uInl.png)

![](http://i.imgur.com/Pjwmz56.png)
