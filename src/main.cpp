// main.cpp - Punto de entrada de la aplicación
#include "ofMain.h"
#include "ofApp.h"

int main(){
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW;
	//settings.windowMode = OF_FULLSCREEN;
	
	auto window = ofCreateWindow(settings);
	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
	
	return 0;
}
