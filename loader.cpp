extern "C"  int main();

extern "C" void _start() __attribute__((section(".text.startup")));

extern "C"
void _start()
{
	main();
	asm("swi 03");
}
