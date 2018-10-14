#define  KEYS  ((volatile u32arm_t *)0x04000130)

struct Keypad
{
    volatile u32arm_t *keys;

    inline constexpr Keypad(volatile u32arm_t *keys=KEYS):keys(keys){}

    enum KEY:u32arm_t
    {
        KEY_A = 1,
        KEY_B = 2,
        KEY_SELECT = 4,
        KEY_START =  8,
        KEY_RIGHT = 16,
        KEY_LEFT = 32,
        KEY_UP = 64,
        KEY_DOWN = 128,
        KEY_R = 256,
        KEY_L = 512
    };
    inline bool iskey(u32arm_t key) const { return !(*keys & key); }
    inline void reset() {*keys=-1;}


};
