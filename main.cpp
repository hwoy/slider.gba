#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Number.hpp"
#include "Square.hpp"

#define WIDTH 50
#define IWIDTH 20

#define CGAP ((GraphicDevice::COL-(WIDTH*3))/4)
#define RGAP ((GraphicDevice::ROW-(WIDTH*3))/4)

static constexpr const Color BOXCOLOR  {31,0,0};
static constexpr const Color IBOXCOLOR {0,31,0};
static constexpr const Color NUMCOLOR  {0,0,31};

static constexpr const Color BGCOLOR   {0,0,0};

extern "C"
int main()
{
	Graphic g(GraphicDevice{});
	g.setbgcolor(BGCOLOR);

	const Square square{WIDTH,IWIDTH,BOXCOLOR,IBOXCOLOR,NUMCOLOR};

	for(u8arm_t i=0,rgap=RGAP,num=1;i<3;++i,rgap+=(RGAP+square.width))
		for(u8arm_t j=0,cgap=CGAP;j<3;++j,cgap+=(CGAP+square.width),++num)
			square.print(g,{cgap,rgap},num);

	return 0;
}
