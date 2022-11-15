#ifndef __GRAPHIC_HPP__
#define __GRAPHIC_HPP__

#include <iterator>

#include "arm7type.h"

#define RGB15(r, g, b) ((r) + (g << 5) + (b << 10))

static constexpr const u32arm_t VRAM = 0x6000000;
static constexpr const u32arm_t VRAM2 = 0x600A000;
static constexpr const u32arm_t PRAM = 0x5000000;
static constexpr const u32arm_t DISPCNT = 0x4000000;
static constexpr const u32arm_t VCOUNT = 0x4000006;

struct Point {
    i32arm_t x, y;

    inline constexpr Point(i32arm_t x, i32arm_t y)
        : x(x)
        , y(y)
    {
    }

    inline constexpr bool operator!=(const Point& p) const
    {
        return x != p.x || y != p.y;
    }

    inline constexpr bool operator==(const Point& p) const
    {
        return x == p.x && y == p.y;
    }

    inline constexpr bool operator>(const Point& p) const
    {
        return y > p.y ? true : (y < p.y ? false : (x > p.x ? true : false));
    }

    inline constexpr bool operator>=(const Point& p) const
    {
        return *this > p || *this == p;
    }

    inline constexpr bool operator<(const Point& p) const
    {
        return !(*this >= p);
    }

    inline constexpr bool operator<=(const Point& p) const
    {
        return !(*this > p);
    }
};

struct GraphicDevice {
    using Dispcnt_t = u32arm_t;
    using PtrDispcnt_t = volatile Dispcnt_t*;

    static inline constexpr volatile Dispcnt_t& refdispcnt(void)
    {
        return *reinterpret_cast<PtrDispcnt_t>(DISPCNT);
    }

    static inline constexpr PtrDispcnt_t ptrdispcnt(void)
    {
        return reinterpret_cast<PtrDispcnt_t>(DISPCNT);
    }

    using Vcount_t = u16arm_t;
    using PtrVcount_t = volatile const Vcount_t*;

    static inline constexpr volatile const Vcount_t& refvcount(void)
    {
        return *reinterpret_cast<PtrVcount_t>(VCOUNT);
    }

    static inline constexpr PtrVcount_t ptrvcount(void)
    {
        return reinterpret_cast<PtrVcount_t>(VCOUNT);
    }
};

template <unsigned long _VRAM_, typename VRAMTYPE, u32arm_t _COL, u32arm_t _ROW>
struct BGMODE {
    using Vram_t = VRAMTYPE;
    using PtrVram_t = volatile Vram_t*;

    using Pram_t = u16arm_t;
    using PtrPram_t = volatile Pram_t*;

    static constexpr const i32arm_t COL = _COL;
    static constexpr const i32arm_t ROW = _ROW;

    static inline constexpr volatile Vram_t& refvid(i32arm_t x, i32arm_t y)
    {
        return reinterpret_cast<PtrVram_t>(_VRAM_)[x + y * COL];
    }

    static inline constexpr PtrVram_t ptrvid(i32arm_t x, i32arm_t y)
    {
        return reinterpret_cast<PtrVram_t>(_VRAM_) + x + y * COL;
    }

    static inline constexpr volatile Pram_t& refplt(usize_t index)
    {
        return reinterpret_cast<PtrPram_t>(PRAM)[index];
    }

    static inline constexpr PtrPram_t ptrplt(usize_t index)
    {
        return reinterpret_cast<PtrPram_t>(PRAM) + index;
    }
};

using BGMODE3 = BGMODE<VRAM, u16arm_t, 240, 160>;
using BGMODE3X32 = BGMODE<VRAM, u32arm_t, 120, 160>;

using BGMODE4X16 = BGMODE<VRAM, u16arm_t, 120, 160>;
using BGMODE4X32 = BGMODE<VRAM, u16arm_t, 60, 160>;
using BGMODE4 = BGMODE4X16;

using BGMODE4X16P2 = BGMODE<VRAM2, u16arm_t, 120, 160>;
using BGMODE4X32P2 = BGMODE<VRAM2, u16arm_t, 60, 160>;
using BGMODE4P2 = BGMODE4X16P2;

using BGMODE5 = BGMODE<VRAM, u16arm_t, 160, 128>;
using BGMODE5X32 = BGMODE<VRAM, u32arm_t, 80, 128>;

using BGMODE5P2 = BGMODE<VRAM2, u16arm_t, 160, 128>;
using BGMODE5X32P2 = BGMODE<VRAM2, u32arm_t, 80, 128>;

template <class _BGMODE_, u32arm_t _mode_>
struct ColorTrait {
    using bgmode = _BGMODE_;
    using Vram_t = typename bgmode::Vram_t;
    using Color_t = Vram_t;

    using Pram_t = typename bgmode::Pram_t;

    static constexpr const u32arm_t mode = _mode_;
    static constexpr const i32arm_t COL = bgmode::COL;
    static constexpr const i32arm_t ROW = bgmode::ROW;
};

template <class BGCOLORMODE>
struct PaletteImp {
    using Color = BGCOLORMODE;
    using bgmode = typename Color::bgmode;
    using Pram_t = typename bgmode::Pram_t;

    static void palette(const Pram_t* buff, usize_t N, usize_t M = 0)
    {
        for (usize_t i = 0; i < N; ++i)
            bgmode::refplt(i + M) = buff[i];
    }

    inline static constexpr volatile Pram_t& palette(usize_t N, usize_t M = 0)
    {
        return bgmode::refplt(N + M);
    }

    template <usize_t N>
    static void palette(const Pram_t (&buff)[N], usize_t M = 0)
    {
        palette(buff, N, M);
    }
};

template <class BGCOLORMODE>
struct Flip {
    using Color = BGCOLORMODE;
    using bgmode = typename Color::bgmode;
    using Pram_t = typename bgmode::Pram_t;

protected:
    inline static bool isshowp2(void)
    {
        return GraphicDevice::refdispcnt() & 0x10;
    }

    inline static bool isshowp1(void) { return !isshowp2(); }

    inline static void showp1(void) { GraphicDevice::refdispcnt() &= ~0x10; }

    inline static void showp2(void) { GraphicDevice::refdispcnt() |= 0x10; }

    inline static void noshowp1(void) { showp2(); }

    inline static void noshowp2(void) { showp1(); }

public:
    static void flip(void)
    {
        if (isshowp1())
            showp2();
        else
            showp1();
    }
};

template <class BGCOLORMODE>
struct Show_p1_Imp : public Flip<BGCOLORMODE> {
    using Color = BGCOLORMODE;
    using bgmode = typename Color::bgmode;
    using Pram_t = typename bgmode::Pram_t;

    inline static void show(void) { Flip<BGCOLORMODE>::showp1(); }
    inline static void noshow(void) { Flip<BGCOLORMODE>::noshowp1(); }

    inline static constexpr bool isshow(void)
    {
        return Flip<BGCOLORMODE>::isshowp1();
    }
};

template <class BGCOLORMODE>
struct Show_p2_Imp : public Flip<BGCOLORMODE> {
    using Color = BGCOLORMODE;
    using bgmode = typename Color::bgmode;
    using Pram_t = typename bgmode::Pram_t;

    inline static void show(void) { Flip<BGCOLORMODE>::showp2(); }
    inline static void noshow(void) { Flip<BGCOLORMODE>::noshowp2(); }

    inline static constexpr bool isshow(void)
    {
        return Flip<BGCOLORMODE>::isshowp2();
    }
};

using Color3 = ColorTrait<BGMODE3, 0x03>;

using Color3x32 = ColorTrait<BGMODE3X32, 0x03>;

struct Color4 : public ColorTrait<BGMODE4, 0x04>,
                public PaletteImp<ColorTrait<BGMODE4, 0x04>>,
                public Show_p1_Imp<ColorTrait<BGMODE4, 0x04>> {
    using bgmode = typename ColorTrait<BGMODE4, 0x04>::bgmode;
};

struct Color4x16 : public ColorTrait<BGMODE4X16, 0x04>,
                   public PaletteImp<ColorTrait<BGMODE4X16, 0x04>>,
                   public Show_p1_Imp<ColorTrait<BGMODE4X16, 0x04>> {
    using bgmode = typename ColorTrait<BGMODE4X16, 0x04>::bgmode;
};

struct Color4x32 : public ColorTrait<BGMODE4X32, 0x04>,
                   public PaletteImp<ColorTrait<BGMODE4X32, 0x04>>,
                   public Show_p1_Imp<ColorTrait<BGMODE4X32, 0x04>> {
    using bgmode = typename ColorTrait<BGMODE4X32, 0x04>::bgmode;
};

using Color4p1 = Color4;

using Color4x16p1 = Color4x16;

using Color4x32p1 = Color4x32;

struct Color4p2 : public ColorTrait<BGMODE4P2, 0x04>,
                  public PaletteImp<ColorTrait<BGMODE4P2, 0x04>>,
                  public Show_p2_Imp<ColorTrait<BGMODE4P2, 0x04>> {
    using bgmode = typename ColorTrait<BGMODE4P2, 0x04>::bgmode;
};

struct Color4x16p2 : public ColorTrait<BGMODE4X16P2, 0x04>,
                     public PaletteImp<ColorTrait<BGMODE4X16P2, 0x04>>,
                     public Show_p2_Imp<ColorTrait<BGMODE4X16P2, 0x04>> {
    using bgmode = typename ColorTrait<BGMODE4X16P2, 0x04>::bgmode;
};

struct Color4x32p2 : public ColorTrait<BGMODE4X32P2, 0x04>,
                     public PaletteImp<ColorTrait<BGMODE4X32P2, 0x04>>,
                     public Show_p2_Imp<ColorTrait<BGMODE4X32P2, 0x04>> {
    using bgmode = typename ColorTrait<BGMODE4X32P2, 0x04>::bgmode;
};

struct Color5 : public ColorTrait<BGMODE5, 0x05>,
                public Show_p1_Imp<ColorTrait<BGMODE5, 0x05>> {
    using bgmode = typename ColorTrait<BGMODE5, 0x05>::bgmode;
};

struct Color5x32 : public ColorTrait<BGMODE5X32, 0x05>,
                   public Show_p1_Imp<ColorTrait<BGMODE5X32, 0x05>> {
    using bgmode = typename ColorTrait<BGMODE5X32, 0x05>::bgmode;
};

using Color5p1 = Color5;

using Color5x32p1 = Color5x32;

struct Color5p2 : public ColorTrait<BGMODE5P2, 0x05>,
                  public Show_p2_Imp<ColorTrait<BGMODE5P2, 0x05>> {
    using bgmode = typename ColorTrait<BGMODE5P2, 0x05>::bgmode;
};

struct Color5x32p2 : public ColorTrait<BGMODE5X32P2, 0x05>,
                     public Show_p2_Imp<ColorTrait<BGMODE5X32P2, 0x05>> {
    using bgmode = typename ColorTrait<BGMODE5X32P2, 0x05>::bgmode;
};

template <class BGCOLORMODE>
struct Grange {
    using Color = BGCOLORMODE;
    using bgmode = typename Color::bgmode;
    using Vram_t = typename bgmode::Vram_t;
    using Color_t = Vram_t;

    Point p1, p2;

    struct Iterator {
        using value_type = Vram_t;
        using difference_type = isize_t;
        using pointer = typename bgmode::PtrVram_t;
        using reference = volatile Vram_t&;
        using const_reference = volatile const Vram_t&;
        using iterator_category = std::random_access_iterator_tag;

        const i32arm_t x1, x2;
        const i32arm_t size;
        Point p;

        explicit inline constexpr Iterator(i32arm_t x1, i32arm_t x2, i32arm_t size,
            const Point& p)
            : x1(x1)
            , x2(x2)
            , size(size)
            , p(p)
        {
        }

        explicit inline constexpr Iterator(i32arm_t x1, i32arm_t x2, i32arm_t size,
            i32arm_t x, i32arm_t y)
            : Iterator(x1, x2, size, { x, y })
        {
        }

        inline constexpr reference operator*() const
        {
            return bgmode::refvid(p.x, p.y);
        }

        Iterator& operator++()
        {
            if (p.x + 1 >= x2) {
                p.x = x1;
                ++p.y;
            }

            else {
                ++p.x;
            }

            return *this;
        }

        Iterator operator++(int)
        {
            Iterator it = *this;
            ++(*this);
            return it;
        }

        constexpr Iterator operator+(isize_t n) const
        {
            return Iterator(x1, x2, size, x1 + ((p.x + n - x1) % (x2 - x1)),
                p.y + ((p.x + n - x1) / (x2 - x1)));
        }

        Iterator& operator+=(isize_t n)
        {
            p = { x1 + ((p.x + n - x1) % (x2 - x1)),
                p.y + ((p.x + n - x1) / (x2 - x1)) };

            return *this;
        }

        constexpr difference_type operator-(const Iterator& i) const
        {
            return (!size && !i.size) ? 0
                                      : (p.x - x1) + p.y * (x2 - x1) - ((i.p.x - i.x1) + i.p.y * (i.x2 - i.x1));
        }

        constexpr reference operator[](isize_t n) const { return *(*this + n); }

        Iterator& operator--()
        {
            if (p.x < x1 + 1) {
                p.x = x2 - 1;
                --p.y;
            }

            else {
                --p.x;
            }

            return *this;
        }

        Iterator operator--(int)
        {
            Iterator it = *this;
            --(*this);
            return it;
        }

        constexpr Iterator operator-(isize_t n) const
        {
            return Iterator(x1, x2, size, x2 - 1 - ((x2 - 1 - p.x + n) % (x2 - x1)),
                p.y - ((x2 - 1 - p.x + n) / (x2 - x1)));
        }

        Iterator& operator-=(isize_t n)
        {
            p = { x2 - 1 - ((x2 - 1 - p.x + n) % (x2 - x1)),
                p.y - ((x2 - 1 - p.x + n) / (x2 - x1)) };

            return *this;
        }

        inline constexpr bool operator==(const Iterator& it) const
        {
            return (p == it.p) || (!size && !it.size);
        }

        inline constexpr bool operator!=(const Iterator& it) const
        {
            return !(*this == it);
        }

        inline constexpr bool operator>(const Iterator& it) const
        {
            return p > it.p;
        }

        inline constexpr bool operator>=(const Iterator& it) const
        {
            return p >= it.p;
        }

        inline constexpr bool operator<(const Iterator& it) const
        {
            return p < it.p;
        }

        inline constexpr bool operator<=(const Iterator& it) const
        {
            return p <= it.p;
        }
    };

    inline constexpr Iterator begin() const
    {
        return Iterator(p1.x, p2.x, size(), p1);
    }

    inline constexpr Iterator end() const { return begin() + size(); }

    inline constexpr isize_t size(void) const
    {
        return (p2.x - p1.x) * (p2.y - p1.y);
    }

    inline constexpr isize_t col(void) const { return p2.x - p1.x; }

    inline constexpr typename Iterator::reference operator[](isize_t n) const
    {
        return begin()[n];
    }

    inline constexpr Grange(const Point& p1, const Point& p2)
        : p1(p1)
        , p2(p2)
    {
    }

    inline constexpr Grange(i32arm_t x1, i32arm_t y1, i32arm_t x2, i32arm_t y2)
        : Grange({ x1, y1 }, { x2, y2 })
    {
    }

    inline constexpr std::reverse_iterator<Iterator> rbegin() const
    {
        return std::make_reverse_iterator(end());
    }

    inline constexpr std::reverse_iterator<Iterator> rend() const
    {
        return std::make_reverse_iterator(begin());
    }
};

template <class BGCOLORMODE>
struct Graphic : public BGCOLORMODE {
    using Color = BGCOLORMODE;
    using bgmode = typename Color::bgmode;
    using Vram_t = typename bgmode::Vram_t;
    using Color_t = Vram_t;
    using Pram_t = typename bgmode::Pram_t;
    using Grange_t = Grange<Color>;
    using Iterator = typename Grange_t::Iterator;

    static constexpr const i32arm_t COL = bgmode::COL;
    static constexpr const i32arm_t ROW = bgmode::ROW;

    static inline void pixel(Color_t color, i32arm_t x, i32arm_t y)
    {
        bgmode::refvid(x, y) = color;
    }

    static inline constexpr Color_t pixel(i32arm_t x, i32arm_t y)
    {
        return bgmode::refvid(x, y);
    }

    static inline constexpr Grange_t grange(i32arm_t x1 = 0, i32arm_t y1 = 0,
        i32arm_t x2 = COL,
        i32arm_t y2 = ROW)
    {
        return Grange_t(x1, y1, x2, y2);
    }

    static void waitVSync(void)
    {
        while (GraphicDevice::refvcount() >= ROW)
            ;
        while (GraphicDevice::refvcount() < ROW)
            ;
    }
};

template <class GRAPHIC>
struct BufferImp {
    using Graphic = GRAPHIC;
    using Color = typename Graphic::Color;
    using bgmode = typename Color::bgmode;
    using Vram_t = typename bgmode::Vram_t;
    using Color_t = Vram_t;
    using Grange_t = typename Graphic::Grange_t;
    using Iterator = typename Grange_t::Iterator;

    static constexpr const i32arm_t COL = bgmode::COL;
    static constexpr const i32arm_t ROW = bgmode::ROW;

    template <typename T>
    static void drawbuffer(T begin, T end, u32arm_t w = COL, i32arm_t x = 0,
        i32arm_t y = 0)
    {
        for (i32arm_t i = 0; begin != end; ++i)
            Graphic::pixel(*begin++, x + i % w, y + i / w);
    }

    template <usize_t N>
    static void drawbuffer(const Color_t (&buffer)[N], u32arm_t w = COL,
        i32arm_t x = 0, i32arm_t y = 0)
    {
        auto buf = reinterpret_cast<const Color_t*>(buffer);
        for (volatile auto& rcolor : Graphic::grange(x, y, x + w, y + N / w))
            rcolor = *buf++;
    }

    template <usize_t N, usize_t M>
    static void drawbuffer(const Color_t (&buffer)[N][M], i32arm_t x = 0,
        i32arm_t y = 0)
    {
        auto buf = reinterpret_cast<const Color_t*>(buffer);
        for (volatile auto& rcolor : Graphic::grange(x, y, x + M, y + N))
            rcolor = *buf++;
    }
};

template <class GRAPHIC>
struct SharpImp {
    using Graphic = GRAPHIC;
    using Color = typename Graphic::Color;
    using bgmode = typename Color::bgmode;
    using Vram_t = typename bgmode::Vram_t;
    using Color_t = Vram_t;
    using Grange_t = typename Graphic::Grange_t;
    using Iterator = typename Grange_t::Iterator;

    static constexpr const i32arm_t COL = bgmode::COL;
    static constexpr const i32arm_t ROW = bgmode::ROW;

    static void rectangle(Color_t color, i32arm_t x1, i32arm_t y1, i32arm_t x2,
        i32arm_t y2)
    {
        for (volatile auto& rpoint : Graphic::grange(x1, y1, x2, y2))
            rpoint = color;
    }

    static void bgcolor(Color_t color) { rectangle(color, 0, 0, COL, ROW); }

    static void frame(Color_t color, i32arm_t x1, i32arm_t y1, i32arm_t x2,
        i32arm_t y2)
    {
        for (volatile auto& rpoint : Graphic::grange(x1, y1, x2, y1 + 1))
            rpoint = color;

        for (volatile auto& rpoint : Graphic::grange(x1, y1, x1 + 1, y2))
            rpoint = color;

        for (volatile auto& rpoint : Graphic::grange(x2 - 1, y1, x2, y2))
            rpoint = color;

        for (volatile auto& rpoint : Graphic::grange(x1, y2 - 1, x2, y2))
            rpoint = color;
    }
};

template <class GRAPHIC>
struct IteratorImp {
    using Graphic = GRAPHIC;
    using Color = typename Graphic::Color;
    using bgmode = typename Color::bgmode;
    using Vram_t = typename bgmode::Vram_t;
    using Grange_t = typename Graphic::Grange_t;
    using Iterator = typename Grange_t::Iterator;

    static constexpr const i32arm_t COL = bgmode::COL;
    static constexpr const i32arm_t ROW = bgmode::ROW;

    static constexpr Iterator input_iterator(i32arm_t x, i32arm_t y,
        i32arm_t x1 = 0, i32arm_t y1 = 0,
        i32arm_t x2 = COL)
    {
        return Graphic::grange(x1, y1, x2, ROW).begin() + (x + y * (x2 - x1));
    }

    static constexpr Iterator input_begin_iterator(i32arm_t x1 = 0,
        i32arm_t y1 = 0,
        i32arm_t x2 = COL)
    {
        return Graphic::grange(x1, y1, x2, ROW).begin();
    }

    static constexpr Iterator input_end_iterator(i32arm_t x1 = 0, i32arm_t y1 = 0,
        i32arm_t x2 = COL)
    {
        return Graphic::grange(x1, y1, x2, ROW).end();
    }

    static constexpr std::reverse_iterator<Iterator> input_reverse_iterator(
        i32arm_t x, i32arm_t y, i32arm_t x1 = 0, i32arm_t y1 = 0,
        i32arm_t x2 = COL)
    {
        return std::make_reverse_iterator(input_iterator(x, y, x1, y1, x2));
    }

    static constexpr std::reverse_iterator<Iterator> input_begin_reverse_iterator(
        i32arm_t x1 = 0, i32arm_t y1 = 0, i32arm_t x2 = COL)
    {
        return std::make_reverse_iterator(input_end_iterator(x1, y1, x2));
    }

    static constexpr std::reverse_iterator<Iterator> input_end_reverse_iterator(
        i32arm_t x1 = 0, i32arm_t y1 = 0, i32arm_t x2 = COL)
    {
        return std::make_reverse_iterator(input_begin_iterator(x1, y1, x2));
    }
};

template <class BGCOLORMODE>
struct Graphicx : public Graphic<BGCOLORMODE>,
                  public SharpImp<Graphic<BGCOLORMODE>>,
                  public BufferImp<Graphic<BGCOLORMODE>>,
                  public IteratorImp<Graphic<BGCOLORMODE>> {
    using Color = typename Graphic<BGCOLORMODE>::Color;
    using bgmode = typename Color::bgmode;
    using Vram_t = typename bgmode::Vram_t;
    using Color_t = Vram_t;
    using Grange_t = typename Graphic<BGCOLORMODE>::Grange_t;
    using Iterator = typename Grange_t::Iterator;

    static constexpr const i32arm_t COL = bgmode::COL;
    static constexpr const i32arm_t ROW = bgmode::ROW;
};

#endif
