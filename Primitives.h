#pragma once

#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "Vector2.h"

namespace Drawing
{	
	enum LINE_TYPE { DOTTED, DASHED, SOLID };

	void DrawLine( const Math::Vector2f lineStart, 
				   const Math::Vector2f lineEnd, 
				   LINE_TYPE lineType = SOLID, 
				   float lineWidth = 2.0f );

	void DrawQuad( int sizeX ,int sizeY,
		const Math::Vector2& uv1 = Math::Vector2( 0.f, 0.f ),
		const Math::Vector2& uv2 = Math::Vector2( 1.f, 0.f ),
		const Math::Vector2& uv3 = Math::Vector2( 0.f, 1.f ),
		const Math::Vector2& uv4 = Math::Vector2( 1.f, 1.f )
		);	

	void DrawBoundinBox(const Math::Vector2& mins , const Math::Vector2& Maxs);
}

#endif