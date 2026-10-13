// controlGui.h - Interfaz GUI para controles
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "midiSender.h"

class Controles {
public:
	void setup(ofxPanel& gui, MidiSender* midiCh);
	void update();
	string mensaje();
	
	static const int NUM_FADERS = 9;
	ofParameter<int> bpm;
	int getBpm() const { return bpm; }
	ofxPanel mixer;
	ofParameter<int> vols[NUM_FADERS];
	
private:
	int prevBpm = -1;
	int prevVols[NUM_FADERS];
	int ccNums[NUM_FADERS];
	MidiSender* midiSender = nullptr;
};


