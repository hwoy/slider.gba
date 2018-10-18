extern int main();

void _start() __attribute__((section(".text.startup")));

void _start()
{
	main();
	
	while(1);
}
