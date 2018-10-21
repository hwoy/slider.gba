#include "slider.h"

#include "arm7type.h"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Font.hpp"
#include "Square.hpp"

//******************** Unit test *************************//

#define SQLIST "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-0"

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

static constexpr const u8arm_t WxH=square(sqlist)-1;

static_assert(WxH*WxH==slen(sqlist)-1,"WxH*WxH !=slen(sqlist) => It's not square!!");


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
static constexpr const Color NUMCOLOR  {0,0,31};

static constexpr const Color COMBOXCOLOR  = {0,31,0};
static constexpr const Color COMNUMCOLOR  = NUMCOLOR;


static constexpr const Color BGCOLOR   {0,0,0};


//********************** Game function ***********************//

static void initgame(u32arm_t* const sq, u32arm_t* seed, u32arm_t index, u32arm_t hw)
{
    initsq(sq, hw);
    randomsq(sq, index, hw, seed);
}


template <usize_t N,usize_t M>
static void drawboard(Graphic &g,const Square &square,const Square &comsquare,const u32arm_t (&sq)[N],const u8arm_t (&sqlist)[M],u32arm_t index)
{

    for(u8arm_t i=0,rgap=FRGAP,k=0;i<WxH;++i,rgap+=(RGAP+square.width))
		for(u8arm_t j=0,cgap=FCGAP;j<WxH;++j,cgap+=(CGAP+square.width),++k)
			if(sq[k]!=index)
                (sq[k]==k? comsquare : square).draw(g,{cgap,rgap},sqlist[sq[k]]);
            else
                g.rectangle(BGCOLOR,cgap,rgap,cgap+square.width-1,rgap+square.width-1);
}

template <usize_t N,usize_t M>
static void movesquare(Graphic &g,const Square &square,const Square &comsquare,const u32arm_t (&sq)[N],const u8arm_t (&sqlist)[M],u8arm_t from,u8arm_t to)
{

    const u8arm_t jfrom=from%WxH;
    const u8arm_t ifrom=from/WxH;
    const u8arm_t jto=to%WxH;
    const u8arm_t ito=to/WxH;

    const u8arm_t xto=FCGAP+jto*(square.width+CGAP);
    const u8arm_t yto=FRGAP+ito*(square.width+RGAP);

    (sq[to]==to? comsquare : square).draw(g,{xto,yto},sqlist[sq[to]]);


    const u8arm_t xfrom=FCGAP+jfrom*(square.width+CGAP);
    const u8arm_t yfrom=FRGAP+ifrom*(square.width+RGAP);
    
    g.rectangle(BGCOLOR,xfrom,yfrom,xfrom+square.width-1,yfrom+square.width-1);


}

extern "C"
int main()
{
    static constexpr const Square square{WIDTH,BOXCOLOR,NUMCOLOR};
    static constexpr const Square comsquare{WIDTH,COMBOXCOLOR,COMNUMCOLOR}; 

    u32arm_t sq[WxH * WxH];
    
    u32arm_t seed, origseed;
    origseed = seed = INITSEED;

    constexpr const u32arm_t index = slen(sqlist)-1-1;

    initgame(sq, &seed, index, WxH);

    Graphic g(GraphicDevice{});
	g.setbgcolor(BGCOLOR);

    drawboard(g,square,comsquare,sq,sqlist,index);

    for(Keypad keypad{KeypadDevice{}};keypad.untilkeypressDown();keypad.untilkeypressUp())
    {
        u32arm_t kid=-1U;

        struct point p;

        u8arm_t indexfrom,indexto;

        getxy(indexto = getindex(sq, index, WxH), &p, WxH);

        if(keypad==Keypad::KEY_UP)
        {
            if (p.y > 0)
            {
                indexfrom=indexto - WxH;

                kid=cmd_up;
            }

        }
        else if(keypad==Keypad::KEY_DOWN)
        {
            if(p.y < WxH - 1)
            {
                indexfrom=indexto + WxH;
                
                kid=cmd_down;
            }
        }
        else if(keypad==Keypad::KEY_LEFT)
        {
            if(p.x > 0)
            {
                indexfrom=indexto - 1;

                kid=cmd_left;
            }
        }
        else if(keypad==Keypad::KEY_RIGHT)
        {
            if(p.x < WxH - 1)
            {
                indexfrom=indexto + 1;

                kid=cmd_right;
            }
        }
        else if(keypad==Keypad::KEY_A)
        {
            kid=cmd_right+1;
            seed=--origseed;
        }
        else if(keypad==Keypad::KEY_B)
        {
            kid=cmd_right+2;
            seed=++origseed;
        }
        else if(keypad==Keypad::KEY_START)
        {
            kid=cmd_right+3;
            seed=origseed;
        } 

        switch(kid)
        {
            case cmd_up:
            case cmd_down:
            case cmd_left:
            case cmd_right:
                    if(slide(sq, kid, index, WxH)!=-1UL)
                        movesquare(g,square,comsquare,sq,sqlist,indexfrom,indexto);
                    break;
                    
            case cmd_right+1:
            case cmd_right+2:
            case cmd_right+3:
                    initgame(sq, &seed, index, WxH);
                    drawboard(g,square,comsquare,sq,sqlist,index);
                    break;

            default: break;

        }
        
    }

	return 0;
}
