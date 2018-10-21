#ifndef __DRAW_HPP__
#define __DRAW_HPP__

#include "arm7type.h"
#include "Graphic.hpp"

struct Draw 
{
	template <usize_t M,usize_t N,usize_t P>
	static void draw(Graphic &g,const Point &point,const Color &color,const u8arm_t (&alpha)[M][N][P],const u8arm_t ch)
	{
		for(u8arm_t y=0;y<N;++y)
			for(u8arm_t x=0;x<P;++x)
				if(alpha[ch][y][x])
					g.pixel(color,x+point.x,y+point.y);
	}
};

#endif

