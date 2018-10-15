#include "arm7type.hpp"
#include "Graphic.hpp"
#include "Keypad.hpp"
#include "Number.hpp"
#include "Square.hpp"

#define WIDTH 50
#define IWIDTH 20

#define RGAP ((GraphicDevice::ROW-(WIDTH*3))/4)
#define CGAP RGAP

#define FCGAP  ((GraphicDevice::COL-(CGAP*2)-(WIDTH*3))/2)
#define FRGAP  ((GraphicDevice::ROW-(RGAP*2)-(WIDTH*3))/2)

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

	for(u8arm_t i=0,rgap=FRGAP,num=1;i<3;++i,rgap+=(RGAP+square.width))
		for(u8arm_t j=0,cgap=FCGAP;j<3;++j,cgap+=(CGAP+square.width),++num)
			square.print(g,{cgap,rgap},num);

	return 0;
}
