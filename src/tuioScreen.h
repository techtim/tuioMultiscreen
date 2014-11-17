//
//  tuioScreen.h
//  tuioMultiscreen
//
//  Created by Tim TVL on 16/11/14.
//
//

#ifndef __tuioMultiscreen__tuioScreen__
#define __tuioMultiscreen__tuioScreen__

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxTuioClient.h"

class tuioScreen: public ofThread {
  public:
    tuioScreen(unsigned int _id, unsigned int port, unsigned int _width, unsigned int _height, ofPoint _pos = ofPoint(0,0));
    ~tuioScreen();
    
    void setPosition(ofPoint _pos);
    void setWidth(const int width);
    void setHeight(const int height);
    
    void showGui(bool show){ bShowGui = show; };

    void draw(float scaleX = 1.f, float scaleY = 1.f);

    void save();
    void load();
    
    ofxUISuperCanvas *gui;
    void guiEvent(ofxUIEventArgs &e);
    
    void threadedFunction();
    void update();
    void exit(ofEventArgs& e);
    const ofPoint getTouchPos();
    
  private:
    ofRectangle region, drawRegion;
    bool bInvertX, bInvertY;
    bool bSwitchXY;
    bool bEnabled, bShowGui;

    ofPoint touchPos;
    
    ofMutex oscLock;
    
    ofxTuioClient Tuio;
    
    unsigned int _id, port;
    float guiPort;
};

#endif /* defined(__tuioMultiscreen__tuioScreen__) */
