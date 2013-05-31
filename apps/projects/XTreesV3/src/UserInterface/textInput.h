//
//  textInput.h
//  textInput
//
//  Created by Elliot Woods on 12/09/2011.
//  Copyright 2011 Kimchi and Chips.
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

#ifndef TEXTINPUT_H_
#define TEXTINPUT_H_

#include "ofMain.h"
#include <FTGL/ftgl.h>

class textInput {
public:
	textInput();
	void	init(FTSimpleLayout*  fontRenderer);
	void	draw() const;
	void	keyPressed(int key);
	void	clear();
  void setText(std::string text_);
	
	string	text;
	int		position;
  bool enabled;
	
	ofEvent<string> evtEnter;
	
protected:
	void	keyPressedEvent(ofKeyEventArgs &a);
	int		cursorx, cursory;
  FTSimpleLayout*  m_fontRenderer;
};

#endif //TEXTINPUT_H_

