#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include "Fx2D.h"
#include <irrKlang/irrKlang.h>

using namespace irrklang;
using namespace Fx2D;
const double m_2pi = 2 * acos(-1);

class Mutex
{
public:
	int status;
	Mutex()
	{
		status = 0;
	}
	void lock()
	{
		status = 1;
	}
	void unlock()
	{
		status = 0;
	}
};

Mutex THREAD, STATUS;
class app : public Singleton< app >
{
public:
	Node segs;
	Image canvas, turtle;
	
	int curdeg;
	int speed;
	bool ispenup;
	COLOR curcolor;
	double curx, cury;

	void init()
	{
		speed = 20;
		canvas.LoadJpeg( "Data/BG/4.jpg" );
		turtle.LoadPng( "Data/dir.png" );	
		turtle.SetPos( 512, 384 );
		turtle.Center();
		reset();
	}
	
	void setdeg( int deg )
	{
		curdeg = deg;
	}	
	
	void setcolor( COLOR color )
	{
		curcolor = color;
	}
	
	void rt( int deg )
	{
		curdeg += deg;
		turtle.SetDeg( curdeg );
	}
	
	void lt( int deg )
	{
		curdeg -= deg;
		turtle.SetDeg( curdeg );
	}
	
	void fd( int len )
	{
		node tmp;
		tmp.color = curcolor;
		tmp.x1 = curx;
		tmp.y1 = cury;
		curx =  curx + len * sin(  m_2pi *  curdeg / 360 );
		cury =  cury + len * cos(  m_2pi *  curdeg / 360 );
		tmp.x2 = curx;
		tmp.y2 = cury;
		if ( !ispenup )
		segs.add( tmp );
		turtle.SetPos( curx, cury );
	}
	
	void bk( int len )
	{
		node tmp;
		tmp.color = curcolor;
		tmp.x1 = curx;
		tmp.y1 = cury;
		curx =  curx - len * sin(  m_2pi *  curdeg / 360 );
		cury =  cury - len * cos(  m_2pi *  curdeg / 360 );
		tmp.x2 = curx;
		tmp.y2 = cury;
		if ( !ispenup )
		segs.add( tmp );
		turtle.SetPos( curx, cury );
	}
	
	void reset()
	{
		curdeg = 0;
		curcolor = WHITE;
		ispenup = false;
		curx = 512.f;
		cury = 384.f;
		
		setcolor( curcolor );
		setdeg( curdeg );
		
		segs.lines.clear();
		turtle.SetPos( curx, cury );		
		turtle.SetDeg( curdeg );
	}
	
};

INITIALISESINGLETON(app);
Fx2DEngine mFx2D;
app LOGO;

int getvalue( string str, int pos, int &exc )
{
	int x = 0;
	string::size_type i;
	for ( i = pos + 1; i < str.size(); i++ )
	{
		if ( str[i] >= '0' && str[i] <= '9' )
		{
			x = ( x + ( str[i] - '0' ) ) * 10;
		}
		else
		{
			break;
		}
	}
	exc = (int) i;
	x /= 10;
	return x;
}

void Parser( string buf )
{
	string cmd;
	for ( string::size_type i = 0; i < buf.size(); i++ )
	{
		if ( buf[i] != ' ' )
		cmd += buf[i];
		else
		{
			int exc = 0;
			int val = 0;
			val = getvalue( buf, i, exc );
			
			if ( cmd == "REPEAT" )
			{
				string sub;
				int match = -1;
				for ( string::size_type subindex = exc + 1; subindex < buf.size(); subindex++ )
				{
					if ( buf[subindex] == '[' )
					match--;
					else if ( buf[subindex] == ']' )
					match++;
					
					if ( match == 0 )
					break;
					else
					sub += buf[subindex];				
				}
				for ( int repeat = 0; repeat < val; repeat++ )
				{
					Parser( sub );
				}
				i += sub.size() - 2;
			}
			else if ( cmd == "FD" )
			{
				while ( 1 )
				{
					if ( THREAD.status == 0 )
					{
						THREAD.lock();
						LOGO.fd( val );
						THREAD.unlock();
						break;
					}
				}
				Sleep(LOGO.speed);
			}
			else if ( cmd == "BK" )
			{
				if ( THREAD.status == 0 )
				{
					THREAD.lock();
					LOGO.bk( val );
					THREAD.unlock();
					break;
				}
				Sleep(LOGO.speed);
			}
			else if ( cmd == "RT" )
			{
				LOGO.rt( val );
			}
			else if ( cmd == "LT" )
			{
				LOGO.lt( val );
			}
			else if ( cmd == "COLOR" )
			{
				LOGO.setcolor( (COLOR)val );
			}
			else if ( cmd == "PEN" )
			{
				LOGO.ispenup = val;
			}
			else if ( cmd == "SPEED" )
			{
				if ( val < 1000 )
				LOGO.speed = 1000 - val;
				else
				LOGO.speed = 500;
			}
			i += exc - 2;
			cmd.clear();
		}
	}
}
void event( void *arg )
{
	string buf( (char*) arg );
	
	if( buf.find( "CLS", 0 ) != string::npos )
		LOGO.reset();
	else
		Parser( mFx2D.mPasser.cmd );
	
	mFx2D.mPasser.cmd.clear();		
	STATUS.unlock();
}

void keytest( int key, int action )
{
	if( action != 1 )
	return;
	if ( key == 295 )
	{
		if ( mFx2D.mPasser.cmd.size() )
		mFx2D.mPasser.cmd.erase( mFx2D.mPasser.cmd.end() - 1 );
	}
	else if ( ( key == 32 )|( key >= 48 && key <= 57 )|( key >= 65 && key <= 90 ) | ( key == 91 )| ( key == 93 ) )	
	{
		mFx2D.mPasser.cmd += key;;
	}
}

int main( void )
{
	mFx2D.Initial();
	mFx2D.mPasser.bind( keytest );	
	ISoundEngine* sound = createIrrKlangDevice();
	sound->play2D( "data/bgm.mp3", true);
	LOGO.init();
	while( mFx2D.run() )
	{		
		mFx2D.Render( LOGO.canvas, WHITE );
		
		
		if ( THREAD.status == 0 )
		{
			THREAD.lock();
			mFx2D.Render( LOGO.segs.lines );
			THREAD.unlock();
		}
		mFx2D.Render( LOGO.turtle, LOGO.curcolor  );		
		
		mFx2D.fonts.Print( mFx2D.mPasser.cmd, 13.75, 15, LOGO.curcolor );
		if ( mFx2D.mPasser.listen( 294 ) )
		{
			if ( STATUS.status == 0 )
			{
				STATUS.lock();
				char cmdd[256];
				strcpy( cmdd, mFx2D.mPasser.cmd.c_str() );
				mFx2D.CreateThread( event, cmdd );
			}			
		}
		mFx2D.Updata();
	}
	
	mFx2D.Release();
	sound->drop(); 
	
	return 0;
}
