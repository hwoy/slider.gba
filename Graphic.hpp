#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__

#include <iterator>
#include "arm7type.h"

#define RGB15(r,g,b)  ((r)+(g<<5)+(b<<10))

static constexpr const u32arm_t VRAM    = 0x6000000;
static constexpr const u32arm_t VRAM2   = 0x600A000;
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

	inline constexpr bool operator > (const Point & p) const
	{
		return y > p.y ? true : ( y < p.y ? false : ( x > p.x ? true : false ) );
	}

	inline constexpr bool operator >= (const Point & p) const
	{
		return *this > p || *this == p;
	}

	inline constexpr bool operator < (const Point & p) const
	{
		return !(*this >= p);
	}

	inline constexpr bool operator <= (const Point & p) const
	{
		return !(*this > p);
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

using BGMODE5      = BGMODE<VRAM,u16arm_t,160,128>;
using BGMODE5X32   = BGMODE<VRAM,u32arm_t,80,128>;

using BGMODE5P2      = BGMODE<VRAM2,u16arm_t,160,128>;
using BGMODE5X32P2   = BGMODE<VRAM2,u32arm_t,80,128>;

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

using Color4p1    = Color4;

using Color4x16p1 = Color4x16;

using Color4x32p1 = Color4x32;

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

using Color5 = ColorTrait<BGMODE5,0x05>;

using Color5x32 = ColorTrait<BGMODE5X32,0x05>;

using Color5p1 = Color5;

using Color5x32p1 = Color5x32;

using Color5p2 = ColorTrait<BGMODE5P2,0x05>;

using Color5x32p2 = ColorTrait<BGMODE5X32P2,0x05>;


template <class BGCOLORMODE>
struct Grange
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Vram_t = typename bgmode::Vram_t;
	using Color_t = Vram_t;

	struct Iterator
	{

		using value_type = Vram_t ;
		using difference_type = usize_t ;
		using pointer = typename bgmode::PtrVram_t ;
		using reference = volatile Vram_t& ;
		using const_reference = volatile const Vram_t& ;
		using iterator_category = std::random_access_iterator_tag ;


		const u32arm_t x1,x2;
		const u32arm_t dim;
		Point p;

		explicit inline constexpr Iterator(u32arm_t x1,u32arm_t x2,u32arm_t dim,const Point &p):x1(x1),x2(x2),dim(dim),p(p) {}

		explicit inline constexpr Iterator(u32arm_t x1,u32arm_t x2,u32arm_t dim,u32arm_t x,u32arm_t y):Iterator(x1,x2,dim,{x,y}) {}

		inline constexpr const_reference operator * () const
		{
			return bgmode::refvid(p.x,p.y);
		}

		inline volatile reference operator * ()
		{
			return bgmode::refvid(p.x,p.y);
		}

		Iterator &operator ++ ()
		{
			if(p.x+1>=x2)
			{
				p.x=x1;
				++p.y;
			}
			
			else
			{
				++p.x;
			}		

			return *this;
			
		}

		Iterator operator ++ (int)
		{
			Iterator it = *this;
			++(*this);
			return it;
		}

		constexpr Iterator operator + (usize_t n) const
		{
			return Iterator{x1,x2,x1+((p.x+n-x1)%(x2-x1)),p.y+((p.x+n-x1)/(x2-x1))};
		}

		Iterator &operator += (usize_t n)
		{
			p = {x1+((p.x+n-x1)%(x2-x1)),p.y+((p.x+n-x1)/(x2-x1))};

			return *this;
		}
		
		constexpr difference_type operator - (const Iterator &i) const
		{

			return (!dim && !i.dim)? 0 : p.x+p.y*(x2-x1) - ( i.p.x+i.p.y*(i.x2-i.x1) );
		}

		constexpr reference operator [] (usize_t n) const
		{
			return *(*this+n);
		}

		Iterator &operator -- ()
		{
			if(p.x<x1+1)
			{
				p.x=x2-1;
				--p.y;
			}
			
			else
			{
				--p.x;
			}
			
			return *this;
			
		}

		Iterator operator -- (int)
		{
			Iterator it = *this;
			--(*this);
			return it;
		}

		constexpr Iterator operator - (usize_t n) const
		{

			return Iterator{x1,x2,x2-1-((x2-1-p.x+n)%(x2-x1)),p.y-((x2-1-p.x+n)/(x2-x1))};
		}

		Iterator &operator -= (usize_t n)
		{
			p = {x2-1-((x2-1-p.x+n)%(x2-x1)),p.y-((x2-1-p.x+n)/(x2-x1))};

			return *this;
		}


		inline constexpr bool operator == (const Iterator & it) const
		{
			return (p == it.p) || (!dim && !it.dim);
		}

		inline constexpr bool operator != (const Iterator & it) const
		{
			return !(*this == it);
		}

		inline constexpr bool operator > (const Iterator & it) const
		{
			return p > it.p;
		}

		inline constexpr bool operator >= (const Iterator & it) const
		{
			return p >= it.p;
		}

		inline constexpr bool operator < (const Iterator & it) const
		{
			return p < it.p;
		}

		inline constexpr bool operator <= (const Iterator & it) const
		{
			return p <= it.p;
		}

	};

	const Iterator itbegin,itend;

	inline constexpr Iterator begin() const
	{
		return itbegin;
	}

	inline constexpr Iterator end() const
	{
		return itend;
	}

	inline constexpr usize_t size(void) const
	{
		return itend-itbegin;
	}

	inline constexpr usize_t col(void) const
	{
		return itbegin.x2-itbegin.x1;
	}

	inline constexpr typename Iterator::reference operator [] (usize_t n) const
	{
		return itbegin[n];
	}

	struct Riterator : public Iterator
	{

		using value_type = typename Iterator::value_type ;
		using difference_type = typename Iterator::difference_type ;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
		using const_reference = typename Iterator::const_reference;
		using iterator_category = typename Iterator::iterator_category ;


		explicit inline constexpr Riterator(u32arm_t x1,u32arm_t x2,u32arm_t dim,const Point &p): Iterator{x1,x2,dim,p} {}

		explicit inline constexpr Riterator(u32arm_t x1,u32arm_t x2,u32arm_t dim,u32arm_t x,u32arm_t y):Riterator(x1,x2,dim,{x,y}) {}


		Riterator &operator ++ ()
		{
			if(Iterator::p.x<Iterator::x1+1)
			{
				Iterator::p.x=Iterator::x2-1;
				--Iterator::p.y;
			}
			
			else
			{
				--Iterator::p.x;
			}
			
			return *this;
			
		}

		Riterator operator ++ (int)
		{
			Riterator it = *this;
			--(*this);
			return it;
		}

		constexpr Riterator operator + (usize_t n) const
		{
			return Riterator{Iterator::x1,Iterator::x2,Iterator::x2-1-((Iterator::x2-1-Iterator::p.x+n)%(Iterator::x2-Iterator::x1)),Iterator::p.y-((Iterator::x2-1-Iterator::p.x+n)/(Iterator::x2-Iterator::x1))};
		}

		Riterator &operator += (usize_t n)
		{
			Iterator::p = {Iterator::x2-1-((Iterator::x2-1-Iterator::p.x+n)%(Iterator::x2-Iterator::x1)),Iterator::p.y-((Iterator::x2-1-Iterator::p.x+n)/(Iterator::x2-Iterator::x1))};

			return *this;
		}
		
		constexpr difference_type operator - (const Riterator &i) const
		{

			return (!Iterator::dim && !i.Iterator::dim)? 0 : ( i.p.x+i.p.y*(i.x2-i.x1) ) - Iterator::p.x+Iterator::p.y*(Iterator::x2-Iterator::x1) ;
		}

		constexpr reference operator [] (usize_t n) const
		{
			return *(*this-n);
		}

		Riterator &operator -- ()
		{
			if(Iterator::p.x+1>=Iterator::x2)
			{
				Iterator::p.x=Iterator::x1;
				++Iterator::p.y;
			}
			
			else
			{
				++Iterator::p.x;
			}		

			return *this;
			
		}

		Riterator operator -- (int)
		{
			Riterator it = *this;
			++(*this);
			return it;
		}

		constexpr Riterator operator - (usize_t n) const
		{

			return Iterator{Iterator::x1,Iterator::x2,Iterator::x1+((Iterator::p.x+n-Iterator::x1)%(Iterator::x2-Iterator::x1)),Iterator::p.y+((Iterator::p.x+n-Iterator::x1)/(Iterator::x2-Iterator::x1))};
		}

		Riterator &operator -= (usize_t n)
		{
			Iterator::p = {Iterator::x1+((Iterator::p.x+n-Iterator::x1)%(Iterator::x2-Iterator::x1)),Iterator::p.y+((Iterator::p.x+n-Iterator::x1)/(Iterator::x2-Iterator::x1))};

			return *this;
		}

	};

	const Riterator ritbegin,ritend;

	inline constexpr Grange(const Point &p1,const Point &p2):
	itbegin(p1.x,p2.x,(p2.x-p1.x)*(p2.y-p1.y),p1),itend(p1.x,p2.x,(p2.x-p1.x)*(p2.y-p1.y),p1.x,p2.y),
	ritbegin(p1.x,p2.x,(p2.x-p1.x)*(p2.y-p1.y),p2),ritend(p1.x,p2.x,(p2.x-p1.x)*(p2.y-p1.y),p1.x-1,p1.y-1)   {}

	inline constexpr Grange(u32arm_t x1,u32arm_t y1,u32arm_t x2,u32arm_t y2):
	Grange({x1,y1},{x2,y2}) {}

	inline constexpr Riterator rbegin() const
	{
		return ritbegin;
	}

	inline constexpr Riterator rend() const
	{
		return ritend;
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

	static inline constexpr Color_t pixel(u32arm_t x,u32arm_t y)
	{
		return bgmode::refvid(x,y);
	}

	static inline constexpr Grange<BGCOLORMODE> grange(u32arm_t x1=0,u32arm_t y1=0,u32arm_t x2=COL,u32arm_t y2=ROW)
	{
		return Grange<BGCOLORMODE>(x1,y1,x2,y2);
	}

};

template <class GRAPHIC>
struct BufferImp
{
	using bgmode = typename GRAPHIC::bgmode;
	using Vram_t = typename bgmode::Vram_t;
	using Color_t = Vram_t;

	static constexpr const u32arm_t COL=bgmode::COL;
	static constexpr const u32arm_t ROW=bgmode::ROW;

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

template <class GRAPHIC>
struct SharpImp
{
	using bgmode = typename GRAPHIC::bgmode;
	using Vram_t = typename bgmode::Vram_t;
	using Color_t = Vram_t;

	static constexpr const u32arm_t COL=bgmode::COL;
	static constexpr const u32arm_t ROW=bgmode::ROW;

	static void rectangle(Color_t color,u32arm_t x1,u32arm_t y1,u32arm_t x2,u32arm_t y2)
	{
		for(volatile auto &rpoint:GRAPHIC::grange(x1,y1,x2,y2))
			rpoint=color;
	}

	static void bgcolor(Color_t color)
	{
		rectangle(color,0,0,COL,ROW);
	}

	static void box(Color_t color,u32arm_t x,u32arm_t y,u32arm_t w,u32arm_t h)
	{
		for(volatile auto &rpoint:GRAPHIC::grange(x,y,x+w,y))
			rpoint=color;

		for(volatile auto &rpoint:GRAPHIC::grange(x,y,x,y+h))
			rpoint=color;

		for(volatile auto &rpoint:GRAPHIC::grange(x+w,y,x+w,y+h))
			rpoint=color;
		
		for(volatile auto &rpoint:GRAPHIC::grange(x,y+h,x+w,y+h))
			rpoint=color;
	}
};

template <class BGCOLORMODE>
struct Graphicx: public Graphic<BGCOLORMODE> ,public SharpImp<Graphic<BGCOLORMODE>>, public BufferImp<Graphic<BGCOLORMODE>>
{
	using bgmode = typename BGCOLORMODE::bgmode;
	using Vram_t = typename bgmode::Vram_t;
	using Color_t = Vram_t;

	static constexpr const u32arm_t COL=bgmode::COL;
	static constexpr const u32arm_t ROW=bgmode::ROW;
};

#endif
