#define  KEYS  ((volatile u16arm_t *)0x04000130)

struct KeypadDevice
{
    volatile u16arm_t *keys;

    inline constexpr KeypadDevice(volatile u16arm_t *keys=KEYS):keys(keys){}

    inline volatile u16arm_t &operator *()
    {
        return *keys;
    }

    inline const volatile u16arm_t &operator *() const
    {
        return *keys;
    }

};

struct Keypad
{
    enum KEY:u16arm_t
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
        KEY_L = 512,
        KEY_INVALID = 0x3ff
    };

    KeypadDevice kd;
    u16arm_t lastkey;

    explicit inline Keypad(const KeypadDevice &kd):kd(kd),lastkey(KEY_INVALID){ }

    inline u16arm_t untilkeypress()
    {
        volatile u16arm_t tmp;

        while( (tmp=*kd) == KEY_INVALID);

        *kd=KEY_INVALID;
        lastkey=tmp;

		return lastkey;
    }
	
	inline bool ispress(u16arm_t key) const
	{
		return !(lastkey & key);
	}

    inline bool operator == (u16arm_t key) const
    {
        return ispress(key);
    }

    inline bool operator != (u16arm_t key) const
    {
        return !ispress(key);
    }

    inline explicit operator bool ()
    {
        return untilkeypress();
    }

};
