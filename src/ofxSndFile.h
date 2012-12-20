//
//  ofxSndFile.h
//  emptyExample
//
//  Created by lukasz karluk on 15/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

class ofxSndFile
{
public:
	
	ofxSndFile();
	ofxSndFile(int channels, int samplerate = 44100) : channels(channels), samplerate(samplerate) {}
	
	bool load(string path);
	bool save(string path);
    
	int getChannels() { return channels; }
	void setChannels(int n) { channels = n; }
	
	int getSamplerate() { return samplerate; }
	void setSamplerate(int n) { samplerate = n; }
	
	int getNumFrame() { return buffer.size() / channels; }
	float getDuration();
	
	void resizeFrame(size_t size);
	
	vector<float>& getBuffer() { return buffer; }
	
	void normalize();
	
protected:
	
	int channels, samplerate;
	
	vector<float> buffer;
	
};