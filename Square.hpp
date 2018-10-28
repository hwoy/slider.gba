#ifndef __SQUARE_HPP__
#define __SQUARE_HPP__

#include "arm7type.h"
#include "Graphic.hpp"
#include "Font.hpp"

template <class COLORMODE>
struct Square
{
	using Font = X11_clR8x8_Font<COLORMODE>;
	using Color = COLORMODE;
	using Color_t = typename Color::Color_t;
	
	u32arm_t width;
	Color_t color,ncolor;

	void draw(const Graphicx<COLORMODE> &g,const Point &point,u8arm_t ch) const
	{
		g.rectangle(color,point.x,point.y,point.x+width,point.y+width);

		const i32arm_t x=point.x+(width-Font::SIZE)/2;
		const i32arm_t y=point.y+(width-Font::SIZE)/2;

		Font::draw(g,{x,y},ncolor,ch);
	}
};


#endif

