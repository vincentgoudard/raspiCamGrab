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
    
    camWidth = 1600;  // try to grab at this size.
    camHeight = 1200;
 
    screenX = 1920;
    screenY = 1200;
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
    vidGrabber.setDesiredFrameRate(15);
    vidGrabber.initGrabber(camWidth, camHeight);

    //videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    //videoTexture.allocate(videoInverted);
    ofSetVerticalSync(true);
    
    //setting up the GUI
    gui.setup(); // most of the time you don't need a name
    gui.add(scalebias.setup("scalebias", ofVec2f(1., 0.), ofVec2f(-5, -5), ofVec2f(5, 5)));
    gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));

    bHide = true;

    plane.set(camDisplayX, camDisplayY);   ///dimensions for width and height in pixels
    plane.setPosition(startX, startY, 0); /// position in x y z
    plane.setResolution(2, 2); /// this resolution (as columns and rows) is enough
    plane.mapTexCoords(0, 0, camWidth, camHeight);
}


//--------------------------------------------------------------
void ofApp::update(){

    vidGrabber.update();

    //if(vidGrabber.isFrameNew()){
    //    ofPixels & pixels = vidGrabber.getPixels();
    //    for(size_t i = 0; i < pixels.size(); i++){
            //invert the color of the pixel
            // videoInverted[i] = 255 - pixels[i];
	//    videoInverted[i] = (255.*scalebias->y) + int(pixels[i] * scalebias->x);
    //    }
        //load the inverted pixels
    //    videoTexture.loadData(videoInverted);
    //}
    
    //videoTexture = vidGrabber.getTexture();
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
    
    // get mouse position relative to center of screen
    //float mousePositionX = ofMap(mouseX, 0, ofGetWidth(), 1.0, -1.0, true);
    //float mousePositionY = ofMap(mouseY, 0, ofGetHeight(), 1.0, -1.0, true);

// ajoute par VG
// #ifndef TARGET_OPENGLES
//    // when texture coordinates are normalised, they are always between 0 and 1.//
//    // in GL2 and GL3 the texture coordinates are not normalised,
//    // so we have to multiply the normalised mouse position by the plane width.
   //mousePositionX *= ofGetWidth();
   //mousePositionY *= ofGetHeight();

// #endif


    //shader.setUniform1f("mouseX", mousePositionX);
    //shader.setUniform1f("mouseY", mousePositionY);
    
    shader.setUniform4f("rgbaGains", scalebias->x, scalebias->y, 0.5, 1.);
    shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    plane.draw();

    ofPopMatrix();
    
    shader.end();

    vidGrabber.unbind();
    
    
    /////////////////////////////////////////////////////
    ofSetHexColor(0xffffff);
    // vidGrabber.draw(startX, startY, camDisplayX, camDisplayY);
    //videoTexture.draw(startX, startY, camDisplayX, camDisplayY);
    
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
