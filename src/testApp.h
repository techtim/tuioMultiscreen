#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxTuioClient.h"
#include "tuioScreen.h"

#include "ofxUI.h"

static const int SCREEN_W = 1440;
static const int SCREEN_H = 720;
static const int TUIO_NUM = 3;
static const int TUIO_PORT = 7001;

class testApp : public ofBaseApp{
	
public:
	void setup();
    void setupScreens();
    
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
    void controlMouse(ofPoint pos, bool isClicked = false);
    
	ofxTuioClient myTuio;
    vector<ofxTuioClient*> Tuios;

    vector<tuioScreen*> Screens;
    unsigned int screenNumber;
    float guiScreenNumber;
    

    unsigned int oscPort;

    bool bClicked, currentClicked, bControl;
    ofPoint pos, currentPos, mousePosOF;
    
    bool bEnabled;
    
    ofxUISuperCanvas *gui;
    bool bShowGui;
    
    float Width, Height;
    
	void touchDown(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);
	
};

#endif
