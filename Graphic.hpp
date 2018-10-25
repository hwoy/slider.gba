#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__


#include "arm7type.h"

#define RGB15(r,g,b)  ((r)+(g<<5)+(b<<10))

static constexpr const u32arm_t VRAM    = 0x6000000;
static constexpr const u32arm_t VRAM2   = 0x6000A00;
static constexpr const u32arm_t PRAM    = 0x5000000;
static constexpr const u32arm_t DISPCNT = 0x4000000;

struct Point
{
	u32arm_t x,y;

	inline constexpr Point(u32arm_t x,u32arm_t y):x(x),y(y){}

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
	using Dispcnt_t = u32arm_t;
	using PtrDispcnt_t = volatile Dispcnt_t *;

	static inline constexpr volatile Dispcnt_t & refdispcnt(void)
	{
		return *reinterpret_cast<PtrDispcnt_t>(DISPCNT);
	}

	static inline constexpr PtrDispcnt_t ptrdispcnt(void)
	{
		return reinterpret_cast<PtrDispcnt_t>(DISPCNT);
	}
};

template <unsigned long _VRAM_,typename VRAMTYPE,u32arm_t _COL,u32arm_t _ROW>
struct BGMODE
{
	using Vram_t = VRAMTYPE;
	using PtrVram_t = volatile Vram_t *;

	using Pram_t = u16arm_t;
	using PtrPram_t = volatile Pram_t *;

	static constexpr const u32arm_t COL=_COL;
	static constexpr const u32arm_t ROW=_ROW;

	static inline constexpr volatile Vram_t &refvid(u32arm_t x,u32arm_t y)
	{
		return reinterpret_cast<PtrVram_t>(_VRAM_)[x+y*COL];
	}

	static inline constexpr PtrVram_t ptrvid(u32arm_t x,u32arm_t y)
	{
		return reinterpret_cast<PtrVram_t>(_VRAM_)+x+y*COL;
	}

	static inline constexpr volatile Pram_t &refplt(usize_t index)
	{
		return reinterpret_cast<PtrPram_t>(PRAM)[index];
	}

	static inline constexpr PtrPram_t ptrplt(usize_t index)
	{
		return reinterpret_cast<PtrPram_t>(PRAM)+index;
	}
};

using BGMODE3      = BGMODE<VRAM,u16arm_t,240,160>;
using BGMODE3X32   = BGMODE<VRAM,u32arm_t,120,160>;

using BGMODE4      = BGMODE<VRAM,u8arm_t,240,160>;
using BGMODE4X16   = BGMODE<VRAM,u16arm_t,120,160>;
using BGMODE4X32   = BGMODE<VRAM,u16arm_t,60,160>;

using BGMODE4P2    = BGMODE<VRAM2,u8arm_t,240,160>;
using BGMODE4X16P2 = BGMODE<VRAM2,u16arm_t,120,160>;
using BGMODE4X32P2 = BGMODE<VRAM2,u16arm_t,60,160>;

template <class _BGMODE_,u32arm_t _mode_>
struct ColorTrait
{
	using bgmode = _BGMODE_;
	using Vram_t = typename bgmode::Vram_t;
	using Color_t = Vram_t;

	using Pram_t = typename bgmode::Pram_t;

	static constexpr const u32arm_t mode = _mode_;
	static constexpr const u32arm_t COL=bgmode::COL;
	static constexpr const u32arm_t ROW=bgmode::ROW;

};

template <class BGCOLORMODE>
struct PaletteImp
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Pram_t = typename bgmode::Pram_t;

	static void palette(const Pram_t *buff,usize_t N,usize_t M=0)
	{
		for(usize_t i=0;i<N;++i)
			bgmode::refplt(i+M)=buff[i];
	}

	inline static constexpr volatile Pram_t &palette(usize_t N,usize_t M=0)
	{
		return bgmode::refplt(N+M);
	}

	template <usize_t N>
	static void palette(const Pram_t (&buff)[N],usize_t M=0)
	{
		palette(buff,N,M);
	}	
};

using Color3 = ColorTrait<BGMODE3,0x03>;

using Color3x32 = ColorTrait<BGMODE3X32,0x03>;

struct Color4 : public ColorTrait<BGMODE4,0x04> , public PaletteImp<ColorTrait<BGMODE4,0x04>>
{
	using bgmode = typename ColorTrait<BGMODE4,0x04>::bgmode;
};

struct Color4x16 : public ColorTrait<BGMODE4X16,0x04> , public PaletteImp<ColorTrait<BGMODE4X16,0x04>>
{
	using bgmode = typename ColorTrait<BGMODE4X16,0x04>::bgmode;
};

struct Color4x32 : public ColorTrait<BGMODE4X32,0x04> , public PaletteImp<ColorTrait<BGMODE4X32,0x04>>
{
	using bgmode = typename ColorTrait<BGMODE4X32,0x04>::bgmode;
};

struct Color4p2 : public ColorTrait<BGMODE4P2,0x04> , public PaletteImp<ColorTrait<BGMODE4P2,0x04>>
{
	using bgmode = typename ColorTrait<BGMODE4P2,0x04>::bgmode;
};

struct Color4x16p2 : public ColorTrait<BGMODE4X16P2,0x04> , public PaletteImp<ColorTrait<BGMODE4X16P2,0x04>>
{
	using bgmode = typename ColorTrait<BGMODE4X16P2,0x04>::bgmode;
};

struct Color4x32p2 : public ColorTrait<BGMODE4X32P2,0x04> , public PaletteImp<ColorTrait<BGMODE4X32P2,0x04>>
{
	using bgmode = typename ColorTrait<BGMODE4X32P2,0x04>::bgmode;
};


template <class BGCOLORMODE>
struct Grange
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Vram_t = typename BGCOLORMODE::Vram_t;
	using Color_t = Vram_t;

	struct Iterator
	{
		const u32arm_t x1,x2;
		Point p;

		explicit inline constexpr Iterator(u32arm_t x1,u32arm_t x2,const Point &p):x1(x1),x2(x2),p(p){}

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


template <class BGCOLORMODE>
struct Graphic: public BGCOLORMODE
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Vram_t = typename bgmode::Vram_t;
	using Color_t = Vram_t;
	using Pram_t = typename bgmode::Pram_t;

	static constexpr const u32arm_t COL=bgmode::COL;
	static constexpr const u32arm_t ROW=bgmode::ROW;



	static inline void pixel(Color_t color,u32arm_t x,u32arm_t y)
	{
		bgmode::refvid(x,y)=color;
	}

	static inline Color_t pixel(u32arm_t x,u32arm_t y)
	{
		return bgmode::refvid(x,y);
	}

	static void rectangle(Color_t color,u32arm_t x1,u32arm_t y1,u32arm_t x2,u32arm_t y2)
	{
		for(volatile auto &rpoint:Grange<BGCOLORMODE>({x1,y1},{x2,y2}))
			rpoint=color;

	}

	static void setbgcolor(Color_t color)
	{
		rectangle(color,0,0,COL-1,ROW-1);
	}

	template <usize_t N>
	static void drawbuffer(const Color_t (&buffer)[N],u32arm_t w=COL,u32arm_t x=0,u32arm_t y=0)
	{
		for(usize_t i=0;i<N;++i)
		{
			bgmode::refvid(x+i%w,y+i/w)=buffer[i];
		}

	}

	template <usize_t N,usize_t M>
	static void drawbuffer(const Color_t (&buffer)[N][M],u32arm_t x=0,u32arm_t y=0)
	{
		for(usize_t i=0;i<N;++i)
			for(usize_t j=0;j<M;++j)
				bgmode::refvid(x+j,y+i)=buffer[i][j];
	}

};


#endif
