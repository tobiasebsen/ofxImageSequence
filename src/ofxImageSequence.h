//
//  ImageSequence.h
//  plankton
//
//  Created by Tobias Ebsen on 20/03/15.
//
//

#pragma once

#include "ofMain.h"

class ofxImage {
public:
	ofxImage();

	ofPixels & getPixelsRef();
	ofTexture & getTextureReference();

	void loadPixels(string path);
	bool isPixelsLoaded();

private:
	ofPixels pixels;
	ofTexture tex;
	bool bIsLoaded;
};

class ofxImageSequence : public ofThread {
public:
	~ofxImageSequence();
    
    virtual void load(string path, float framerate = 30.f, bool useRam = false);
	virtual void loadAsync(string path, float framerate = 30.f, bool useRam = false);

	virtual void draw(int frame);
	virtual void draw(int frame, float x, float y, float z = 0.f);
    virtual void drawCentered(int frame);
    virtual void drawCentered(int frame, float x, float y, float z = 0.f);
    
    int getWidth() { return width; }
    int getHeight() { return height; }
	string getPath() { return path; }
    
    int getNumFrames() { return images.size(); }
    float getFramerate() { return framerate; }
    float getDuration() { return (float)images.size() / framerate; }

	void threadedFunction();

	class CompleteEventArgs : public ofEventArgs {
	public:
		string path;
		ofxImageSequence* sequence;
	};
	ofEvent<CompleteEventArgs> onLoadComplete;

protected:
    int width;
    int height;
    float framerate;
	string path;
	bool useRam;

private:
    vector<ofxImage> images;
};

class ofxImageSequencePlayer {
public:
    
    void setup(ofPtr<ofxImageSequence> & sequence);
    void play();
    void setPosition(float percent);// { this->offset = percent; }

	ofxImageSequence* getSequence();
    
    void update();
    void draw();
    void draw(float x, float y, float z);
    void drawCentered();
    
    void setLoop(bool loop) { this->loop = loop; }
    bool isPlaying() { return loop || currentFrame < sequence->getNumFrames()-1; }

private:
    ofPtr<ofxImageSequence> sequence;
    float startTime;
    int currentFrame;
    float offset;
    bool loop;
};