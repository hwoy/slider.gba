#ifndef __KEYPAD_HPP__
#define __KEYPAD_HPP__

#include <utility>

static constexpr const u32arm_t KEYS = 0x04000130;

struct KeypadDevice {
    using Kp_t = u16arm_t;
    using PtrKp_t = volatile Kp_t*;

    static inline constexpr volatile Kp_t& refkp(void)
    {
        return *reinterpret_cast<PtrKp_t>(KEYS);
    }

    static inline constexpr PtrKp_t ptrkp(void)
    {
        return reinterpret_cast<PtrKp_t>(KEYS);
    }
};

template <class KD>
struct Keypad {
    using Device = KD;
    using Kp_t = typename Device::Kp_t;
    using PtrKp_t = typename Device::PtrKp_t;

    struct Key {
        enum KEY : Kp_t {
            KEY_A = 1,
            KEY_B = 2,
            KEY_SELECT = 4,
            KEY_START = 8,
            KEY_RIGHT = 16,
            KEY_LEFT = 32,
            KEY_UP = 64,
            KEY_DOWN = 128,
            KEY_R = 256,
            KEY_L = 512,
            KEY_ALL = 0x03ff
        };

        const KEY key;

        inline constexpr Key(KEY key)
            : key(key)
        {
        }

        inline constexpr bool operator==(KEY key) const { return this->key == key; }

        inline constexpr bool operator!=(KEY key) const { return !(*this == key); }

        inline constexpr bool operator==(const Key& key) const
        {
            return *this == key.key;
        }

        inline constexpr bool operator!=(const Key& key) const
        {
            return *this != key.key;
        }
    };

    struct Keyevent {
        enum EVENT : u16arm_t {
            EVENT_NONE,
            EVENT_DOWN,
            EVENT_HOLD,
            EVENT_UP,
            EVENT_INVALID
        };

        const EVENT event;

        inline constexpr Keyevent(EVENT event)
            : event(event)
        {
        }

        inline constexpr bool operator==(EVENT e) const { return event == e; }

        inline constexpr bool operator!=(EVENT e) const { return event != e; }
    };

    inline static constexpr auto defaultkeyfunc(i32arm_t ret)
    {
        return [ret](const Key&) -> i32arm_t { return ret; };
    }

    typename Key::KEY lastkey;

    inline constexpr Keypad()
        : lastkey(Key::KEY_ALL)
    {
    }

    inline static constexpr typename Key::KEY tokey(const typename Key::KEY key)
    {
        return static_cast<typename Key::KEY>(~((key | ~Key::KEY_ALL) & -1));
    }

    template <class DOWN, class HOLD, class UP, class NONE>
    i32arm_t dispatch(DOWN keydownfunc, HOLD keyholdfunc, UP keyupfunc,
        NONE keynonefunc)
    {
        volatile const auto press = static_cast<typename Key::KEY>(KD::refkp());

        if ((press & Key::KEY_ALL) != Key::KEY_ALL) {
            if (lastkey != press)
                return keydownfunc(tokey(lastkey = press));
            else
                return keyholdfunc(tokey(lastkey));
        }

        else if (lastkey != Key::KEY_ALL) {
            auto const key = lastkey;
            lastkey = Key::KEY_ALL;

            return keyupfunc(tokey(key));
        }

        return keynonefunc(tokey(Key::KEY_ALL));
    }

    const std::pair<Keyevent, Key> message(void)
    {
        volatile const auto press = static_cast<typename Key::KEY>(KD::refkp());

        auto event = Keyevent::EVENT_NONE;

        auto key = Key::KEY_ALL;

        if ((press & Key::KEY_ALL) != Key::KEY_ALL) {
            if (lastkey != press) {
                event = Keyevent::EVENT_DOWN;
                key = lastkey = press;
            } else {
                event = Keyevent::EVENT_HOLD;
                key = lastkey;
            }
        }

        else if (lastkey != Key::KEY_ALL) {
            event = Keyevent::EVENT_UP;
            key = lastkey;

            lastkey = Key::KEY_ALL;
        } else {
            lastkey = key;
        }

        return {
            event, static_cast<typename Key::KEY>(~((key | ~Key::KEY_ALL) & -1))
        };
    }
};

#endif
