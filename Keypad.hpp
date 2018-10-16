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
        KEY_ALL= 0x03ff
    };

    KeypadDevice kd;
    u16arm_t lastkey;

    explicit inline Keypad(const KeypadDevice &kd):kd(kd),lastkey(KEY_ALL){ }

    u16arm_t untilkeypressDownUp()
    {
        volatile u16arm_t tmp;

        while(( (tmp=*kd) & KEY_ALL) == KEY_ALL ){} //Press Down

        lastkey=tmp;

        while( ((*kd) &KEY_ALL ) != KEY_ALL){} //Press Up

		return lastkey;
    }

    u16arm_t untilkeypressDown()
    {
        volatile u16arm_t tmp;

        while(( (tmp=*kd) & KEY_ALL) == KEY_ALL ){} //Press Down

        lastkey=tmp;


		return lastkey;
    }

    u16arm_t untilkeypressUp()
    {
        volatile u16arm_t tmp=*kd;

        while( ((*kd) &KEY_ALL ) != KEY_ALL){} //Press Up

        return lastkey=tmp;
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

};
