/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#include "ofxScene.h"

/// RUNNER SCENE

//--------------------------------------------------------------
void ofxRunnerScene::setup() {
	if(!scene->_bSetup){
		scene->setup();
		scene->_bSetup = true;
	}
}

//--------------------------------------------------------------
void ofxRunnerScene::update() {
 	if(!scene->_bSetup || !scene->_bRunning)
		return;

	if(scene->_bEntering) {
		scene->updateEnter();
		scene->_bEnteringFirst = false;
	}
	else if(scene->_bExiting) {
		scene->updateExit();
		scene->_bExitingFirst = false;
	}
	else {
		scene->update();
	}
}
		
//--------------------------------------------------------------
void ofxRunnerScene::draw() {
	if(!scene->_bSetup)
		return;
	scene->draw();			
}
