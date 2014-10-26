#pragma once

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
#include <cstdlib>
#include <assert.h>

#if defined( _WIN32 ) && defined( _DEBUG )
#	define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#	define new DEBUG_NEW
#endif

namespace Utilities
{
	std::string GetFileContents( const char *filename );
	void		OutPutText( float x, float y, int fontSize, unsigned const char* text );
	void		ReportError( const std::string& message );
	void		ReportMemoryLeaks();	

	template< typename T >
	T Clamp( T x, T min, T max )
	{
		assert( max >= min );
		return std::min( std::max( x, min), max);
	}

	template< typename T >
	T Lerp( T a, T b, float n )
	{
		T min = a;
		T max = b;

		return (max-min)*n + min;
	}
}

namespace Drawing
{
	void ApplyTransformations( float x = 0.0f, float y = 0.0f, float angle = 0.0f, float scaleX = 0.0f, float scaleY = 0.0f );
}

#endif