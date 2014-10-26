#pragma once

#ifndef _MATH_UTILITIES_H_
#define _MATH_UTILITIES_H_

#include <time.h>
#include <cstdlib>

namespace Math
{
	const float PI = 3.141592653f;

	inline int RandInRange( int min, int max )
	{		
		return min + rand() % ( max - min + 1 );
	}

	inline float RandInRange( float min, float max )
	{		
		return min + ( max - min ) * ( rand() / (float) RAND_MAX );
	}

	inline double RandInRange( double min, double max )
	{
		return min + ( max - min ) * ( rand() / (double) RAND_MAX );
	}

	inline float ConvertToRad( float degrees )
	{
		return degrees * ( PI / 180 );
	}

	inline float ConvertToDegrees( float radians )
	{
		return radians * ( 180 / PI );
	}

	inline float CalculateSquaredDistance( float x1, float y1, float x2, float y2 )
	{
		float xdiff = x2 - x1;
		float ydiff = y2 - y1;

		return ( xdiff * xdiff ) + ( ydiff * ydiff );
	}

	template< typename T >
	T Clamp( T x, T min, T max )
	{
		assert( max >= min );
		return min( max( x, min), max);
	}

	template< typename T >
	T Sign( T x )
	{	
		return ( x < 0 ) ?  static_cast< T >( -1 ) : ( x > 0) ? static_cast< T >( 1 ) : 0;
	}

	struct Rectangle
	{		
		Rectangle()
			: left( 0 )
			, top( 0 )
			, right( 0 )
			, bottom( 0 )
		{}

		// DATA
		//
		int left;
		int right;
		int top;
		int bottom;
	};

}

#endif