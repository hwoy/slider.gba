static constexpr const u32arm_t KEYS = 0x04000130;

struct KeypadDevice
{
	using Kp_t = u16arm_t;
	using PtrKp_t = volatile Kp_t *;

	static inline constexpr volatile Kp_t & refkp(void)
	{
		return *reinterpret_cast<PtrKp_t>(KEYS);
	}

	static inline constexpr PtrKp_t ptrkp(void)
	{
		return reinterpret_cast<PtrKp_t>(KEYS);
	}

};

template<class KD>
struct Keypad
{
    using Kp_t = typename KD::Kp_t;

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

    Kp_t lastkey;

    inline constexpr Keypad():lastkey(KEY_ALL){ }

    Kp_t untilkeypressDownUp()
    {
        volatile Kp_t tmp;

        while(( (tmp=KD::refkp()) & KEY_ALL) == KEY_ALL ){} //Press Down

        lastkey=tmp;

        while( ((KD::refkp()) &KEY_ALL ) != KEY_ALL){} //Press Up

		return lastkey;
    }

    Kp_t untilkeypressDown()
    {
        volatile Kp_t tmp;

        while(( (tmp=KD::refkp()) & KEY_ALL) == KEY_ALL ){} //Press Down

        lastkey=tmp;


		return lastkey;
    }

    Kp_t untilkeypressUp()
    {
        volatile Kp_t tmp=KD::refkp();

        while( ((KD::refkp()) &KEY_ALL ) != KEY_ALL){} //Press Up

        return lastkey=tmp;
    }
	
	inline constexpr bool ispress(Kp_t key) const
	{
		return !(lastkey & key);
	}

    inline constexpr bool operator == (Kp_t key) const
    {
        return ispress(key);
    }

    inline constexpr bool operator != (Kp_t key) const
    {
        return !ispress(key);
    }

};
