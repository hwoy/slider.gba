extern "C"  int main();

extern "C"
void start()
{
	main();
	asm("swi 03");
}
