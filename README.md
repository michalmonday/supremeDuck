## supremeDuck
Wireless "Rubber ducky" controlled by smarphone application.  


**What is it?**  
It's a device that can be connected to a PC and pretend to be keyboard and mouse. The user can trigger specific actions using Android through wifi or bluetooth.  


**What can you do with it?**  
Plug it in to your friend's PC and by pressing a button on your android smartphone:
* access a website
* play a youtube video
* type pre-defined text of your choice
* download and execute file
* move mouse cursor
* and [much more](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Payloads)

Preview feature allows to see what youtube video or website will be launched on the target PC.  
![Preview-feature-gif](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/preview_option_for_youtube_and_websites.gif)  

**Video**  
[Review and presentation video](https://www.youtube.com/watch?v=FsTeedpYeg4&index=7&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2) thanks to:  
[![image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/JackkTutorials.png)](https://www.youtube.com/watch?v=FsTeedpYeg4&index=7&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2)

**Implementation details**  
The Android application was made using "MIT App Inventor 2" and is open source (see source/supremeDuck.aia). Initially it was made with Arduino Pro Micro and HC-06 bluetooth module. Currently it can also be made and used with Esp8266 wi-fi module instead of HC-06 using the same hardware setup spacehuhn used [here](https://github.com/spacehuhn/wifi_ducky).  


**Is it going to work on any PC and work work instantly?**  
It works well with Windows 10 and Windows 8, it is problematic on Windows 7 (driver installation popup takes long time and sometimes requires replugging device). It should work on Linux/Mac but great majority of functions is designed specifically for Windows.   


**What makes it stand out from other similar projects?**  
Ease of use + no need to reupload code for different languages.  
  
Keyboard simulating devices work differently depending on the system language setting. At the moment (03/07/2018) the original HAK5 [rubber ducky](https://hakshop.com/products/usb-rubber-ducky-deluxe), Seytonic's [Malduino](http://malduino.com), spacehuhn's [wifi_ducky](https://github.com/spacehuhn/wifi_ducky) or [The Darkwing Duck](https://www.youtube.com/watch?v=oh7TcU3XbuQ) created by Amateur Security Research Group require the user to pick a keyboard language setting before uploading the code. It means that device has to be re-programmed before using it with different PCs if they have different language settings. supremeDuck handles the aforementioned problem in 2 ways:
  1. It dynamically changes the desired language settings through Android app whenever needed (the device must be turned on while changing it).  
  2. It takes advantage of the fact that Windows allows a tricky method of typing characters (by using ALT + NUMPAD keys it is possible to type the ASCII code and the desired character will be typed). As a result the device can be set to work with all of the most frequently used language settings without the need to change anything (as long as it's used on Windows). In the options of the android app it's possible to dynamically enable and disable it (it's called "MultiLang - Windows only").
  
  
**How to make it or get it?**
1. Visit and follow [this tutorial](https://github.com/michalmonday/supremeDuck/wiki/DIY-Tutorial) about the bluetooth version.  
1a. See [this guide](https://github.com/michalmonday/supremeDuck/wiki/How-to-make-wifi-based-version-(Esp8266)) to see how to make the wifi based version.  
2. Buy it from me. It's available at: http://prankingdevice.co.uk

**Usage and more info is available in [wiki section](https://github.com/michalmonday/supremeDuck/wiki)**  
**List of updates is [here](https://github.com/michalmonday/supremeDuck/blob/master/UPDATES.md)**  

**Credits / thanks to / kudos**  
[HAK5](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Downloads#duck-encoder) and [mame82](https://github.com/mame82/duckencoder.py) - encoding for diffent languages used in this project is in 99% based on their work.  
[Seytonic](https://maltronics.com/) - in a series of [youtube tutorials](https://www.youtube.com/watch?v=_yJWwKO3_Z0) presented how to use cheap Arduino Pro Micro as "Rubber ducky". It's worth to mention that Samy Kamkar also presented this kind of functionality [with Teensy](https://www.youtube.com/watch?v=aSLEq7-hlmo) in 2014.  
Dejan from howtomechatronics.com - thanks for the tutorials about using Arduino with [bluetooth module](https://howtomechatronics.com/tutorials/arduino/arduino-and-hc-05-bluetooth-module-tutorial/) and [App inventor](https://howtomechatronics.com/tutorials/arduino/how-to-build-custom-android-app-for-your-arduino-project-using-mit-app-inventor/).  

**Future**  
According to [MIT App Inventor Team](http://appinventor.mit.edu/explore/blogs/evan/2018/09/mit.html) it will be possible to run application made using App Inventor on iOS soon (first quarter of 2019) which means that the supremeDuck application will not be limited to Android only. The progress of their work can be checked at: http://doesappinventorrunonios.com/  


**Images**  

![](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/readme_1.png)  

![](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/readme_2.png)  

![](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/readme_3.png)

![](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/readme_4.jpeg)

![](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/readme_5.png)
