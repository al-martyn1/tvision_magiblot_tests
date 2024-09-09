#define Uses_TKeys

// #define UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_DISABLE
// #define UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_DISABLE
// UMBA_TOKENIZER_TRIE_INDEX_TYPE_COMPACT

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
#include <iomanip>
#include <array>



// umba::tokenizer::generation::

umba::tokenizer::TrieBuilder  trieBuilder;


    // TrieNode& addTokenSequence(const std::string &seqStr, payload_type payload)
    // TrieNode& addTokenSequence(const char* seqStr, payload_type payload)


// https://ru.wikipedia.org/wiki/ASCII





    // kbLeftShift   = SHIFT_PRESSED,
    // kbRightShift  = SHIFT_PRESSED,
    // kbShift       = kbLeftShift | kbRightShift,
    // kbLeftCtrl    = LEFT_CTRL_PRESSED,
    // kbRightCtrl   = RIGHT_CTRL_PRESSED,
    // kbCtrlShift   = kbLeftCtrl | kbRightCtrl,
    // kbLeftAlt     = LEFT_ALT_PRESSED,
    // kbRightAlt    = RIGHT_ALT_PRESSED,
    // kbAltShift    = kbLeftAlt | kbRightAlt,
    // kbScrollState = SCROLLLOCK_ON,
    // kbNumState    = NUMLOCK_ON,
    // kbCapsState   = CAPSLOCK_ON,
    // kbEnhanced    = ENHANCED_KEY,
    // kbInsState    = 0x200,  // Ensure this doesn't overlap above values
    // kbPaste       = 0x400;

// Ctrl+1 - ничего
// Ctrl+2 - 0x00
// Ctrl+3 - ESC
// Ctrl+4 - 0x1C
// Ctrl+5 - 0x1D
// Ctrl+6 - 0x1E
// Ctrl+7 - 0x1F
// Ctrl+8 - 0x7F
// Ctrl+9 - ничего
// Ctrl+- - 0x1F

struct TerminalAsciiCharInfo
{
   ushort          scanCode;        // std::uint8_t
   ushort          controlKeyState; // kbShift | kbLeftAlt | kbLeftCtrl
   ushort          remapKeyCode;

};

// TerminalAsciiCharInfo занимает 2+2+2 - 6 байт, 128*6=768, + плюс какое-то место (но небольшое) займет дополнительный код

// TrieNode - в минимальном варианте - занимает 12 байт
// На 128 символов + 128 символов с нажатым Alt (ESC-префикс) - 12*256=3072


inline
const std::array<TerminalAsciiCharInfo, 128>& getTerminalAsciiCharInfo()
{
    // static std::array<TerminalAsciiCharInfo, 2> _ { { { 0x00, 0 , 0 }, { 0x00, 0 , 0 } } };

    static std::array<TerminalAsciiCharInfo, 128> _ {
    //  Scan   Flags     Remap     Ascii Char/Key
    //  code
    { { 0x00,          0 ,       0 }  // 0x00  NUL  - null
    , { 0x1E, kbLeftCtrl , kbCtrlA }  // 0x01  SOH  - start of heading
    , { 0x30, kbLeftCtrl , kbCtrlB }  // 0x02  STX  - start of text
    , { 0x2E, kbLeftCtrl , kbCtrlC }  // 0x03  ETX  - end of text
    , { 0x20, kbLeftCtrl , kbCtrlD }  // 0x04  EOT  - end of transmission
    , { 0x12, kbLeftCtrl , kbCtrlE }  // 0x05  ENQ  - enquire
    , { 0x21, kbLeftCtrl , kbCtrlF }  // 0x06  ACK  - acknowledgement
    , { 0x22, kbLeftCtrl , kbCtrlG }  // 0x07  BEL  - bell
    , { 0x0E,    kbShift ,  kbBack }  // 0x08  BS   - backspace   kbCtrlH generates Backsace, Shift+Backspace generates backspace instead of 0x7F
    , { 0x0F,          0 ,       0 }  // 0x09  HT   - tabulation  kbCtrlI generates Tab, no way to distinguish it in terminal
    , { 0x24, kbLeftCtrl , kbCtrlJ }  // 0x0A  LF   - line feed, not used, allow it as kbCtrlJ
    , { 0x25, kbLeftCtrl , kbCtrlK }  // 0x0B  VT   - vertical tab
    , { 0x26, kbLeftCtrl , kbCtrlL }  // 0x0C  FF   - form feed
    , { 0x1C,          0 ,       0 }  // 0x0D  CR   - carriage return kbCtrlM generates CR, no way to distinguish it in terminal
    , { 0x31, kbLeftCtrl , kbCtrlN }  // 0x0E  SO   - shift out
    , { 0x18, kbLeftCtrl , kbCtrlO }  // 0x0F  SI   - shift in
    , { 0x19, kbLeftCtrl , kbCtrlP }  // 0x10  DLE  - data link escape
    , { 0x10, kbLeftCtrl , kbCtrlQ }  // 0x11  DC1  - device control 1
    , { 0x13, kbLeftCtrl , kbCtrlR }  // 0x12  DC2  - device control 2
    , { 0x1F, kbLeftCtrl , kbCtrlS }  // 0x13  DC3  - device control 3
    , { 0x14, kbLeftCtrl , kbCtrlT }  // 0x14  DC4  - device control 4
    , { 0x16, kbLeftCtrl , kbCtrlU }  // 0x15  NAK  - negative acknowledgment
    , { 0x2F, kbLeftCtrl , kbCtrlV }  // 0x16  SYN  - synchronization
    , { 0x11, kbLeftCtrl , kbCtrlW }  // 0x17  ETB  - end of text block
    , { 0x2D, kbLeftCtrl , kbCtrlX }  // 0x18  CAN  - cancel
    , { 0x15, kbLeftCtrl , kbCtrlY }  // 0x19  EM   - end of medium
    , { 0x2C, kbLeftCtrl , kbCtrlZ }  // 0x1A  SUB  - substitute
    , { 0x01,          0 ,       0 }  // 0x1B  ESC  - escape
    , { 0x00,          0 ,       0 }  // 0x1C  FS   - file separator
    , { 0x00,          0 ,       0 }  // 0x1D  GS   - group separator
    , { 0x00,          0 ,       0 }  // 0x1E  RS   - record separator
    , { 0x00,          0 ,       0 }  // 0x1F  US   - unit separator    kbCtrlMinus?
    , { 0x39,          0 ,       0 }  // 0x20  space
    , { 0x02,    kbShift ,       0 }  // 0x21  ! + Shift
    , { 0x28,    kbShift ,       0 }  // 0x22  " + Shift
    , { 0x04,    kbShift ,       0 }  // 0x23  # + Shift
    , { 0x05,    kbShift ,       0 }  // 0x24  $ + Shift
    , { 0x06,    kbShift ,       0 }  // 0x25  % + Shift
    , { 0x08,    kbShift ,       0 }  // 0x26  & + Shift
    , { 0x28,    kbShift ,       0 }  // 0x27  ' + Shift
    , { 0x0A,    kbShift ,       0 }  // 0x28  ( + Shift
    , { 0x0B,    kbShift ,       0 }  // 0x29  ) + Shift
    , { 0x09,    kbShift ,       0 }  // 0x2a  * + Shift
    , { 0x0D,    kbShift ,       0 }  // 0x2b  + + Shift
    , { 0x33,          0 ,       0 }  // 0x2c  ,
    , { 0x0C,          0 ,       0 }  // 0x2d  -
    , { 0x34,          0 ,       0 }  // 0x2e  .
    , { 0x35,          0 ,       0 }  // 0x2f  /
    , { 0x0B,          0 ,       0 }  // 0x30  0
    , { 0x02,          0 ,       0 }  // 0x31  1
    , { 0x03,          0 ,       0 }  // 0x32  2
    , { 0x04,          0 ,       0 }  // 0x33  3
    , { 0x05,          0 ,       0 }  // 0x34  4
    , { 0x06,          0 ,       0 }  // 0x35  5
    , { 0x07,          0 ,       0 }  // 0x36  6
    , { 0x08,          0 ,       0 }  // 0x37  7
    , { 0x09,          0 ,       0 }  // 0x38  8
    , { 0x0A,          0 ,       0 }  // 0x39  9
    , { 0x27,    kbShift ,       0 }  // 0x3a  : + Shift
    , { 0x27,          0 ,       0 }  // 0x3b  ;
    , { 0x33,    kbShift ,       0 }  // 0x3c  < + Shift
    , { 0x0D,          0 ,       0 }  // 0x3d  =
    , { 0x34,    kbShift ,       0 }  // 0x3e  > + Shift
    , { 0x34,          0 ,       0 }  // 0x3f  ?
    , { 0x03,          0 ,       0 }  // 0x40  @
    , { 0x1E,    kbShift ,       0 }  // 0x41  A  + Shift
    , { 0x30,    kbShift ,       0 }  // 0x42  B  + Shift
    , { 0x2E,    kbShift ,       0 }  // 0x43  C  + Shift
    , { 0x20,    kbShift ,       0 }  // 0x44  D  + Shift
    , { 0x12,    kbShift ,       0 }  // 0x45  E  + Shift
    , { 0x21,    kbShift ,       0 }  // 0x46  F  + Shift
    , { 0x22,    kbShift ,       0 }  // 0x47  G  + Shift
    , { 0x23,    kbShift ,       0 }  // 0x48  H  + Shift
    , { 0x17,    kbShift ,       0 }  // 0x49  I  + Shift
    , { 0x24,    kbShift ,       0 }  // 0x4a  J  + Shift
    , { 0x25,    kbShift ,       0 }  // 0x4b  K  + Shift
    , { 0x26,    kbShift ,       0 }  // 0x4c  L  + Shift
    , { 0x32,    kbShift ,       0 }  // 0x4d  M  + Shift
    , { 0x31,    kbShift ,       0 }  // 0x4e  N  + Shift
    , { 0x18,    kbShift ,       0 }  // 0x4f  O  + Shift
    , { 0x19,    kbShift ,       0 }  // 0x50  P  + Shift
    , { 0x10,    kbShift ,       0 }  // 0x51  Q  + Shift
    , { 0x13,    kbShift ,       0 }  // 0x52  R  + Shift
    , { 0x1F,    kbShift ,       0 }  // 0x53  S  + Shift
    , { 0x14,    kbShift ,       0 }  // 0x54  T  + Shift
    , { 0x16,    kbShift ,       0 }  // 0x55  U  + Shift
    , { 0x2F,    kbShift ,       0 }  // 0x56  V  + Shift
    , { 0x11,    kbShift ,       0 }  // 0x57  W  + Shift
    , { 0x2D,    kbShift ,       0 }  // 0x58  X  + Shift
    , { 0x15,    kbShift ,       0 }  // 0x59  Y  + Shift
    , { 0x2C,    kbShift ,       0 }  // 0x5a  Z  + Shift
    , { 0x1A,          0 ,       0 }  // 0x5b  [
    , { 0x2B,          0 ,       0 }  // 0x5c  \
    , { 0x1B,          0 ,       0 }  // 0x5d  ]
    , { 0x07,    kbShift ,       0 }  // 0x5e  ^  + Shift
    , { 0x0C,    kbShift ,       0 }  // 0x5f  _  + Shift
    , { 0x29,          0 ,       0 }  // 0x60  `
    , { 0x1E,          0 ,       0 }  // 0x61  a
    , { 0x30,          0 ,       0 }  // 0x62  b
    , { 0x2E,          0 ,       0 }  // 0x63  c
    , { 0x20,          0 ,       0 }  // 0x64  d
    , { 0x12,          0 ,       0 }  // 0x65  e
    , { 0x21,          0 ,       0 }  // 0x66  f
    , { 0x22,          0 ,       0 }  // 0x67  g
    , { 0x23,          0 ,       0 }  // 0x68  h
    , { 0x17,          0 ,       0 }  // 0x69  i
    , { 0x24,          0 ,       0 }  // 0x6a  j
    , { 0x25,          0 ,       0 }  // 0x6b  k
    , { 0x26,          0 ,       0 }  // 0x6c  l
    , { 0x32,          0 ,       0 }  // 0x6d  m
    , { 0x31,          0 ,       0 }  // 0x6e  n
    , { 0x18,          0 ,       0 }  // 0x6f  o
    , { 0x19,          0 ,       0 }  // 0x70  p
    , { 0x10,          0 ,       0 }  // 0x71  q
    , { 0x13,          0 ,       0 }  // 0x72  r
    , { 0x1F,          0 ,       0 }  // 0x73  s
    , { 0x14,          0 ,       0 }  // 0x74  t
    , { 0x16,          0 ,       0 }  // 0x75  u
    , { 0x2F,          0 ,       0 }  // 0x76  v
    , { 0x11,          0 ,       0 }  // 0x77  w
    , { 0x2D,          0 ,       0 }  // 0x78  x
    , { 0x15,          0 ,       0 }  // 0x79  y
    , { 0x2C,          0 ,       0 }  // 0x7a  z
    , { 0x1A,    kbShift ,       0 }  // 0x7b  {  + Shift
    , { 0x2B,    kbShift ,       0 }  // 0x7c  |  + Shift
    , { 0x1B,    kbShift ,       0 }  // 0x7d  }  + Shift
    , { 0x29,    kbShift ,       0 }  // 0x7e  ~  + Shift
    , { 0x0E,          0 ,  kbBack }  // 0x7F - Backspace/Del - remap to backspace
    } };

    return _;
}



using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{

    // auto key = TKey('A', kbCtrlShift);
    //
    // std::cout << key.code << "\n";

    // for(std::uint8_t c=32; c!=0x7F; ++c)
    // {
    //     std::cout << "0x" << std::hex << (unsigned)c << std::dec << "  " << (char)c << "\n";
    // }

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

#if 0
    trieBuilder.addTokenSequence("\x1B", (payload_type));
#endif

    std::vector<umba::tokenizer::TrieNode> trie;

    trieBuilder.buildTokenTrie(trie);

// inline
// const std::array<TerminalAsciiCharInfo, 128>& getTerminalAsciiCharInfo()
// {

    // cout << "#if defined(UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_DISABLE)\n"
    //      << "    #define UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_INIT(p)\n"
    //      << "#else\n"
    //      << "    #define UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_INIT(p)  , (p)\n"
    //      << "#endif\n\n";
    //
    // cout << "#if defined(UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_DISABLE)\n"
    //      << "    #define UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_INIT(l)\n"
    //      << "#else\n"
    //      << "    #define UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_INIT(l)  , (l)\n"
    //      << "#endif\n\n";


// #if !defined(UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_DISABLE)
//     UMBA_TOKENIZER_PAYLOAD_FLAGS_TYPE    payloadExtra         ; // Пользовательские флаги или данные, влезает указатель
// #endif



    cout << "static const\n";
    cout << "std::array<umba::tokenizer::TrieNode, " << trie.size() << "> ansiTerminalSequences\n{\n";

    cout << "\n// parendIdx, lookupStartIdx, chunkSize, childsIdx[, extra], tokenId[, level]\n\n";

    auto printTrieIndex = [&](UMBA_TOKENIZER_TRIE_INDEX_TYPE idx)
    {
        if (idx==UMBA_TOKENIZER_TRIE_INDEX_INVALID)
            cout << "UMBA_TOKENIZER_TRIE_INDEX_INVALID";
        else
            cout << idx;
    };

    auto printPayloadValue = [&](UMBA_TOKENIZER_PAYLOAD_TYPE p)
    {
        if (p==UMBA_TOKENIZER_PAYLOAD_INVALID)
            cout << "UMBA_TOKENIZER_PAYLOAD_INVALID";
        else
            cout << p;
    };

    for(std::size_t i=0; i!=trie.size(); ++i)
    {
        const umba::tokenizer::TrieNode& node = trie[i];

        if (!i)
            cout << "{ ";
        else
            cout << ", ";

        cout << "{ "; printTrieIndex(node.parentNodeIndex); cout << ", "; printTrieIndex(node.lookupChunkStartIndex); cout << ", "; printTrieIndex(node.lookupChunkSize); cout << ", "; printTrieIndex(node.childsIndex);
        cout << ", "; printPayloadValue(node.payload);

#if !defined(UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_DISABLE)
        cout << " /*,*/ UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_INITIALIZER_LIST_INIT(" << node.payloadExtra << ")";
#else
        cout << " /*,*/ UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_INITIALIZER_LIST_INIT(0)";
#endif

        cout << ", " << node.token;

#if !defined(UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_DISABLE)
        cout << " /*,*/ UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_INITIALIZER_LIST_INIT(" << node.level << ")";
#else
        cout << " /*,*/ UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_INITIALIZER_LIST_INIT(0)";
#endif

        cout << "}\n";

    }

    cout << "}\n};\n\n";

    return 0;
}


// typedef struct tag_umba_tokenizer_trie_node
// {
//     UMBA_TOKENIZER_TRIE_INDEX_TYPE       parentNodeIndex      ;
//     UMBA_TOKENIZER_TRIE_INDEX_TYPE       lookupChunkStartIndex; /* Одно и то же значение для всех элементов lookupChunk'а */
//     UMBA_TOKENIZER_TRIE_INDEX_TYPE       lookupChunkSize      ; /* Одно и то же значение для всех элементов lookupChunk'а */
//     UMBA_TOKENIZER_TRIE_INDEX_TYPE       childsIndex          ;
//     UMBA_TOKENIZER_PAYLOAD_TYPE          payload              ; // Полезная нагрузка
// #if !defined(UMBA_TOKENIZER_TRIE_NODE_PAYLOAD_EXTRA_FIELD_DISABLE)
//     UMBA_TOKENIZER_PAYLOAD_FLAGS_TYPE    payloadExtra         ; // Пользовательские флаги или данные, влезает указатель
// #endif
//     UMBA_TOKENIZER_TOKEN_TYPE            token                ; // Токен или символ
//
// #if !defined(UMBA_TOKENIZER_TRIE_NODE_LEVEL_FIELD_DISABLE)
//     UMBA_TOKENIZER_TRIE_INDEX_TYPE       level                ; // Нужно, чтобы делать красивый граф таблицы trie
// #endif
//
// } umba_tokenizer_trie_node;



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
http://kbdlayout.info/kbdusx
http://kbdlayout.info/kbdusx/scancodes

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

