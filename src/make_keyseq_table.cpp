#define Uses_TKeys

#include "umba/umba.h"
//
//#include "umba/tokenizer/trie_builder.h"
#include "umba/tokenizer.h"
#include "umba/char_class.h"
#include "umba/c_tokenizer.h"

//
#include "tvision/tv.h"

// 
#include "tvision/ttypes.h"
//
#include "tvision/tkeys.h"

// #include "tvision/umba/c_tokenizer.h"

#include <iostream>
#include <vector>



// umba::tokenizer::generation::

umba::tokenizer::TrieBuilder  trieBuilder;


    // TrieNode& addTokenSequence(const std::string &seqStr, payload_type payload)
    // TrieNode& addTokenSequence(const char* seqStr, payload_type payload)



using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{
    using umba::tokenizer::payload_type;

    // ESC - 0x1B
    trieBuilder.addTokenSequence("\x1B", (payload_type)kbEsc)   ;

    trieBuilder.addTokenSequence("\x1B[11~", (payload_type)kbF1 );
    trieBuilder.addTokenSequence("\x1B[12~", (payload_type)kbF2 );
    trieBuilder.addTokenSequence("\x1B[13~", (payload_type)kbF3 );
    trieBuilder.addTokenSequence("\x1B[14~", (payload_type)kbF4 );
    trieBuilder.addTokenSequence("\x1B[15~", (payload_type)kbF5 );
    trieBuilder.addTokenSequence("\x1B[17~", (payload_type)kbF6 );
    trieBuilder.addTokenSequence("\x1B[18~", (payload_type)kbF7 );
    trieBuilder.addTokenSequence("\x1B[19~", (payload_type)kbF8 );
    trieBuilder.addTokenSequence("\x1B[20~", (payload_type)kbF9 );
    trieBuilder.addTokenSequence("\x1B[21~", (payload_type)kbF10);
    trieBuilder.addTokenSequence("\x1B[23~", (payload_type)kbF11);
    trieBuilder.addTokenSequence("\x1B[24~", (payload_type)kbF12);


    trieBuilder.addTokenSequence("\x1B[23~", (payload_type)kbShiftF1 );
    trieBuilder.addTokenSequence("\x1B[24~", (payload_type)kbShiftF2 );
    trieBuilder.addTokenSequence("\x1B[25~", (payload_type)kbShiftF3 );
    trieBuilder.addTokenSequence("\x1B[26~", (payload_type)kbShiftF4 );
    trieBuilder.addTokenSequence("\x1B[28~", (payload_type)kbShiftF5 );
    trieBuilder.addTokenSequence("\x1B[29~", (payload_type)kbShiftF6 );
    trieBuilder.addTokenSequence("\x1B[31~", (payload_type)kbShiftF7 );
    trieBuilder.addTokenSequence("\x1B[32~", (payload_type)kbShiftF8 );
    trieBuilder.addTokenSequence("\x1B[33~", (payload_type)kbShiftF9 );
    trieBuilder.addTokenSequence("\x1B[34~", (payload_type)kbShiftF10);
    trieBuilder.addTokenSequence("\x1B[23~", (payload_type)kbShiftF11);
    trieBuilder.addTokenSequence("\x1B[24~", (payload_type)kbShiftF12);

    trieBuilder.addTokenSequence("\x1B\x1B[11~", (payload_type)kbAltF1 );
    trieBuilder.addTokenSequence("\x1B\x1B[12~", (payload_type)kbAltF2 );
    trieBuilder.addTokenSequence("\x1B\x1B[13~", (payload_type)kbAltF3 );
    trieBuilder.addTokenSequence("\x1B\x1B[14~", (payload_type)kbAltF4 );
    trieBuilder.addTokenSequence("\x1B\x1B[15~", (payload_type)kbAltF5 );
    trieBuilder.addTokenSequence("\x1B\x1B[17~", (payload_type)kbAltF6 );
    trieBuilder.addTokenSequence("\x1B\x1B[18~", (payload_type)kbAltF7 );
    trieBuilder.addTokenSequence("\x1B\x1B[19~", (payload_type)kbAltF8 );
    trieBuilder.addTokenSequence("\x1B\x1B[20~", (payload_type)kbAltF9 );
    trieBuilder.addTokenSequence("\x1B\x1B[21~", (payload_type)kbAltF10);
    trieBuilder.addTokenSequence("\x1B\x1B[23~", (payload_type)kbAltF11);
    trieBuilder.addTokenSequence("\x1B\x1B[24~", (payload_type)kbAltF12);

    // // Alt+Shift not supported currently
    // trieBuilder.addTokenSequence("\x1B\x1B[23~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[24~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[25~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[26~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[28~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[29~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[31~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[32~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[33~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[34~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[23~", (payload_type));
    // trieBuilder.addTokenSequence("\x1B\x1B[24~", (payload_type));

#if 0
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
    trieBuilder.addTokenSequence("\x1B", (payload_type));
#endif

    std::vector<umba::tokenizer::TrieNode> trie;

    trieBuilder.buildTokenTrie(trie);


    return 0;
}



#if 0



THardwareInfo::getTickCount();

system.h

struct TEvent
{
    ushort what;  // ev.what = evKeyDown;
    union
    {
        MouseEventType mouse;
        KeyDownEvent keyDown;
        MessageEvent message;
    };

    void getMouseEvent() noexcept;
    void getKeyEvent() noexcept;
    static void waitForEvent(int timeoutMs) noexcept;
    static void putNothing() noexcept;
};


struct CharScanType
{
#if !defined( TV_BIG_ENDIAN ) // LITTLE_ENDIAN
    uchar charCode;             // KeyEvent.uChar.AsciiChar            - ASCII код клавиши - младшая часть uint16
    uchar scanCode;             // KEY_EVENT_RECORD.wVirtualScanCode;  - скан-код
#else
    // Due to the reverse byte order, swap the fields in order to preserve
    // the aliasing with KeyDownEvent::keyCode.
    uchar scanCode;
    uchar charCode;
#endif
};

/* Turbo Vision stores key events in a KeyDownEvent struct, defined in
 * system.h. Its first field is a key code (16 bit), which can be decomposed
 * into the ASCII equivalent (lower byte) and a scan code (higher byte).
 * It has a second field with the state of the modifier keys, which can be
 * retrieved by performing a bit-wise AND with the kbShift, kbCtrlShift and
 * kbAltShift bitmasks. Turbo Vision expects this field to be filled even
 * if the key code is already named Shift/Ctrl/Alt+something. */

/* The support for key combinations is the following:
   - PrintScreen, Break are not likely to be captured by the terminal, but
     Ctrl+C could be used as a replacement of the Ctrl+Break interrupt.
   - Ctrl/Alt+F(n) don't work on the linux console and I strongly advice against
     using them.
   - Ctrl+Letter works, except for ^H, ^I, ^J and ^M, which have a special
     meaning.
   - Alt+Letter/Number seem to work quite well.
   - Ctrl+Backspace/Enter can't be recognized on terminal emulators.
   - Shift/Ctrl+Ins/Del/Home/End/PgDn/PgUp seem to work, too.
   - Arrow keys work, as well as combined with Shift, but Turbo Vision doesn't
     support Ctrl+Up/Down (EDIT: it now does).
   - Tab and Backtab are supported too, although the linux console confuses the
     latter with Alt+Tab.
   - Some other key combinations are supported on terminal but not in Turbo Vision.
 * Still, it's up to your luck that ncurses manages to grab any of these
 * combinations from your terminal application. */

Скан-код - https://ru.wikipedia.org/wiki/%D0%A1%D0%BA%D0%B0%D0%BD-%D0%BA%D0%BE%D0%B4
https://snipp.ru/handbk/scan-codes

tkeys.h

ncursinp.cpp - таблицы 
static constexpr KeyDownEvent fromNonPrintableAscii[32]
static const const_unordered_map<ushort, KeyDownEvent> fromCursesKeyCode =

far2l.cpp
static const const_unordered_map<uchar, ushort> virtualKeyCodeToKeyCode =
static const const_unordered_map<uchar, uchar> virtualKeyCodeToScanCode =

terminal.cpp
static const const_unordered_map<ushort, constarray<ushort, 3>> moddedKeyCodes =


__FLAT__ - определено для любых 32+ как я понял, и не определено для 16 битных программ

compat/windows/windows.h

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12

#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5



#define RIGHT_ALT_PRESSED     0x0001 // the right alt key is pressed.
#define LEFT_ALT_PRESSED      0x0002 // the left alt key is pressed.
#define RIGHT_CTRL_PRESSED    0x0004 // the right ctrl key is pressed.
#define LEFT_CTRL_PRESSED     0x0008 // the left ctrl key is pressed.
#define SHIFT_PRESSED         0x0010 // the shift key is pressed.
#define NUMLOCK_ON            0x0020 // the numlock light is on.
#define SCROLLLOCK_ON         0x0040 // the scrolllock light is on.
#define CAPSLOCK_ON           0x0080 // the capslock light is on.
#define ENHANCED_KEY          0x0100 // the key is enhanced.


    kbLeftShift   = SHIFT_PRESSED,
    kbRightShift  = SHIFT_PRESSED,
    kbShift       = kbLeftShift | kbRightShift,
    kbLeftCtrl    = LEFT_CTRL_PRESSED,
    kbRightCtrl   = RIGHT_CTRL_PRESSED,
    kbCtrlShift   = kbLeftCtrl | kbRightCtrl,
    kbLeftAlt     = LEFT_ALT_PRESSED,
    kbRightAlt    = RIGHT_ALT_PRESSED,
    kbAltShift    = kbLeftAlt | kbRightAlt,
    kbScrollState = SCROLLLOCK_ON,
    kbNumState    = NUMLOCK_ON,
    kbCapsState   = CAPSLOCK_ON,
    kbEnhanced    = ENHANCED_KEY,
    kbInsState    = 0x200,  // Ensure this doesn't overlap above values
    kbPaste       = 0x400;


struct KeyDownEvent
{
    union
        {
        ushort keyCode;
        CharScanType charScan;
        };
    ushort controlKeyState;     // KeyEvent.dwControlKeyState & (kbShift | kbCtrlShift | kbAltShift | kbScrollState | kbNumState | kbCapsState | kbEnhanced );
    char text[4];               // NOT null-terminated.
    uchar textLength;

    TStringView getText() const;
    operator TKey() const;
};


kbCtrlZ     = 0x001a

        if (res & (1 << KG_SHIFT))
            actualModifiers |= kbShift;
        if (res & (1 << KG_CTRL))
            actualModifiers |= kbLeftCtrl;
        if (res & (1 << KG_ALT))
            actualModifiers |= kbLeftAlt;


bool getWin32Key(const KEY_EVENT_RECORD &KeyEvent, TEvent &ev, InputState &state) noexcept
{
    if (!getWin32KeyText(KeyEvent, ev, state))
        return false;

    ev.what = evKeyDown;
    ev.keyDown.charScan.scanCode = KeyEvent.wVirtualScanCode;
    ev.keyDown.charScan.charCode = KeyEvent.uChar.AsciiChar;
    ev.keyDown.controlKeyState = KeyEvent.dwControlKeyState & (
        kbShift | kbCtrlShift | kbAltShift |
        kbScrollState | kbNumState | kbCapsState | kbEnhanced
    );

    if (ev.keyDown.textLength)
    {
        ev.keyDown.charScan.charCode = CpTranslator::fromUtf8(ev.keyDown.getText());
        if (KeyEvent.wVirtualKeyCode == VK_MENU)
            // This is enabled when pasting certain characters, and it confuses
            // applications. Clear it.
            ev.keyDown.charScan.scanCode = 0;
        if (!ev.keyDown.charScan.charCode || ev.keyDown.keyCode <= kbCtrlZ)
            // If the character cannot be represented in the current codepage,
            // or if it would accidentally trigger a Ctrl+Key combination,
            // make the whole keyCode zero to avoid side effects.
            ev.keyDown.keyCode = kbNoKey;
    }

    if ( ev.keyDown.keyCode == 0x2A00 || ev.keyDown.keyCode == 0x1D00 ||
         ev.keyDown.keyCode == 0x3600 || ev.keyDown.keyCode == 0x3800 ||
         ev.keyDown.keyCode == 0x3A00 )
        // Discard standalone Shift, Ctrl, Alt, Caps Lock keys.
        ev.keyDown.keyCode = kbNoKey;
    else if ( (ev.keyDown.controlKeyState & kbCtrlShift) &&
              (ev.keyDown.controlKeyState & kbAltShift) ) // Ctrl+Alt is AltGr.
    {
        // When AltGr+Key does not produce a character, a
        // keyCode with unwanted effects may be read instead.
        if (!ev.keyDown.textLength)
            ev.keyDown.keyCode = kbNoKey;
    }
    else if (KeyEvent.wVirtualScanCode < 89)
    {
        // Convert NT style virtual scan codes to PC BIOS codes.
        uchar index = KeyEvent.wVirtualScanCode;
        ushort keyCode = 0;
        if ((ev.keyDown.controlKeyState & kbAltShift) && THardwareInfo::AltCvt[index])
            keyCode = THardwareInfo::AltCvt[index];
        else if ((ev.keyDown.controlKeyState & kbCtrlShift) && THardwareInfo::CtrlCvt[index])
            keyCode = THardwareInfo::CtrlCvt[index];
        else if ((ev.keyDown.controlKeyState & kbShift) && THardwareInfo::ShiftCvt[index])
            keyCode = THardwareInfo::ShiftCvt[index];
        else if ( !(ev.keyDown.controlKeyState & (kbShift | kbCtrlShift | kbAltShift)) &&
                  THardwareInfo::NormalCvt[index] )
            keyCode = THardwareInfo::NormalCvt[index];

        if (keyCode != 0)
        {
            ev.keyDown.keyCode = keyCode;
            if (ev.keyDown.charScan.charCode < ' ')
                ev.keyDown.textLength = 0;
            else if (ev.keyDown.charScan.charCode < 0x7F && !ev.keyDown.textLength)
            {
                ev.keyDown.text[0] = ev.keyDown.charScan.charCode;
                ev.keyDown.textLength = 1;
            }
        }
    }

    return ev.keyDown.keyCode != kbNoKey || ev.keyDown.textLength;
}



#endif

