![title-image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/github_banner.png)  

## What is it?  
It's a device that can be connected to a PC and pretend to be keyboard and mouse. The user can trigger specific actions using smartphone through wifi or bluetooth.  


## What can you do with it?  
Plug it in to your friend's PC and by pressing a button on your smartphone:
* :satellite: access a website  
* :tv: play a youtube video  
* :capital_abcd: type pre-defined text of your choice  
* :open_file_folder: download and execute file  
* :squirrel: [exfiltrate files](https://www.youtube.com/watch?v=TBBT1c2zjms) to Dropbox and Gmail  
* :diamond_shape_with_a_dot_inside: move mouse cursor  
* and [much more](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Payloads)  


## Notable features  
* Automatic [OS detection](https://github.com/keyboardio/FingerprintUSBHost), allowing it to work on Windows, Linux, and macOS.  
![os logos image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/os_logos.png)  
* Preview feature, it allows to see what youtube video, wallpaper or website will be launched on the target PC.  
![Preview-feature-gif](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/preview_option_for_youtube_and_websites.gif)  
* Option to use [alt+numpad](https://github.com/michalmonday/supremeDuck/wiki/MultiLang-method) combinations on Windows (to type correct characters regardless of system language)  
* Language switching to match the language setting on target machine without the need to reprogram the device. Supported settings are:  

|||||||
|:-:|:-:|:-:|:-:|:-:|:-:|
| Belgian | Brazilian | Canadian | Switzerland | Czech | German |
| Dannish | Spannish | Finnish | French | UK | Croatian |
| Italian | Norwegian | Portuguese | Slovenian | El Salvador | US |
> You can see how reliable is each of the settings [here](https://github.com/michalmonday/supremeDuck/wiki/Encoding-effectiveness)  
* "Live text execution" checkbox  
<img src="https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/live_text.png" height="100">  

## Video  
[Review and presentation video](https://www.youtube.com/watch?v=FsTeedpYeg4&index=7&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2) thanks to:  
[![image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/JackkTutorials.png)](https://www.youtube.com/watch?v=FsTeedpYeg4&index=7&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2)


## Is it going to work on any PC and work instantly?  
It was tested and working well with **Windows 10** :heavy_check_mark:, **Windows 8** :heavy_check_mark:, **Lubuntu 18.10** :heavy_check_mark:, **Ubuntu 18.04** :heavy_check_mark:, straight after pluggin-in. On **macOS 10.12**:zap: it prompted the user to setup the keyboard, after that it worked well, see [macOS setup](https://github.com/michalmonday/supremeDuck/wiki/macOS-setup) for details. It is problematic on **Windows 7**:zap: (driver installation popup takes long time and sometimes requires replugging device, after that it usually works). 

  
## Implementation details  
The smartphone application was made using "MIT App Inventor 2" and is [open source](https://github.com/michalmonday/supremeDuck/blob/master/source/mobile%20app/supremeDuck.aia). Initially it was made with Arduino Pro Micro and HC-06 bluetooth module. Currently it can also be made and used with Esp8266 wi-fi module instead of HC-06 using the same hardware setup spacehuhn used in [wifi_ducky](https://github.com/spacehuhn/wifi_ducky).  


## Resources  
- [Application](https://play.google.com/store/apps/details?id=appinventor.ai_michalmonday17.supremeDuck)  
[![google store image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/google_play.png)](https://play.google.com/store/apps/details?id=appinventor.ai_michalmonday17.supremeDuck)  
- [DIY guide - bluetooth version](https://github.com/michalmonday/supremeDuck/wiki/DIY-Tutorial)  
![bluetooth version image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/bluetooth_small_img.png)  
- [DIY guide - wifi version](https://github.com/michalmonday/supremeDuck/wiki/How-to-make-wifi-based-version-(Esp8266) )  
![wifi version image](https://raw.githubusercontent.com/michalmonday/supremeDuck/master/resources/repository%20stuff/readme_images/wifi_small_img.png)  
- [OS specific functionality details](https://github.com/michalmonday/supremeDuck/wiki/OS-specific-functionality)  
- [Usage and more info - wiki](https://github.com/michalmonday/supremeDuck/wiki)  
- [List of updates](https://github.com/michalmonday/supremeDuck/blob/master/UPDATES.md)  
- [Assembling service](http://prankingdevice.co.uk)  


## Credits / thanks to / kudos  
:star: [HAK5](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Downloads#duck-encoder) and :star: [mame82](https://github.com/mame82/duckencoder.py) - encoding for different languages used in this project is in 99% based on their work. This project was created thanks to the long chain of people building on top of other people's ideas. If HAK5 did not popularize HID attacks with Rubber Ducky then most of projects like this would not exist. Thanks to :star: authors of ducky scripts posted on [HAK5Darren's page with payloads](https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Payloads) this project is richer in features. Thanks to :star: Darren Kitchen for [Dropbox Exfiltration](https://www.youtube.com/watch?v=TBBT1c2zjms) and similar videos that all contributed in one way or another to this project.  
:star: [Seytonic](https://maltronics.com/) - in a series of [youtube tutorials](https://www.youtube.com/watch?v=_yJWwKO3_Z0) presented how to use cheap Arduino Pro Micro as "Rubber ducky". It's worth to mention that :star: [Samy Kamkar](https://samy.pl) also presented this kind of functionality [with Teensy](https://www.youtube.com/watch?v=aSLEq7-hlmo) in 2014.  
:star: Dejan from howtomechatronics.com - thanks for the tutorials about using Arduino with [bluetooth module](https://howtomechatronics.com/tutorials/arduino/arduino-and-hc-05-bluetooth-module-tutorial/) and [App inventor](https://howtomechatronics.com/tutorials/arduino/how-to-build-custom-android-app-for-your-arduino-project-using-mit-app-inventor/).  
:star: Mr [Jesse Vincent](https://shop.keyboard.io/pages/about-us) who created [FingerprintUSBHost](https://github.com/keyboardio/FingerprintUSBHost) which allows to recognize operating system of the target machine. Noteworthy is that :star: [gloglas](https://github.com/gloglas) used it in [WifiDuckV2](https://github.com/gloglas/WifiDuckV2) (giving me idea to use it too) which is rewrite of :star: [spacehuhns](https://github.com/spacehuhn)'s [wifi_ducky](https://github.com/spacehuhn/wifi_ducky).  
:star: [JackkTutorials](https://www.youtube.com/user/JackkTutorials/videos) - thanks for presenting this project in a video.  
:star: [Taifun](https://puravidaapps.com/index.php) - thanks for great App Inventor resources and [extensions](https://puravidaapps.com/extensions.php), this project is using few of them.   


## Future  
According to [MIT App Inventor Team](http://appinventor.mit.edu/explore/blogs/evan/2018/09/mit.html) it will be possible to run application made using App Inventor on iOS soon (first quarter of 2019) which means that the supremeDuck application will not be limited to Android only. The progress of their work can be checked at: http://doesappinventorrunonios.com/  

I'm designing 2 PCBs for it. Both will have a very similar components to CJMCU Beetle / Malduino / Arduino Pro Micro. 1 of them will allow easy soldering Esp-12F to the back of it, another one will most likely allow soldering JDY-08 or JDY-10 BLE modules to the back of it. I'm planning to fully share the source files so anyone can check, improve, produce or sell them.  
The current state of the Esp8266 based board can be already checked at:  
https://easyeda.com/michalmonday17/supremeduck_wifi  
I just have to wait till the revised version arrives and make sure it works. Here's a preview of the first version that turned out working in the end but had few imperfections.  

![wifi first preview image](https://github.com/michalmonday/supremeDuck/tree/master/resources/repository%20stuff/PCBs/first_version_preview.jpg)  

I'd like to keep the BLE version project private until I get a chance to play with the modules and make sure that they will work as expected.  

I'm planning to create additional tutorial for the process of making these. I used to be "scared" of schematics, soldering very small components and idea of designing PCB, it turns out that it can be fun and does not require some very expensive equipment. I'll try to list all the steps and resources once I complete the boards, however it takes time because waiting for parts takes long time...   

## More images  
[Pics](https://github.com/michalmonday/supremeDuck/tree/master/resources/repository%20stuff/random_images/images.md)  
