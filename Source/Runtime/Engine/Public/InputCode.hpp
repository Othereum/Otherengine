#pragma once

namespace oeng::engine
{
	enum class CtrlAxis { LX, LY, RX, RY, LT, RT };
	enum class CtrlBtn
	{
	    A, B, X, Y,
	    BACK, GUIDE, START,
	    LS, RS,
	    LB, RB,
	    UP, DOWN, LEFT, RIGHT
	};

	
	enum class MouseBtn { L = 1, M, R, X1, X2 };
	enum class MouseAxis { X, Y };

	constexpr uint32_t MouseMask(MouseBtn btn) noexcept
	{
		return 1u << (uint32_t(btn) - 1u);
	}

	
	enum class KeyMod
	{
	    NONE = 0x0000,
	    SHIFT = 0x0003,
	    CTRL = 0x00C0,
	    ALT = 0x0300,
	    GUI = 0x0C00,
	    NUM = 0x1000,
	    CAPS = 0x2000,
	    MODE = 0x4000
	};

	constexpr KeyMod operator|(KeyMod a, KeyMod b) noexcept
	{
		return KeyMod(static_cast<int>(a) | static_cast<int>(b));
	}

	constexpr KeyMod operator&(KeyMod a, KeyMod b) noexcept
	{
		return KeyMod(static_cast<int>(a) & static_cast<int>(b));
	}
	
	constexpr KeyMod& operator|=(KeyMod& a, KeyMod b) noexcept { return a = a | b; }
	constexpr KeyMod& operator&=(KeyMod& a, KeyMod b) noexcept { return a = a & b; }

	
	constexpr int ScanToKey(int scan) noexcept
	{
		return (1 << 30) | scan;
	}

	enum class Keycode
	{
	    RETURN = '\r',
	    ESCAPE = '\033',
	    BACKSPACE = '\b',
	    TAB = '\t',
		
	    SPACE = ' ',
	    EXCLAIM = '!',
	    QUOTE_DBL = '"',
	    HASH = '#',
	    DOLLAR = '$',
	    PERCENT = '%',
	    AMPERSAND = '&',
	    QUOTE = '\'',
	    LEFT_PAREN = '(',
	    RIGHT_PAREN = ')',
	    ASTERISK = '*',
	    PLUS = '+',
	    COMMA = ',',
	    MINUS = '-',
	    PERIOD = '.',
	    SLASH = '/',
	    ZERO = '0',
	    ONE = '1',
	    TWO = '2',
	    THREE = '3',
	    FOUR = '4',
	    FIVE = '5',
	    SIX = '6',
	    SEVEN = '7',
	    EIGHT = '8',
	    NINE = '9',
	    COLON = ':',
	    SEMICOLON = ';',
	    LESS = '<',
	    EQUALS = '=',
	    GREATER = '>',
	    QUESTION = '?',
	    AT = '@',
		
		LEFT_BRACKET = '[',
	    BACKSLASH = '\\',
	    RIGHT_BRACKET = ']',
	    CARET = '^',
	    UNDERSCORE = '_',
	    BACK_QUOTE = '`',
		
	    A = 'a',
	    B = 'b',
	    C = 'c',
	    D = 'd',
	    E = 'e',
	    F = 'f',
	    G = 'g',
	    H = 'h',
	    I = 'i',
	    J = 'j',
	    K = 'k',
	    L = 'l',
	    M = 'm',
	    N = 'n',
	    O = 'o',
	    P = 'p',
	    Q = 'q',
	    R = 'r',
	    S = 's',
	    T = 't',
	    U = 'u',
	    V = 'v',
	    W = 'w',
	    X = 'x',
	    Y = 'y',
	    Z = 'z',

	    CAPS_LOCK = ScanToKey(57),

	    F1,
	    F2,
	    F3,
	    F4,
	    F5,
	    F6,
	    F7,
	    F8,
	    F9,
	    F10,
	    F11,
	    F12,

	    PRINT_SCREEN,
	    SCROLL_LOCK,
	    PAUSE,
	    INSERT,
	    HOME,
	    PAGE_UP,
	    DELETE = '\177',
	    END = ScanToKey(77),
	    PAGE_DOWN,
	    RIGHT,
	    LEFT,
	    DOWN,
	    UP,

	    NUM_LOCK_CLEAR,
	    KP_DIVIDE,
	    KP_MULTIPLY,
	    KP_MINUS,
	    KP_PLUS,
	    KP_ENTER,
	    KP_1,
	    KP_2,
	    KP_3,
	    KP_4,
	    KP_5,
	    KP_6,
	    KP_7,
	    KP_8,
	    KP_9,
	    KP_0,
	    KP_PERIOD,

	    APPLICATION = ScanToKey(101),
	    POWER,
	    KP_EQUALS,
	    F13,
	    F14,
	    F15,
	    F16,
	    F17,
	    F18,
	    F19,
	    F20,
	    F21,
	    F22,
	    F23,
	    F24,
	    EXECUTE,
	    HELP,
	    MENU,
	    SELECT,
	    STOP,
	    AGAIN,
	    UNDO,
	    CUT,
	    COPY,
	    PASTE,
	    FIND,
	    MUTE,
	    VOLUME_UP,
	    VOLUME_DOWN,
	    KP_COMMA = ScanToKey(133),
	    KP_EQUALS_AS400,

	    ALT_ERASE = ScanToKey(153),
	    SYS_REQ,
	    CANCEL,
	    CLEAR,
	    PRIOR,
	    RETURN2,
	    SEPARATOR,
	    OUT,
	    OPER,
	    CLEAR_AGAIN,
	    CRSEL,
	    EXSEL,

	    KP_00 = ScanToKey(176),
	    KP_000,
	    THOUSANDS_SEPARATOR,
	    DECIMAL_SEPARATOR,
	    CURRENCY_UNIT,
	    CURRENCY_SUBUNIT,
	    KP_LEFT_PAREN,
	    KP_RIGHT_PAREN,
	    KP_LEFT_BRACE,
	    KP_RIGHT_BRACE,
	    KP_TAB,
	    KP_BACKSPACE,
	    KP_A,
	    KP_B,
	    KP_C,
	    KP_D,
	    KP_E,
	    KP_F,
	    KP_XOR,
	    KP_POWER,
	    KP_PERCENT,
	    KP_LESS,
	    KP_GREATER,
	    KP_AMPERSAND,
	    KP_DBL_AMPERSAND,
	    KP_VERTICAL_BAR,
	    KP_DBL_VERTICAL_BAR,
	    KP_COLON,
	    KP_HASH,
	    KP_SPACE,
	    KP_AT,
	    KP_EXCLAM,
	    KP_MEM_STORE,
	    KP_MEM_RECALL,
	    KP_MEM_CLEAR,
	    KP_MEM_ADD,
	    KP_MEM_SUBTRACT,
	    KP_MEM_MULTIPLY,
	    KP_MEM_DIVIDE,
	    KP_PLUS_MINUS,
	    KP_CLEAR,
	    KP_CLEAR_ENTRY,
	    KP_BINARY,
	    KP_OCTAL,
	    KP_DECIMAL,
	    KP_HEXADECIMAL,

	    L_CTRL = ScanToKey(224),
	    L_SHIFT,
	    L_ALT,
	    L_GUI,
	    R_CTRL,
	    R_SHIFT,
	    R_ALT,
	    R_GUI,

	    MODE = ScanToKey(257),

	    AUDIO_NEXT,
	    AUDIO_PREV,
	    AUDIO_STOP,
	    AUDIO_PLAY,
	    AUDIO_MUTE,
	    MEDIA_SELECT,
	    WWW,
	    MAIL,
	    CALCULATOR,
	    COMPUTER,
	    AC_SEARCH,
	    AC_HOME,
	    AC_BACK,
	    AC_FORWARD,
	    AC_STOP,
	    AC_REFRESH,
	    AC_BOOKMARKS,

	    BRIGHTNESS_DOWN,
	    BRIGHTNESS_UP,
	    DISPLAY_SWITCH,
	    KBD_ILLUM_TOGGLE,
	    KBD_ILLUM_DOWN,
	    KBD_ILLUM_UP,
	    EJECT,
	    SLEEP,
	    APP1,
	    APP2,

	    AUDIO_REWIND,
	    AUDIO_FAST_FORWARD
	};

	
	ENGINE_API String8 GetName(Keycode btn) noexcept;
	ENGINE_API std::u8string_view GetName(CtrlAxis axis) noexcept;
	ENGINE_API std::u8string_view GetName(CtrlBtn btn) noexcept;
	ENGINE_API DyArr<std::u8string_view> GetNames(KeyMod mod) noexcept;
	
	ENGINE_API std::optional<Keycode> ToKeycode(std::u8string_view name) noexcept;
	ENGINE_API std::optional<CtrlAxis> ToCtrlAxis(std::u8string_view name) noexcept;
	ENGINE_API std::optional<CtrlBtn> ToCtrlBtn(std::u8string_view name) noexcept;
	ENGINE_API std::optional<MouseAxis> ToMouseAxis(std::u8string_view name) noexcept;
	ENGINE_API std::optional<MouseBtn> ToMouseBtn(std::u8string_view name) noexcept;
	ENGINE_API std::optional<KeyMod> ToKeyMod(std::u8string_view name) noexcept;

	constexpr std::u8string_view GetName(KeyMod mod) noexcept
	{
		switch (mod)
		{
		case KeyMod::SHIFT: return u8"Shift"sv;
		case KeyMod::CTRL: return u8"Ctrl"sv;
		case KeyMod::ALT: return u8"Alt"sv;
		case KeyMod::GUI: return u8"GUI"sv;
		case KeyMod::NUM: return u8"Numlock"sv;
		case KeyMod::CAPS: return u8"CapsLock"sv;
		case KeyMod::MODE: return u8"ModeSwitch"sv;
		default: return {};
		}
	}

	constexpr std::u8string_view GetName(MouseAxis axis) noexcept
	{
		switch (axis)
		{
		case MouseAxis::X: return u8"X"sv;
		case MouseAxis::Y: return u8"Y"sv;
		default: return {};
		}
	}

	constexpr std::u8string_view GetName(MouseBtn btn) noexcept
	{
		switch (btn)
		{
		case MouseBtn::L: return u8"L"sv;
		case MouseBtn::M: return u8"M"sv;
		case MouseBtn::R: return u8"R"sv;
		case MouseBtn::X1: return u8"X1"sv;
		case MouseBtn::X2: return u8"X2"sv;
		default: return {};
		}
	}
}
