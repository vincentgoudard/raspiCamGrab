#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 1600;  // try to grab at this size.
    camHeight = 1200;
 
    screenX = 1980;
    screenY = 1080;
    paperA3ratio = 297./210.;
    camDisplayY = screenY;
    camDisplayX = int(camDisplayY*paperA3ratio);
    startX = int((screenX - camDisplayX)/2);
    startY = int((screenY - camDisplayY)/2);
    
    ofBackground(0, 0, 0);
    ofHideCursor();
    
    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);

    videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    ofSetVerticalSync(true);
    
    //setting up the GUI
    gui.setup(); // most of the time you don't need a name
	gui.add(scalebias.setup("scalebias", ofVec2f(1., 0.), ofVec2f(-5, -5), ofVec2f(5, 5)));
	gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
	gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));

	bHide = false;
}


//--------------------------------------------------------------
void ofApp::update(){

    vidGrabber.update();

    if(vidGrabber.isFrameNew()){
        ofPixels & pixels = vidGrabber.getPixels();
        for(size_t i = 0; i < pixels.size(); i++){
            //invert the color of the pixel
            // videoInverted[i] = 255 - pixels[i];
	    videoInverted[i] = (255.*scalebias->y) + int(pixels[i] * scalebias->x);
        }
        //load the inverted pixels
        videoTexture.loadData(videoInverted);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    // vidGrabber.draw(startX, startY, camDisplayX, camDisplayY);
    videoTexture.draw(startX, startY, camDisplayX, camDisplayY);
    
   	// auto draw?
	// should the gui control hiding?
	if(!bHide){
		gui.draw();
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // in fullscreen mode, on a pc at least, the 
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...

    // Video settings no longer works in 10.7
    // You'll need to compile with the 10.6 SDK for this
    // For Xcode 4.4 and greater, see this forum post on instructions on installing the SDK
    // http://forum.openframeworks.cc/index.php?topic=10343
    //if(key == 's' || key == 'S'){
    //    vidGrabber.videoSettings();
    //}
    
    if(key == 'h'){
		bHide = !bHide;
		if (bHide) {ofHideCursor();} else {ofShowCursor();}
	}
	else if(key == 's'){
		gui.saveToFile("settings.xml");
	}
	else if(key == 'l'){
		gui.loadFromFile("settings.xml");
	}
	else if(key == ' '){
		color = ofColor(255);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
