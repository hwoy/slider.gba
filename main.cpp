#include "slider.h"

#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Font.hpp"
#include "Square.hpp"

//******************** Unit test *************************//

#define SQLIST {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,0}

template <typename T,usize_t N>
static constexpr usize_t square(const T (&a)[N],usize_t n=2)
{
    return n*n>=N ? n : square(a,n+1);
}

template <typename T,usize_t N>
static constexpr u32arm_t slen(const T (&)[N])
{
	return N ? N:0;
}

static const u8arm_t sqlist[] = SQLIST;

static constexpr const u8arm_t WxH=square(sqlist);

static_assert(WxH*WxH==slen(sqlist),"WxH*WxH !=slen(sqlist) => It's not square!!");


//********************** SEED ***********************//

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
static void drawboard(Graphic &g,const Square &square,const u32arm_t (&sq)[N],const u8arm_t *sqlist,u32arm_t index)
{

    for(u8arm_t i=0,rgap=FRGAP,k=0;i<WxH;++i,rgap+=(RGAP+square.width))
		for(u8arm_t j=0,cgap=FCGAP;j<WxH;++j,cgap+=(CGAP+square.width),++k)
			if(sq[k]!=index)
                square.draw(g,{cgap,rgap},sqlist[sq[k]]);
            else
                g.rectangle(BGCOLOR,cgap,rgap,cgap+square.width-1,rgap+square.width-1);
}


static void movesquare(Graphic &g,const Square &square,u8arm_t from,u8arm_t to,u8arm_t num)
{
    u8arm_t jfrom=from%WxH;
    u8arm_t ifrom=from/WxH;
    u8arm_t jto=to%WxH;
    u8arm_t ito=to/WxH;

    u8arm_t xto=FCGAP+jto*(square.width+CGAP);
    u8arm_t yto=FRGAP+ito*(square.width+RGAP);; 

    square.draw(g,{xto,yto},num);

    u8arm_t xfrom=FCGAP+jfrom*(square.width+CGAP);
    u8arm_t yfrom=FRGAP+ifrom*(square.width+RGAP);
    
    g.rectangle(BGCOLOR,xfrom,yfrom,xfrom+square.width-1,yfrom+square.width-1);


}

extern "C"
int main()
{

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

    for(;keypad.untilkeypressDown();keypad.untilkeypressUp())
    {
        u32arm_t kid=-1U;

        struct point p;
        u32arm_t _index;
        u8arm_t indexfrom,indexto;
        u8arm_t num;

        getxy(_index = getindex(sq, index, WxH), &p, WxH);

        if(keypad==Keypad::KEY_UP)
        {
            if (p.y > 0)
            {
                indexfrom=_index - WxH;
                indexto=_index;

                kid=0;
            }

        }
        else if(keypad==Keypad::KEY_DOWN)
        {
            if(p.y < WxH - 1)
            {
                indexfrom=_index + WxH;
                indexto=_index;
                
                kid=1;
            }
        }
        else if(keypad==Keypad::KEY_LEFT)
        {
            if(p.x > 0)
            {
                indexfrom=_index - 1;
                indexto=_index;

                kid=2;
            }
        }
        else if(keypad==Keypad::KEY_RIGHT)
        {
            if(p.x < WxH - 1)
            {
                indexfrom=_index + 1;
                indexto=_index;

                kid=3;
            }
        }
        else if(keypad==Keypad::KEY_A)
        {
            kid=4;
            seed=--origseed;
        }
        else if(keypad==Keypad::KEY_B)
        {
            kid=5;
            seed=++origseed;
        }
        else if(keypad==Keypad::KEY_START)
        {
            kid=6;
            seed=origseed;
        } 

        switch(kid)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                    num=sqlist[sq[indexfrom]];
                    if(slide(sq, kid, index, WxH)!=-1UL)
                        movesquare(g,square,indexfrom,indexto,num);
                    break;
            case 4:
            case 5:
            case 6:
                    initgame(sq, &seed, index, WxH);
                    drawboard(g,square,sq,sqlist,index);
                    break;

            default: break;

        }
        
    }

	return 0;
}
