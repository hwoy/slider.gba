#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__


#include "arm7type.h"

#define RGB15(r,g,b)  ((r)+(g<<5)+(b<<10))

#define DISPCNT ((volatile u32arm_t *)0x4000000)
#define VRAM ((volatile u16arm_t *)0x6000000)


struct Point
{
	u8arm_t x,y;

	inline constexpr Point(u8arm_t x,u8arm_t y):x(x),y(y){}

	inline constexpr bool operator != (const Point & p) const
	{
		return x != p.x || y != p.y;
	}

	inline constexpr bool operator == (const Point & p) const
	{
		return x == p.x && y == p.y;
	}

};


struct GraphicDevice
{
	static inline void setreg(u32arm_t bgmode)
	{
		*DISPCNT=bgmode;
	}

	static inline u32arm_t getreg(void)
	{
		return *DISPCNT;
	}
};

template <typename VRAMTYPE,u8arm_t _COL,u8arm_t _ROW>
struct BGMODE
{
	using Vram_t = VRAMTYPE;

	static constexpr const u8arm_t COL=_COL;
	static constexpr const u8arm_t ROW=_ROW;

	static inline Vram_t read(Vram_t index)
	{
		return reinterpret_cast<volatile Vram_t *>(VRAM)[index];
	}

	static inline void write(Vram_t index,Vram_t value)
	{
		reinterpret_cast<volatile Vram_t *>(VRAM)[index]=value;
	}

	static inline Vram_t read(u8arm_t x,u8arm_t y)
	{
		return read(x+y*COL);
	}

	static inline void write(u8arm_t x,u8arm_t y,Vram_t value)
	{
		write(x+y*COL,value);
	}

	static inline constexpr volatile Vram_t &refvid(u8arm_t x,u8arm_t y)
	{
		return reinterpret_cast<volatile Vram_t *>(VRAM)[x+y*COL];
	}

	static inline constexpr volatile Vram_t *ptrvid(u8arm_t x,u8arm_t y)
	{
		return reinterpret_cast<volatile Vram_t *>(VRAM)+x+y*COL;
	}
};

using BGMODE3 = BGMODE<u16arm_t,240,160>;
using BGMODE4 = BGMODE<u8arm_t,240,160>;

template <class BGCOLORMODE>
struct Grange
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Vram_t = typename BGCOLORMODE::Vram_t;
	using Color_t = Vram_t;

	struct Iterator
	{
		const u8arm_t x1,x2;
		Point p;

		explicit inline constexpr Iterator(u8arm_t x1,u8arm_t x2,const Point &p):x1(x1),x2(x2),p(p){}

		inline constexpr const volatile Vram_t &operator * () const
		{
			return bgmode::refvid(p.x,p.y);
		}

		inline volatile Vram_t &operator * ()
		{
			return bgmode::refvid(p.x,p.y);
		}

		volatile Vram_t *operator ++ ()
		{
			if(p.x+1>x2)
			{
				p.x=x1;
				++p.y;

			}
			
			else
			{
				++p.x;
			}
			

			return bgmode::ptrvid(p.x,p.y);
			
		}

		volatile Vram_t *operator -- ()
		{
			if(p.x-1<x1)
			{
				p.x=x2;
				--p.y;

			}
			
			else
			{
				--p.x;
			}
			

			return bgmode::ptrvid(p.x,p.y);
			
		}

		inline constexpr bool operator != (const Iterator & it) const
		{
			return p != it.p;
		}

	}itbegin,itend;


	inline constexpr Grange(const Point &p1,const Point &p2):
	itbegin(p1.x,p2.x,p1),itend(p1.x,p2.x,Point(p1.x,p2.y+1)) {}


	inline Iterator begin()
	{
		return itbegin;
	}

	inline Iterator end()
	{
		return itend;
	}

	inline const Iterator begin() const
	{
		return itbegin;
	}

	inline const Iterator end() const
	{
		return itend;
	}

};

struct Color3
{
	using bgmode = BGMODE3;
	using Vram_t = bgmode::Vram_t;
	using Color_t = Vram_t;

	static constexpr const u32arm_t mode = 0x03;
	static constexpr const u8arm_t COL=bgmode::COL;
	static constexpr const u8arm_t ROW=bgmode::ROW;

};

struct Color4
{
	using bgmode = BGMODE4;
	using Vram_t = bgmode::Vram_t;
	using Color_t = Vram_t;

	static constexpr const u32arm_t mode = 0x04;
	static constexpr const u8arm_t COL=bgmode::COL;
	static constexpr const u8arm_t ROW=bgmode::ROW;

};

template <class BGCOLORMODE>
struct Graphic: public BGCOLORMODE
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Vram_t = typename BGCOLORMODE::Vram_t;
	using Color_t = Vram_t;

	static constexpr const u8arm_t COL=bgmode::COL;
	static constexpr const u8arm_t ROW=bgmode::ROW;



	static inline void pixel(Color_t color,u8arm_t x,u8arm_t y)
	{
		bgmode::write(x,y,color);
	}

	static inline Color_t pixel(u8arm_t x,u8arm_t y)
	{
		return bgmode::read(x,y);
	}

	static void rectangle(Color_t color,u8arm_t x1,u8arm_t y1,u8arm_t x2,u8arm_t y2)
	{
		for(auto &rpoint:Grange<BGCOLORMODE>({x1,y1},{x2,y2}))
			rpoint=color;

	}

	static void setbgcolor(Color_t color)
	{
		rectangle(color,0,0,COL-1,ROW-1);
	}

	static void drawbuffer(const Color_t *buffer,u8arm_t x=0,u8arm_t  y=0,u8arm_t w=COL,u8arm_t h=ROW)
	{
		for(auto &rpoint:Grange<BGCOLORMODE>({x,y},Point(x+w-1,y+h-1)))
			rpoint=*buffer++;
	}
};


#endif
