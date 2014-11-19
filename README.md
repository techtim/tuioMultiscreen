tuioMultiscreen
===============

Tuio client for Multiscreen setup, allows to control mouse with multiple Tuio interfaces 

On touch kCGEventLeftMouseDown, on relaese kCGEventLeftMouseUp, on move kCGEventLeftMouseDragged + kCGEventLeftMouseDown

Configure: dimensions of each interface, position on shared OSX screen, inverts x/y for right merging of screens, and switch x->y for vertical interface setup

Hotkeys: space - activate, 's' - save, 'l' - load, 'g' - hide GUI

Build with openFrameworks 0.8

Dependencies: ofxOsc, ofxUI, ofxXmlSettings

Tested OSX 10.9, build using 10.6.sdk, but think it can be any sdk above
