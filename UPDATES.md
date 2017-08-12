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
