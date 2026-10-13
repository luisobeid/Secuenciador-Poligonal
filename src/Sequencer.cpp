// Sequencer.cpp - Implementación del secuenciador circular
#include "Sequencer.h"

// Helper: Convertir nota MIDI a string (ej: 60 -> "C3")
string midiNoteToString(int midiNote){
	static string nombres[12] = {
		"C", "C#", "D", "D#", "E", "F",
		"F#", "G", "G#", "A", "A#", "B"
	};
	
	int notaIndex = midiNote % 12;
	int octava = midiNote / 12 - 1;
	
	return nombres[notaIndex] + ofToString(octava);
}

//--------------------------------------------------------------
void Sequencer::setup(ofVec2f centro, float radio, float radioM,
					  int midiCh, int _nota){
	setCentro(centro);    // posicion de centro del circulo mayor
	setRadio(radio);      // radio circulo marcador
	radioExt = radioM;    // Radio mayor
	nota = _nota;         // nota central del sequencer
	tiempoInicio = 0.0f;
	
	midiSender.setup(0, midiCh);
	midiSender.sendControlChange(ccDestino, ccValor);
}

//--------------------------------------------------------------
void Sequencer::update(){
	// Calcular velocidad angular: ω = 0.5 * 2π * (BPM/60) / figura
	omega =  TWO_PI * (bpm / 60.0f) / figura;
	
	// Actualizar ángulo
	angulo = omega * (ofGetElapsedTimef() - tiempoInicio);
	angulo = fmod(angulo, TWO_PI);
	
	// Determinar paso actual
	if(figura >= 0 ) {paso = int(ofMap(angulo, 0, TWO_PI, 0, pasos)) % pasos;}
	// para giros en sentido antihorario
	if(figura < 0 ) {paso = int(ofMap(abs(angulo), TWO_PI, 0, 1, pasos + 1)) % pasos;} // lo corregí porque si se desfasaba un paso.
	
	// Calcular posición del marcador
	pos.x = centro.x + radioExt * cos(angulo - HALF_PI);
	pos.y = centro.y + radioExt * sin(angulo - HALF_PI);
	
	// Enviar nota MIDI si alcanzamos un paso activo
	if(paso != ultimoPaso && hitPos[paso]){
		midiSender.sendNoteOn(notas[paso], 127);
		activeNotes[paso] = ofGetElapsedTimef();
	}
	
	vector<int> notesToStop;
		for(auto& noteEntry : activeNotes){
			float elapsed = ofGetElapsedTimef() - noteEntry.second;
			if(elapsed >= noteDuration){
				midiSender.sendNoteOff(notas[noteEntry.first]);
				notesToStop.push_back(noteEntry.first);
			}
		}
	// Limpiar notas apagadas
	for(int paso : notesToStop){
		activeNotes.erase(paso);
	}
	//else{midiSender.sendNoteOff(notas[paso]);}
	ultimoPaso = paso;
}

//--------------------------------------------------------------
void Sequencer::draw(){
	// Círculo reactivo al audio
	
	ofEnableAlphaBlending();
	// El brillo depende del nivel de entrada de audio
	if(drawAudioCircle){
		float brillo = ofMap(audioLevel, 0.0f, 0.03f, 0.0f, 1, true);
		
	// el relleno del circulo se expande
		ofFill();
		for(int i = 0; i < 6; i++){
			float r = 2 * radioExt/3 + i * 6;
			float alpha = ofMap(i, 0, 5, 80, 5);

			ofSetColor(153 * brillo, 0, 0, alpha);
			ofDrawCircle(centro, r);
		}
	}
	// dibujamos rectas entre puntos activos
	if(poligonos){
		vector<ofVec2f> activePoints;
		for(int i = 0; i < hitPos.size(); i++){
			if(hitPos[i]){
				activePoints.push_back(memoPos[i]);
			}
		}
	// si hitPos o activePointos si >= 3 podemos llenar el poligono resultante
		int n = activePoints.size();
			if(n >= 2){

				//---- RELLENO ----
				if(relleno)
				{ ofFill();
					//ofSetColor(180, 180, 180, 30); // gris con alpha
					ofSetColor(color.r, color.g, color.b, 40); // gris con alpha
					
					ofBeginShape();
					for(auto &p : activePoints){
						ofVertex(p.x, p.y);
					}
					ofEndShape(true); // true = cerrar figura
				}
				// ---- CONTORNO ----
				ofNoFill();
				ofSetColor(ofColor::grey);
				ofSetLineWidth(1.5f);
				for(int i = 0; i < n; i++){
					ofDrawLine(
						activePoints[i],
						activePoints[(i + 1) % n]
					);
					ofFill();
				}
			}
	}
	
	// Dibujar pasos ----
	
		for(int i = 0; i < pasos; i++){
		// Verificar y redimensionar vectores si es necesario
		if(memoPos.size() != pasos){
			memoPos.resize(pasos);
			hitPos.resize(pasos, false);
		}
		
		// Calcular posición del paso
		
		float ang = (i * TWO_PI / pasos) - HALF_PI;
		ofVec2f posPaso;
		posPaso.x = centro.x + radioExt * cos(ang);
		posPaso.y = centro.y + radioExt * sin(ang);
		memoPos[i] = posPaso;
		
		// Color del paso según estado
		
		if(i == paso){
			ofSetColor(230, 255, 255);     // Paso actual: blanco
		} else if(hitPos[i]){
			ofSetColor(255, 51, 153);      // Si el paso está activo: magenta
		} else {
			ofSetColor(80, 80, 80);        // Paso inactivo: gris
		}
		
		ofDrawCircle(posPaso.x, posPaso.y, getRadio()/2);
	}
	
	// Dibujar marcador rotatorio
	ofSetColor(color);
	ofDrawCircle(getPos().x, getPos().y, 3 * getRadio()/4);
	
	// Mostrar nota si mouse está sobre un paso
	showNote(ofGetMouseX(), ofGetMouseY());
	
}

//--------------------------------------------------------------
void Sequencer::setColor(ofColor c){
	color = c;
}

//--------------------------------------------------------------
void Sequencer::setBpm(int _bpm){
	bpm = _bpm;
}

//--------------------------------------------------------------
void Sequencer::setFigura(float _figura){
	figura = _figura;
}

//--------------------------------------------------------------
void Sequencer::setPasos(int _pasos){
	
	pasos = _pasos;
	memoPos.resize(_pasos);
	hitPos.resize(_pasos, false);
	notas.resize(_pasos);
	
	// Inicializar todas las notas con la nota base
	for(int i = 0; i < _pasos; i++){
		notas[i] = nota;
	}
}

//--------------------------------------------------------------
void Sequencer::checkClick(int x, int y){
	ofVec2f mouse(x, y);
	
	// Click para activar/desactivar pasos
	for(int i = 0; i < pasos; i++){
		if(i < memoPos.size()){
			float distancia = memoPos[i].distance(mouse);
			if(distancia < getRadio()){
				hitPos[i] = !hitPos[i];  // Toggle
				ofLogNotice("Sequencer") << "Paso " << i
										<< (hitPos[i] ? " ON" : " OFF");
				break;
			}
		}
	}
	
	// Click para activar/desactivar mute
	for(auto &b : mutedButtons){
		float d = b.centroMute.distance(mouse);
		if(d < b.radioMute){
			b.isMuted = !b.isMuted;
			int valor = b.isMuted ? 0 : 127;
			midiSender.sendControlChange(b.ccNumber, valor);
			ofLogNotice("Sequencer") << "Mute " << (b.isMuted ? "ON" : "OFF");
		}
	}
}

//--------------------------------------------------------------
void Sequencer::play(){
	isPlaying = true;
}

//--------------------------------------------------------------
void Sequencer::stop(){
	isPlaying = false;
	midiSender.sendNoteOff(nota);
}

//--------------------------------------------------------------
void Sequencer::resetAngulo(){
	tiempoInicio = ofGetElapsedTimef();
	ultimoPaso = -1;
}

//--------------------------------------------------------------
// seteamos cada boton  de mute.
void Sequencer::setMute(float x, float y, float radio, int cc, string _nombre){
	MuteButton b;
	b.centroMute.set(x, y);
	b.radioMute = radio;
	b.isMuted = false;
	b.ccNumber = cc;
	b.nombre = _nombre;
	mutedButtons.push_back(b);
}

//--------------------------------------------------------------
void Sequencer::drawMuteAndName(){
	ofSetLineWidth(2.0f);
	
	for(auto &b : mutedButtons){
		// Color según estado
		if(b.isMuted){
			ofSetColor(255, 0, 0);    // Rojo: mute
		} else {
			ofSetColor(0, 150, 0);    // Verde: activo
		}
		
		ofNoFill();
		ofDrawCircle(b.centroMute, b.radioMute);
		ofFill();
		
		// Dibujar "M"
		float textX = b.centroMute.x - b.radioMute/3.0f;
		float textY = b.centroMute.y + b.radioMute/3.0f;
		ofDrawBitmapString("M", textX, textY);
	
		// dibujar el nombre del secuenciador
		ofDrawBitmapString( b.nombre, textX - 23, textY - 25);

	}
}

  

//--------------------------------------------------------------
void Sequencer::showNote(int x, int y){
	ofVec2f mouse(x, y);
	selectedStep = -1;
	
	for(int i = 0; i < pasos; i++){
		if(i < memoPos.size()){
			float distancia = memoPos[i].distance(mouse);
			if(distancia < getRadio()){
				selectedStep = i;
				string notaStr = midiNoteToString(notas[i]);
				float textX = ofGetWidth()/2.0f - 35.0f;
				float textY = 20.0f;
				ofDrawBitmapString("Nota: " + notaStr, textX, textY);
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void Sequencer::changeNote(int delta){
	if(selectedStep >= 0 && selectedStep < notas.size()){
		notas[selectedStep] += delta;
		notas[selectedStep] = ofClamp(notas[selectedStep], 0, 127);
		ofLogNotice("Sequencer") << "Paso " << selectedStep
								<< " nota: " << notas[selectedStep]
								<< " (" << midiNoteToString(notas[selectedStep]) << ")";
	}
}

//--------------------------------------------------------------
void Sequencer::keyPressed(int key){
	if(key == 'p'){
		poligonos = !poligonos;
	}
	if(key == 'f'){
		relleno = !relleno;
	}
}

void Sequencer::setNota(int _nota) {
	nota = _nota;
}

//--------------------------------------------------------------
void Sequencer::setCcValor(int _ccDestino, int _ccValor){
	ccValor = _ccValor;
	ccDestino = _ccDestino;
	midiSender.sendControlChange(ccDestino, ccValor);
}

void Sequencer::setNoteDuration(float duracion){
	noteDuration = ofClamp(duracion, 0.01f, 2.0f);
}

void Sequencer::ChangeNoteDuration(float duracion){
	noteDuration = ofClamp(duracion, 0.01f, 2.0f);
}
