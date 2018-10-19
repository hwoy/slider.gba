#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__


#include "arm7type.h"

#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))

#define IOMEM ((volatile u32arm_t *)0x4000000)
#define VIDMEM ((volatile u16arm_t *)0x6000000)


struct Point
{
	u8arm_t x,y;
};


struct GraphicDevice
{
	static constexpr const u8arm_t COL=240;
	static constexpr const u8arm_t ROW=160;

	static volatile u32arm_t * const iomem;
	static volatile u16arm_t * const vidmem;

	static inline void setmode(u32arm_t mode)
	{
		*iomem=mode;
	}

	static inline u32arm_t getmode(void)
	{
		return *iomem;
	}

	static inline volatile u16arm_t *getvidmem(void)
	{
		return vidmem;
	}


	explicit inline GraphicDevice(u32arm_t mode=0x403)
	{
		*iomem=mode;
	}

	static inline u16arm_t read(u16arm_t index)
	{
		return vidmem[index];
	}

	static inline void write(u16arm_t index,u16arm_t value)
	{
		vidmem[index]=value;
	}

	static inline u16arm_t read(u8arm_t x,u8arm_t y)
	{
		return read(x+y*COL);
	}

	static inline void write(u8arm_t x,u8arm_t y,u16arm_t value)
	{
		write(x+y*COL,value);
	}
};

volatile u32arm_t * const GraphicDevice::iomem=IOMEM;
volatile u16arm_t * const GraphicDevice::vidmem=VIDMEM;

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
		}__rgb__;
		
		inline constexpr Color(u16arm_t r,u16arm_t g,u16arm_t b):__rgb__{r,g,b} {}

		inline constexpr Color(u16arm_t color):color(color) {}

		inline constexpr u16arm_t get(void) const
		{
			return color;
		}

		inline Rgb getrgb(void) const
		{
			return __rgb__;
		}

		static constexpr u16arm_t rgb(u16arm_t r,u16arm_t g,u16arm_t b) { return RGB16(r,g,b); }
	};
};

struct Graphic: public Graphic_Type
{
	using Color = typename Graphic_Type::Color;

	GraphicDevice gd;

	explicit inline constexpr Graphic(const GraphicDevice &gd):gd(gd) {}


	inline void pixel(u16arm_t color,u8arm_t x,u8arm_t y)
	{
		gd.write(x,y,color);
	}

	inline void pixel(const Color &color,u8arm_t x,u8arm_t y)
	{
		pixel(color.get(),x,y);
	}

	inline Color::Rgb pixel(u8arm_t x,u8arm_t y)
	{
		return Color(gd.read(x,y)).getrgb();
	}

	void rectangle(u16arm_t color,u8arm_t x1,u8arm_t y1,u8arm_t x2,u8arm_t y2)
	{
		for(u8arm_t y=y1;y<=y2;++y)
			for(u8arm_t x=x1;x<=x2;++x)
				gd.write(x,y,color);
	}

	void rectangle(const Color &color,u8arm_t x1,u8arm_t y1,u8arm_t x2,u8arm_t y2)
	{
		rectangle(color.get(),x1,y1,x2,y2);
	}

	void setbgcolor(u16arm_t color)
	{
		rectangle(color,0,0,GraphicDevice::COL-1,GraphicDevice::ROW-1);
	}

	void setbgcolor(const Color &color)
	{
		setbgcolor(color.get());
	}

	void drawbuffer(const u16arm_t *buffer,u16arm_t x=0,u16arm_t  y=0,u16arm_t w=GraphicDevice::COL,u16arm_t h=GraphicDevice::ROW)
	{
		for(u16arm_t i=y;i<y+h;++i)
			for(u16arm_t j=x;j<x+w;++j)
				pixel(buffer[i*w+j],j,i);

	}
};

using Color = Graphic_Type::Color;


#endif
