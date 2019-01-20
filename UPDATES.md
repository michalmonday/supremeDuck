Please check the [commits](https://github.com/michalmonday/supremeDuck/commits/master) for further updates. I'm trying to keep commit messages/titles to be descriptive so there's no point in rewriting the same in this file anymore.   

**16/01/2019 - 13:07**  
Fixed what I messed up with last update... (ducky script not working properly)  

**16/01/2019**  
Fixed ducky script bug (e.g. "ALT TAB" not doing what it supposed to)  
  
**24/07/2018**  
-increased baud rate of Esp8266 version to 115200 (it's necessary to upload both: Esp8266 and Arduino code for the change to take place)  

**23/07/2018**  
-fixed critical bug (Bluetooth version froze the device if "LOG_SERIAL" was disabled)  

**03/07/2018**  
-added wi-fi Esp8266 module support as an alternative to HC-06 bluetooth module (user can control 2 devices simultaneously, 1 using bluetooth, 1 using wi-fi)  

**28/05/2018**  
-ability to change bluetooth name and pin from user interface (Categories -> Settings)  
-added ctrl+alt+del, right mouse click (RMB) and backspace button (available in "Plain text" and "Execute program" categories)  
-added "Live text execution" checkbox, if it's checked then any letters typed into text box will be automatically typed on the target PC (available in "Plain text" and "Execute program" categories)  

**17/05/2018**  
-small app update (1.041 - does not require reflashing of supremeDuck device if its version is 1.04)  
-fixed vertical order for "Manage presets" panel because it was too low and while editing the "Content" textbox it was hidden behind keypad  

**10/05/2018 - 6pm**  
-added Alt+Tab  
-added arrow keys in "Plain text" and "Execute program" categories  
-fixed Enter button bug  

**10/05/2018**  
-updated mobile app  
-adding/editing/removing presets is now available through the following ways:  
1. Directly from "Categories -> Manage presets" menu  
2. By modifying files at "/supremeDuck_data/presets/custom_Category.txt" (for all categories except ducky scripts)  
3. By adding/modifying files at "/supremeDuck_data/ducky_scripts/" (ducky scripts only, files must have .txt extension)  

**06/04/2018**  
-organized mit app inventor project (collapsed all the blocks so they're close together now, don't lag as much and are much easier to handle)  
-fixed small bug  
-moved comments to the side in the code because they made it less readable 

**02/09/2017**  
-fixed Arduino code bug, in some language settings "Enter" and other key modifiers were substituted with other keys(thanks to HydrexHD who reported it)  

**19/08/2017**  
-added new presets, ducky scripts (also moved them from pastebin into github), youtube videos (songs + bunch of "one liners") and websites (bunch of gifs)  
-removed few not working presets (kind of dead links)  
-added "Settings" category in the mobile app  
-added version check. After picking the bluetooth device and connecting to it the version check will be performed to make sure that the movile app version is the same as the Arduino code version. Notification will be displayed if these don't match. (This feature will work only from version 1.03 and newer)  
-further simplified the MIT app inventor 2 project (supremeDuck.aia) 

**14/08/2017**  
-added ability to use "ducky script" ("PAUSE", "BREAK", "DEFAULTDELAY", "PRINTSCREEN", "SCROLLLOCK" are not implemented though)  
-fixed minor bugs  
-simplified some of the MIT app inventor 2 components (implemented global lists variables instead of using countless "if then else" statements, the lists are located around the bottom of the project "block canvas" and can be easily edited/expanded)  

**12/08/2017**  
-Just to let you know I'm still working on this project, it isn't just published and left. Soon it should be able to use the "ducky script" language. Also the MIT app inventor app will probably become easier to modify and it will be easier to add your own presets. Currently it seems like a mess, mostly because I was new to it when I started building the app.

**06/08/2017**  
-fixed minor bugs

**04/08/2017 - 18:50**  
-the application is now available in the Google Play Store :) [link](https://play.google.com/store/apps/details?id=appinventor.ai_michalmonday17.supremeDuck)

**04/08/2017 - 13:30**   
-decreased buffer size of encoding data (it was unnecessarily big)  
-used F() function for debugging strings (so now they are stored in flash memory instead of the dynamic memory)  

Both changes prevent the "Low memory, stability issues may occur" warning in Arduino IDE.  


**04/08/2017**  
-implemented 2-way communication so now the arduino tells the mobile app about its current language setting and whether it uses "MultiLang" method  
-added built in web browser so it's possible to search funny images around the internet and copy url by a button (there's also a button to extract Youtube video ID that is currently in the browser)  
-fixed small bug regarding "MultiLang" method (upper case were not affected by it till now)  

![pic](http://i.imgur.com/Hu1tfxg.png)  

![Pic](http://i.imgur.com/CiTAJom.png)
