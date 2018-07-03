# supremeDuck
Wireless "Rubber ducky" controlled by Android.  


**What is it?**  
It's a device that can be connected to a PC and pretend to be keyboard and mouse. The user can trigger specific actions using Android through bluetooth.  


**What can you do with it?**  
Plug it in to your friend's PC and by pressing a button on your android smartphone:
* access a website
* play a youtube video
* type pre-defined text of your choice
* download and execute file
* move mouse cursor
* and [much more](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Payloads)

[Youtube video - review and presentation](https://www.youtube.com/watch?v=FsTeedpYeg4)  
[Youtube video - functionality](https://www.youtube.com/watch?v=iL2pS2EvqkY&index=3&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2)  
[Youtube video - early version](https://www.youtube.com/watch?v=mwDT1gX9wzU&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2&index=1)  


**Implementation details**  
The Android application was made using "MIT App Inventor 2" and is open source (see source/supremeDuck.aia). Initially it was made with Arduino Pro Micro and HC-06 bluetooth module. Currently it can also be made and used with Esp8266 wi-fi module instead of HC-06.  


**Is it going to work on any PC and work work instantly?**  
It works well with Windows 10 and Windows 8, it is problematic on Windows 7 (driver installation popup takes long time and sometimes requires replugging device). It should work on Linux/Mac but great majority of functions is designed specifically for Windows.   


**What makes it stand out from other similar projects?**  
Keyboard simulating devices work differently depending on the system language setting. At the moment (03/07/2018) the original HAK5 [rubber ducky](https://hakshop.com/products/usb-rubber-ducky-deluxe), Seytonic's [Malduino](http://malduino.com), spacehuhn's [wifi_ducky](https://github.com/spacehuhn/wifi_ducky) or [The Darkwing Duck](https://www.youtube.com/watch?v=oh7TcU3XbuQ) created by Amateur Security Research Group require the user to pick a keyboard language setting before uploading the code. It means that device has to be re-programmed before using it with different PCs if they have different language settings. supremeDuck handles the aforementioned problem in 2 ways:
  1. It dynamically changes the desired language settings through Android app whenever needed (the device must be turned on while changing it). 
  2. It takes advantage of the fact that Windows allows a tricky method of typing characters (by using ALT + NUMPAD keys it is possible to type the ASCII code and the desired character will be typed). As a result the device can be set to work with all of the most frequently used language settings without the need to change anything (as long as it's used on Windows). In the options of the android app it's possible to dynamically enable and disable it (it's called "MultiLang - Windows only").
  
  
**How to make it or get it?**
1. Visit and follow [this tutorial](https://github.com/michalmonday/supremeDuck/wiki/DIY-Tutorial) about the bluetooth version.  
1a. See [wifi_ducky](https://github.com/spacehuhn/wifi_ducky) project to see how to assemble wifi based version. Or check out Seytonic's [video](https://www.youtube.com/watch?v=Utq4C9S3-uI) about it.  
2. Buy it from me. It's available at: http://prankingdevice.co.uk


**Usage and more info is available in [wiki section](https://github.com/michalmonday/supremeDuck/wiki)**  
**List of updates is [here](https://github.com/michalmonday/supremeDuck/blob/master/UPDATES.md)**  

![](https://i.imgur.com/z6XgbdW.png)  

![](https://i.imgur.com/VBExUiM.png)  

![](http://i.imgur.com/R53zLZQ.png)

![](http://i.imgur.com/LV6Ix5R.jpg)

![](http://i.imgur.com/ofYEmmX.png)
