(x)trees
======

### What is this

[(x)trees](http://www.agneschavez.com/xtreeproject) is an audio-visual installation created using [openFrameworks](http://www.openframeworks.cc) for the visual part, and [Pure Data](http://puredata.info) for the interactive audio. Check the [project page](http://www.agneschavez.com/xtreeproject) to better understand what is it about.

### Credits

* Agnes Chavez: Art Director ([www.agneschavez.com](http://www.agneschavez.com))
* Alessandro Saccoia: Programming ([www.keplero.com](http://www.keplero.com))

### Disclaimer

This code has been used in multiple exhibition and has seen a number of changes, hence you will find parts that are commented out, obscure #ifdefs, and misleading settings in the control panel. Still, it's provided in the hope that someone will benefit from it. It's licensed under the MIT license, as Openframeworks itself. 

##IMPORTANT

This release includes a lot of third party add-ons and the openFrameworks trunk as well.
Many add-ons were modified and customizes in order to make this possible and it would need a huge amount of time to give a "clean" OF app. 

All the directories can be licensed in different way, so, USER BEWARE, before using any part of the application you must read the appropriate README of LICENSE file in the directory of the file you are reading. In case there isn't you can mostly find the licensing information at the top of the source files. In case there is not such information and you want to use it, you NEED to contact me (alex@keplero.com) and I will look for the missing licensing informations.

If anybody feels that this work is breaking their intellectual property, please inform me asap.


### How does it work

The code has been tested on OSX only. Select the target XTreesV3_looping in Xcode and build, it should build smoothly. Bear in mind that some OF add-ons, especially ofxControlPanel, have been hacked and it would take to much time to do a recursive diff, so the code is only compiling in the OF directory structure provided (based on v007).

All the required libraries and add-ons are included in the source code.


### How to use it

Briefly: seeds are places with the mouse button, and it's possible to move them around by dragging them. When a seed is selected, typing with the keyboard will change the keyboard that seed is associated to. Press Return to make the change permanent. Once the GO button is pressed (there are also keyboard shortcuts for everything, I direct you to the code), the trees will start growing by fetching related messages from Twitter(public API), and Twilio (private API, to make it work you need to register with them and pay some costs for each message received).

### Visual part configuration

In the bin directory there are some files: screenDimensions.txt that is checked at initialization time and sets automatically the screen dimensions to the ones written in the file. This is useful for configurations with multiple projectors.

The file init.xml defines the initial position of the seeds, that is saved when the program is exited gracefully (ESC).

All the customizable parts of the installation are available by pressing "," (comma) during the execution.

### Audio part configuration

The interactive audio part can work in two modes: using PDLib, or by opening the OSCWrapper.pd Pure Data patch in the background, and having the OF app sending OSC messages to the PD patch.

These two modes can be changed in the file InteractiveAudio.h 

define USE_PDLIB 1   // 1 for pdlib, 0 otherwise

define USE_OSC 0  // 1 for osc, 0 otherwise




TO BE CONTINUED......
