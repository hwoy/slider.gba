#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Number.hpp"

extern "C"
int main()
{
	Graphic g(GraphicDevice{});
	g.setbgcolor(Color::rgb(0,0,31));
	
	Color color{31,0,0};

	for(u8arm_t i=0;i<10;++i)
		Number::print(g,{0,static_cast<u8arm_t>(i*8)},color,i);



	while(true);


	return 0;
}
