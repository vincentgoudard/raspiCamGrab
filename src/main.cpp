#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
	//ofSetupOpenGL(1920,1080, OF_WINDOW);			// <-------- setup the GL context

	ofGLESWindowSettings settings;
	settings.glesVersion = 2;
	settings.setSize(1680, 1050);
	//settings.setSize(1920,1200);
	//settings.setSize(640,480);
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
