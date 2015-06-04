//
//  ImageSequence.cpp
//  plankton
//
//  Created by Tobias Ebsen on 20/03/15.
//
//

#include "ofxImageSequence.h"

void ofxImageSequence::load(string path, float framerate) {

    ofDirectory dir(path);
    int n = dir.listDir();

    images.resize(n);
    
    for (int i=0; i<n; i++) {
        ofFile file = dir[i];
        ofLoadImage(images[i], file.getAbsolutePath());
    }
    
    this->width = images[0].getWidth();
    this->height = images[0].getHeight();
    this->framerate = framerate;
}

void ofxImageSequence::draw(int frame) {
    if (frame >= 0 && frame < images.size()) {
        images[frame].draw(0, 0);
    }
}

void ofxImageSequence::drawCentered(int frame) {
    if (frame >= 0 && frame < images.size()) {
        ofTexture & tex = images[frame];
        tex.draw(-tex.getWidth()*0.5f, -tex.getHeight()*0.5f);
    }
}

void ofxImageSequencePlayer::setup(ofPtr<ofxImageSequence> sequence) {
    this->sequence = sequence;
    this->loop = false;
}

void ofxImageSequencePlayer::play() {
    currentFrame = 0;
    startTime = ofGetElapsedTimef();
}

void ofxImageSequencePlayer::setPosition(float percent) {
    float dur = sequence->getDuration();
    startTime = (ofGetElapsedTimef()) - percent * dur;
    currentFrame = (int)floorf(percent * sequence->getFramerate() + 0.5f);
}

void ofxImageSequencePlayer::update() {
    
    float progress = ofGetElapsedTimef() - startTime;
    int n = sequence->getNumFrames();
    currentFrame = (int)floorf(progress * sequence->getFramerate() + 0.5f);
    
    if (currentFrame >= n) {
        if (loop)
            currentFrame %= n;
        else
            currentFrame = n - 1;
    }
}

void ofxImageSequencePlayer::draw() {
    sequence->draw(currentFrame);
}

void ofxImageSequencePlayer::drawCentered() {
    sequence->drawCentered(currentFrame);
}
