#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup shader
    #ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
    #else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
    #endif
    
    frozenFramesCount = 0; //for debug
    
    camWidth = 1600;  // try to grab at this size.
    camHeight = 1200;
 
    screenX = 1920;
    screenY = 1200;
    paperA3ratio = 297./210.;
    camDisplayY = screenY;
    camDisplayX = int(camDisplayY*paperA3ratio); // = 1697px for a screen rez 1920x1200
    startX = 10;//int((float(screenX - camDisplayX))/4.0);
    startY = int((screenY - camDisplayY)/2.0); // = 0px
    
    ofBackground(0, 0, 0);
    ofHideCursor();
  
  // send a single string message, setting the log level
    ofLog(OF_LOG_NOTICE, "******************** Hello world of grabbers! ************************");  
    
    ////////// WEBCAM SETTINGS /////////////////////
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
    vidGrabber.setDesiredFrameRate(15);
    vidGrabber.initGrabber(camWidth, camHeight);

    ofSetVerticalSync(true);

    ////////// GUI SETTINGS /////////////////////

    gui.setup(); // most of the time you don't need a name
    gui.add(_hue.setup("hue", 0.5, 0., 1.));
    gui.add(_saturation.setup("saturation", 1., 0., 1.));
    gui.add(_brightness.setup("brightness", 1., 0., 2.));
    gui.add(_contrast.setup("contrast", 1., 0., 2.));
 
    gui.add(_Rgain.setup("Red", 1., 0., 2.));
    gui.add(_Ggain.setup("Green", 1., 0., 2.));
    gui.add(_Bgain.setup("Blue", 1., 0., 2.));
 
    gui.add(_planePosX.setup("posX", 0, -100, 100));
    gui.add(_planePosY.setup("posY", 0, -100, 100));
    gui.add(_planeScaleX.setup("scaleX", 1640, 800, screenX));
    gui.add(_planeScaleY.setup("scaleY", 1200, 600, screenY));
    // gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));
    gui.add(startTime.setup("started",  ofToString(ofGetHours()) + ":" + 
                                    ofToString(ofGetMinutes()) + ":" +
                                    ofToString(ofGetSeconds())));
    gui.add(currentTime.setup("now",  ofToString(ofGetHours()) + ":" + 
                                    ofToString(ofGetMinutes()) + ":" +
                                    ofToString(ofGetSeconds())));
    gui.add(frozenFrameCountLabel.setup("frozenFrameCountLabel", ofToString(frozenFramesCount)));

    // load settings at startup
    gui.loadFromFile("settings.xml");
    // hide GUI by default
    bHide = true;

    ////////// CREATE VIDEO PLANE /////////////////////

    //plane.set(camDisplayX, camDisplayY);   ///dimensions for width and height in pixels
    //plane.setPosition(-30, 50, 0); /// position in x y z
    plane.set(_planeScaleX, _planeScaleY);   ///dimensions for width and height in pixels
    plane.setPosition(_planePosX, _planePosY, 0); /// position in x y z
    plane.setResolution(2, 2); /// this resolution (as columns and rows) is enough
    //plane.mapTexCoords(0, camHeight/10, camWidth, 0);
    
    
}


//--------------------------------------------------------------ss
void ofApp::update(){

    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        if(!bHide){
            plane.set(_planeScaleX, _planeScaleY);   ///dimensions for width and height in pixels
            plane.setPosition(_planePosX, _planePosY, 0); /// position in x y z
            //plane.mapTexCoords(_planePosX, _planePosY, _planePosX+_planeScaleX, _planePosY+_planeScaleY);
            currentTime.setup("now",    ofToString(ofGetHours()) + ":" + 
                                        ofToString(ofGetMinutes()) + ":" +
                                        ofToString(ofGetSeconds()));
        }
        frozenFramesCount = 0;
    }
    else {
        frozenFramesCount += 1;
        frozenFrameCountLabel.setup("frozenFrameCountLabel", ofToString(frozenFramesCount));
   }
    
    if (frozenFramesCount >= 10){
        ofLog(OF_LOG_NOTICE,    ofToString(ofGetHours()) + ":" + 
                                ofToString(ofGetMinutes()) + ":" +
                                ofToString(ofGetSeconds()) + " - frozenFrameCount : " +
                                ofToString(frozenFramesCount));
        if (frozenFramesCount >= 200){
            ofLog(OF_LOG_NOTICE, "!!!!!!!!!!!!!!!!!! Frozen camera, exiting !!!!!!!!!!!!!!!!!!!!!");
            ofExit();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
    vidGrabber.bind();
    
    // start our shader, in our OpenGL3 shader this will automagically set
    // up a lot of matrices that we want for figuring out the texture matrix
    // and the modelView matrix
    shader.begin();
    
    // pass the parameters
    shader.setUniform3f("RGBgains", _Rgain, _Ggain, _Bgain);
    shader.setUniform4f("vHSVC", _hue, _saturation, _brightness, _contrast);
    shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    plane.draw();

    ofPopMatrix();
    
    shader.end();

    vidGrabber.unbind();
    
    
    /////////////////////////////////////////////////////
    //ofSetHexColor(0xffffff);
    
    // this is a shortcut if we jut do direct rendering to the window without using shaders
    // vidGrabber.draw(startX, startY, camDisplayX, camDisplayY);
    
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
    else if(key == 'q'){
	    ofExit();
    }
    else if(key == 'x'){
        snapshot.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        snapshot.save("screenshot.png");
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
