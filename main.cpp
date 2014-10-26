#include "IncludeGL.h"
#include "Vector2.h"
#include "Primitives.h"
#include "Utility.h"
#include <sstream>
#include <iostream>
#include "MathUtilities.h"
#include "TimeManager.h"

enum GameMode { IDLE, GAMEOVER, PLAYING, SCORED };

GameMode currentGameMode = IDLE;

TimeManager* timer;
int frameCounter = 0;

// WORLD PROPERTIES
const int TRIES							= 10;
const float PLAYFIELD_OFFSET			= 1.0f;
const float PADDLE_PLAYFIELD_OFFSET		= 10.0f;
const float WORLD_DIMENSIONS[ 2 ]	    = { 1136.f, 640.f };
const float WORLD_DIMENSIONS_HALF[ 2 ]  = { WORLD_DIMENSIONS[ 0 ] * 0.5f, WORLD_DIMENSIONS[ 1 ] * 0.5f };

// PLAYER INFO
float AI_DRAG	  = 1.2f;//0.8f;
float player1PosX =  WORLD_DIMENSIONS_HALF[0] - PADDLE_PLAYFIELD_OFFSET;
float player2PosX = -WORLD_DIMENSIONS_HALF[0] + PADDLE_PLAYFIELD_OFFSET;
float player1PosY = 0.f;
float player2PosY = 0.f;
int player1Score  = 0;
int player2Score  = 0;

// QUAD SIZES
const float PADDLE_HEIGHT = 80.0f;
const float PADDLE_WIDTH  = 20.0f;
const float BALL_SIZE	  = 15.0f;

// BALL PROPERTIES
float BALL_SPEED = 4.0f;
Math::Vector2f BALL_VELOCITY( 1.f, 1.f );
Math::Vector2f BALL_POSITION( 0.f, 0.f );


const float halfWorldY					= WORLD_DIMENSIONS_HALF[1] - PADDLE_HEIGHT * 0.5f;

const float FRAMES_PER_SECOND = 1 / 60.f;



Math::Vector2f PADDLE_MINS( -PADDLE_WIDTH  * 0.5f, -PADDLE_HEIGHT * 0.5f );
Math::Vector2f PADDLE_MAXS(  PADDLE_WIDTH  * 0.5f,  PADDLE_HEIGHT * 0.5f );

Math::Vector2f BALL_MINS( -BALL_SIZE  * 0.5f, -BALL_SIZE * 0.5f );
Math::Vector2f BALL_MAXS(  BALL_SIZE  * 0.5f,  BALL_SIZE * 0.5f );

void shutdown(void);

void DrawPlayField()
{
	// Upper line
	Drawing::DrawLine( Math::Vector2f(  WORLD_DIMENSIONS_HALF[0], WORLD_DIMENSIONS_HALF[1] - PLAYFIELD_OFFSET ), 
		Math::Vector2f( -WORLD_DIMENSIONS_HALF[0], WORLD_DIMENSIONS_HALF[1] - PLAYFIELD_OFFSET ), 
		Drawing::SOLID,
		3.0f );


	// Lower line
	Drawing::DrawLine( Math::Vector2f(  WORLD_DIMENSIONS_HALF[0], -WORLD_DIMENSIONS_HALF[1] + PLAYFIELD_OFFSET ), 
		Math::Vector2f( -WORLD_DIMENSIONS_HALF[0], -WORLD_DIMENSIONS_HALF[1] + PLAYFIELD_OFFSET ), 
		Drawing::SOLID,
		3.0f );

	// Dividing line
	Drawing::DrawLine( Math::Vector2f( 0.f, WORLD_DIMENSIONS_HALF[1] ), Math::Vector2f( 0.0f, -WORLD_DIMENSIONS_HALF[1] ), Drawing::DASHED );
}

void DrawScores()
{
	std::stringstream player1;
	std::stringstream player2;

	if( player1Score < 10 )
		player1 << "0" << player1Score;
	else
		player1 << player1Score;

	if( player2Score < 10 )
		player2 << "0" << player2Score;
	else				 
		player2 << player2Score;

	Utilities::OutPutText( -100.f, WORLD_DIMENSIONS_HALF[1] - 50.f, 24, (unsigned char*) player1.str().c_str() );
	Utilities::OutPutText(  100.f, WORLD_DIMENSIONS_HALF[1] - 50.f, 24, (unsigned char*) player2.str().c_str() );
}

void DrawPaddle( float x, float y )
{
	glPushMatrix();

	Drawing::ApplyTransformations( x, y, 0.0f, 1.0f, 1.0f );

	Drawing::DrawQuad( PADDLE_WIDTH, PADDLE_HEIGHT );
	//Drawing::DrawBoundinBox( PADDLE_MINS, PADDLE_MAXS );

	glPopMatrix();
}

void DrawBall( float x, float y )
{
	glPushMatrix();

	Drawing::ApplyTransformations( x, y, 0.0f, 1.0f, 1.0f );

	Drawing::DrawQuad( BALL_SIZE, BALL_SIZE );
	//Drawing::DrawBoundinBox( BALL_MINS, BALL_MAXS );

	glPopMatrix();
}

// Callback functions for major events.
//
void onResize( int width, int height )
{
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 
		-WORLD_DIMENSIONS_HALF[ 0 ], WORLD_DIMENSIONS_HALF[ 0 ], 
		-WORLD_DIMENSIONS_HALF[ 1 ], WORLD_DIMENSIONS_HALF[ 1 ], 
		-1.0f, 1.0f );
}

bool CheckColission( float positionX, float positionY )
{
	Math::Vector2 minsPaddle = Math::Vector2 ( positionX, positionY) + PADDLE_MINS;
	Math::Vector2 maxsPaddle = Math::Vector2 ( positionX, positionY) + PADDLE_MAXS;

	// Calculating the bounding box points for the object to collide with
	Math::Vector2 minsBall = Math::Vector2( BALL_POSITION.x, BALL_POSITION.y ) + BALL_MINS;
	Math::Vector2 maxsBall = Math::Vector2( BALL_POSITION.x, BALL_POSITION.y ) + BALL_MAXS;

	for (int i = 0; i < 2; ++i)
	{
		if (maxsPaddle[i] < minsBall[i] || minsPaddle[i] > maxsBall[i])
		{
			return false;
		}
	}

	// Overlapping on all axes means AABBs are intersecting
	return true;
}

void Draw()
{
	// FIELD
	DrawPlayField();

	// HUD
	DrawScores();

	// Drawing paddles
	DrawPaddle(  WORLD_DIMENSIONS_HALF[0] - PADDLE_PLAYFIELD_OFFSET, player1PosY  );
	DrawPaddle( -WORLD_DIMENSIONS_HALF[0] + PADDLE_PLAYFIELD_OFFSET, player2PosY  );


	if( currentGameMode == SCORED )
	{
		frameCounter++;
		if( frameCounter == 30.f )
		{
			frameCounter = 0;
			BALL_POSITION.x = 0;
			BALL_POSITION.y = 0;
			currentGameMode = PLAYING;
		}
	}

	if( currentGameMode != SCORED )
	{
		// Draw the ball
		DrawBall( BALL_POSITION.x, BALL_POSITION.y );
	}
}

void Update()
{	
	// Collisions!

	// Upper & lower bounds!
	if( BALL_POSITION.y + BALL_SPEED > WORLD_DIMENSIONS_HALF[1] )
		BALL_VELOCITY.y = -1;
	else if ( BALL_POSITION.y - BALL_SPEED < -WORLD_DIMENSIONS_HALF[1] )
		BALL_VELOCITY.y = 1;

	// You're scoring baby! 
	if( BALL_POSITION.x + BALL_SPEED >  WORLD_DIMENSIONS_HALF[0] )
	{
		BALL_VELOCITY.x = -1;
		player1Score++;
		currentGameMode = SCORED;
		if( player1Score == TRIES ) currentGameMode = GAMEOVER;
	}
	else if ( BALL_POSITION.x - BALL_SPEED < -WORLD_DIMENSIONS_HALF[0] )
	{
		BALL_VELOCITY.x = 1;
		player2Score++;
		currentGameMode = SCORED;
		if( player2Score == TRIES ) currentGameMode = GAMEOVER;
	}

	// Collision with left paddle.
	bool left = CheckColission( player2PosX, player2PosY );
	bool right = CheckColission( player1PosX, player1PosY );
	if( left || right)
	{
		BALL_VELOCITY.x *= -1;
	}

	BALL_POSITION.x += BALL_SPEED * BALL_VELOCITY.x * 0.9f;
	BALL_POSITION.y += BALL_SPEED * BALL_VELOCITY.y * 0.9f;

	player2PosY = Utilities::Clamp( BALL_POSITION.y * AI_DRAG, -halfWorldY, halfWorldY );
}

void updateFrame()
{
	// Clearing the screen
	glClearColor( 0.1f, 0.1f, 0.15f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	double startTime = timer->getElapsedTime();//Utilities::GetAbsoluteTimeSeconds();
	
	if( currentGameMode == PLAYING)
	{
		// Update
		Update();
	}

	if( currentGameMode != IDLE )
	{
		// Draw!
		Draw();	
		if( currentGameMode == GAMEOVER)
		{
			Utilities::OutPutText( -80.f, 0.f, 24, (unsigned char*)"GAME OVER!" );
			Utilities::OutPutText( -100.0f, -100.f, 24, (unsigned char*)"Click anywhere to Start" );
		}
	}
	else if( currentGameMode == IDLE )
	{
		Utilities::OutPutText( -100.f, 0.f, 24, (unsigned char*)"WELCOME TO PONG!" );
		Utilities::OutPutText( -100.0f, -100.f, 24, (unsigned char*)"Click anywhere to Start" );
	}

	double endTime = timer->getElapsedTime();//Utilities::GetAbsoluteTimeSeconds();
	double nextTimeFrame = startTime + FRAMES_PER_SECOND;

	while( endTime < nextTimeFrame )
	{
		// Spin lock
		endTime = timer->getElapsedTime();//Utilities::GetAbsoluteTimeSeconds();
	}

	// Swapping the buffers
	glutSwapBuffers();

	// Calling update frame again
	glutPostRedisplay();
}

void onKeyboardEvent( unsigned char key, int x, int y )
{
	if( key == 'Q' || key == 'q' )
    {
        shutdown();
    }
}

void onMouseMovementEvent( int x, int y )
{
	float tempY =
		- ( ( y - WORLD_DIMENSIONS_HALF[ 1 ] ) 
		+ ( PADDLE_HEIGHT / 2) );

	player1PosY = Utilities::Clamp( tempY, -halfWorldY, halfWorldY );
}

void onMouseClick(int button, int state, int x, int y)
{
	if ( button == GLUT_LEFT_BUTTON )
	{ 
		if( currentGameMode != PLAYING )
		{

			//store the x,y value where the click happened
			currentGameMode = PLAYING;
			player1Score = 0;
			player2Score = 0;

			float rdm = Math::RandInRange( -10.0, 10.0 );
			BALL_POSITION.x = 0;
			BALL_POSITION.y = 0;
			Math::Vector2f BALL_VELOCITY( (rdm > 0) ? 1.f : -1.0f, 1.f );

		}
	}	
}

void shutdown()
{
    delete timer;    

    exit( 0 );  
}

int main( int argc, char** argv )
{
	// Initialize
	//
	glutInit( &argc, argv );			// Initialize GLUT
	glutInitDisplayMode( GLUT_DOUBLE );	// Define the graphics buffer setup
	glutInitWindowSize( static_cast< int >( WORLD_DIMENSIONS[0] ), static_cast< int >( WORLD_DIMENSIONS[1] ) );		// Set the size of the window
	glutCreateWindow( "Pong!" );		// Create the window

	// Tell GLUT what functions to call for important events.
	//
	glutDisplayFunc( updateFrame );		// Every frame
	glutReshapeFunc( onResize );		// When the window changes size
	glutKeyboardFunc( onKeyboardEvent );	// When a keystroke occurs

	// TODO setup other call backs such as special keys, mouse input.
	// Mouse events
	glutPassiveMotionFunc( onMouseMovementEvent );
	glutMouseFunc( onMouseClick );

	timer = new TimeManager;

	std::cout << "Click anywhere on the screen to start.\n";

	// Start the main loop
	//
	glutMainLoop(); 

	return 0;
}
