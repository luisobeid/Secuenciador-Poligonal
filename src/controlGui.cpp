// controlGui.cpp - Implementación de controles GUI
#include "controlGui.h"

//--------------------------------------------------------------
void Controles::setup(ofxPanel& gui, MidiSender* midiCh){
	midiSender = midiCh;
	mixer.setup("Mixer");
	
	// Nombres de faders
	static const string faderNames[NUM_FADERS] = {
		" Seq 1", " Seq 2", " Seq 3", " Seq 4", " Seq 5",
		" Seq 5A", " Seq 5B", " Seq 5C", " Seq 5D"
	};
	
	// Crear faders de volumen
	for(int i = 0; i < NUM_FADERS; i++){
		string name = "Vol" + faderNames[i];
		vols[i].set(name, 100, 0, 127);
		mixer.add(vols[i]);
		prevVols[i] = -1;      // Forzar primer envío
		ccNums[i] = 100 + i;   // CC 100-108
	}
	
	// Control de BPM
	bpm.set("BPM", 100, 40, 200);
	mixer.add(bpm);
	prevBpm = -1;
	
	gui.add(&mixer);
	ofLogNotice("Controles") << "GUI configurado";
}

//--------------------------------------------------------------
void Controles::update(){
	if(bpm != prevBpm){
		prevBpm = bpm;
	}
	
	if(!midiSender) return;
	
	// Enviar CC solo cuando cambian
	for(int i = 0; i < NUM_FADERS; i++){
		if(vols[i] != prevVols[i]){
			midiSender->sendControlChange(ccNums[i], vols[i]);
			prevVols[i] = vols[i];
			ofLogNotice("Controles") << "Vol " << i
									<< " CC " << ccNums[i]
									<< " = " << vols[i];
		}
	}
}

//--------------------------------------------------------------
string Controles::mensaje(){
	return " SPACE -> Play/Pause\n"
		   " m -> Mixer | q -> note up \n"
		   " i -> info | w -> note down \n"
	" p -> Poligonos | f -> fill \n";
	}


