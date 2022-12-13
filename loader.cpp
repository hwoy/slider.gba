
extern "C" [[noreturn]] void main();

extern "C" [[noreturn]] void _start()
{
    main();

    while (true)
        ;
}
