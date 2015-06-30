//
//  ImageSequence.cpp
//  plankton
//
//  Created by Tobias Ebsen on 20/03/15.
//
//

#include "ofxImageSequence.h"

ofxImage::ofxImage() {
	bIsLoaded = false;
}

ofPixels & ofxImage::getPixelsRef() {
	return pixels;
}

ofTexture & ofxImage::getTextureReference() {
	return tex;
}

void ofxImage::loadPixels(string path) {
	ofLoadImage(pixels, path);
	bIsLoaded = pixels.getWidth() > 0 && pixels.getHeight() > 0;
}

bool ofxImage::isPixelsLoaded() {
	return bIsLoaded;
}

ofxImageSequence::~ofxImageSequence() {
	waitForThread();
}

void ofxImageSequence::load(string path, float framerate, bool useRam) {

    this->framerate = framerate;
	this->path = path;
	this->useRam = useRam;

	ofDirectory dir(path);
    int n = dir.listDir();

    images.resize(n);
    
    for (int i=0; i<n; i++) {
        ofFile file = dir[i];
		ofxImage & image = images[i];
		ofPixels & pixels = image.getPixelsRef();
		ofTexture & tex = image.getTextureReference();
        image.loadPixels(file.getAbsolutePath());
		if (!useRam) {
			tex.allocate(pixels);
			tex.loadData(pixels);
			pixels.clear();
		}
    }
    
	this->width = images[0].getTextureReference().getWidth();
    this->height = images[0].getTextureReference().getHeight();
}

void ofxImageSequence::loadAsync(string path, float framerate, bool useRam) {

    this->framerate = framerate;
	this->path = path;
	this->useRam = useRam;

	this->startThread();
}

void ofxImageSequence::draw(int frame) {
	draw(frame, 0., 0., 0.);
}

void ofxImageSequence::draw(int frame, float x, float y, float z) {
    if (frame >= 0 && frame < images.size()) {
		ofxImage & image = images[frame];
		ofPixels & pixels = image.getPixelsRef();
		ofTexture & tex = image.getTextureReference();
		if (!tex.isAllocated() && image.isPixelsLoaded()) {
			tex.allocate(pixels);
			tex.loadData(pixels);
			if (!useRam)
				pixels.clear();
		}
		if (tex.isAllocated())
			tex.draw(x, y, z);
		if (useRam)
			tex.clear();
    }
}

void ofxImageSequence::drawCentered(int frame) {
	drawCentered(frame, 0., 0., 0.);
}

void ofxImageSequence::drawCentered(int frame, float x, float y, float z) {
    if (frame >= 0 && frame < images.size()) {
		ofxImage & image = images[frame];
		ofPixels & pixels = image.getPixelsRef();
		ofTexture & tex = image.getTextureReference();
		if (pixels.isAllocated())
			draw(frame, x-pixels.getWidth()*0.5f, y-pixels.getHeight()*0.5f, z);
		else if (tex.isAllocated())
			draw(frame, x-tex.getWidth()*0.5f, y-tex.getHeight()*0.5f, z);
    }
}

void ofxImageSequence::threadedFunction() {

	ofDirectory dir(path);
	int n = dir.listDir();
	images.resize(n);

	for (int i=0; i<n && isThreadRunning(); i++) {
		ofxImage & image = images[i];
		ofPixels & pixels = image.getPixelsRef();
		image.loadPixels(dir.getFile(i).getAbsolutePath());
		if (i == 0) {
			width = pixels.getWidth();
			height = pixels.getHeight();
		}
	}
	CompleteEventArgs args;
	args.path = path;
	args.sequence = this;
	if (isThreadRunning())
		ofNotifyEvent(onLoadComplete, args);
}

void ofxImageSequencePlayer::setup(ofPtr<ofxImageSequence> & sequence) {
    this->sequence = sequence;
    this->loop = false;
	startTime = ofGetElapsedTimef();
}


void ofxImageSequencePlayer::play() {
    currentFrame = 0;
    startTime = ofGetElapsedTimef();
}

void ofxImageSequencePlayer::setPosition(float percent) {

	if (sequence == NULL)
		return;

	float dur = sequence->getDuration();
    startTime -= percent * dur;
    currentFrame = (int)floorf(percent * sequence->getNumFrames() + 0.5f);
}

ofxImageSequence* ofxImageSequencePlayer::getSequence() {
	return sequence.get();
}

void ofxImageSequencePlayer::update() {
    
	if (sequence == NULL)
		return;

    float seconds = ofGetElapsedTimef() - startTime;
    int n = sequence->getNumFrames();
    currentFrame = (int)floorf(seconds * sequence->getFramerate() + 0.5f);
    
    if (currentFrame >= n) {
        if (loop && n > 0)
            currentFrame %= n;
        else
            currentFrame = n - 1;
    }
}

void ofxImageSequencePlayer::draw() {
	if (sequence != NULL)
	    sequence->draw(currentFrame);
}

void ofxImageSequencePlayer::draw(float x, float y, float z) {
    ofPushMatrix();
    ofTranslate(x, y, z);
	if (sequence != NULL)
	    sequence->draw(currentFrame);
    ofPopMatrix();
}

void ofxImageSequencePlayer::drawCentered() {
	if (sequence != NULL)
	    sequence->drawCentered(currentFrame);
}
