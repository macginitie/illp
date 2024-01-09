#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	frameByframe = false;

	// cut point between black & white
	threshold = 0.5;

	// Uncomment this to show movies with alpha channels
	// fingerMovie.setPixelFormat(OF_PIXELS_RGBA);

	//fingerMovie.loadMovie("movies/fingers.mov");
	//fingerMovie.loadMovie("movies/scout.mov");
	fingerMovie.loadMovie("movies/scout.mp4");
	fingerMovie.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerMovie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xFFFFFF);

    fingerMovie.draw(20,20); // magic #s for border size
    unsigned char * pixels = fingerMovie.getPixels();
	int width = fingerMovie.getWidth();
    int height = fingerMovie.getHeight();
    int nChannels = fingerMovie.getPixelsRef().getNumChannels();
    
    // let's move through the "RGB(A)" char array
    // using the brightness to control the size of a circle.
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            unsigned char r = pixels[(j * width + i)*nChannels];
            unsigned char g = pixels[(j * width + i)*nChannels + 1];
            unsigned char b = pixels[(j * width + i)*nChannels + 2];
            float val = 1 - ((float)(r+g+b) / 765.0f);
			if (val > threshold)
			{
				ofSetHexColor(0x000000);	
			}
			else
			{
				ofSetHexColor(0xffffff);	
			}
			// 32 = 20 + a 12-pixel vertical gutter for spacing
			ofCircle(width + 32 + i, 20+j, 1);
        }
    }

    ofSetHexColor(0x000000);
	ofDrawBitmapString("press f to change",20,320);
    if(frameByframe) ofSetHexColor(0xCCCCCC);
    ofDrawBitmapString("mouse speed position",20,340);
    if(!frameByframe) ofSetHexColor(0xCCCCCC); else ofSetHexColor(0x000000);
    ofDrawBitmapString("keys <- -> frame by frame " ,190,340);
    ofSetHexColor(0x000000);

    ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/"+ofToString(fingerMovie.getTotalNumFrames()),20,380);
    ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition()*fingerMovie.getDuration(),2) + "/"+ofToString(fingerMovie.getDuration(),2),20,400);
    ofDrawBitmapString("speed: " + ofToString(fingerMovie.getSpeed(),2),20,420);

    if(fingerMovie.getIsMovieDone()){
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie",20,440);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case 'f':
            frameByframe=!frameByframe;
            fingerMovie.setPaused(frameByframe);
			break;
        case OF_KEY_LEFT:
            fingerMovie.previousFrame();
			break;
        case OF_KEY_RIGHT:
            fingerMovie.nextFrame();
			break;
        case '0':
            fingerMovie.firstFrame();
			break;
		case '+':
			if (threshold < 0.9)
			{
				threshold += 0.1;
			}
			break;
		case '-':
			if (threshold >= 0.1)
			{
				threshold -= 0.1;
			}
			break;
		default:
			break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        float speed = (2 * pct - 1) * 5.0f;
        fingerMovie.setSpeed(speed);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        fingerMovie.setPosition(pct);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(!frameByframe){
        fingerMovie.setPaused(true);
	}
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(!frameByframe){
        fingerMovie.setPaused(false);
	}
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
