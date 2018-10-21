#ifndef __SQUARE_HPP__
#define __SQUARE_HPP__

#include "arm7type.h"
#include "Graphic.hpp"
#include "Font.hpp"


struct Square
{
	u8arm_t width;
	Color color,ncolor;

	void draw(Graphic &g,const Point &point,u8arm_t ch) const
	{
		g.rectangle(color,point.x,point.y,point.x+width-1,point.y+width-1);

		const u8arm_t x=point.x+(width-Font::SIZE)/2;
		const u8arm_t y=point.y+(width-Font::SIZE)/2;

		Font::draw(g,{x,y},ncolor,ch);
	}
};


#endif

