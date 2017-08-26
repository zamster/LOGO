#include "Fx2D.h"
#include <GL/glfw.h>
#define drawOneLine(x1,y1,x2,y2)  glBegin(GL_LINES);  \
	glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();

namespace Fx2D
{
	void Image::LoadJpeg( char *address )
	{
		stbi_uc *buff;
		int comp;
		buff = stbi_jpeg_load( address, &sizex, &sizey, &comp, 0 );
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, sizex, sizey, 0, GL_RGB, GL_UNSIGNED_BYTE, buff);
	}

	void Image::LoadPng( char *address )
	{
		stbi_uc *buff;
		int comp;
		buff = stbi_png_load( address, &sizex, &sizey, &comp, 4 );
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizex, sizey, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);
	}
	
	void Image::SetPos( double x, double y )
	{
		posx = x;
		posy = y;
	}
	void Image::Center()
	{
		centerx = - sizex / 2;
		centery = - sizey / 2;
	}
	void Image::SetDeg( int deg )
	{
		rotate = deg;
	}
	
	void Fontmgr::LoadFont( char *address )
	{
		memset( letter, 0, sizeof(letter) );
		freopen( "Data/font", "r", stdin );
		char x;
		while ( scanf( "%c", &x ) != EOF )
		{	
			getchar();
			int loc = (int) x;
			for ( int i = 0 ; i < 8 ; i++ )
			{
				scanf( "%f", &letter[loc].a[i] );
			}
			getchar();
		}
		alphabet.LoadPng( address );
	}
	
	void Fontmgr::Print( string buff, double x, double y, int color )
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslated( x, y, 0 );
		glScaled( 0.5, 0.5, 0 );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, alphabet.texture);
		for ( unsigned int i = 0 ; i < buff.size(); i++ )
		{
			int tmpletter = (int)buff[i];
			glColor3fv( colors[color] );
			glBegin(GL_QUADS);
			glTexCoord2f( letter[tmpletter].a[0], letter[tmpletter].a[1] ); glVertex2f( -27.5f,  -30.0f );
			glTexCoord2f( letter[tmpletter].a[2], letter[tmpletter].a[3] ); glVertex2f(  27.5f,  -30.0f );
			glTexCoord2f( letter[tmpletter].a[4], letter[tmpletter].a[5] ); glVertex2f(  27.5f,   30.0f );
			glTexCoord2f( letter[tmpletter].a[6], letter[tmpletter].a[7] ); glVertex2f( -27.5f,   30.0f );
			glEnd();
			glTranslated( 40, 0, 0 );		
			glColor3f (1.0, 1.0, 1.0);
		}
		glDisable( GL_TEXTURE_2D );	
		glPopMatrix();
	}


	void Node::add( node x )
	{
		lines.push_back( x );
	}

	void Messager::bind( GLFWkeyfun cbfun )
	{
		glfwSetKeyCallback( cbfun );
	}
	
	bool Messager::listen( int action )
	{
		return glfwGetKey( action );
	}
	
	void Fx2DEngine::Initial()
	{
		glfwInit();
		glfwOpenWindow( 1024, 768, 0, 0, 0, 0, 0, 0, GLFW_WINDOW );
		glfwEnable( GLFW_KEY_REPEAT );
		glfwSwapInterval( 0 );	
		
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClearDepth(1.0f);
		glLineWidth (7);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
		
		glfwSetWindowTitle( "LOGO Fx2D inside!" );
		
		fonts.LoadFont( "Data/font.png" );
		
		status = 1;	
	}

	bool Fx2DEngine::run( )
	{
		glfwGetWindowSize( &width, &height );
		height = height > 0 ? height : 1;
		glViewport( 0, 0, width, height );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D ( 0.0, 1024.f, 0.0, 768.f );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		
		return status;
	}
	
	int Fx2DEngine::CreateThread(  GLFWthreadfun fun, void *arg  )
	{
		thread = glfwCreateThread( fun, arg );
		glfwWaitThread( thread, GLFW_NOWAIT );
		return thread;
	}
	
	void Fx2DEngine::Render( Image img, int blend )
	{
		glPushMatrix();
		glLoadIdentity();
		glEnable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, img.texture );
		
		glTranslated( img.posx, img.posy, 0 );	
		glRotatef(-img.rotate, 0.0f, 0.0f, 1.0f);	
		glColor3fv( colors[blend] );
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( img.centerx, img.centery );
		glTexCoord2f(1.0f, 1.0f); glVertex2f( img.sizex + img.centerx, img.centery );
		glTexCoord2f(1.0f, 0.0f); glVertex2f( img.sizex + img.centerx, img.sizey + img.centery );
		glTexCoord2f(0.0f, 0.0f); glVertex2f( img.centerx, img.sizey + img.centery );
		glEnd();
		glColor3fv( colors[WHITE] );
		glDisable( GL_TEXTURE_2D );
		glPopMatrix();
	}
	
	void Fx2DEngine::Render( vector< node > pipeline )
	{
		glPushMatrix();
		glLoadIdentity();
		for ( unsigned int i = 0 ; i < pipeline.size(); i++ )
		{
			glColor3fv( colors[pipeline[i].color] );
			drawOneLine (pipeline[i].x1, pipeline[i].y1, pipeline[i].x2, pipeline[i].y2);
			glColor3fv( colors[WHITE] );
		}
		glPopMatrix();
	}
	
	void Fx2DEngine::Updata()
	{
		status = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
		glfwSwapBuffers();
	}
	
	void Fx2DEngine::Release()
	{
		glfwTerminate();
	}
};
