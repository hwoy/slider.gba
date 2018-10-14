
#include "arm7type.hpp"

#define VIDMEM ((volatile u16arm_t *)0x6000000)
#define CONMEM ((volatile u32arm_t *)0x4000000)

struct GraphicDevice
{
	static constexpr const u8arm_t COL=240;
	static constexpr const u8arm_t ROW=160;

	volatile u16arm_t *vidmem;
	volatile u32arm_t *conmem;

	inline void setmode(u32arm_t mode)
	{
		*conmem=mode;
	}

	inline u32arm_t getmode(void) const
	{
		return *conmem;
	}

	inline void setvidmem(volatile u16arm_t *vd)
	{
		vidmem=vd;
	}

	inline volatile u16arm_t *getvidmem(void)
	{
		return vidmem;
	}


	explicit inline GraphicDevice(volatile u16arm_t *vidmem=VIDMEM ,volatile u32arm_t *conmem=CONMEM,u32arm_t mode=0x403)
	:vidmem(vidmem),conmem(conmem)
	{
		*conmem=mode;
	}

	inline u16arm_t read(u16arm_t index) const
	{
		return vidmem[index];
	}

	inline void write(u16arm_t index,u16arm_t value)
	{
		vidmem[index]=value;
	}

	inline u16arm_t read(u8arm_t x,u8arm_t y) const
	{
		return read(x+y*COL);
	}

	inline void write(u8arm_t x,u8arm_t y,u16arm_t value)
	{
		write(x+y*COL,value);
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
		}rgb;
		
		inline constexpr Color(u16arm_t r,u16arm_t g,u16arm_t b):rgb{r,g,b} {}

		inline constexpr Color(u16arm_t color):color(color) {}

		inline constexpr u16arm_t get(void) const
		{
			return color;
		}

		inline Rgb getrgb(void) const
		{
			return rgb;
		}
	};
};

struct Graphic: public Graphic_Type
{
	using Color = typename Graphic_Type::Color;

	GraphicDevice gd;

	Graphic(const GraphicDevice &gd):gd(gd) {}


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
};

