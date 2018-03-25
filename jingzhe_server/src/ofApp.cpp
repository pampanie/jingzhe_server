#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	receiver.setup(PORT);

}

//--------------------------------------------------------------
void ofApp::update(){
	getAudioData();
}

//--------------------------------------------------------------
void ofApp::draw(){

}
//--------------------------------------------------------------
void ofApp::exit(){
	
//--------------------------------------------------------------

}

void ofApp::getAudioData(){
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
		
		// check for mouse moved message
		if(m.getAddress() == RMS){
			rms = m.getArgAsInt(0);
		}
		// check for mouse button message
		else if(m.getAddress() == FFT){
			// the single argument is a string
			fftSize = m.getArgAsInt(0);
			for (int i = 0; i < fftSize; i++) {
				// 1st arg is fftsize
				fft.push_back(m.getArgAsFloat(i+1));
				cout << fft.at(i) << endl;
			}
	
		}
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// ===========  need better handle error info =========  TODO
			cout << msg_string << endl;
		}
		
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
