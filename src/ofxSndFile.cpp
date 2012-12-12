//
//  ofxSndFile.cpp
//  emptyExample
//
//  Created by lukasz karluk on 15/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ofxSndFile.h"

#include <sndfile.h>

ofxSndFile::ofxSndFile() : channels(0), samplerate(0) {}

bool ofxSndFile::load(string path)
{
	path = ofToDataPath(path);
	
	SNDFILE* sf = NULL;
	SF_INFO sinfo;
	
	sinfo.format = 0;
	
	sf = sf_open(path.c_str(), SFM_READ, &sinfo);
	
	if (sf == NULL)
	{
		ofLogError("ofxSndFile") << "can't load file";
		return false;
	}
	
	channels = sinfo.channels;
	samplerate = sinfo.samplerate;
	
	const size_t read_frames = 2048;
	const size_t read_samples = read_frames * channels;
	
	float buf[read_samples];
	
	while (true)
	{
		sf_count_t n = sf_read_float(sf, buf, read_frames);
		buffer.insert(buffer.end(), buf, buf + n);
		if (n < read_frames) break;
	}
	
	sf_close(sf);
	
	return true;
}

bool ofxSndFile::save(string path)
{
	path = ofToDataPath(path);
	
	SNDFILE* sf = NULL;
	SF_INFO sinfo;
	
	ofFile file;
	if (!file.open(path, ofFile::WriteOnly, true))
	{
		ofLogError("ofxSndFile") << "can't load file";
		return false;
	}
	
	string ext = file.getExtension();
	
	file.close();
	
	if (ext == "aiff" || ext == "aif")
	{
		sinfo.format = SF_FORMAT_AIFF | SF_FORMAT_PCM_16;
	}
//	else if (ext == "wav") // TODO: support wav format
//	{
//		sinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
//	}
	else
	{
		ofLogError("ofxSndFile") << "unsupported file format";
		return false;
	}
	
	sinfo.samplerate = samplerate;
	sinfo.channels = channels;
	sinfo.frames = 0;
	
	sf = sf_open(path.c_str(), SFM_WRITE, &sinfo);
	int n = sf_write_float(sf, &buffer[0], buffer.size());
	sf_close(sf);
	
	return true;
}
