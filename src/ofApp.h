// ofApp.h - Clase principal de la aplicación
#pragma once

#include "ofMain.h"
#include "midiSender.h"
#include "MidiReceiver.h"
#include "ofxGui.h"
#include "Sequencer.h"
#include "controlGui.h"

class ofApp : public ofBaseApp {
public:
	// Métodos principales
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;
	
	// Eventos de entrada
	void mousePressed(int x, int y, int button) override;
	void keyPressed(int key);
	void onMidiControlChange(ofxMidiMessage& msg);
	void audioIn(ofSoundBuffer& input);
	
	// Audio
	ofSoundStream soundStream;
	float audioLevel = 0.0f;
	
	// MIDI
	MidiSender midiSender;
	MidiReceiver midiReceiver;
	
	// Secuenciadores
	vector<Sequencer> sequencers;
	
	// GUI
	ofxPanel gui;
	Controles control;
	ofFbo fbo;
	
	// Geometría
	float w, h;      // Dimensiones de ventana
	float r1, r2, r3; // Radios (grande, marcador, botón)
	float bpm;
	
	// Estados
	bool isPlaying = false;
	bool showGui = false;
	bool info = true;
	int nota;
};
