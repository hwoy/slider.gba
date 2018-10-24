#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__


#include "arm7type.h"

#define RGB15(r,g,b)  ((r)+(g<<5)+(b<<10))

#define IOMEM ((volatile u32arm_t *)0x4000000)
#define VIDMEM ((volatile u16arm_t *)0x6000000)


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
	static constexpr const u8arm_t COL=240;
	static constexpr const u8arm_t ROW=160;

	static inline void setmode(u32arm_t mode)
	{
		*IOMEM=mode;
	}

	static inline u32arm_t getmode(void)
	{
		return *IOMEM;
	}
};

template <typename VIDMEMTYPE>
struct BGMODE
{
	using Vidmem_t = VIDMEMTYPE;

	static inline Vidmem_t read(Vidmem_t index)
	{
		return reinterpret_cast<volatile Vidmem_t *>(VIDMEM)[index];
	}

	static inline void write(Vidmem_t index,Vidmem_t value)
	{
		reinterpret_cast<volatile Vidmem_t *>(VIDMEM)[index]=value;
	}

	static inline Vidmem_t read(u8arm_t x,u8arm_t y)
	{
		return read(x+y*GraphicDevice::COL);
	}

	static inline void write(u8arm_t x,u8arm_t y,Vidmem_t value)
	{
		write(x+y*GraphicDevice::COL,value);
	}

	static inline constexpr volatile Vidmem_t &refvid(u8arm_t x,u8arm_t y)
	{
		return reinterpret_cast<volatile Vidmem_t *>(VIDMEM)[x+y*GraphicDevice::COL];
	}

	static inline constexpr volatile Vidmem_t *ptrvid(u8arm_t x,u8arm_t y)
	{
		return reinterpret_cast<volatile Vidmem_t *>(VIDMEM)+x+y*GraphicDevice::COL;
	}
};

using BG3 = BGMODE<u16arm_t>;
using BG4 = BGMODE<u8arm_t>;

template <class BGCOLORMODE>
struct Grange
{
	using bgmode = typename BGCOLORMODE::bgmod;
	using Vidmem_t = typename BGCOLORMODE::Vidmem_t;
	using Color_t = Vidmem_t;

	struct Iterator
	{
		const u8arm_t x1,x2;
		Point p;

		explicit inline constexpr Iterator(u8arm_t x1,u8arm_t x2,const Point &p):x1(x1),x2(x2),p(p){}

		inline constexpr const volatile Vidmem_t &operator * () const
		{
			return bgmode::refvid(p.x,p.y);
		}

		inline volatile Vidmem_t &operator * ()
		{
			return bgmode::refvid(p.x,p.y);
		}

		volatile Vidmem_t *operator ++ ()
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

		volatile Vidmem_t *operator -- ()
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
	using bgmod = BG3;
	using Vidmem_t = bgmod::Vidmem_t;
	using Color_t = Vidmem_t;

};

struct Color4
{
	using bgmod = BG4;
	using Vidmem_t = bgmod::Vidmem_t;
	using Color_t = Vidmem_t;

};

template <class BGCOLORMODE>
struct Graphic: public BGCOLORMODE
{
	using bgmode = typename BGCOLORMODE::bgmod;
	using Vidmem_t = typename BGCOLORMODE::Vidmem_t;
	using Color_t = Vidmem_t;



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
		rectangle(color,0,0,GraphicDevice::COL-1,GraphicDevice::ROW-1);
	}

	static void drawbuffer(const Color_t *buffer,u8arm_t x=0,u8arm_t  y=0,u8arm_t w=GraphicDevice::COL,u8arm_t h=GraphicDevice::ROW)
	{
		for(auto &rpoint:Grange<BGCOLORMODE>({x,y},Point(x+w-1,y+h-1)))
			rpoint=*buffer++;
	}
};


#endif
