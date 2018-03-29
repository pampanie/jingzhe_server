#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	receiver.setup(PORT);
	
	drawWidth = 1920;
	drawHeight = 1080;
	flowWidth = 640;
	flowHeight = 360;
	
	ratio = 4;
	
//	syphonServerName = "Composition";//==================  TODO
	syphonServerName = "Syphon2NDIClient";//==================  TODO
	syphonClient.setup();
	syphonClient.setServerName(syphonServerName);
	
	
	// init fbos
	kinect1Fbo.allocate(flowWidth, flowHeight);
	kinect2Fbo.allocate(flowWidth, flowHeight);
	animateFbo.allocate(flowWidth, flowHeight);
	
	// int MyFLowTools
	myFlowTools1.setup(drawWidth, drawHeight, ratio,"myFlow1");
	myFlowTools2.setup(drawWidth, drawHeight, ratio,"myFlow2");
	
	// init fft
	for (int i = 0; i < 128; i ++) {
		fft.push_back(0.0f);
	}
	setupGui();
	
	// countour
	contourArea = 0.0f;
	contourCentroid.set(0, 0);
	contourBoundingBox.set(0, 0, 0, 0);
	
	particleWindSpeedLast = 0;
	particleWindSpeedDelta = 0;

}

//--------------------------------------------------------------
void ofApp::setupGui() {
	
	gui.setup("settings");
	gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
	gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
	//	gui.add(guiFPS.set("average FPS", 0, 0, 60));
	//	gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
	gui.add(toggleGuiDraw.set("show gui (G)", false));
	gui.add(rmsFactor.set("rms rect height",1,10,100));
	
	//	gui.add(doFlipCamera.set("flip camera", true));
	//	gui.add(doDrawCamBackground.set("draw camera (C)", true));
	
	
	
	gui.add(fftInfoX.set("fft data x",0,50,drawWidth));
	gui.add(fftInfoY.set("fft data y",0,50,drawHeight));
	
	
	gui.add(contourInfoX.set("contour data x",0,50,drawWidth));
	gui.add(contourInfoY.set("contour data y",0,50,drawHeight));
	
	
	gui.add(particleWindSpeedThreshold.set("particle wind threshold",1,10,400));
	gui.add(particleWindSpeedDamping.set("particle wind damping",1,10,400));
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	getAudioData();
	updateByOSCData();
	
	// not sure if update both drummer and animate particle ========  TODO
	myFlowTools1.setParticleSpeedX(particleWindSpeed);
	myFlowTools1.setParticleSpeedX(particleWindSpeed);

	ofTexture syphonTex = syphonClient.getTexture();
	
	// update kinect1 texture
	kinect1Fbo.begin();
	syphonTex.draw(0,0, flowWidth, flowHeight);
	kinect1Fbo.end();
	
	// update kinect2 texture
	kinect2Fbo.begin();
	syphonTex.draw(flowWidth,0, flowWidth, flowHeight);
	kinect2Fbo.end();
	
	// update animate texture
	animateFbo.begin();
	ofSetColor(255);
	ofDrawRectangle(0, drawHeight - rms * rmsFactor.get(), drawWidth,rms * rmsFactor.get());
	animateFbo.end();
	
	//	// update particle color
	//	int r = 0;
	//	int g = 0;
	//	int b = 0;
		myFlowTools1.setParticleColor(particleColor);
	//
	//	r = 255;
	//	g = 255;
	//	b = 255;
		myFlowTools2.setParticleColor(particleColor);
	//
	//
	// MyFlowTools update
	myFlowTools2.update(&animateFbo, &kinect2Fbo);
	myFlowTools1.update(&kinect1Fbo, &kinect2Fbo);
	
}
//--------------------------------------------------------------
void ofApp::updateByOSCData(){
	particleColor = ofColor(
							ofMap(contourBoundingBox.getWidth(),1,100,1,255,true),
							ofMap(contourBoundingBox.getHeight(),1,100,1,255,true),
							ofMap(contourBoundingBox.getCenter().x,1,640,1,255,true)
							);
	
	particleWindSpeedDelta = contourCentroid.x - particleWindSpeedLast;
	particleWindSpeedLast = contourCentroid.x;
	if(abs(particleWindSpeedDelta) > particleWindSpeedThreshold.get() ){
		if(particleWindSpeed > 0){
			particleWindSpeed -= particleWindSpeedDamping.get();
		}
		if(particleWindSpeed < 0){
			particleWindSpeed += particleWindSpeedDamping.get();
		}
	}
	
	
	
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0,0);
	
	myFlowTools1.draw();
	myFlowTools2.draw();
//	syphonClient.draw(0,0);
	if(toggleGuiDraw.get()){
		drawGui();
		myFlowTools1.drawGui();
		myFlowTools2.drawGui();
	}
	
	
	// debug osc receive
	//	ofSetColor(255);
	//	ofDrawBitmapString(ofToString(fft.at(60)), 10, 200);
}
//--------------------------------------------------------------
//void ofApp::drawGui(ofEventArgs & args){
//	drawGui();
//}
//--------------------------------------------------------------
void ofApp::drawGui(){
	gui.draw();
	drawAudioData();
	drawContourData();
	
}
//--------------------------------------------------------------
void ofApp::drawContourData(){
	string info = "";
	info += "FPS: " + ofToString(ofGetFrameRate()) + "\n\n";
	
	info += "contour centroid : " +
	ofToString(contourCentroid.x) +
	"," +
	ofToString(contourCentroid.y) +
	"\n";
	
	info += "contour area : " + ofToString(contourArea) + "\n";
	
	info += "contour boudingbox x : " +
	ofToString(contourBoundingBox.x) +
	" , y : " +
	ofToString(contourBoundingBox.y) +
	" , w : " +
	ofToString(contourBoundingBox.width) +
	" , h : " +
	ofToString(contourBoundingBox.height) +
	"\n";
	
	ofSetColor(255);
	ofDrawBitmapString(info, contourInfoX.get(), contourInfoY.get());
	
}
//--------------------------------------------------------------
void ofApp::drawAudioData(){
	ofSetColor(255,0,0);
	for(int i = 0;i < fft.size();i++){
		
		ofDrawRectangle(fftInfoX.get() + 5 * i, fftInfoY.get() - fft.at(i) * rmsFactor.get(), 5, fft.at(i) * rmsFactor.get());
		
	}
	
	
	
}
//--------------------------------------------------------------
void ofApp::exit(){
	
	receiver.stop();
	
}
//--------------------------------------------------------------

void ofApp::getAudioData(){
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
		
		if(m.getAddress() == RMS){
			rms = m.getArgAsInt(0);
			//			cout << rms << endl;
		}
		else if(m.getAddress() == FFT){
			// the single argument is a string
			fftSize = m.getArgAsInt(0);
			for (int i = 0; i < fftSize; i++) {
				// 1st arg is fftsize
				fft.at(i) = m.getArgAsFloat(i+1);
			}
			
		}
		else if(m.getAddress() == CONTOURAREA){
			contourArea = m.getArgAsFloat(0);
		}
		else if(m.getAddress() == CONTOURCENTROID){
			contourCentroid.set(m.getArgAsFloat(0), m.getArgAsFloat(1));
		}
		else if(m.getAddress() == CONTOURBOUNDINGBOX){
			contourBoundingBox.set(
								   // x
								   m.getArgAsFloat(0),
								   // y
								   m.getArgAsFloat(1),
								   // w
								   m.getArgAsFloat(2),
								   // h
								   m.getArgAsFloat(3));
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
	switch (key) {
		case 'G':
		case 'g':
			toggleGuiDraw = !toggleGuiDraw; break;
		default:
			break;
	}
	
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
