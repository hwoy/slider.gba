
#define VIDMEM ((volatile unsigned short *)0x6000000)
#define CONMEM ((volatile unsigned long *)0x4000000)

struct GraphicDrvice
{
	static constexpr const unsigned char COL=240;
	static constexpr const unsigned char ROW=160;

	volatile unsigned short *vidmem;
	volatile unsigned long *conmem;

	inline void setmode(unsigned long mode)
	{
		*conmem=mode;
	}

	inline unsigned long getmode(void) const
	{
		return *conmem;
	}

	inline void setvidmem(volatile unsigned short *vd)
	{
		vidmem=vd;
	}

	inline volatile unsigned short *getvidmem(void)
	{
		return vidmem;
	}


	explicit inline GraphicDrvice(volatile unsigned short *vidmem=VIDMEM ,volatile unsigned long *conmem=CONMEM,unsigned long mode=0x403)
	:vidmem(vidmem),conmem(conmem)
	{
		*conmem=mode;
	}

	inline unsigned short read(unsigned short index) const
	{
		return vidmem[index];
	}

	inline void write(unsigned short index,unsigned short value)
	{
		vidmem[index]=value;
	}

	inline unsigned short read(unsigned char x,unsigned char y) const
	{
		return read(x+y*COL);
	}

	inline void write(unsigned char x,unsigned char y,unsigned short value)
	{
		write(x+y*COL,value);
	}
};

struct Graphic_Type
{

	union Color
	{
		
		unsigned short color;
		struct Rgb
		{
				unsigned short r:5;
				unsigned short g:5;
				unsigned short b:5;
		}rgb;
		
		inline constexpr Color(unsigned short r,unsigned short g,unsigned short b):rgb{r,g,b} {}

		inline constexpr Color(unsigned short color):color(color) {}

		inline constexpr unsigned short get(void) const
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

	GraphicDrvice &gd;

	Graphic(GraphicDrvice &gd):gd(gd) {}


	inline void pixel(unsigned short color,unsigned char x,unsigned char y)
	{
		gd.write(x,y,color);
	}

	inline void pixel(const Color &color,unsigned char x,unsigned char y)
	{
		pixel(color.get(),x,y);
	}

	inline Color::Rgb pixel(unsigned char x,unsigned char y)
	{
		return Color(gd.read(x,y)).getrgb();
	}

	void rectangle(unsigned short color,unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
	{
		for(unsigned short i=(x1+y1*GraphicDrvice::COL);i<=(x2+y2*GraphicDrvice::COL);++i)
			gd.write(i,color);
	}

	void rectangle(const Color &color,unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
	{
		rectangle(color.get(),x1,y1,x2,y2);
	}

	void setbgcolor(unsigned short color)
	{
		rectangle(color,0,0,GraphicDrvice::ROW-1,GraphicDrvice::COL-1);
	}

	void setbgcolor(const Color &color)
	{
		setbgcolor(color.get());
	}
};

