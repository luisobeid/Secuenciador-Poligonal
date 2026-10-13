// midiSender.cpp - Implementación de envío MIDI
#include "midiSender.h"

//--------------------------------------------------------------
void MidiSender::setup(int port, int midiCh){
	midiOut.listOutPorts();
	midiOut.openPort(port);
	channel = midiCh;
	ofLogNotice("MidiSender") << "Puerto " << port << " | Canal " << channel;
}

//--------------------------------------------------------------
void MidiSender::sendNoteOn(int note, int velocity){
	midiOut.sendNoteOn(channel, note, velocity);
}

//--------------------------------------------------------------
void MidiSender::sendNoteOff(int note){
	midiOut.sendNoteOff(channel, note, 0);
}

//--------------------------------------------------------------
void MidiSender::sendControlChange(int controlador, int valor){
	midiOut.sendControlChange(channel, controlador, valor);
}

//--------------------------------------------------------------
void MidiSender::allNotesOff(){
	for(int n = 0; n < 128; ++n){
		sendNoteOff(n);
	}
	ofLogNotice("MidiSender") << "All notes off";
}

//--------------------------------------------------------------
void MidiSender::exit(){
	midiOut.closePort();
	ofLogNotice("MidiSender") << "Puerto cerrado";
}

