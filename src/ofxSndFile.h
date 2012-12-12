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
	
	bool load(string path);
	bool save(string path);
    
	int getChannels() { return channels; }
	void setChannels(int n) { channels = n; }
	
	int getSamplerate() { return samplerate; }
	void setSamplerate(int n) { samplerate = n; }
	
	vector<float>& getBuffer() { return buffer; }
	
protected:
	
	int channels, samplerate;
	
	vector<float> buffer;
	
};