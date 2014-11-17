//
//  tuioScreen.cpp
//  tuioMultiscreen
//
//  Created by Tim TVL on 16/11/14.
//
//

#include "tuioScreen.h"

tuioScreen::tuioScreen(unsigned int __id, unsigned int _port, unsigned int _width, unsigned int _height, ofPoint _pos):
    _id(__id), port(_port),
    region(_pos.x, _pos.y, _width, _height),
    bInvertX(false), bInvertY(false), bSwitchXY(false), bEnabled(false), bClicked(false), bShowGui(true)
{
    drawRegion = region;
    guiPort = port;
    
    gui = new ofxUISuperCanvas(ofToString(_id), 0,0,50,50);
    gui->setGlobalSliderHeight(5);
    //    gui->addTextInput("host", udpHost, 50.f);
    gui->addToggle("active", &bEnabled);
    gui->addToggle("invert X", &bInvertX);
    gui->addToggle("invert Y", &bInvertY);
    gui->addToggle("switch XY", &bSwitchXY);
    gui->addNumberDialer("port", 1000, 10000, &guiPort, 0);
    //    gui->addNumberDialer("pix", 0.5, 10, &pixelsInLed, 1)->setDrawOutline(true);
    gui->addNumberDialer("x", 0, 5000, &region.x, 0);
    gui->addNumberDialer("y", 0, 5000, &region.y, 0);
    gui->addNumberDialer("w", 100, 5000, &region.width, 0);
    gui->addNumberDialer("h", 100, 5000, &region.height, 0);

    
    gui->disableAppDrawCallback();
    gui->setAutoUpdate(true);
    
    load();

    gui->autoSizeToFitWidgets();
    gui->getFontSmall()->loadFont("GUI/Helvetica.ttf", 8);
    
    Tuio.start(port);
    
    ofAddListener(ofEvents().exit, this, &tuioScreen::exit);
    startThread();
//    ofAddListener(gui->newGUIEvent, this, &Column::guiEvent);
    ofAddListener(ofEvents().mousePressed, this, &tuioScreen::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &tuioScreen::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &tuioScreen::mouseReleased);
}

tuioScreen::~tuioScreen(){;;}

void tuioScreen::setPosition(const ofPoint _pos) {
    region.x = _pos.x;
    region.y = _pos.y;
}

void tuioScreen::setWidth(const int width) {
    region.width = drawRegion.width = width;

}

void tuioScreen::setHeight(const int height){
    region.height = drawRegion.height =  height;
    
}

void tuioScreen::update(){
    if (port != static_cast<unsigned int>(guiPort)) {
        oscLock.lock();
        port = static_cast<unsigned int>(guiPort);
        Tuio.start(port);
        oscLock.unlock();
    }
    Tuio.getMessage();

    list<ofxTuioCursor*> list = Tuio.getTuioCursors();
    
    if (list.size() > 0) {
        
        for (std::list<ofxTuioCursor*>::iterator it=list.begin(); it != list.end(); it++) {
            //        (*it)->getSessionId()
            //                ofLog(OF_LOG_NOTICE, ofToString((*it)->getSessionId())+":  X= "+ofToString((*it)->getPosition().getX())+ " Y= "+ofToString((*it)->getPosition().getY()));
            float x = region.x+(bInvertX ? 1-(*it)->getPosition().getX() : (*it)->getPosition().getX())*region.width;
            float y = region.y+(bInvertY ? 1-(*it)->getPosition().getY() : (*it)->getPosition().getY())*region.height;
            touchPos = bSwitchXY ? ofPoint(y, x) : ofPoint(x, y);
            break;
        }
    }
    else {
        touchPos = ofPoint(-1,-1);
    }
    list.clear();
}

const ofPoint tuioScreen::getTouchPos() {
    return touchPos;
}

void tuioScreen::draw(float scaleX, float scaleY) {
    ofNoFill();
    ofSetLineWidth(5);
    drawRegion = region;
    drawRegion.scale(scaleX, scaleY);
    drawRegion.x *= scaleX;
    drawRegion.y *= scaleY;

    ofSetColor(_id % 1 == 0?255:0, _id % 2 == 0?255:0, _id % 3 == 0?255:0);
    ofRect(drawRegion);
    ofFill();

    if (bShowGui) {
        gui->enable();
        gui->setPosition(drawRegion.x+drawRegion.width/2-gui->getRect()->getHalfWidth(), drawRegion.height - gui->getRect()->getHeight());
        gui->draw();
    } else {
        gui->disable();
    }

}
void tuioScreen::save() {
    gui->saveSettings("screen-"+ofToString(_id));
}
    
void tuioScreen::load() {
    gui->loadSettings("screen-"+ofToString(_id));
}

void tuioScreen::mousePressed(ofMouseEventArgs & mouse) {
    if (!drawRegion.inside(mouse.x,mouse.y)) return;
    if (gui->getRect()->inside(mouse.x,mouse.y)) return;
    bClicked = true;
    mouseClickPos = ofVec2f(mouse.x,mouse.y);
    regionClickPos = ofVec2f(region.x, region.y);
}

void tuioScreen::mouseDragged(ofMouseEventArgs & mouse) {
    if (!bClicked) return;
    
    ofVec2f mouseDragDist = ofVec2f(mouse.x,mouse.y)-mouseClickPos;
    region.x = regionClickPos.x+mouseDragDist.x;
    region.y = regionClickPos.y+mouseDragDist.y;
}

void tuioScreen::mouseReleased(ofMouseEventArgs & mouse) {
    bClicked = false;
}


void tuioScreen::threadedFunction(){
    while(isThreadRunning()){
        lock();
//        oscLock.lock();
        update();

//        oscLock.unlock();
        unlock();
        
        ofSleepMillis(1);
    }
}

void tuioScreen::exit(ofEventArgs& e){
    lock();
    
    ofRemoveListener(ofEvents().exit, this, &tuioScreen::exit);
    gui->disable();

    unlock();
    
    //	ofLogNotice("DurationController") << "waiting for thread on exit";
    waitForThread(true);
}