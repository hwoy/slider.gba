#ifndef __PRINT_HPP__
#define __PRINT_HPP__

#include "arm7type.hpp"
#include "Graphic.hpp"

struct Print 
{
	template <usize_t M,usize_t N>
	static void print(Graphic &g,const Point &point,const Color &color,const u8arm_t (&number)[M][N][N],const u8arm_t num)
	{
		for(u8arm_t y=0;y<N;++y)
			for(u8arm_t x=0;x<N;++x)
				if(number[num][y][x])
					g.pixel(color,x+point.x,y+point.y);
	}

    virtual inline void print(Graphic &g,const Point &point,const Color &color,const u8arm_t num) = 0;
};

#endif

