#ifndef __SQUARE_HPP__
#define __SQUARE_HPP__

#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Number.hpp"


struct Square
{
	u8arm_t width,iwidth;
	Color color,icolor,ncolor;

	void draw(Graphic &g,const Point &point,u8arm_t num) const
	{
		g.rectangle(color,point.x,point.y,point.x+width-1,point.y+width-1);

		u8arm_t ix=point.x+(width-iwidth)/2;
		u8arm_t iy=point.y+(width-iwidth)/2;

		g.rectangle(icolor,ix,iy,ix+iwidth-1,iy+iwidth-1);

		u8arm_t nx=ix+(iwidth-Number::SIZE)/2;
		u8arm_t ny=iy+(iwidth-Number::SIZE)/2;

		Number::draw(g,{nx,ny},ncolor,num);
	}
};


#endif

