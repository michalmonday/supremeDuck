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


[Youtube video - current functionality presentation](https://www.youtube.com/watch?v=iL2pS2EvqkY&index=3&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2)  
[Youtube video - dynamic language encoding](https://www.youtube.com/watch?v=4nbv-LyDWvk&index=2&list=PLnVVAaZSdNGtcMunS1_Wy3smTZLlzIaV2)



**Is it going to work on any PC and work work instantly?**  
Idk, it's based on arduino, after connecting it to the PC it's recognised as "USB input device" and "Arduino", it worked instantly on my freshly re-installed windows 8 and also on windows 10 where I deleted arduino drivers. But for example at my college (win 7) it pops up "Installing drivers" in the bottom right corner, it works in both scenarios if the installation is finished or cancelled, however it doesn't work until it's ended in some way. It's recognised by PC the same as "[Malduino](https://www.indiegogo.com/projects/malduino-badusb-arduino-usb#/)" by Seytonic:
> Will the MalDuino work on computer X?
> The MalDuino works just like an arduino leonardo, so if a leonoardo can do it on a certain computer, so can a MalDuino. It should be noted that I have had some reports of people having difficulty running scripts on windows 7, however this is not widespread as far as I know and I am looking into this.


**Can the device perform the same functions on any keyboard encoding?**  
Probably not but I didn't test them all, the encoding has been adapted from the orginal HAK5's "rubber ducky" which isn't perfect but it's more than enough to have fun with it.

![](http://i.imgur.com/LV6Ix5R.jpg)
