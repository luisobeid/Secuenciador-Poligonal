// Sequencer.h - Secuenciador circular con control MIDI
#pragma once

#include "ofMain.h"
#include "midiSender.h"

// Estructura para botones de mute
struct MuteButton{
	ofVec2f centroMute;
	float radioMute;
	bool isMuted = false;
	string nombre;
	int ccNumber;
};

class Sequencer {
public:
	// Setup
	void setup(ofVec2f centro, float radio, float radioExt, int midiCh, int nota);
	
	// Ciclo de vida
	void update();
	void draw();
	void play();
	void stop();
	void resetAngulo();
	
	// Configuración
	void setColor(ofColor c);    // Color del circulo marcador
	void setBpm(int bpm);        // beats por minuto
	void setFigura(float figura); // Duracion en figura musical de un giro completo.
	void setPasos(int pasos); // cantidad de pasos en que se divide el circulo
	
	void setMute(float x, float y, float radio, int cc, string nombre); // accionamos mute de cada secuenciador
																		// y seteamos su nombre 
	void setNoteDuration(float duracion);  // duracion de las notacion
	void ChangeNoteDuration(float duracion);

	// Interacción
	void checkClick(int x, int y);
	void keyPressed(int key);
	void showNote(int x, int y);   // muestra la nota del paso sobre el que posamos el puntero del mouse
	void changeNote(int delta);
	void drawMuteAndName();        // dibuja el boton de Mute y el nombre del sequencer.
	
	// Audio reactivo
	void setAudioLevel(float level) { audioLevel = level; }
	bool drawAudioCircle = false;
	
	// MIDI
	void setNota(int _nota) ;
	void setCcValor(int ccDestino, int ccValor);
	int getNota() { return nota; }
	
	// Getters
	float getRadio() { return radio; }
	ofVec2f getCentro() { return centro; }
	ofVec2f getPos() { return pos; }
	void setCentro(ofVec2f p) { centro = p; }
	void setPos(ofVec2f p) { pos = p; }
	void setRadio(float r) { radio = r; }
	
	// Estado público
	bool isPlaying = false;
	int selectedStep = -1;
	bool poligonos = false;  
	bool relleno = false;
	vector<MuteButton> mutedButtons; // conjunto de botones mute
	ofVec2f pos, centro;
	ofColor color;
	
	
private:
	// MIDI
	MidiSender midiSender;
	int nota = 0;
	int ccValor = 0;
	int ccDestino = 0;
	
	// Geometría
	float radio = 0.0f;     // radio circulo marcador
	float radioExt = 0.0f;  // radio del sequencer
	float angulo = 0.0f;
	float omega = 0.0f;      // velocidad angular
	float figura = 1.0f;	 // tiempo 'musical' en completar un giro

	float tiempoInicio = 0.0f;
	
	// Tempo
	float bpm = 120.0f;
	
	// Secuencia
	int pasos = 0;
	int paso = 0;
	int ultimoPaso = -1;// En la sección private:
	float noteDuration = 0.1f;  // Duración en segundos
	vector<float> duracionNota; // guarda la duracion de las notas del seq.
	map<int, float> activeNotes;  // paso -> tiempo de inicio
	vector<ofVec2f> memoPos;   // guarda la posicion de los pasos del sequencer
	vector<bool> hitPos;   // vector booleano, guarda los pasos "activos", los que van a sonar.
	vector<int> notas;   // guarda las notas de cada sequencer
	// Audio
	float audioLevel = 0.0f;
};
