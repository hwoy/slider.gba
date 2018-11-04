#include <tuple>

#include "slider.h"

#include "arm7type.h"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Font.hpp"
#include "Square.hpp"

//******************** Unit test *************************//

#ifndef SQLIST
#define SQLIST "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-0"
#endif

template <typename T,usize_t N>
static constexpr usize_t square(const T (&a)[N],usize_t n=2)
{
    return n*n>=N ? n : square(a,n+1);
}

template <typename T,usize_t N>
static constexpr usize_t slen(const T (&)[N])
{
	return N;
}

static const u8arm_t sqlist[] = SQLIST;

static constexpr const u32arm_t WxH=square(sqlist)-1;

static_assert(WxH*WxH==slen(sqlist)-1,"WxH*WxH !=slen(sqlist) => It's not square!!");


//********************** SEED ***********************//

#ifndef INITSEED
#define INITSEED (1)
#endif


//********************** Graphic ***********************//
#define FRGAP 4

#define RGAP 1
#define CGAP RGAP


#define WIDTH ((ROW-RGAP*(WxH-1)-FRGAP*2)/WxH)

#define FCGAP ((COL-CGAP*(WxH-1)-WIDTH*WxH)/2)

using Color = Color3;
using Color_t = Color::Color_t;

using Font = X11_clR8x8_Font<Color>;

static constexpr const GraphicDevice::Dispcnt_t regcontrol = 0x400 | Color::mode;
static constexpr const i32arm_t COL=Color::COL;
static constexpr const i32arm_t ROW=Color::ROW;


static constexpr const Color_t BOXCOLOR = RGB15(31,0,0);
static constexpr const Color_t NUMCOLOR  = RGB15(0,0,31);

static constexpr const Color_t COMBOXCOLOR  = RGB15(0,31,0);
static constexpr const Color_t COMNUMCOLOR  = NUMCOLOR;


static constexpr const Color_t BGCOLOR = RGB15(0,0,0);


//********************** Game function ***********************//


template <class FONT,class COLORMODE,usize_t N,usize_t M>
static void drawboard(const Graphicx<COLORMODE> &g,const Square<FONT,COLORMODE> &square,const Square<FONT,COLORMODE> &comsquare,const u32arm_t (&sq)[N],const u8arm_t (&sqlist)[M],u32arm_t index)
{
    for(usize_t i=0,rgap=FRGAP,k=0;i<WxH;++i,rgap+=(RGAP+square.width))
		for(usize_t j=0,cgap=FCGAP;j<WxH;++j,cgap+=(CGAP+square.width),++k)
			if(sq[k]!=index)
                (sq[k]==k? comsquare : square).draw(g,Point(cgap,rgap),sqlist[sq[k]]);
            else
                g.rectangle(BGCOLOR,cgap,rgap,cgap+square.width,rgap+square.width);
}

template <class FONT,class COLORMODE,usize_t N,usize_t M>
static void movesquare(const Graphicx<COLORMODE> &g,const Square<FONT,COLORMODE> &square,const Square<FONT,COLORMODE> &comsquare,const u32arm_t (&sq)[N],const u8arm_t (&sqlist)[M],u32arm_t from,u32arm_t to)
{
    const u32arm_t jfrom=from%WxH;
    const u32arm_t ifrom=from/WxH;
    const u32arm_t jto=to%WxH;
    const u32arm_t ito=to/WxH;

    const u32arm_t xto=FCGAP+jto*(square.width+CGAP);
    const u32arm_t yto=FRGAP+ito*(square.width+RGAP);

    (sq[to]==to? comsquare : square).draw(g,Point(xto,yto),sqlist[sq[to]]);


    const u32arm_t xfrom=FCGAP+jfrom*(square.width+CGAP);
    const u32arm_t yfrom=FRGAP+ifrom*(square.width+RGAP);
    
    g.rectangle(BGCOLOR,xfrom,yfrom,xfrom+square.width,yfrom+square.width);
}

template <u32arm_t N>
struct SlidingPuzzle
{
    static constexpr const u32arm_t WxH=N;
    static constexpr const u32arm_t index=WxH*WxH-1;

    static void initgame(u32arm_t* const sq, u32arm_t* seed, u32arm_t index)
    {
        initsq(sq, WxH);
        randomsq(sq, index, WxH, seed);
    }

    u32arm_t seed, origseed;

    u32arm_t sq[WxH * WxH];

    inline SlidingPuzzle(u32arm_t seed):seed(seed),origseed(seed)
    {
        initgame(sq,&seed,index);
    }

    void initgame(u32arm_t seed)
    {
        origseed = seed;
        this->seed = seed;
        initgame(sq,&this->seed,index);
    } 

    inline u32arm_t slide(u32arm_t kid)
    {
        return ::slide(sq ,kid, index, WxH);
    }

};

template <u32arm_t N,typename KeypadDevice>
static std::tuple<u32arm_t,u32arm_t,u32arm_t,u32arm_t> keypadaction(const SlidingPuzzle<N> &game,Keypad<KeypadDevice> &keypad)
{
    u32arm_t indexfrom=0,indexto=0;
    auto newseed = game.seed;

    auto keydownfunc = [&indexfrom,&indexto,&newseed,origseed=game.origseed,sq=game.sq,index=game.index,WxH=game.WxH]
    (const typename Keypad<KeypadDevice>::Key &key) mutable ->i32arm_t 
    {
        u32arm_t kid=-1U;
        struct point p;

        getxy(indexto = getindex(sq, index, WxH), &p, WxH);

        switch(key.key)
        {
            case key.KEY_UP:
                if (p.y > 0)
                {
                    indexfrom=indexto - WxH;

                    kid=cmd_up;
                }
                break;

            case key.KEY_DOWN:
                if(p.y < WxH - 1)
                {
                    indexfrom=indexto + WxH;
                                    
                    kid=cmd_down;
                }
                break;

            case key.KEY_LEFT:
                if(p.x > 0)
                {
                    indexfrom=indexto - 1;

                    kid=cmd_left;
                }
                break;

            case key.KEY_RIGHT:
                if(p.x < WxH - 1)
                {
                    indexfrom=indexto + 1;

                    kid=cmd_right;
                }
                break;

            case key.KEY_A:
                kid=cmd_right+1;
                newseed=origseed-1;
                break;

            case key.KEY_B:
                kid=cmd_right+2;
                newseed=origseed+1;
                break;

            case key.KEY_START:
                kid=cmd_right+3;
                newseed=origseed;
                break;

            case key.KEY_SELECT:
                kid=cmd_right+4;
                break; 

            default: break;       

            }

            return kid;

        };


        auto keyupfunc = [](const typename Keypad<KeypadDevice>::Key &key) -> i32arm_t
        {
            return (key == key.KEY_SELECT)? cmd_right+5:-1U;
        };


        auto keyfunc = [](const typename Keypad<KeypadDevice>::Key &) -> i32arm_t
        {
            return -1U;
        };

        return std::tuple<u32arm_t,u32arm_t,u32arm_t,u32arm_t>(keypad.dispatch(keydownfunc,keyfunc,keyupfunc,keyfunc),indexfrom,indexto,newseed);

}

extern "C"
int main()
{
    static constexpr const Square<Font,Color> square{WIDTH,BOXCOLOR,NUMCOLOR};
    static constexpr const Square<Font,Color> comsquare{WIDTH,COMBOXCOLOR,COMNUMCOLOR}; 

    SlidingPuzzle<WxH> game(INITSEED);
    const auto &sq       = game.sq;
    constexpr const auto index = game.index;

    GraphicDevice::refdispcnt()=regcontrol;

    constexpr const Graphicx<Color> g;
    g.bgcolor(BGCOLOR);
    drawboard(g,square,comsquare,sq,sqlist,index);

    Keypad<KeypadDevice> keypad;

    while(true)
    {
        const auto tp        = keypadaction(game,keypad);
        const auto kid       = std::get<0>(tp);
        const auto indexfrom = std::get<1>(tp);
        const auto indexto   = std::get<2>(tp);
        const auto newseed   = std::get<3>(tp);

        g.waitVSync();
    
        switch(kid)
        {
            case cmd_up:
            case cmd_down:
            case cmd_left:
            case cmd_right:
                    if(game.slide(kid)!=-1UL)
                        movesquare(g,square,comsquare,sq,sqlist,indexfrom,indexto);
                    break;

            case cmd_right+1:
            case cmd_right+2:
            case cmd_right+3:
                    game.initgame(newseed);
                    drawboard(g,square,comsquare,sq,sqlist,index);
                    break;

            case cmd_right+4:
                    {
                        u32arm_t sq[WxH * WxH];
                        initsq(sq, WxH);
                        drawboard(g,square,comsquare,sq,sqlist,index);

                    }
                    break;

            case cmd_right+5:
                    drawboard(g,square,comsquare,sq,sqlist,index);
                    break;

            default: break;

        }

    }

	return 0;
}
