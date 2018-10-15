#include "slider.h"

#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Number.hpp"
#include "Square.hpp"

//******************** Board *************************//

#define SQUARE 5

#define SQSTR "123456789"

#define WxH 3


#ifndef INITSEED
#define INITSEED (1)
#endif


//********************** Graphic ***********************//

#define WIDTH 50
#define IWIDTH 20

#define RGAP ((GraphicDevice::ROW-(WIDTH*3))/4)
#define CGAP RGAP

#define FCGAP  ((GraphicDevice::COL-(CGAP*2)-(WIDTH*3))/2)
#define FRGAP  ((GraphicDevice::ROW-(RGAP*2)-(WIDTH*3))/2)

static constexpr const Color BOXCOLOR  {31,0,0};
static constexpr const Color IBOXCOLOR {0,31,0};
static constexpr const Color NUMCOLOR  {0,0,31};

static constexpr const Color BGCOLOR   {0,0,0};


//********************** Game function ***********************//

static void initgame(u32arm_t* const sq, u32arm_t* seed, u32arm_t index, u32arm_t hw)
{
    initsq(sq, hw);
    randomsq(sq, index, hw, seed);
}


template <usize_t N>
static constexpr u32arm_t slen(const i8arm_t (&)[N])
{
	return N ? N-1:0;
}


template <usize_t N>
static void drawboard(Graphic &g,const Square &square,const u32arm_t (&sq)[N],const i8arm_t *sqstr,u32arm_t index)
{
    for(u8arm_t i=0,rgap=FRGAP,k=0;i<3;++i,rgap+=(RGAP+square.width))
		for(u8arm_t j=0,cgap=FCGAP;j<3;++j,cgap+=(CGAP+square.width),++k)
			if(sq[k]!=index)
                square.draw(g,{cgap,rgap},sqstr[sq[k]]-'0');
            else
                g.rectangle(BGCOLOR,cgap,rgap,cgap+square.width-1,rgap+square.width-1);
}

extern "C"
int main()
{
    static const i8arm_t sqstr[] = SQSTR;

    u32arm_t sq[WxH * WxH];
    u32arm_t seed, origseed;

    origseed = seed = INITSEED;

    constexpr const u32arm_t index = slen(sqstr) - 1;

    initgame(sq, &seed, index, WxH);

    Graphic g(GraphicDevice{});
	g.setbgcolor(BGCOLOR);

    Keypad keypad{KeypadDevice{}};


    const Square square{WIDTH,IWIDTH,BOXCOLOR,IBOXCOLOR,NUMCOLOR};

    drawboard(g,square,sq,sqstr,index);

    while(keypad)
    {
        u32arm_t kid;

        if(keypad==Keypad::KEY_UP)
        {
            kid=0;
        }
        else if(keypad==Keypad::KEY_DOWN)
        {
            kid=1;
        }
        else if(keypad==Keypad::KEY_LEFT)
        {
            kid=2;
        }
        else if(keypad==Keypad::KEY_RIGHT)
        {
            kid=3;
        }
        else if(keypad==Keypad::KEY_SELECT)
        {
            kid=4;
            seed=++origseed;
            initgame(sq, &seed, index, WxH);
        }
        else if(keypad==Keypad::KEY_START)
        {
            kid=5;
            seed=origseed;
            initgame(sq, &seed, index, WxH);
        } 
        else
        {
            continue;
        }

        if(kid==4 || kid==5 || (slide(sq, kid, index, WxH)!=-1UL)) drawboard(g,square,sq,sqstr,index);        
        
    }

	return 0;
}
