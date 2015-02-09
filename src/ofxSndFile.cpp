//
//  ofxSndFile.cpp
//  emptyExample
//
//  Created by lukasz karluk on 15/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ofxSndFile.h"

#include <sndfile.h>
#include <sndfile.hh>

ofxSndFile::ofxSndFile() : channels(0), samplerate(0),sfhandle(NULL) {}

bool ofxSndFile::load(const string &path)
{
	openStream(path);
	
	if (sfhandle == NULL)
	{
		ofLogError("ofxSndFile") << "can't load file";
		return false;
	}
	
	const size_t read_frames = 2048;
	const size_t read_samples = read_frames * channels;
	
	float *buf = new float[read_samples];
	
	while (true)
	{
		sf_count_t n = sfhandle->read(buf, read_frames);
		buffer.insert(buffer.end(), buf, buf + n);
		if (n < read_frames) break;
	}
	
	closeStream();

	delete[] buf;
	
	return true;
}

bool ofxSndFile::save(const string &path)
{
	SNDFILE* sf = NULL;
	SF_INFO sinfo;
	
	ofFile file;
	if (!file.open(ofToDataPath(path), ofFile::WriteOnly, true))
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

bool ofxSndFile::openStream(const string &path)
{
	closeStream();
	
	sfhandle = new SndfileHandle(ofToDataPath(path).c_str(), SFM_READ);
	
	if (sfhandle == NULL)
	{
		ofLogError("ofxSndFile") << "can't load file";
		return false;
	}
	
	channels = sfhandle->channels();
	samplerate = sfhandle->samplerate();
	return true;
}

bool ofxSndFile::closeStream()
{
	if(sfhandle) {
		delete sfhandle;
		sfhandle = NULL;
		return true;
	}
	return false;
}

bool ofxSndFile::seekStream(size_t frame)
{
	if (sfhandle == NULL)
	{
		ofLogError("ofxSndFile") << "file not loaded";
		return false;
	}
	sf_count_t count = sfhandle->seek(frame, SFM_READ);
	
	return count != -1;
}

vector<float>& ofxSndFile::readStream(size_t read_frames)
{
	buffer.clear();
	if (sfhandle == NULL)
	{
		ofLogError("ofxSndFile") << "file not loaded";
		return buffer;
	}
	const size_t read_samples = read_frames * channels;
	
	float *buf = new float[read_samples];
	
	sf_count_t n = sfhandle->read(buf, read_frames);
	buffer.insert(buffer.end(), buf, buf + n);

	delete[] buf;

	return buffer;
}

float ofxSndFile::getDuration()
{
	return (float)getNumFrame() / getSamplerate();
}

int ofxSndFile::getNumFrame()
{
	if(sfhandle) {
		return sfhandle->frames();
	}
	else {
		return buffer.size() / channels;
	}
}

void ofxSndFile::resizeFrame(size_t size)
{
	buffer.resize(size * channels);
}

void ofxSndFile::normalize()
{
	float max_sample = 0;
	
	for (int i = 0; i < buffer.size(); i++)
	{
		max_sample = max(max_sample, buffer[i]);
	}
	
	if (max_sample <= 0) return;
	
	float inv_max_sample = 1. / max_sample;
	
	for (int i = 0; i < buffer.size(); i++)
	{
		buffer[i] *= inv_max_sample;
	}
}

