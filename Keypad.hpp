#define  KEYS  ((volatile u32arm_t *)0x04000130)

struct KeypadDevice
{
    volatile u32arm_t *keys;

    inline constexpr KeypadDevice(volatile u32arm_t *keys=KEYS):keys(keys){}

    inline volatile u32arm_t &operator *()
    {
        return *keys;
    }

    inline const volatile u32arm_t &operator *() const
    {
        return *keys;
    }

};

struct Keypad
{
    KeypadDevice kd;

    explicit inline constexpr Keypad(const KeypadDevice &kd):kd(kd){}

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
    inline bool iskey(u32arm_t key) const { return !(*kd & key); }
    inline void reset() {*kd=-1;}


};
