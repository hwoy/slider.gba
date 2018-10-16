#include "slider.h"

#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Number.hpp"
#include "Square.hpp"

//******************** Board *************************//

#define SQLIST {1,2,3,4,5,6,7,8,9}

#define WxH 3


#ifndef INITSEED
#define INITSEED (1)
#endif


//********************** Graphic ***********************//
#define FRGAP 4

#define RGAP 1
#define CGAP RGAP


#define WIDTH ((GraphicDevice::ROW-RGAP*(WxH-1)-FRGAP*2)/WxH)
#define IWIDTH (WIDTH*2/5)

#define FCGAP ((GraphicDevice::COL-CGAP*(WxH-1)-WIDTH*WxH)/2)


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
	return N ? N:0;
}



template <usize_t N>
static void drawboard(Graphic &g,const Square &square,const u32arm_t (&sq)[N],const i8arm_t *sqlist,u32arm_t index)
{
    static_assert(WxH*WxH==N,"WxH*WxH !=N => It's not square!!");

    for(u8arm_t i=0,rgap=FRGAP,k=0;i<WxH;++i,rgap+=(RGAP+square.width))
		for(u8arm_t j=0,cgap=FCGAP;j<WxH;++j,cgap+=(CGAP+square.width),++k)
			if(sq[k]!=index)
                square.draw(g,{cgap,rgap},sqlist[sq[k]]);
            else
                g.rectangle(BGCOLOR,cgap,rgap,cgap+square.width-1,rgap+square.width-1);
}

extern "C"
int main()
{
    static const i8arm_t sqlist[] = SQLIST;

    static_assert(WxH*WxH==slen(sqlist),"WxH*WxH !=slen(sqlist) => It's not square!!");

    u32arm_t sq[WxH * WxH];
    u32arm_t seed, origseed;

    origseed = seed = INITSEED;

    constexpr const u32arm_t index = slen(sqlist)-1;

    initgame(sq, &seed, index, WxH);

    Graphic g(GraphicDevice{});
	g.setbgcolor(BGCOLOR);

    Keypad keypad{KeypadDevice{}};


    const Square square{WIDTH,IWIDTH,BOXCOLOR,IBOXCOLOR,NUMCOLOR};

    drawboard(g,square,sq,sqlist,index);

    for(u32arm_t kid=0;keypad.untilkeypressDown();keypad.untilkeypressUp())
    {

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

        if(kid==4 || kid==5 || (slide(sq, kid, index, WxH)!=-1UL)) drawboard(g,square,sq,sqlist,index);

        
    }

	return 0;
}
