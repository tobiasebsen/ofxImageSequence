//
//  ImageSequence.h
//  plankton
//
//  Created by Tobias Ebsen on 20/03/15.
//
//

#pragma once

#include "ofMain.h"


class ofxImageSequence {
public:
    
    virtual void load(string path, float framerate = 30.f);
    virtual void draw(int frame);
    virtual void drawCentered(int frame);
    
    int getWidth() { return width; }
    int getHeight() { return height; }
    
    int getNumFrames() { return images.size(); }
    float getFramerate() { return framerate; }
    float getDuration() { return (float)images.size() / framerate; }

protected:
    int width;
    int height;
    float framerate;

private:
    vector<ofTexture> images;
};


class ofxImageSequenceRAM : public ofxImageSequence {
public:

    void load(string path, float framerate = 30.f);
    void draw(int frame);
    void drawCentered(int frame);

private:
    vector<ofPixels> images;
    ofTexture texture;
};

class ofxImageSequencePlayer {
public:
    
    void setup(ofPtr<ofxImageSequence> & sequence);
    void play();
    void setPosition(float percent);// { this->offset = percent; }
    
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