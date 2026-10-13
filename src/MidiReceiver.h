// MidiReceiver.h - Recepción de mensajes MIDI
#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class MidiReceiver : public ofxMidiListener {
public:
	void setupInput(int port);
	void newMidiMessage(ofxMidiMessage& msg) override;
	void exit();
	
	// Eventos
	ofEvent<ofxMidiMessage> noteOnEvent;
	ofEvent<ofxMidiMessage> noteOffEvent;
	ofEvent<ofxMidiMessage> controlChangeEvent;
	
	// Getters
	int getLastNote() { return lastNote; }
	int getLastVelocity() { return lastVelocity; }
	int getLastCC() { return lastCC; }
	int getLastCCValue() { return lastCCValue; }
	
private:
	ofxMidiIn midiIn;
	int lastNote;
	int lastVelocity;
	int lastCC;
	int lastCCValue;
};

