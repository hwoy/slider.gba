#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"

using Color = Graphic_Type::Color;

struct Point
{
	u8arm_t x,y;
};

static inline void updategraphic(Graphic &g,const Point &point,const Color &color)
{
	g.pixel(color,point.x,point.y);
}

extern "C"
int main()
{
	Graphic g(GraphicDevice{});
	g.setbgcolor(Color::rgb(0,0,31));

	Keypad keypad(KeypadDevice{});
	Point point{0,0};
	Color color{31,0,0};

	while(true)
	{

		if(keypad.iskey(Keypad::KEY_RIGHT))
		{
			if(point.x<GraphicDevice::COL-1) ++point.x;
			else point.x=0;
		}
		else if(keypad.iskey(Keypad::KEY_LEFT))
		{
			if(point.x>0) --point.x;
			else point.x=GraphicDevice::COL-1;
		}
		else if(keypad.iskey(Keypad::KEY_UP))
		{
			if(point.y>0) --point.y;
			else point.y=GraphicDevice::ROW-1;
		}
		else if(keypad.iskey(Keypad::KEY_DOWN))
		{
			if(point.y<GraphicDevice::ROW-1) ++point.y;
			else point.y=0;
		}
		else if(keypad.iskey(Keypad::KEY_START))
		{
			g.setbgcolor(Color::rgb(0,0,31));
		}
		else if(keypad.iskey(Keypad::KEY_SELECT))
		{
			break;
		}
		else 
		{
			continue;
		}

		keypad.reset();
		updategraphic(g,point,color);
	}

	return 0;
}
