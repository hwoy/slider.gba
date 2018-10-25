#ifndef __DRAW_HPP__
#define __DRAW_HPP__

#include "arm7type.h"
#include "Graphic.hpp"

template <class COLORMODE>
struct Draw 
{
	using Color_t = typename COLORMODE::Color_t;

	template <usize_t M,usize_t N,usize_t P>
	static void draw(const Graphicx<COLORMODE> &g,const Point &point,const Color_t &color,const u8arm_t (&alpha)[M][N][P],const u8arm_t ch)
	{
		for(u8arm_t y=0;y<N;++y)
			for(u8arm_t x=0;x<P;++x)
				if(alpha[ch][y][x])
					g.pixel(color,x+point.x,y+point.y);
	}
};

#endif

