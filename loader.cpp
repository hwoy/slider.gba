extern "C"  int main();

extern "C"
void _start()
{
	main();
	asm("swi 03");
}
