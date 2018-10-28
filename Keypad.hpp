#ifndef __KEYPAD_HPP__
#define __KEYPAD_HPP__

#include <utility>

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

    struct Key
    {
        enum KEY:Kp_t
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

        const Kp_t key;

    inline constexpr Key(Kp_t key):key(key) {}

    inline constexpr bool operator == (Kp_t key) const
    {
        return !(this->key & key);
    }

    inline constexpr bool operator != (Kp_t key) const
    {
        return (this->key & key);
    }

    inline constexpr bool operator == (const Key &key) const
    {
        return *this == key.key;
    }

    inline constexpr bool operator != (const Key &key) const
    {
        return *this != key.key;
    }

    };

    struct Keyevent
    {
        enum EVENT:u16arm_t
        {
            EVENT_NONE,
            EVENT_DOWN,
            EVENT_HOLD,
            EVENT_UP,
            EVENT_INVALID
        };
        const u16arm_t event;
       inline constexpr Keyevent(u16arm_t event):event(event) {}

    inline constexpr bool operator == (u16arm_t e) const
    {
        return event == e;
    }

    inline constexpr bool operator != (u16arm_t e) const
    {
        return event != e;
    }

    };


    Kp_t lastkey;

    inline constexpr Keypad():lastkey(Key::KEY_ALL){ }

    std::pair<Keyevent,Key> message(void)
    {
        volatile const auto press=KD::refkp();

        if((press & Key::KEY_ALL) != Key::KEY_ALL)
        {
            if(lastkey != press)
            {
                return std::make_pair(Keyevent::EVENT_DOWN,lastkey=press);
            }
            
            return std::make_pair(Keyevent::EVENT_HOLD,lastkey);
        }

        else if(lastkey!=Key::KEY_ALL)
        {
                auto tmp = lastkey;
                lastkey=Key::KEY_ALL;
                return std::make_pair(Keyevent::EVENT_UP,tmp);
        }

        return std::make_pair(Keyevent::EVENT_NONE,lastkey=Key::KEY_ALL);
    }
};

#endif
