#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__


#include "arm7type.h"

#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))

#define IOMEM ((volatile u32arm_t *)0x4000000)
#define VIDMEM ((volatile u16arm_t *)0x6000000)


struct Point
{
	u8arm_t x,y;

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

	explicit inline GraphicDevice(u32arm_t mode=0x403)
	{
		*IOMEM=mode;
	}

	static inline u16arm_t read(u16arm_t index)
	{
		return VIDMEM[index];
	}

	static inline void write(u16arm_t index,u16arm_t value)
	{
		VIDMEM[index]=value;
	}

	static inline u16arm_t read(u8arm_t x,u8arm_t y)
	{
		return read(x+y*COL);
	}

	static inline void write(u8arm_t x,u8arm_t y,u16arm_t value)
	{
		write(x+y*COL,value);
	}

	static inline constexpr volatile u16arm_t &refvid(u8arm_t x,u8arm_t y)
	{
		return VIDMEM[x+y*COL];
	}

	static inline constexpr volatile u16arm_t *ptrvid(u8arm_t x,u8arm_t y)
	{
		return VIDMEM+x+y*COL;
	}
};

template <class GD>
struct Grange
{

	struct Iterator
	{
		const u8arm_t x1,x2;
		Point p;

		explicit inline constexpr Iterator(u8arm_t x1,u8arm_t x2,const Point &p):x1(x1),x2(x2),p(p){}

		inline constexpr const volatile u16arm_t &operator * () const
		{
			return GD::refvid(p.x,p.y);
		}

		inline volatile u16arm_t &operator * ()
		{
			return GD::refvid(p.x,p.y);
		}

		volatile u16arm_t *operator ++ ()
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
			

			return GD::ptrvid(p.x,p.y);
			
		}

		volatile u16arm_t *operator -- ()
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
			

			return GD::ptrvid(p.x,p.y);
			
		}

		inline constexpr bool operator != (const Iterator & it) const
		{
			return p != it.p;
		}

	}itbegin,itend;


	inline constexpr Grange(const Point &p1,const Point &p2):
	itbegin(p1.x,p2.x,p1),itend(p1.x,p2.x,{static_cast<u8arm_t>(p1.x),static_cast<u8arm_t>(p2.y+1)}) {}


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


struct Graphic_Type
{

	union Color
	{
		
		u16arm_t color;
		struct Rgb
		{
				u16arm_t r:5;
				u16arm_t g:5;
				u16arm_t b:5;
				u16arm_t unused:1;
		}__rgb__;

		inline static constexpr u16arm_t rgb(u16arm_t r,u16arm_t g,u16arm_t b) { return RGB16(r,g,b); }

		inline constexpr Color(u16arm_t color):color(color) {}
		
		inline constexpr Color(u16arm_t r,u16arm_t g,u16arm_t b):__rgb__{r,g,b,0} {}

		inline constexpr u16arm_t get(void) const
		{
			return color;
		}

		inline Rgb getrgb(void) const
		{
			return __rgb__;
		}
	};
};

template <class GD>
struct Graphic: public Graphic_Type
{
	using Color = typename Graphic_Type::Color;


	static inline void pixel(u16arm_t color,u8arm_t x,u8arm_t y)
	{
		GD::write(x,y,color);
	}

	static inline void pixel(const Color &color,u8arm_t x,u8arm_t y)
	{
		pixel(color.get(),x,y);
	}

	static inline Color::Rgb pixel(u8arm_t x,u8arm_t y)
	{
		return Color(GD::read(x,y)).getrgb();
	}

	static void rectangle(u16arm_t color,u8arm_t x1,u8arm_t y1,u8arm_t x2,u8arm_t y2)
	{
		for(auto &rpoint:Grange<GD>({x1,y1},{x2,y2}))
			rpoint=color;

	}

	static void rectangle(const Color &color,u8arm_t x1,u8arm_t y1,u8arm_t x2,u8arm_t y2)
	{
		rectangle(color.get(),x1,y1,x2,y2);
	}

	static void setbgcolor(u16arm_t color)
	{
		rectangle(color,0,0,GD::COL-1,GD::ROW-1);
	}

	static void setbgcolor(const Color &color)
	{
		setbgcolor(color.get());
	}

	static void drawbuffer(const u16arm_t *buffer,u16arm_t x=0,u16arm_t  y=0,u16arm_t w=GD::COL,u16arm_t h=GD::ROW)
	{
		for(u16arm_t i=y;i<y+h;++i)
			for(u16arm_t j=x;j<x+w;++j)
				pixel(buffer[i*w+j],j,i);

	}
};

using Color = Graphic_Type::Color;


#endif
