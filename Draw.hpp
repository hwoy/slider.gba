#ifndef __DRAW_HPP__
#define __DRAW_HPP__

#include "arm7type.h"
#include "Graphic.hpp"

struct Draw 
{
	template <usize_t M,usize_t N>
	static void draw(Graphic &g,const Point &point,const Color &color,const u8arm_t (&alpha)[M][N],const u8arm_t ch,const u8arm_t square)
	{
		for(u8arm_t y=0;y<square;++y)
			for(u8arm_t x=0;x<square;++x)
				if(alpha[ch][x+y*square])
					g.pixel(color,x+point.x,y+point.y);
	}
};

#endif

