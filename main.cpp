#include "Graphic.hpp"
#include "noreturn.hpp"


extern "C"
int main()
{
	GraphicDrvice gd;
	Graphic g(gd);

	g.setbgcolor({0,0,31});

	for(unsigned char x = 20; x<=60; x+=15)
		for(unsigned char y = 30; y<50; y++)  
			g.pixel({31,31,31},x,y);
				
	for (unsigned char x = 20; x < 35; x++)
		g.pixel({31,31,31},x,40);

	noreturn();
}
