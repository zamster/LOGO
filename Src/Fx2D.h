#ifndef FX2D_H_
#define FX2D_H_

#include "Singleton.h"
#include "stdbi.h"
#include "FxDLL.h"

#include <string>
#include <cmath>
#include <vector>

using namespace std;

typedef struct
{
	int color; 
	double x1, y1, x2, y2;
} node;

typedef struct
{
	float a[8];
} font;

namespace Fx2D
{
	enum COLOR
	{
		BLACK = 0,
		RED = 1,
		GREEN = 2,
		YELLOW = 3,
		BLUE = 4,
		PINK = 5,
		IVY = 6,
		WHITE = 7
		
	};
	const float colors[][3] = 
	{
		{0, 0, 0},
		{1, 0, 0},
		{0, 1, 0},
		{1, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{0, 1, 1},
		{1, 1, 1},
	};

	//��ȡ��ͼ
	class FxDLL Image
	{				
	public:
		
		unsigned int texture;		
		int rotate;
		int sizex, sizey;
		double centerx, centery;
		double posx, posy;
		Image()
		{
			rotate = 0;
			sizex = 0;
			sizey = 0;
			posx =0;
			posy = 0;
			centerx = 0;
			centery = 0;
		}
		
		void LoadJpeg( char *address );
		void LoadPng( char *address );
		
		void SetDeg( int deg );
		void SetPos( double x, double y );
		void Center( );//����
	};
	
	//����
	class FxDLL Fontmgr
	{
	private:
		font letter[256];
		Image alphabet;
	public:
		void LoadFont( char *address );
		void Print( string buff, double x, double y, int color );
	};	
	
	class FxDLL Messager
	{
	public:
		int key;
		string cmd;
		void bind( void cbfun(int, int)  ); //����Ϣ��Ӧ����
		bool listen( int action ); //�����¼�
	};
	
	//����Ⱦ�߶�
	class FxDLL Node
	{
	public:
		vector< node > lines;
		Node()
		{
			lines.clear();
		}
		void add( node x );
	};
	
	//�������壬�ۺ��������һЩ����
	class FxDLL Fx2DEngine
	{
	public:
		int width, height, status;
		Fontmgr fonts;
		Messager mPasser;
		int thread;//�����߳�		
		
		void Initial();
		bool run();
		int CreateThread( void fun( void * ), void *arg );
		void Render( Image img, int blend );
		void Render( vector< node > pipeline );
		void Updata();
		void Release();
	};
	
	FxDLL Fx2DEngine *createFX2DDevice();
};

#endif
