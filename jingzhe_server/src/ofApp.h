#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxOsc.h"

#include "ofxFlowTools.h"
#include "MyFlowTools.h"

#include "ofxSyphon.h"
// listen on port 12345
#define PORT 12345

// protocol for osc address
#define FFT 				"/fft"
#define RMS					"/rms"
#define CONTOURCENTROID		"/cen"
#define CONTOURBOUNDINGBOX	"/bb"
#define CONTOURAREA			"/area"


class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
//	void drawGui(ofEventArgs & args);
	void exit();
	
	void getAudioData();
	void drawAudioData();
	void updateByOSCData();

	ofParameter<bool>	toggleGuiDraw;

	
	ofxOscReceiver 		receiver;
	
	ofParameter<int>	fftInfoX;
	ofParameter<int>	fftInfoY;
	int					fftSize;
	vector<float>		fft;
	float				rms;
	ofParameter<int>   	rmsFactor;	
	
	// contour data
	void				drawContourData();
	ofPoint				contourCentroid;
	float				contourArea;
	ofRectangle			contourBoundingBox;
	
	ofParameter<int>	contourInfoX;
	ofParameter<int>	contourInfoY;
	ofColor				particleColor;
	int					particleWindSpeed;
	int					particleWindSpeedLast;
	int 				particleWindSpeedDelta;
	ofParameter<int>	particleWindSpeedThreshold;
	ofParameter<int>	particleWindSpeedDamping;
	string				syphonServerName;
	ofxSyphonClient		syphonClient;
	
	
	ofFbo				kinect1Fbo;
	ofFbo				kinect2Fbo;
	ofFbo				animateFbo;
	
	MyFlowTools			myFlowTools1;
	MyFlowTools			myFlowTools2;
	
	int					flowWidth;
	int					flowHeight;
	int					drawWidth;
	int					drawHeight;
	int					ratio;
	// GUI
	ofxPanel			gui;
	void				setupGui();
	void 				drawGui();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
