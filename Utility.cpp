#ifdef WIN32
	#include <crtdbg.h>
#endif
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <cerrno>
#include "IncludeGL.h"
#include "Utility.h"

namespace Utilities
{
	void ReportError( const std::string& message )
	{
		std::cerr << message << std::endl;
#ifdef WIN32
		MessageBoxA( NULL, message.c_str(), "ERROR", MB_ICONERROR );
#endif
	}

	void ReportMemoryLeaks()
	{
#ifdef WIN32
		assert( _CrtCheckMemory() );
		_CrtDumpMemoryLeaks();
#endif
	}


	std::string GetFileContents(const char *filename)
	{
		std::ifstream in( filename, std::ios::in | std::ios::binary );

		// If the file exists
		if (in)
		{
			std::string contents;

			// Reading the file
			in.seekg( 0, std::ios::end );
			contents.resize( static_cast< unsigned int >( in.tellg() ) );		
			in.seekg( 0, std::ios::beg );
			in.read( &contents[0], contents.size() );
			in.close();

			return(contents);
		}
		throw( errno );
	}

	void OutPutText( float x, float y, int fontSize, unsigned const char* text )
	{
#ifdef EMSCRIPTEN
#else		
		glDisable( GL_TEXTURE_2D );

		glPushMatrix();
		
		glRasterPos2f( x, y );
		
		glutBitmapString( GLUT_BITMAP_TIMES_ROMAN_24, text );

		glPopMatrix();

		glEnable( GL_TEXTURE_2D );
#endif		
	}
}

void Drawing::ApplyTransformations( float x, float y, float angle, float scaleX, float scaleY )
{
	glTranslatef( x, y, 0.0f );
	glRotatef( angle, 0.0f, 0.0f, 1.0f );
	glScalef( scaleX, scaleY, 1.0f );
}