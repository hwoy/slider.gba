#include "arm7type.hpp"
#include "Graphic.hpp"
#include "noreturn.hpp"

using Color = Graphic_Type::Color;

extern "C"
int main()
{
	Graphic g(GraphicDevice{});

	g.setbgcolor(Color::rgb(0,0,31));

	for(u8arm_t x = 20; x<=60; x+=15)
		for(u8arm_t y = 30; y<50; y++)  
			g.pixel(Color::rgb(31,31,31),x,y);
				
	for (u8arm_t x = 20; x < 35; x++)
		g.pixel(Color::rgb(31,31,31),x,40);

	noreturn();
}
