/*
 * Copyright (c) 2012-2013 Agnes Chavez and Alessandro Saccoia
 * Written by Alessandro Saccoia, <alessandro.saccoia@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Sotware.
 */


#pragma once

#include "ofMain.h"

class AlphaRect
{
	
public:

    AlphaRect(){}
   
    AlphaRect( float w, float h )
    {
        init( w, h );
    }
    
    void init ( float w, float h )
    {
        rectPoints[ 0 ]  = 0;
        rectPoints[ 1 ]  = 0;
        rectPoints[ 2 ]  = w;
        rectPoints[ 3 ]  = 0;
        rectPoints[ 4 ]  = w;
        rectPoints[ 5 ]  = h;
        rectPoints[ 6 ]  = 0;
        rectPoints[ 7 ]  = h;
        
        rectColors[ 0 ]  = 1;
        rectColors[ 1 ]  = 0;
        rectColors[ 2 ]  = 0;
        rectColors[ 3 ]  = 1;
        rectColors[ 4 ]  = 1;
        rectColors[ 5 ]  = 0;
        rectColors[ 6 ]  = 0;
        rectColors[ 7 ]  = 1;
        rectColors[ 8 ]  = 0;
        rectColors[ 9 ]  = 0;
        rectColors[ 10 ] = 1;
        rectColors[ 11 ] = 1;
        rectColors[ 12 ] = 0;
        rectColors[ 13 ] = 0;
        rectColors[ 14 ] = 1;
        rectColors[ 15 ] = 1;
    }
    
    void SetColor(const ofColor& c) {
      setCornerColor(c, 0, 255);
      setCornerColor(c, 1, 255);
      setCornerColor(c, 2, 0);
      setCornerColor(c, 3, 0);
    }
    
    void setCornerColor( const ofColor& c, int cornerIndex, int alpha )
    {
        int i = ofClamp( cornerIndex, 0, 3 );
        
        rectColors[ i * 4 + 0 ] = c.r / 255.0;
        rectColors[ i * 4 + 1 ] = c.g / 255.0;
        rectColors[ i * 4 + 2 ] = c.b / 255.0;
        rectColors[ i * 4 + 3 ] = alpha / 255.0;
    }
    
    void draw ()
    {
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY );
        
        glVertexPointer( 2, GL_FLOAT, 0, rectPoints );
        glColorPointer(  4, GL_FLOAT, 0, rectColors );
        
        glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
        
        glDisableClientState( GL_COLOR_ARRAY );
        glDisableClientState( GL_VERTEX_ARRAY );
    }
	
	GLfloat	rectPoints[ 8 ];
	GLfloat rectColors[ 16 ];

};