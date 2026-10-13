#include "ofApp.h"

//--------------------------------------------------------------

// ofApp.cpp - Implementación de la aplicación principal


void ofApp::setup(){
	// ====== CONFIGURACIÓN DE AUDIO ======
		
	ofSoundStreamSettings settings;
	auto devices = soundStream.getDeviceList();
	
	// IMPORTANTE INFORMACION RESPECTO DEL ID PORQUE PUEDE VARIAR EN CADA REINICIO
	
	for(auto & d : devices){
		ofLogNotice() << "ID: " << d.deviceID
					  << " | " << d.name
					  << " | in:" << d.inputChannels
					  << " | out:" << d.outputChannels;
		if(d.deviceID == 136){  // este debe coincidir con el dispositivo del audio setup de Mac y de Ableton
			
			settings.setInDevice(d);
			ofLogNotice() << "Audio input: " << d.name;
			ofLogNotice() << "ID: " << d.deviceID
						  << " | " << d.name
						  << " | in:" << d.inputChannels
						  << " | out:" << d.outputChannels;
		}
	}
	
	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numInputChannels = 4;
	settings.numOutputChannels = 0;
	settings.bufferSize = 512;
	soundStream.setup(settings);
	
	// ====== CONFIGURACIÓN DE GUI Y MIDI ======
	gui.setup("Panel");
	midiSender.setup(0, 1);  // Puerto 0, Canal 1
	control.setup(gui, &midiSender);
	
	// ====== GEOMETRÍA ======
	w = ofGetWidth();
	h = ofGetHeight();
	r1 = 9.0f * h / 55.0f;   // Radio Mayor
	r2 = h / 70.0f;          // Radio menor-marcador
	r3 = r2;                 // Radio botón mute
	bpm = control.bpm;
	
	ofSetFrameRate(60);
	ofBackground(0);
	fbo.allocate(w, h);
	
	// ====== POSICIONES DE LOS 5 GRUPOS VISUALES ======
	ofVec2f posSq01 = ofVec2f(w/4, h/4);
	ofVec2f posSq02 = ofVec2f(3*w/4, h/4);
	ofVec2f posSq03 = ofVec2f(w/4, 3*h/4);
	ofVec2f posSq04 = ofVec2f(w/2, h/2);
	ofVec2f posSq05 = ofVec2f(3*w/4, 3*h/4);
	
	// ====== CONFIGURACIÓN DE SECUENCIADORES ======
	Sequencer s;
	// s.setup(Posicion, radio chico, radio grande, factor, ch. MIDI, nota MIDI)
	// Seq 1: Melodía 1 (audio reactivo)
	s.setup(posSq01, r2, r1, 2, 36);
	s.setNoteDuration(0.15f);
	s.setColor(ofColor(255, 255, 102));
	s.setBpm(bpm);
	s.setFigura(2);
	s.setPasos(16);
	s.setMute(w/4, h/22, r3, 120, "Seq 1");
	sequencers.push_back(s);
	
	// Seq 2A: Melodía 2 - Voz 1
	s.setup(posSq02, r2, r1,  3, 60);
	s.setNoteDuration(0.15f);
	s.setColor(ofColor(51, 153, 255));
	s.setBpm(bpm);
	s.setFigura(2.0f);
	s.setPasos(8);
	s.setMute(3*w/4, h/22, r3, 121, "Seq 2");
	sequencers.push_back(s);
	
	// Seq 2B: Melodía 2 - Voz 2 (inversa)
	s.setup(posSq02, r2, r1/2,  3, 67);
	s.setColor(ofColor(153, 204, 255));
	s.setBpm(bpm);
	s.setFigura(-2.0f);
	s.setPasos(16);
	sequencers.push_back(s);
	
	// Seq 3: Bajo
	s.setup(posSq03, r2, r1,  1, 24);
	s.setNoteDuration(0.1f);
	s.setColor(ofColor(255, 153, 51));
	s.setBpm(bpm);
	s.setFigura(2.0);
	s.setPasos(16);
	s.setMute(w/4, 23*h/24 + 10 , r3, 122, "Seq 3");
	sequencers.push_back(s);
	
	// Seq 4: Kick
	s.setup(posSq04, r2, r1*0.7f, 10, 38);
	s.setColor(ofColor(153, 0, 0));
	s.setFigura(2);
	s.setPasos(16);
	s.setMute(w/2, h/4, r3, 125, "Seq 4");
	sequencers.push_back(s);
	
	// Seq 5A: Hi-Hat
	s.setup(posSq05, r2, r1,  4, 42);
	s.setColor(ofColor(255, 153, 204));
	s.setBpm(bpm);
	s.setFigura(2.0f);
	s.setPasos(16);
	s.setMute(3*w/4, 23*h/24 + 10, r3, 124, "Seq 5");
	sequencers.push_back(s);
	
	// Seq 5B: Snare (inversa)
	s.setup(posSq05, r2, 3*r1/4.0f,  5, 38);
	s.setColor(ofColor(255, 153, 255));
	s.setBpm(bpm);
	s.setFigura(4.0f);
	s.setPasos(16);
	sequencers.push_back(s);
	
	// Seq 5C: Clap (doble velocidad)
	s.setup(posSq05, r2, r1/2.0f,  6, 39);
	s.setColor(ofColor(204, 153, 255));
	s.setBpm(bpm);
	s.setFigura(-2.0f);
	s.setPasos(8);
	sequencers.push_back(s);
	
	// Seq 5D: Open Hat (mitad velocidad)
	s.setup(posSq05, r2, r1/4.0f,  7, 46);
	s.setColor(ofColor(153, 153, 255));
	s.setBpm(bpm);
	s.setFigura(3);
	s.setPasos(8);
	sequencers.push_back(s);
	
	// Activar visualización de audio en Seq 4 (Kick)
	sequencers[4].drawAudioCircle = true;
		
	// ====== CONFIGURACIÓN DE MIDI IN ======
	midiReceiver.setupInput(3);
	ofAddListener(midiReceiver.controlChangeEvent, this, &ofApp::onMidiControlChange);
	
	ofLogNotice("ofApp") << sequencers.size() << " secuenciadores creados";
	isPlaying = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	control.update();
	int guiBpm = control.getBpm();
	
	// Actualizar audio level en Seq 4
	if(sequencers.size() > 4){
		sequencers[4].setAudioLevel(audioLevel);
	}
	
	if(!isPlaying) return;
	
	// Actualizar todos los secuenciadores
	fbo.begin();
	for(auto &seq : sequencers){
		seq.setBpm(guiBpm);
		seq.update();
	}
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Dibujar secuenciadores
	for(auto &seq : sequencers){
		seq.draw();
		seq.drawMuteAndName();
	}
	
	// Aplicar FBO
	ofSetColor(0, 0, 0);
	fbo.draw(0, 0);
	ofSetColor(255);
	
	// GUI
	if(showGui){
		gui.setPosition(ofGetWidth()/2 - gui.getWidth()/2,
					   ofGetHeight() - gui.getHeight() - 20);
		gui.draw();
	}
	
	// Info
	if(info){
		ofDrawBitmapString(control.mensaje(), 10, ofGetHeight() - 50);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(button == 0){  // Click izquierdo
		for(auto &seq : sequencers){
			seq.checkClick(x, y);
		}
	}
}

//--------------------------------------------------------------
void ofApp::onMidiControlChange(ofxMidiMessage &msg){
	// Recibe un mensajes desde el nanoKontrol
	
	
	// ====== CONTROL DE FILTROS (CC 5-13) ======
	for(int i = 0; i < sequencers.size(); i++){
		if(msg.control == 5+i){
			int ccValor = msg.value;
			int ccDestino = 70 + i;  // CC 70-78 en Ableton
			sequencers[i].setCcValor(ccDestino, ccValor);
			ofLogNotice("ofApp") << "Seq " << i << " filtro: " << ccValor;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// SPACE: Play/Pause
	if(key == OF_KEY_SPACE){
		isPlaying = !isPlaying;
		for(auto &seq : sequencers){
			if(isPlaying){
				seq.resetAngulo();
				seq.play();
			} else {
				seq.stop();
			}
		}
		ofLogNotice("ofApp") << (isPlaying ? "PLAY" : "STOP");
	}
	
	// M: Toggle GUI
	if(tolower(key) == 'm'){
		showGui = !showGui;
	}
	
	// I: Toggle info
	if(key == 'i'){
		info = !info;
	}
	
	// Q: Subir nota del paso seleccionado
	if(key == 'q'){
		for(auto &seq : sequencers){
			seq.changeNote(+1);
			seq.selectedStep = -1;
		}
	}
	
	// W: Bajar nota del paso seleccionado
	if(key == 'w'){
		for(auto &seq : sequencers){
			seq.changeNote(-1);
			seq.selectedStep = -1;
		}
	}
	
	// P: Toggle polígonos
	for(auto &seq : sequencers){
		seq.keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofApp::exit(){
	midiReceiver.exit();
	ofLogNotice("ofApp") << "Saliendo";
}

//--------------------------------------------------------------
/*
void ofApp::audioIn(ofSoundBuffer& input){
	// Calcular nivel RMS
	float sum = 0.0f;
	for(size_t i = 0; i < input.getNumFrames(); i++){
		float sample = input[i * input.getNumChannels()];
		sum += sample * sample;
	}
	audioLevel = sqrt(sum / input.getNumFrames());
	
	// Log cada 60 frames (1 seg a 60fps)
	if(ofGetFrameNum() % 120 == 0){
		ofLogNotice("Audio") << "Level: " << audioLevel;
	}
}
*/
void ofApp::audioIn(ofSoundBuffer& input){
	float sum = 0.0f;
	int numCh = input.getNumChannels();

	for(size_t i = 0; i < input.getNumFrames(); i++){
		float ch3 = input[i * numCh + 2];
		float ch4 = input[i * numCh + 3];

		float sample = 0.5f * (ch3 + ch4);   // promedio L/R
		sum += sample * sample;
	}

	audioLevel = sqrt(sum / input.getNumFrames());

	// Log cada ~2 segundos
	if(ofGetFrameNum() % 120 == 0){
		ofLogNotice("Audio") << "Level (ch 3+4): " << audioLevel;
	}
}
