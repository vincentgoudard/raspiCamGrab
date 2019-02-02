#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
	ofSetupOpenGL(1920,1080, OF_WINDOW);			// <-------- setup the GL context

	ofGLESWindowSettings settings;
	settings.glesVersion = 2;
	settings.width = 1920;
	settings.height = 1080;

	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
