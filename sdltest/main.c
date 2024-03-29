//
//  main.c
//  sdltest
//
//  Created by Alexander Wingård on 2012-11-13.
//  Copyright (c) 2012 Alexander Wingård. All rights reserved.
//
#include <SDL/SDL.h>
#ifdef __APPLE__
#include "SDLMain.h"
#include <GLUT/GLUT.h>
#elif __gnu_linux__
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <stdio.h>

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

/* Define our booleans */
#define TRUE  1
#define FALSE 0

#define PI 3.1415926535897932384626433832795

typedef struct 
{
	GLfloat x;
	GLfloat y;
	int active;
	int quit;
} sState;

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{
    /* clean up the window */
    SDL_Quit( );
    
    /* and exit appropriately */
    exit( returnCode );
}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;
    
    /* Protect against a divide by zero */
    if ( height == 0 )
        height = 1;
    
    ratio = ( GLfloat )width / ( GLfloat )height;
    
    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
    
    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    
    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    
    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );
    
    /* Reset The View */
    glLoadIdentity( );
    
    return( TRUE );
}

/* function to handle key press events */
void handleKeyPress(SDL_Surface *surface, sState *state, SDL_keysym const *keysym)
{
    switch ( keysym->sym )
	{
        case SDLK_ESCAPE:
            /* ESC key was pressed */
            Quit( 0 );
            break;
        case SDLK_F1:
            /* F1 key was pressed
             * this toggles fullscreen mode
             */
            SDL_WM_ToggleFullScreen( surface );
            break;
        case SDLK_w:
            state->x = 0.1f;
            break;
        case SDLK_s:
            state->x -= 0.1f;
            break;
        default:
            break;
	}
    
    return;
}

/* general OpenGL initialization function */
int initGL( GLvoid )
{
    
    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );
    
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    
    /* Depth buffer setup */
    glClearDepth( 1.0f );
    
    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );
    
    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );
    
    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    
    return( TRUE );
}

/* Here goes our drawing code */
int drawGLScene( sState const *state )
{
    /* rotational vars for the triangle and quad, respectively */
    static GLfloat rtri, rquad;
    /* These are to calculate our fps */
    static GLint T0     = 0;
    static GLint Frames = 0;
    
    
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    gluLookAt(state->x, state->y, -20, 0, 0, 0, 0, 1, 0);
    glPushMatrix();
    glTranslatef( -1.5f, 0.0f, 0.0f );
        
    /* Rotate The Triangle On The Y axis ( NEW ) */
    glRotatef( rtri, 0.0f, 1.0f, -6.0f );
    
    glBegin( GL_TRIANGLES );             /* Drawing Using Triangles       */
    glColor3f(   1.0f,  0.0f,  0.0f ); /* Red                           */
    glVertex3f(  0.0f,  1.0f,  0.0f ); /* Top Of Triangle               */
    glColor3f(   0.0f,  1.0f,  0.0f ); /* Green                         */
    glVertex3f( -1.0f, -1.0f,  0.0f ); /* Left Of Triangle              */
    glColor3f(   0.0f,  0.0f,  1.0f ); /* Blue                          */
    glVertex3f(  1.0f, -1.0f,  0.0f ); /* Right Of Triangle             */
    glEnd( );                            /* Finished Drawing The Triangle */
    
    /* Move Right 3 Units */
    glPopMatrix();
    glTranslatef( 1.5f, 0.0f, 0.0f );
    
    /* Rotate The Quad On The X axis ( NEW ) */
    glRotatef( rquad, 1.0f, 0.0f, -6.0f );
    
    /* Set The Color To Blue One Time Only */
    glColor3f( 0.5f, 0.5f, 1.0f);
    
    glBegin( GL_QUADS );                 /* Draw A Quad              */
    glVertex3f(  1.0f,  1.0f,  0.0f ); /* Top Right Of The Quad    */
    glVertex3f( -1.0f,  1.0f,  0.0f ); /* Top Left Of The Quad     */
    glVertex3f( -1.0f, -1.0f,  0.0f ); /* Bottom Left Of The Quad  */
    glVertex3f(  1.0f, -1.0f,  0.0f ); /* Bottom Right Of The Quad */
    glEnd( );                            /* Done Drawing The Quad    */
    
    /* Draw it to the screen */
    SDL_GL_SwapBuffers( );
    
    /* Gather our frames per second */
    Frames++;
    {
        GLint t = SDL_GetTicks();
        if (t - T0 >= 5000) {
            GLfloat seconds = (t - T0) / 1000.0;
            GLfloat fps = Frames / seconds;
            printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
            T0 = t;
            Frames = 0;
        }
    }
    
    /* Increase The Rotation Variable For The Triangle ( NEW ) */
    rtri  += 0.2f;
    /* Decrease The Rotation Variable For The Quad     ( NEW ) */
    rquad -=0.15f;
    
    return( TRUE );
}

//@param flags Video flags for SDL video subsystem
//@return 0 on success.
static int setupVideo(int *flags, SDL_Surface *surface)
{
    int videoFlags = 0;
    /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
	    fprintf( stderr, "Video initialization failed: %s\n",
                SDL_GetError( ) );
	    return 1;
	}
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );
    
    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
                SDL_GetError( ) );
	    return 1;
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
    //videoFlags |= SDL_FULLSCREEN;
    
    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
        videoFlags |= SDL_HWSURFACE;
    else
        videoFlags |= SDL_SWSURFACE;
    
    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
        videoFlags |= SDL_HWACCEL;
    
    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, TRUE);

    *flags = videoFlags;

    /* get a SDL surface */
    surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
		videoFlags );

    /* Verify there is a surface */
    if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    return 1;
	}

    /* initialize OpenGL */
    initGL( );
    
    /* resize the initial window */
    resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );
    SDL_WM_GrabInput( SDL_GRAB_ON );
    SDL_ShowCursor(0);
    
    return 0;
}

static void handleEvent(SDL_Event const *event, sState *state,
	SDL_Surface *surface, int videoFlags)
{
	switch(event->type)
	{
		case SDL_ACTIVEEVENT:
		{
			state->active = event->active.gain;
			break;
		}	
		case SDL_VIDEORESIZE:
		{
			/* handle resize event */
			surface = SDL_SetVideoMode( event->resize.w,
				event->resize.h, 16, videoFlags );
			if ( !surface )
			{
				fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				Quit( 1 );
			}
			resizeWindow( event->resize.w, event->resize.h );
			break;
		}
		case SDL_KEYDOWN:
		{
			/* handle key presses */
			handleKeyPress(surface, state, &event->key.keysym);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			state->y += event->motion.yrel/10.0f;
			state->x += event->motion.xrel/10.0f;
			break;
		}
		case SDL_QUIT:
		{
			state->quit = TRUE;
			break;
		}
		default:
			break;
	}
}

static int eventLoop(int videoFlags, SDL_Surface *surface)
{
    int done = FALSE;
    SDL_Event event;
	sState state;
	state.active = TRUE;
    state.quit = FALSE;

    /* wait for events */
    while(!state.quit)
	{
	    while(SDL_PollEvent(&event))
		{
			handleEvent(&event, &state, surface, videoFlags);
		}
        
	    if(state.active)
		{
            drawGLScene(&state);
		}
	}
}

int main( int argc, char **argv )
{
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags = 0;
	/* This is our SDL surface */
	SDL_Surface *surface = NULL;
    {
        int retval = 0;
        if((retval = setupVideo(&videoFlags, surface)) != 0)
        {
            Quit(retval);
        }
    }
	Quit(eventLoop(videoFlags, surface));
}
