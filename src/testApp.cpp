#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(200, 200, 200);
	ofSetVerticalSync(false);
    ofSetFrameRate(90);
    
    ofSetDataPathRoot("../Resources/data/");
    
	//Connect to Port
//	myTuio.start(7001);
//	myTuio.start(7001);
    
//    for (int i=0; i<TUIO_NUM; i++) {
//        ofxTuioClient * tmpTuio = new ofxTuioClient();
//        Tuios.push_back(tmpTuio);
//        Tuios[i]->start(TUIO_PORT+i);
//    }
    
    bControl = bClicked = currentClicked = false;
    pos = currentPos = ofPoint(0,0);
	
    bShowGui = true;
    bEnabled = false;
    
    Width = SCREEN_W, Height = SCREEN_H;
    
    guiScreenNumber = screenNumber = TUIO_NUM;

    gui = new ofxUISuperCanvas("conf", 0,0,50,50);
    gui->setGlobalSliderHeight(5);
    //    gui->addTextInput("host", udpHost, 50.f);
    gui->addToggle("active", &bControl);
//    gui->addToggle("invert", &bInvert);
//    gui->addToggle("switch XY", &bSwitchXY);
    //    gui->addNumberDialer("pix", 0.5, 10, &pixelsInLed, 1)->setDrawOutline(true);
    gui->addNumberDialer("total width", 100, 10000, &Width, 0);
    gui->addNumberDialer("total height", 100, 10000, &Height, 0);
    
    gui->addNumberDialer("num screns", 1, 10, &guiScreenNumber, 0);
    
    gui->loadSettings("GUI.xml");
    gui->autoSizeToFitWidgets();
    gui->getFontSmall()->loadFont("GUI/Helvetica.ttf", 8);

    gui->setPosition(10, 10);

    gui->disableAppDrawCallback();
    gui->setAutoUpdate(true);
    
    setupScreens();
}

//--------------------------------------------------------------
void testApp::setupScreens(){

    screenNumber = static_cast<unsigned int>(guiScreenNumber);
    Screens.clear();
    for (int i=0; i<screenNumber; i++){
        tuioScreen * tmpScreen = new tuioScreen(i+1,TUIO_PORT+i,Width/screenNumber, Height, ofPoint(Width/screenNumber*i, 0));
        Screens.push_back(tmpScreen);
    }

}

//--------------------------------------------------------------
void testApp::update(){

    bool wasTouched = false;
//    for (int i=0; i<Tuios.size(); i++) {
//        Tuios[i]->getMessage();
//        list<ofxTuioCursor*> list = Tuios[i]->getTuioCursors();
//        
//        if (list.size() > 0) {
//            
//            for (std::list<ofxTuioCursor*>::iterator it=list.begin(); it != list.end(); it++) {
//        //        (*it)->getSessionId()
////                ofLog(OF_LOG_NOTICE, ofToString((*it)->getSessionId())+":  X= "+ofToString((*it)->getPosition().getX())+ " Y= "+ofToString((*it)->getPosition().getY()));
//                pos = ofPoint(Width/TUIO_NUM*i+(*it)->getPosition().getX()*Width/TUIO_NUM, (*it)->getPosition().getY()*Height);
//                wasTouched = true;
//                controlMouse(pos, true);
//                break;
//            
//            }
//        }
//    }
    if (Screens.size() != static_cast<size_t>(guiScreenNumber))
        setupScreens();

    for (int i=0; i<Screens.size(); i++) {
        pos = Screens[i]->getTouchPos();
        if (pos != ofPoint(-1,-1)){
            wasTouched = true;
            controlMouse(pos, true);
            break;
        }
    }
    
    if (!wasTouched)
        controlMouse(pos, false);
}

//--------------------------------------------------------------
void testApp::draw(){
	
	//render TUIO Cursors and Objects
//	myTuio.drawCursors();
//	myTuio.drawObjects();
    
    ofSetColor(255, 255,255,255);
    ofRect(mousePosOF, 20, 20);

    float scaleX = ofGetWidth()/Width;
    float scaleY = ofGetHeight()/Height;
    for (int i=0; i<Screens.size(); i++)
        Screens[i]->draw(scaleX, scaleY);
    
    if (bShowGui) gui->draw();
    
    ofSetWindowTitle("FPS: " + ofToString((int)ofGetFrameRate()));
}

void testApp::controlMouse(ofPoint pos, bool isClicked) {
    
    if (!bControl) return;
    
    CGPoint pt;
    pt.x = pos.x; pt.y = pos.y;
    
    if (currentClicked != isClicked) {
        currentClicked = isClicked;
        if (isClicked) {
//            //            cout << "Mouse Down!!+++++++" << endl;
            CGEventRef mouseDownEv = CGEventCreateMouseEvent (NULL,kCGEventLeftMouseDown,pt,kCGMouseButtonLeft);
            CGEventPost (kCGHIDEventTap, mouseDownEv);
            CFRelease(mouseDownEv);
            ofLog(OF_LOG_NOTICE, "DOWN");
        } else {
            //            cout << "Mouse Up!!+++++++-----------" << endl;
            CGEventRef mouseUpEv = CGEventCreateMouseEvent (NULL,kCGEventLeftMouseUp,pt,kCGMouseButtonLeft);
            CGEventPost (kCGHIDEventTap, mouseUpEv );
            CFRelease(mouseUpEv);
            ofLog(OF_LOG_NOTICE, "UP");
        }
    }
    
    if (currentPos != pos) {
        currentPos = pos;
        CGEventRef mouseMoveEv;
        if (currentClicked) {
            mouseMoveEv = CGEventCreateMouseEvent (NULL,kCGEventLeftMouseDragged,pt,kCGMouseButtonLeft) ;
        }
        else {
            mouseMoveEv = CGEventCreateMouseEvent (NULL, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
        }
        CGEventPost (kCGHIDEventTap, mouseMoveEv);
        CFRelease(mouseMoveEv);
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (key == ' ') bControl = !bControl;
    
    if (key == 's') {
        gui->saveSettings("GUI.xml");
        for (int i=0; i<Screens.size(); i++) Screens[i]->save();
    }
    else if (key == 'l') {
        gui->loadSettings("GUI.xml");
        for (int i=0; i<Screens.size(); i++) Screens[i]->load();
    }
    
    if (key == 'g') {
        bShowGui = !bShowGui;
        for (int i=0; i<Screens.size(); i++) Screens[i]->showGui(bShowGui);
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    mousePosOF = ofPoint(x,y);

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


void testApp::touchDown(ofTouchEventArgs & touch){
	cout << " cursor added: " + ofToString(touch.id)+
	" X: "+ofToString(touch.x)+
	" Y: "+ofToString(touch.y)
	<< endl;
    controlMouse(ofPoint(touch.x, touch.y), true);
	
}

void testApp::touchUp(ofTouchEventArgs & touch){
	ofLog(OF_LOG_VERBOSE, " cursor removed: " + ofToString(touch.id)+
	 " X: "+ofToString(touch.x)+
	 " Y: "+ofToString(touch.y)
      );
    controlMouse(ofPoint(touch.x, touch.y), false);
}

void testApp::touchMoved(ofTouchEventArgs & touch){
	cout << " cursor updated: " + ofToString(touch.id)+
	 " X: "+ofToString(touch.x)+
	 " Y: "+ofToString(touch.y)
	 << endl;
    
    controlMouse(ofPoint(touch.x, touch.y), true);
}

