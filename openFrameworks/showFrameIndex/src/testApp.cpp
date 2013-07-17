#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	scaleX = ofGetWidth();
	scaleY = ofGetHeight();
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	float scale = min(scaleX / 80, scaleY / 10);
	ofScale(scale, scale, 1.0f);

	string text = ofToString(ofGetFrameNum());
	while (text.size() < 10) {
		text = "0" + text;
	}
	ofDrawBitmapString(text, 0, 10);
	
	const float w = 80.0f / 7.0f;
	ofSetColor(255, 0, 0);
	ofRect(0, 11, w, 10.0f);
	ofSetColor(0, 255, 0);
	ofRect(w, 11, w, 10.0f);
	ofSetColor(0, 0, 255);
	ofRect(w*2, 11, w, 10.0f);
	ofSetColor(255, 255, 0);
	ofRect(w*3, 11, w, 10.0f);
	ofSetColor(0, 255, 255);
	ofRect(w*4, 11, w, 10.0f);
	ofSetColor(255, 0, 255);
	ofRect(w*5, 11, w, 10.0f);
	ofSetColor(255, 255, 255);
	ofRect(w*6, 11, w, 10.0f);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	scaleX = w;
	scaleY = h;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}