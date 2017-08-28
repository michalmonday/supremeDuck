# supremeDuck
"Rubber ducky" with android support using bluetooth



**What is it?**  
It's an arduino based device which can be connected to a PC and pretend to be "Human Interface Device" like keyboard and mouse. This project utilizes a smartphone with an android to trigger specific actions. The android application was made using "MIT app inventor 2" and is open source as well as the arduino sketch so the user can adjust its functionality.


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


**Is it going to work on any PC and work work instantly?**  
Idk, it's based on Arduino, after connecting it to the PC it's recognised as "USB input device" and "Arduino", it worked instantly on my freshly re-installed windows 8 and also on windows 10 where I deleted arduino drivers. But for example at my college (win 7) it pops up "Installing drivers" in the bottom right corner, it works in both scenarios if the installation is finished or cancelled, however it doesn't work until it's ended in some way. It's recognised by PC the same as "[Malduino](https://www.indiegogo.com/projects/malduino-badusb-arduino-usb#/)" by Seytonic:
> Will the MalDuino work on computer X?
> The MalDuino works just like an arduino leonardo, so if a leonoardo can do it on a certain computer, so can a MalDuino. It should be noted that I have had some reports of people having difficulty running scripts on windows 7, however this is not widespread as far as I know and I am looking into this.
Potentially it could work with any system however the functions are designed to work with Windows.  

**What makes it stand out from other similar projects?**  
Keyboard simulating devices work differently depending on the system language setting. At the moment (25/7/17) the original HAK5 "rubber ducky", "Malduino" and other arduino based HID projects require the user to pick a keyboard language setting before uploading the code. It means that device has to be re-programmed before using it with different PCs if they have different language settings. That's not the case with supremeDuck, it handles the aforementioned problem in 2 ways:
  1. It gives an ability to dynamically change the desired language settings by using the android app whenever needed (the device must    be turned on while changing it). 
  2. It takes advantage of the fact that Windows allows a tricky method of typing characters (by using ALT + NUMPAD keys it is possible to type the ASCII code and the desired character will be typed). As a result the device can be set to work with all of the most frequently used language settings without the need to change anything (as long as it's used on Windows). In the options of the android app it's possible to dynamically enable and disable it (it's called "MultiLang - Windows only" in the categories list).
  
**How to get it?**
1. Visit and follow the tutorial mentioned below.
2. Buy it from me. It's available at: http://monday.pythonanywhere.com


**Tutorial and usage info are available in [wiki section](https://github.com/michalmonday/supremeDuck/wiki)**  


![](http://i.imgur.com/R53zLZQ.png)

![](http://i.imgur.com/LV6Ix5R.jpg)

![](http://i.imgur.com/ofYEmmX.png)
