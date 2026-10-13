// MidiReceiver.cpp - Implementación de recepción MIDI
#include "MidiReceiver.h"

//--------------------------------------------------------------
void MidiReceiver::setupInput(int port){
	midiIn.listInPorts();
	midiIn.openPort(port);
	midiIn.addListener(this);
	
	lastNote = 0;
	lastVelocity = 0;
	lastCC = 0;
	lastCCValue = 0;
	
	ofLogNotice("MidiReceiver") << "Puerto " << port << " abierto";
}

//--------------------------------------------------------------
void MidiReceiver::newMidiMessage(ofxMidiMessage& msg){
	// Note ON
	if(msg.status == MIDI_NOTE_ON && msg.velocity > 0){
		lastNote = msg.pitch;
		lastVelocity = msg.velocity;
		ofLogNotice("MIDI") << "Note ON: " << lastNote << " vel: " << lastVelocity;
		ofNotifyEvent(noteOnEvent, msg, this);
	}
	// Note OFF
	else if(msg.status == MIDI_NOTE_OFF ||
		   (msg.status == MIDI_NOTE_ON && msg.velocity == 0)){
		lastNote = msg.pitch;
		lastVelocity = 0;
		ofLogNotice("MIDI") << "Note OFF: " << lastNote;
		ofNotifyEvent(noteOffEvent, msg, this);
	}
	// Control Change
	else if(msg.status == MIDI_CONTROL_CHANGE){
		lastCC = msg.control;
		lastCCValue = msg.value;
		ofLogNotice("MIDI") << "CC " << lastCC << " = " << lastCCValue;
		ofNotifyEvent(controlChangeEvent, msg, this);
	}
}

//--------------------------------------------------------------
void MidiReceiver::exit(){
	midiIn.closePort();
	midiIn.removeListener(this);
	ofLogNotice("MidiReceiver") << "Puerto cerrado";
}

