#ifndef __FONT_HPP__
#define __FONT_HPP__

#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Draw.hpp"


struct Font
{
	static constexpr const u8arm_t SIZE=7;
	static constexpr const u8arm_t NUM=36;

	static const u8arm_t FONT[NUM][SIZE][SIZE];

	static inline void draw(Graphic &g,const Point &point,const Color &color,const u8arm_t id)
	{
		Draw::draw(g,point,color,FONT,id);
	}
};

const u8arm_t Font::FONT[Font::NUM][Font::SIZE][Font::SIZE] =
{

		{ // 0
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,0,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,0,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // 1
		{0,0,1,1,0,0,0} , \
		{0,1,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,1,1,1,1,1,0} ,
		} , \

		{ // 2
		{0,1,1,1,1,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} ,
		}, \


		{ // 3
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,0,1,1,1,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // 4
		{0,1,0,0,1,0,0} , \
		{0,1,0,0,1,0,0} , \
		{0,1,0,0,1,0,0} , \
		{0,1,1,1,1,1,0} , \
		{0,0,0,0,1,0,0} , \
		{0,0,0,0,1,0,0} , \
		{0,0,0,1,1,1,0} ,
		} , \

		{ // 5
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // 6
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // 7
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,0,0,0,1,0,0} , \
		{0,0,0,0,1,0,0} , \
		{0,0,0,0,1,0,0} , \
		{0,0,0,1,1,1,0} ,
		}, \

		{ // 8
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // 9
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,0,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // A
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // B
		{0,1,1,1,1,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,0,0} ,
		}, \

		{ // C
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // D
		{0,1,1,1,1,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,0,0} ,
		}, \

		{ // E
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // F
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} ,
		}, \

		{ // G
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,1,1,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,1,1,0,0} ,
		}, \

		{ // H
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // I
		{0,1,1,1,1,1,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // J
		{0,0,1,1,1,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,1,0,1,0,0,0} , \
		{0,1,1,1,0,0,0} ,
		}, \

		{ // K
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,1,0,0} , \
		{0,1,0,1,0,0,0} , \
		{0,1,1,0,0,0,0} , \
		{0,1,0,1,0,0,0} , \
		{0,1,0,0,1,0,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // L
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,1,1,1,1,0} ,
		}, \

		{ // M
		{0,1,1,1,1,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // N
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,0,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,0,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // O
		{0,0,1,1,1,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,1,1,0,0} ,
		}, \

		{ // P
		{0,0,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} , \
		{0,1,0,0,0,0,0} ,
		}, \

		{ // Q
		{0,0,1,1,1,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,1,1,0,1} ,
		}, \

		{ // R
		{0,0,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,0,0} , \
		{0,1,0,1,0,0,0} , \
		{0,1,0,0,1,0,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // S
		{0,0,1,1,1,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,0,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,0,1,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,1,1,0,0} ,
		}, \

		{ // T
		{0,1,1,1,1,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} ,
		}, \

		{ // U
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,1,1,0,0} ,
		}, \

		{ // V
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,0,1,0,0} , \
		{0,0,0,1,0,0,0} ,
		}, \

		{ // W
		{0,1,0,0,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,1,0,1,0,1,0} , \
		{0,0,1,0,1,0,0} ,
		}, \


		{ // X
		{0,1,0,0,0,1,0} , \
		{0,0,1,0,1,0,0} , \
		{0,0,1,0,1,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,1,0,1,0,0} , \
		{0,0,1,0,1,0,0} , \
		{0,1,0,0,0,1,0} ,
		}, \

		{ // Y
		{0,1,0,0,0,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,1,0,1,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,0,1,0,0,0} ,
		}, \

		{ // Z
		{0,1,1,1,1,1,0} , \
		{0,1,0,0,0,1,0} , \
		{0,0,0,0,1,0,0} , \
		{0,0,0,1,0,0,0} , \
		{0,0,1,0,0,0,0} , \
		{0,1,0,0,0,1,0} , \
		{0,1,1,1,1,1,0} ,
		}

};

#endif

