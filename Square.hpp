#ifndef __SQUARE_HPP__
#define __SQUARE_HPP__

#include "arm7type.h"
#include "Graphic.hpp"
#include "Font.hpp"

template <class COLORMODE>
struct Square
{
	using Color_t = typename COLORMODE::Color_t;
	
	u8arm_t width;
	Color_t color,ncolor;

	void draw(const Graphic<COLORMODE> &g,const Point &point,u8arm_t ch) const
	{
		g.rectangle(color,point.x,point.y,point.x+width-1,point.y+width-1);

		const u8arm_t x=point.x+(width-Font<COLORMODE>::SIZE)/2;
		const u8arm_t y=point.y+(width-Font<COLORMODE>::SIZE)/2;

		Font<COLORMODE>::draw(g,{x,y},ncolor,ch);
	}
};


#endif

