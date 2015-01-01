//
//  ofxSndFile.h
//  emptyExample
//
//  Created by lukasz karluk on 15/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

class SndfileHandle;
class ofxSndFile
{
public:
	
	ofxSndFile();
	ofxSndFile(int channels, int samplerate = 44100) : channels(channels), samplerate(samplerate),sfhandle(NULL) {}
	~ofxSndFile(){closeStream();};
	
	bool load(const string &path);
	bool save(const string &path);
	
	bool openStream(const string &path);
	bool closeStream();
	bool isOpenStream() { return sfhandle != NULL; }
    
	int getChannels() { return channels; }
	void setChannels(int n) { channels = n; }
	
	int getSamplerate() { return samplerate; }
	void setSamplerate(int n) { samplerate = n; }
	
	int getNumFrame();
	float getDuration();
	
	void resizeFrame(size_t size);
	
	bool seekStream(size_t frame);
	vector<float>& readStream(size_t read_frames);
	vector<float>& getBuffer() { return buffer; }
	
	void normalize();
	
protected:
	SndfileHandle *sfhandle;
	int channels, samplerate;
	
	vector<float> buffer;
	
};