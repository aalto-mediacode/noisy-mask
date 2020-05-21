//Noisy Narcissus - The Noise Behind the Mask - May 2020
//A reactive face tracking program that reacts and visualises different amplitudes of different frequency bands coming from microphone input

//Made by Lu Priace Michavila - a.k.a Lucio Celomundo
//this is part of the course Generative Media Coding 2020 at Aalto University taught by Matti Niinimäki and Nuno Correia

//this code uses parts of the code of a Facetracking example by Matti Niinimäki - https://github.com/Mnstri
//it also uses some of the code of FFT analysis visualisation by Nuno Correia
//https://github.com/nunocorreia

//it also uses code from the FFT-Audio In program by EDAP: https://github.com/edap/examplesOfxMaxim/tree/master/04-FFT-AudioIn

//the program uses some external addones such as ofxCV, ofxFacetracker, ofxGui, ofxMaxim and ofxOpenCv


#include "ofApp.h"

//important to make use of names coming from ofxCV library
using namespace ofxCv;

void ofApp::setup(){
    
    
    
    
    //shows the camera in the log
    vector<ofVideoDevice> devices = cam.listDevices();
    for(int i=0; i < devices.size(); i++)
    {
        ofLogNotice() << devices[i].id << ":" << devices[i].deviceName;
    }
    
    //camera ID in this case 0
    cam.setDeviceID(0);
    //setup of the frame dimensions where the video capture will appear
    cam.initGrabber(1280, 720);
   
    //the tracker is setup
    tracker.setup();
    //camera stuff finishes
    
    
    
    //FFT example from edap starts
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofBackground(0,0,0);
    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */

    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    // the higher the value, the more accurate will be the fft analysis
    fftSize = 1024;
    fft.setup(fftSize, 512, 256);
    /* this has to happen at the end of setup - it switches on the DAC */
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
    
    //FFT example edapt finishes
    
    //smoothing Factor to ease the output of values
    smoothingFactor = 0.5;
    
    //ofSetWindowShape(1440, 900);
    //ofSetWindowPosition(500, 500);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //camera is updated
    cam.update();
    
    //camera starts this every frame
    if(cam.isFrameNew())
    {
        //camera connects to computer vision
        tracker.update(toCv(cam));
        //if tracker find a face to track this process starts
        if(tracker.getFound()){
            //facePoints becomes the points in the face found by the camera
            facePoints = tracker.getImagePoints();
            //facePos is the position of the face in the screen
            facePos = tracker.getPosition();
            //this resizes Psizes to the facePoints sizes
            
            pSizes.resize(facePoints.size());
            //this checks the JAW OPENNESS gesture from the facetracker, in case I want to use face gestures
            //mouthHeight= tracker.getGesture(ofxFaceTracker::JAW_OPENNESS);
            
            
            
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
   //set color to white to make the image captured not to be affected by the color definition later
    ofSetColor(255);
    //camera draws 0
    cam.draw(0,0);
    
    //FFT example edap starts
       
    //if the tracker finds a face to track the points
    if(tracker.getFound()){
        
        //numObjects defines the amount of facepoints in the face in this case 60
        int numObjects = facePoints.size();
        //this is to make a jump of 60 dividing the fftSize of 1024/60
        
        
        //the for loop goes through all the 60 Facepoints and constantly change the radius of the spheres based in the amplitude of 60 frequency bands
       for(int i = 0; i < numObjects ;i++){
           
         
           //this sets the value of the RED, GREEN and BLUE contents
           // the color palette is altered by the movement in the X axis
           ofSetColor(ofMap(i,0,numObjects/2,200,255),ofMap(facePos.x,0,ofGetWidth(),0,100),ofMap(i,0,numObjects/2,25,255));
           
           //this variable stores the values of each individual amplitude of the 60 different bands and multiplies to the Height of the screen divided by 500 to make a circle radius value easier to show in the screen
           // An improvement could be to have control over the radius in case the incoming sound is too loud
           float radius = (fft.magnitudes[i]) * ofGetHeight()/1000;
           
           
           
           //this draw a circle in each of the face points with a radius with values coming from the different frequency bands amplitudes
           ofDrawCircle(facePoints[i],radius);
           //prints the radius value
           ofLogNotice() << "RADIUS: " << radius;
           
           //an improvement could be playing with other shapes, even 3D particles, lines, spirals etc...
           //playing with forces, gravity and acceleration could bring more interesting effects
           //a system to control the type of shapes or particles that are altered by the sound could work as way to make a music performance more interesting: every song could have a repertoire of different particles and different behaviour
           
       }
    
    }
    
   
    
    
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    //FFT example edap starts
    //methods to get live audio input
    double input0, input1;
    for (int i = 0; i < bufferSize; i++){
        //fft
        input0 = input[i*2    ];
        input1 = input[i*2 + 1];

        if(fft.process(input0)){
            fft.magsToDB();
        }
        if(fft.process(input1)){
            fft.magsToDB();
        }
    }
    //FFT example edap starts
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
