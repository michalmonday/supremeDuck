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
