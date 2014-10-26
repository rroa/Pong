#include "Primitives.h"
#include "IncludeGL.h"

namespace Drawing
{
	void DrawLine( const Math::Vector2f lineStart, const Math::Vector2f lineEnd, LINE_TYPE lineType, float lineWidth )
	{
#ifdef EMSCRIPTEN
#else
		glPushMatrix();

		// LINES
		glLineWidth( lineWidth );

		if( lineType != SOLID )
		{
			glEnable ( GL_LINE_STIPPLE );

			if( lineType == DASHED )
				glLineStipple (1, 0x00FF);   /*  dashed   */
			else
				glLineStipple (1, 0x0101); /* dotted */
		}

		glBegin( GL_LINES );
			glVertex2f( lineStart.x, lineStart.y );
			glVertex2f( lineEnd.x, lineEnd.y );
		glEnd();

		if( lineType != SOLID )
		{
			glDisable( GL_LINE_STIPPLE );
		}

		glPopMatrix();
#endif		
	}

	void DrawQuad( int sizeX ,int sizeY, 
		const Math::Vector2& uv1, 
		const Math::Vector2& uv2,
		const Math::Vector2& uv3,
		const Math::Vector2& uv4
		)
	{
		Math::Vector2 mins( -sizeX * 0.5f,-sizeY * 0.5f );
		Math::Vector2 maxs(  sizeX * 0.5f, sizeY * 0.5f );

		glBegin( GL_TRIANGLE_STRIP );

			glTexCoord2f( uv1.x, uv1.y );
			glVertex2f( mins.x,  maxs.y );

			glTexCoord2f( uv2.x, uv2.y );
			glVertex2f(  maxs.x,  maxs.y );

			glTexCoord2f( uv3.x, uv3.y );
			glVertex2f( mins.x, mins.y);

			glTexCoord2f( uv4.x, uv4.y );
			glVertex2f(  maxs.x, mins.y );

		glEnd();
	}

	void DrawBoundinBox( const Math::Vector2& mins , const Math::Vector2& maxs )
	{
		Math::Vector2 topLeft    ( mins.x,  maxs.y );
		Math::Vector2 bottomLeft ( mins.x,  mins.y );
		Math::Vector2 topRight   ( maxs.x,  maxs.y );
		Math::Vector2 bottomRight( maxs.x,  mins.y );		

		glPushMatrix();

		// Disabling texture 2D
		glDisable( GL_TEXTURE_2D );		

		glBegin( GL_LINE_STRIP );

			glVertex2fv( &topLeft.x );
			glVertex2fv( &bottomLeft.x );
			glVertex2fv( &bottomRight.x );
			glVertex2fv( &topRight.x );
			glVertex2fv( &topLeft.x );

		glEnd();

		// Enabling texture 2D
		glEnable(GL_TEXTURE_2D);

		glPopMatrix();
	}

}