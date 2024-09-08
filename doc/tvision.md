# Заметки по портированию TurboVision (magiblot) на голое железо

  - [Классы и структуры](#user-content-классы-и-структуры)
    - [class StdioCtl final](#user-content-class-stdioctl-final)
    - [InputStrategy и его производные](#user-content-inputstrategy-и-его-производные)
      - [class EventSource](#user-content-class-eventsource)
      - [class InputStrategy : public EventSource](#user-content-class-inputstrategy--public-eventsource)
      - [class GpmInput final : public InputStrategy](#user-content-class-gpminput-final--public-inputstrategy)
      - [class NcursesInput : public InputStrategy](#user-content-class-ncursesinput--public-inputstrategy)
      - [class Win32Input final : public InputStrategy](#user-content-class-win32input-final--public-inputstrategy)
      - [struct LinuxConsoleInput final : public EventSource](#user-content-struct-linuxconsoleinput-final--public-eventsource)
    - [DisplayStrategy и его производные](#user-content-displaystrategy-и-его-производные)
      - [class DisplayStrategy](#user-content-class-displaystrategy)
      - [class TerminalDisplay : public DisplayStrategy](#user-content-class-terminaldisplay--public-displaystrategy)
      - [class AnsiDisplayBase](#user-content-class-ansidisplaybase)
      - [template<class DisplayBase> class AnsiDisplay : public DisplayBase, public AnsiDisplayBase](#user-content-templateclass-displaybase-class-ansidisplay--public-displaybase-public-ansidisplaybase)
      - [class NcursesDisplay : public TerminalDisplay](#user-content-class-ncursesdisplay--public-terminaldisplay)
      - [class Win32Display : public TerminalDisplay](#user-content-class-win32display--public-terminaldisplay)
    - [ConsoleStrategy и его производные](#user-content-consolestrategy-и-его-производные)
      - [struct ConsoleStrategy](#user-content-struct-consolestrategy)
      - [class LinuxConsoleStrategy : public ConsoleStrategy](#user-content-class-linuxconsolestrategy--public-consolestrategy)
      - [class Win32ConsoleStrategy final : public ConsoleStrategy](#user-content-class-win32consolestrategy-final--public-consolestrategy)
      - [class UnixConsoleStrategy : public ConsoleStrategy](#user-content-class-unixconsolestrategy--public-consolestrategy)
      - [Platform::createConsole() - создание консоли](#user-content-platformcreateconsole---создание-консоли)
    - [class Platform](#user-content-class-platform)
    - [class DisplayBuffer](#user-content-class-displaybuffer)
    - [class THardwareInfo](#user-content-class-thardwareinfo)
    - [struct InputState](#user-content-struct-inputstate)
    - [class InputGetter](#user-content-class-inputgetter)
    - [struct CSIData](#user-content-struct-csidata)
    - [namespace TermIO](#user-content-namespace-termio)
  - [Ввод](#user-content-ввод)
    - [TVISION_ESCDELAY](#user-content-tvision_escdelay)
  - [Резюме по DisplayStrategy](#user-content-резюме-по-displaystrategy)
    - [Методы DisplayStrategy](#user-content-методы-displaystrategy)
    - [getScreenSize](#user-content-getscreensize)
    - [getCaretSize](#user-content-getcaretsize)
    - [clearScreen](#user-content-clearscreen)
    - [getScreenMode](#user-content-getscreenmode)
    - [reloadScreenInfo](#user-content-reloadscreeninfo)
    - [lowlevelWriteChars](#user-content-lowlevelwritechars)
    - [lowlevelMoveCursor](#user-content-lowlevelmovecursor)
    - [lowlevelMoveCursorX](#user-content-lowlevelmovecursorx)
    - [lowlevelCursorSize](#user-content-lowlevelcursorsize)
    - [lowlevelFlush](#user-content-lowlevelflush)
    - [screenChanged](#user-content-screenchanged)


Данная статья помогла мне добавить в TurboVision (magiblot) возможности для работы на голом железе, в микроконтроллере, без какой-либо операционной системы.

Первоначально я пытался разобраться, что мне требуется сделать, гуляя по исходным текстам в IDE, но, потратив некоторое время,
я понял, что ни на йоту не приблизился к пониманию того, что мне требуется сделать, и решил начать с описания существующих классов,
вставляя фрагменты кода непосредственно в документ. В этом мне оказала бесценную помощь утилита [umba-md-pp](https://github.com/al-martyn1/umba-md-pp/blob/main/README.md),
которая позволяет вставлять произвольные фрагменты кода из сторонних файлов по сигнатуре (и не только), и умеющая искать блоки кода по блочным символам (`{}` для `С++`) и не только.
Также мне очень помогла утилита `umba-md-pp-view`, которая при помощи **Doxygen** умеет формировать `RTF`-файл из `MD` документа для локального просмотра.

По мере того, как я описывал существующее положение дел, ко мне стало приходить понимание того, что требуется сделать.
Был потрачен день на блуждание по исходникам, и в результате выхлопа было ровно ноль. Затем было потрачено полтора дня на описание существующей системы,
и полдня на реализацию новой подсистемы.

Получившийся документ в результате может служить некоторым руководством по подсистеме вывода графики и пользовательского ввода библиотеки TurboVision.


# Классы и структуры



## class StdioCtl final

Хранит хэндлы файловых объектов, которые используются для вывода на экран и для полчения пользовательского ввода.

tvision/include/tvision/internal/stdioctl.h:14
```cpp
class StdioCtl final
{
#ifdef _WIN32
    enum { input = 0, startupOutput = 1, activeOutput = 2 };
    struct
    {
        HANDLE handle {INVALID_HANDLE_VALUE};
        bool owning {false};
    } cn[3];
    bool ownsConsole {false};
#elif defined(TV_BARE_METAL)
    // Nothing for bare metal
#else
    int fds[2] {-1, -1};
    FILE *files[2] {nullptr, nullptr};
    bool ownsFiles {false};
#endif // _WIN32

    static StdioCtl *instance;

    StdioCtl() noexcept;
    ~StdioCtl();

public:

    // On Windows, the StdioCtl instance is created every time the alternate
    // screen buffer is enabled and it is destroyed when restoring the console.
    // On Unix, the StdioCtl instance is created just once at the beginning
    // of the program execution (in static initialization) and destroyed when
    // exiting the program.

    // Creates a global instance if none exists, and returns it.
    static StdioCtl &getInstance() noexcept;
    // Destroys the global instance if it exists.
    static void destroyInstance() noexcept;

    void write(const char *data, size_t bytes) const noexcept;
#if defined(TV_BARE_METAL)
    // For bare metal platform there is no standard implementation of write/read methods
    // User must implement it in his project itself
    void read(char *data, size_t bytesToRead, size_t *pBytesReaded) const noexcept;
#else
    TPoint getSize() const noexcept;
    TPoint getFontSize() const noexcept;
#endif

#ifdef _WIN32
    HANDLE in() const noexcept { return cn[input].handle; }
    HANDLE out() const noexcept { return cn[activeOutput].handle; }
#elif defined(TV_BARE_METAL)
    // Nothing for bare metal
#else
    int in() const noexcept { return fds[0]; }
    int out() const noexcept { return fds[1]; }
    FILE *fin() const noexcept { return files[0]; }
    FILE *fout() const noexcept { return files[1]; }
#ifdef __linux__
    bool isLinuxConsole() const noexcept;
#endif
#endif // _WIN32
};
```

Для `TV_BARE_METAL` будем хранить два `int`-а - "дескрипторы" UART-портов (по факту - индексы портов), но реализация может
не учитывать их и писать/читать в/из порт/а по своему усмотрению.

Не хватает метода `read` - видимо, все просто берут дескриптор, и делают с ним, что хотят. Надо абстрагироватся от дескриптора,
и работать через метод `read`. 

Есть странные методы - `getSize` и `getFontSize`. Странные потому, что не очень понятно, почему они сюда засунуты.

`getFontSize` используется в `TerminalDisplay::getScreenMode()` для установки флага `TDisplay::smFont8x8` - тоже не очень понятно, что это.

`getSize` используется в `NcursesDisplay::reloadScreenInfo()`, `TerminalDisplay::screenChanged()` и `Win32Display::reloadScreenInfo()`.

Реализация `UNIX`:

tvision/source/platform/stdioctl.cpp:96
```cpp
TPoint StdioCtl::getSize() const noexcept
{
    struct winsize w;
    for (int fd : fds)
    {
        if (ioctl(fd, TIOCGWINSZ, &w) != -1)
        {
            int env_col = getEnv<int>("COLUMNS", INT_MAX);
            int env_row = getEnv<int>("LINES", INT_MAX);
            return {
                min(max(w.ws_col, 0), max(env_col, 0)),
                min(max(w.ws_row, 0), max(env_row, 0)),
            };
        }
    }
    return {0, 0};
}
```

tvision/source/platform/stdioctl.cpp:114
```cpp
TPoint StdioCtl::getFontSize() const noexcept
{
#ifdef KDFONTOP
    struct console_font_op cfo {};
    cfo.op = KD_FONT_OP_GET;
    cfo.width = cfo.height = 32;
    for (int fd : fds)
        if (ioctl(fd, KDFONTOP, &cfo) != -1)
            return {
                max(cfo.width, 0),
                max(cfo.height, 0),
            };
#endif
    struct winsize w;
    for (int fd : fds)
        if (ioctl(fd, TIOCGWINSZ, &w) != -1)
            return {
                w.ws_xpixel / max(w.ws_col, 1),
                w.ws_ypixel / max(w.ws_row, 1),
            };
    return {0, 0};
}
```

 - [console_font_op](https://coral.googlesource.com/busybox/+/refs/tags/1_15_1/console-tools/loadfont.c)
 - [TIOCGWINSZ](https://www.opennet.ru/man.shtml?topic=tty_ioctl&category=4&russian=0)

Реализация `WIN32`:

tvision/source/platform/stdioctl.cpp:348
```cpp
TPoint StdioCtl::getSize() const noexcept
{
    CONSOLE_SCREEN_BUFFER_INFO sbInfo;
    auto &srWindow = sbInfo.srWindow;
    if (GetConsoleScreenBufferInfo(out(), &sbInfo))
        return {
            max(srWindow.Right - srWindow.Left + 1, 0),
            max(srWindow.Bottom - srWindow.Top + 1, 0),
        };
    return {0, 0};
}
```

tvision/source/platform/stdioctl.cpp:360
```cpp
TPoint StdioCtl::getFontSize() const noexcept
{
    CONSOLE_FONT_INFO fontInfo;
    if (GetCurrentConsoleFont(out(), FALSE, &fontInfo))
        return {
            fontInfo.dwFontSize.X,
            fontInfo.dwFontSize.Y,
        };
    return {0, 0};
}
```

 - [GetConsoleScreenBufferInfo](https://learn.microsoft.com/ru-ru/windows/console/getconsolescreenbufferinfo)
 - [GetCurrentConsoleFont](https://learn.microsoft.com/en-us/windows/console/getcurrentconsolefont)

В целом, для `TV_BARE_METAL` это не нужно.

Добавляемая реализация метода `read` должна будет возвращать управление сразу, без блокировки, и возвращать то количество байт, 
которое доступно для чтения, или `0`, если в буфере ничего нет.



## InputStrategy и его производные


### class EventSource

Базовый класс для InputStrategy и LinuxConsoleInput.

tvision/include/tvision/internal/events.h:0
```cpp
```

tvision/include/tvision/internal/events.h:169
```cpp
class EventSource
{
public:

    const SysHandle handle;

    EventSource(SysHandle handle) noexcept :
        handle(handle)
    {
    }

    virtual ~EventSource() {}

    virtual bool hasPendingEvents() noexcept;
    virtual bool getEvent(TEvent &) noexcept;
};
```



### class InputStrategy : public EventSource

Базовый класс для конкретных реализаций InputStrategy.

tvision/include/tvision/internal/platform.h:37
```cpp
class InputStrategy : public EventSource
{
public:

    InputStrategy(SysHandle aHandle) noexcept :
        EventSource(aHandle)
    {
    }

    virtual ~InputStrategy() {}

    virtual int getButtonCount() noexcept { return 0; }
    virtual void cursorOn() noexcept {}
    virtual void cursorOff() noexcept {}
};
```

`getButtonCount()` - для `TV_BARE_METAL` должно возвращать 0, так как мышь мы не поддерживаем.

`cursorOn()` - включение курсора (мышиного, наверное)?

`cursorOff()` - выключение курсора (мышиного, наверное)?

Реализации:

tvision/source/platform/win32con.cpp:195
```cpp
void Win32Input::cursorOn() noexcept
{
    DWORD consoleMode = 0;
    GetConsoleMode(io.in(), &consoleMode);
    SetConsoleMode(io.in(), consoleMode | ENABLE_MOUSE_INPUT);
}
```

tvision/source/platform/win32con.cpp:202
```cpp
void Win32Input::cursorOff() noexcept
{
    DWORD consoleMode = 0;
    GetConsoleMode(io.in(), &consoleMode);
    SetConsoleMode(io.in(), consoleMode & ~ENABLE_MOUSE_INPUT);
}
```



### class GpmInput final : public InputStrategy

tvision/include/tvision/internal/gpminput.h:18
```cpp
class GpmInput final : public InputStrategy
{
    NegativeScreenCursor cursor;
    uchar buttonState;

    static void fitEvent(Gpm_Event&) noexcept;
    GpmInput() noexcept;

public:

    static GpmInput *create() noexcept;
    ~GpmInput();
    bool getEvent(TEvent &ev) noexcept;
    int getButtonCount() noexcept;
};
```


### class NcursesInput : public InputStrategy

tvision/include/tvision/internal/ncursinp.h:28
```cpp
class NcursesInput : public InputStrategy
{
    enum : char { KEY_ESC = '\x1B' };
    enum { readTimeoutMs = 10 };

    StdioCtl &io;
    InputState &state;
    bool mouseEnabled;
    NcursesInputGetter in;

    int getChNb() noexcept;
    void detectAlt(int keys[4], bool &Alt) noexcept;
    void parsePrintableChar(TEvent &ev, int keys[4], int &num_keys) noexcept;
    void readUtf8Char(int keys[4], int &num_keys) noexcept;

    bool parseCursesMouse(TEvent&) noexcept;

public:

    // Lifetimes of 'io', 'display' and 'state' must exceed that of 'this'.
    NcursesInput(StdioCtl &io, NcursesDisplay &display, InputState &state, bool mouse) noexcept;
    ~NcursesInput();

    bool getEvent(TEvent &ev) noexcept override;
    int getButtonCount() noexcept override;
    bool hasPendingEvents() noexcept override;
};
```


### class Win32Input final : public InputStrategy

tvision/include/tvision/internal/win32con.h:49
```cpp
class Win32Input final : public InputStrategy
{
    StdioCtl &io;
    InputState state;

    bool getEvent(const INPUT_RECORD &, TEvent &ev) noexcept;
    bool getMouseEvent(MOUSE_EVENT_RECORD, TEvent &ev) noexcept;

public:

    // The lifetime of 'aIo' must exceed that of 'this'.
    Win32Input(StdioCtl &aIo) noexcept :
        InputStrategy(aIo.in()),
        io(aIo)
    {
    }

    bool getEvent(TEvent &ev) noexcept override;
    int getButtonCount() noexcept override;
    void cursorOn() noexcept override;
    void cursorOff() noexcept override;
};
```


### struct LinuxConsoleInput final : public EventSource

Использует InputStrategy.

tvision/include/tvision/internal/linuxcon.h:18
```cpp
struct LinuxConsoleInput final : public EventSource
{
    StdioCtl &io;
    InputStrategy &input;

    LinuxConsoleInput(StdioCtl &aIo, InputStrategy &aInput) noexcept :
        EventSource(aInput.handle),
        io(aIo),
        input(aInput)
    {
    }

    bool getEvent(TEvent &ev) noexcept override;
    bool hasPendingEvents() noexcept override;

    static ushort getKeyboardModifiers(StdioCtl &io) noexcept;
};
```




## DisplayStrategy и его производные

### class DisplayStrategy

tvision/include/tvision/internal/platform.h:18
```cpp
class DisplayStrategy
{
public:

    virtual ~DisplayStrategy() {}

    virtual TPoint getScreenSize() noexcept { return {}; }
    virtual int getCaretSize() noexcept { return 0; } // Range [0, 100].
    virtual void clearScreen() noexcept {}
    virtual ushort getScreenMode() noexcept { return 0; }
    virtual void reloadScreenInfo() noexcept {}
    virtual void lowlevelWriteChars(TStringView /*chars*/, TColorAttr /*attr*/) noexcept {}
    virtual void lowlevelMoveCursor(uint /*x*/, uint /*y*/) noexcept {};
    virtual void lowlevelMoveCursorX(uint x, uint y) noexcept { lowlevelMoveCursor(x, y); }
    virtual void lowlevelCursorSize(int /*size*/) noexcept {};
    virtual void lowlevelFlush() noexcept {};
    virtual bool screenChanged() noexcept { return false; }
};
```

Данный класс позволяет:

  - Получать размер экрана
  - Получать размер каретки
  - Получать режим экрана
  - Очищать экран
  - Предоставляет низкоуровневые сервисы по установке размера курсора, перемещению позиции вывода, и по сбросу экранного буфера


### class TerminalDisplay : public DisplayStrategy

tvision/include/tvision/internal/termdisp.h:35
```cpp
class TerminalDisplay : public DisplayStrategy
{
    TermCap getCapabilities() noexcept;
    TPoint lastSize {};

protected:

    StdioCtl &io;
    TermCap termcap;

    // The subclass must invoke this in the constructor.
    void initCapabilities() noexcept
    {
        termcap = getCapabilities();
    }

public:

    // The lifetime of 'aIo' exceeds that of 'this'.
    TerminalDisplay(StdioCtl &aIo) noexcept :
        io(aIo)
    {
    }

    virtual int getColorCount() noexcept = 0;
    ushort getScreenMode() noexcept override;
    bool screenChanged() noexcept override;
};
```


### class AnsiDisplayBase

tvision/include/tvision/internal/ansidisp.h:83
```cpp
class AnsiDisplayBase
{
    class Buffer
    {
        char *head {nullptr};
        size_t capacity {0};
    public:
        char *tail {nullptr};

        ~Buffer();
        char *data() noexcept;
        size_t size() const noexcept;
        void clear() noexcept;
        void push(TStringView) noexcept;
        void push(char) noexcept;
        void reserve(size_t) noexcept;
    };

    const StdioCtl &io;
    Buffer buf;
    TermAttr lastAttr {};

    void bufWriteCSI1(uint a, char F) noexcept;
    void bufWriteCSI2(uint a, uint b, char F) noexcept;

protected:

    AnsiDisplayBase(const StdioCtl &aIo) noexcept :
        io(aIo)
    {
    }

    ~AnsiDisplayBase();

    void clearAttributes() noexcept;
    void clearScreen() noexcept;

    void lowlevelWriteChars(TStringView chars, TColorAttr attr, const TermCap &) noexcept;
    void lowlevelMoveCursor(uint x, uint y) noexcept;
    void lowlevelMoveCursorX(uint x, uint y) noexcept;
    void lowlevelFlush() noexcept;
};
```


### template<class DisplayBase> class AnsiDisplay : public DisplayBase, public AnsiDisplayBase

tvision/include/tvision/internal/ansidisp.h:126
```cpp
template<class DisplayBase>
class AnsiDisplay : public DisplayBase, public AnsiDisplayBase
{

public:

    template <typename ...Args>
    AnsiDisplay(Args&& ...args) noexcept :
        DisplayBase(static_cast<Args&&>(args)...),
        AnsiDisplayBase(TerminalDisplay::io)
    {
        static_assert(std::is_base_of<TerminalDisplay, DisplayBase>::value,
            "The base class of AnsiDisplay must be a derived of TerminalDisplay."
        );
    }

    void lowlevelWriteChars(TStringView chars, TColorAttr attr) noexcept override
        { AnsiDisplayBase::lowlevelWriteChars(chars, attr, TerminalDisplay::termcap); }
    void lowlevelMoveCursor(uint x, uint y) noexcept override
        { AnsiDisplayBase::lowlevelMoveCursor(x, y); }
    void lowlevelMoveCursorX(uint x, uint y) noexcept override
        { AnsiDisplayBase::lowlevelMoveCursorX(x, y); }
    void lowlevelFlush() noexcept override
        { AnsiDisplayBase::lowlevelFlush(); }

    void clearScreen() noexcept override
    {
        clearAttributes();
        AnsiDisplayBase::clearScreen();
    }

    void reloadScreenInfo() noexcept override
    {
        DisplayBase::reloadScreenInfo();
        clearAttributes();
    }

};
```

The base class of AnsiDisplay must be a derived of TerminalDisplay.


### class NcursesDisplay : public TerminalDisplay

tvision/include/tvision/internal/ncurdisp.h:14
```cpp
class NcursesDisplay : public TerminalDisplay
{
    SCREEN *term;

    bool hasColors;
    std::unordered_map<ushort, int> pairIdentifiers;
    ushort definedPairs;

    bool usesNcursesDraw;

    void getCaretPosition(int &x, int &y) noexcept;
    uint translateAttributes(TColorAttr attr) noexcept;
    uint getColorPair(uchar pairKey) noexcept;

public:

    // The lifetime of 'aIo' exceeds that of 'this'.
    NcursesDisplay(StdioCtl &io) noexcept;
    ~NcursesDisplay();

    void reloadScreenInfo() noexcept override;
    TPoint getScreenSize() noexcept override;
    int getCaretSize() noexcept override;
    int getColorCount() noexcept override;

    void clearScreen() noexcept override;

protected:

    void lowlevelWriteChars(TStringView chars, TColorAttr attr) noexcept override;
    void lowlevelMoveCursor(uint x, uint y) noexcept override;
    void lowlevelCursorSize(int size) noexcept override;
    void lowlevelFlush() noexcept override;
};
```


### class Win32Display : public TerminalDisplay

tvision/include/tvision/internal/win32con.h:72
```cpp
class Win32Display : public TerminalDisplay
{
    TPoint size {};
    uchar lastAttr {'\x00'};
    std::vector<char> buf;
    CONSOLE_FONT_INFO lastFontInfo {};

public:

    // The lifetime of 'aIo' must exceed that of 'this'.
    Win32Display(StdioCtl &aIo) noexcept :
        TerminalDisplay(aIo)
    {
        initCapabilities();
    }

    void reloadScreenInfo() noexcept override;
    TPoint getScreenSize() noexcept override;
    int getCaretSize() noexcept override;
    int getColorCount() noexcept override;
    void clearScreen() noexcept override;
    bool screenChanged() noexcept override;

protected:

    void lowlevelWriteChars(TStringView chars, TColorAttr attr) noexcept override;
    void lowlevelMoveCursor(uint x, uint y) noexcept override;
    void lowlevelCursorSize(int size) noexcept override;
    void lowlevelFlush() noexcept override;
};
```










## ConsoleStrategy и его производные

### struct ConsoleStrategy

tvision/include/tvision/internal/platform.h:53
```cpp
struct ConsoleStrategy
{
    DisplayStrategy &display;
    InputStrategy &input;
    const std::vector<EventSource *> sources;

    ConsoleStrategy( DisplayStrategy &aDisplay, InputStrategy &aInput,
                     std::vector<EventSource *> &&aSources ) noexcept :
        display(aDisplay),
        input(aInput),
        sources(std::move(aSources))
    {
    }

    virtual ~ConsoleStrategy() {}

    virtual bool isAlive() noexcept { return true; }
    virtual bool setClipboardText(TStringView) noexcept { return false; }
    virtual bool requestClipboardText(void (&)(TStringView)) noexcept { return false; }
};
```

Базовый класс для конкретных реализаций ConsoleStrategy.


### class LinuxConsoleStrategy : public ConsoleStrategy

tvision/include/tvision/internal/linuxcon.h:36
```cpp
class LinuxConsoleStrategy : public ConsoleStrategy
{
    StderrRedirector errRedir;

    InputState &inputState;
    SigwinchHandler *sigwinch;
    LinuxConsoleInput &wrapper;
    GpmInput *gpm;

    LinuxConsoleStrategy( DisplayStrategy &, LinuxConsoleInput &,
                          InputState &, SigwinchHandler *,
                          GpmInput * ) noexcept;

public:

    // Pre: 'io.isLinuxConsole()' returns 'true'.
    // The lifetime of 'io' must exceed that of the returned object.
    // Takes ownership over 'inputState', 'display' and 'input'.
    static LinuxConsoleStrategy &create( StdioCtl &io,
                                         InputState &inputState,
                                         DisplayStrategy &display,
                                         InputStrategy &input ) noexcept;
    ~LinuxConsoleStrategy();

    static int charWidth(uint32_t) noexcept;
};
```


### class Win32ConsoleStrategy final : public ConsoleStrategy

tvision/include/tvision/internal/win32con.h:21
```cpp
class Win32ConsoleStrategy final : public ConsoleStrategy
{
    StdioCtl &io;
    UINT cpInput, cpOutput;

    Win32ConsoleStrategy( StdioCtl &aIo,
                          UINT cpInput, UINT cpOutput,
                          DisplayStrategy &aDisplay,
                          InputStrategy &aInput ) noexcept :
        ConsoleStrategy(aDisplay, aInput, {&aInput}),
        io(aIo),
        cpInput(cpInput),
        cpOutput(cpOutput)
    {
    }

    ~Win32ConsoleStrategy();

    bool isAlive() noexcept override;
    bool setClipboardText(TStringView) noexcept override;
    bool requestClipboardText(void (&)(TStringView)) noexcept override;

public:

    static Win32ConsoleStrategy &create() noexcept;
    static int charWidth(uint32_t) noexcept;
};
```


Win32ConsoleStrategy работает через AnsiDisplay, если система это поддерживает:

win32con.cpp:103 (ф-я Win32ConsoleStrategy::create())
'''cpp
    auto &display = supportsVT ? *new AnsiDisplay<Win32Display>(io)
                               : *new Win32Display(io);
'''


### class UnixConsoleStrategy : public ConsoleStrategy

tvision/include/tvision/internal/unixcon.h:16
```cpp
class UnixConsoleStrategy : public ConsoleStrategy
{
    StderrRedirector errRedir;

    StdioCtl &io;
    DisplayBuffer &displayBuf;
    InputState &inputState;
    SigwinchHandler *sigwinch;

    UnixConsoleStrategy( DisplayStrategy &, InputStrategy &, StdioCtl &,
                         DisplayBuffer &, InputState &,
                         SigwinchHandler * ) noexcept;

public:

    // The lifetime of 'io' and 'displayBuf' must exceed that of the returned object.
    // Takes ownership over 'inputState', 'display' and 'input'.
    static UnixConsoleStrategy &create( StdioCtl &io,
                                        DisplayBuffer &displayBuf,
                                        InputState &inputState,
                                        DisplayStrategy &display,
                                        InputStrategy &input ) noexcept;

    ~UnixConsoleStrategy();

    bool setClipboardText(TStringView) noexcept override;
    bool requestClipboardText(void (&)(TStringView)) noexcept override;

    static int charWidth(uint32_t) noexcept;
};
```


### Platform::createConsole() - создание консоли

tvision/source/platform/platfcon.cpp:19
```cpp
ConsoleStrategy &Platform::createConsole() noexcept
{
#ifdef _WIN32
    return Win32ConsoleStrategy::create();
#else
    auto &io = StdioCtl::getInstance();
    InputState &inputState = *new InputState;
    NcursesDisplay *display;
    if (getEnv<TStringView>("TVISION_DISPLAY") == "ncurses")
        display = new NcursesDisplay(io);
    else
        display = new AnsiDisplay<NcursesDisplay>(io);
#ifdef __linux__
    if (io.isLinuxConsole())
        return LinuxConsoleStrategy::create(io, inputState, *display, *new NcursesInput(io, *display, inputState, false));
#endif // __linux__
    return UnixConsoleStrategy::create(io, displayBuf, inputState, *display, *new NcursesInput(io, *display, inputState, true));
#endif // _WIN32
}
```



## class Platform

tvision/include/tvision/internal/platform.h:74
```cpp
class Platform
{
    EventWaiter waiter;
    DisplayBuffer displayBuf;
    DisplayStrategy dummyDisplay;
    InputStrategy dummyInput {(SysHandle) 0};
    ConsoleStrategy dummyConsole {dummyDisplay, dummyInput, {}};
    // Invariant: 'console' contains either a non-owning reference to 'dummyConsole'
    // or an owning reference to a heap-allocated ConsoleStrategy object.
    SignalSafeReentrantMutex<ConsoleStrategy *> console {&dummyConsole};

    static Platform *instance;

    void setUpConsole(ConsoleStrategy *&) noexcept;
    void restoreConsole(ConsoleStrategy *&) noexcept;
    void checkConsole() noexcept;
    bool sizeChanged(TEvent &ev) noexcept;
    ConsoleStrategy &createConsole() noexcept;

    static int initAndGetCharWidth(uint32_t) noexcept;
    static void initEncodingStuff() noexcept;
    static void signalCallback(bool) noexcept;

    bool screenChanged() noexcept
        { return console.lock([] (auto *c) { return c->display.screenChanged(); }); }

public:

    static int (*charWidth)(uint32_t) noexcept;

    // Platform is a singleton. It gets created and destroyed by THardwareInfo.
    Platform() noexcept;
    ~Platform();

    // Note: explicit 'this' required by GCC 5.
    void setUpConsole() noexcept
        { console.lock([&] (auto *&c) { this->setUpConsole(c); }); }
    void restoreConsole() noexcept
        { console.lock([&] (auto *&c) { this->restoreConsole(c); }); }

    bool getEvent(TEvent &ev) noexcept;
    void waitForEvent(int ms) noexcept { checkConsole(); waiter.waitForEvent(ms); }
    void interruptEventWait() noexcept { waiter.interruptEventWait(); }

    int getButtonCount() noexcept
        { return console.lock([] (auto *c) { return c->input.getButtonCount(); }); }
    void cursorOn() noexcept
        { console.lock([] (auto *c) { c->input.cursorOn(); }); }
    void cursorOff() noexcept
        { console.lock([] (auto *c) { c->input.cursorOff(); }); }

    // Adjust the caret size to the range 1 to 100 because that's what the original
    // THardwareInfo::getCaretSize() does and what TScreen expects.
    int getCaretSize() noexcept { return min(max(displayBuf.caretSize, 1), 100); }
    bool isCaretVisible() noexcept { return displayBuf.caretSize > 0; }
    void clearScreen() noexcept
        { console.lock([&] (auto *c) { displayBuf.clearScreen(c->display); }); }
    int getScreenRows() noexcept { return displayBuf.size.y; }
    int getScreenCols() noexcept { return displayBuf.size.x; }
    void setCaretPosition(int x, int y) noexcept { displayBuf.setCaretPosition(x, y); }
    ushort getScreenMode() noexcept
        { return console.lock([] (auto *c) { return c->display.getScreenMode(); }); }
    void setCaretSize(int size) noexcept { displayBuf.setCaretSize(size); }
    void screenWrite(int x, int y, TScreenCell *b, int l) noexcept { displayBuf.screenWrite(x, y, b, l); }
    void flushScreen() noexcept
        { console.lock([&] (auto *c) { displayBuf.flushScreen(c->display); }); }
    TScreenCell *reloadScreenInfo() noexcept
        { return console.lock([&] (auto *c) { return displayBuf.reloadScreenInfo(c->display); }); }

    bool setClipboardText(TStringView text) noexcept
        { return console.lock([&] (auto *c) { return c->setClipboardText(text); }); }
    bool requestClipboardText(void (&accept)(TStringView)) noexcept
        { return console.lock([&] (auto *c) { return c->requestClipboardText(accept); }); }
};
```

Делает кучу всего, надо разбираться.


## class DisplayBuffer

tvision/include/tvision/internal/dispbuff.h:21
```cpp
class DisplayBuffer
{
    friend FlushScreenAlgorithm;

    struct Range {
        int begin, end;
    };

    std::vector<TScreenCell> buffer, flushBuffer;
    std::vector<Range> rowDamage;

    const bool wideOverlapping;
    bool screenTouched {true};
    bool caretMoved {false};
    TPoint caretPosition {-1, -1};
    int newCaretSize {0};

    bool limitFPS;
    std::chrono::microseconds flushDelay {};
    std::chrono::time_point<std::chrono::steady_clock> lastFlush {};

    static DisplayBuffer *instance;
#ifdef _WIN32
    static constexpr int defaultFPS = 120; // Just 60 feels notably slower on Windows, I don't know why.
#else
    static constexpr int defaultFPS = 60;
#endif

    bool inBounds(int x, int y) const noexcept;

    void resizeBuffer() noexcept;
    void setDirty(int x, int y, int len) noexcept;
    void validateCell(TScreenCell &cell) const noexcept;

    std::vector<ScreenCursor*> cursors;
    void drawCursors() noexcept;
    void undrawCursors() noexcept;

    bool needsFlush() const noexcept;
    bool timeToFlush() noexcept;

public:

    TPoint size {};
    int caretSize {};

    DisplayBuffer() noexcept;
    ~DisplayBuffer();

    void setCaretSize(int size) noexcept;
    void setCaretPosition(int x, int y) noexcept;
    void screenWrite(int x, int y, TScreenCell *buf, int len) noexcept;
    void clearScreen(DisplayStrategy &) noexcept;
    void redrawScreen(DisplayStrategy &) noexcept;
    void flushScreen(DisplayStrategy &) noexcept;
    TScreenCell *reloadScreenInfo(DisplayStrategy &) noexcept;

    static void addCursor(ScreenCursor *cursor) noexcept;
    static void removeCursor(ScreenCursor *cursor) noexcept;
    static void changeCursor() noexcept;
};
```


## class THardwareInfo

tvision/include/tvision/hardware.h:44
```cpp
class THardwareInfo
{

public:

    THardwareInfo() noexcept;
    ~THardwareInfo();

    static uint32_t getTickCount() noexcept;

#if defined( __FLAT__ )

    enum ConsoleType { cnInput = 0, cnOutput = 1, cnStartup = 2 };
    enum PlatformType { plDPMI32 = 1, plWinNT = 2, plOS2 = 4 };

    static PlatformType getPlatform() noexcept;

// Caret functions.

    static void setCaretSize( ushort size ) noexcept;
    static ushort getCaretSize() noexcept;
    static void setCaretPosition( ushort x, ushort y ) noexcept;
    static BOOL isCaretVisible() noexcept;

// Screen functions.

    static ushort getScreenRows() noexcept;
    static ushort getScreenCols() noexcept;
    static ushort getScreenMode() noexcept;
    static void setScreenMode( ushort mode ) noexcept;
    static void clearScreen( ushort w, ushort h ) noexcept;
    static void flushScreen() noexcept;
    static void screenWrite( ushort x, ushort y, TScreenCell *buf, DWORD len ) noexcept;
    static TScreenCell *allocateScreenBuffer() noexcept;
    static void freeScreenBuffer( TScreenCell *buffer ) noexcept;
    static void setUpConsole() noexcept;
    static void restoreConsole() noexcept;

// Mouse functions.

    static DWORD getButtonCount() noexcept;
    static void cursorOn() noexcept;
    static void cursorOff() noexcept;

// Event functions.

    static BOOL getMouseEvent( MouseEventType& event ) noexcept;
    static BOOL getKeyEvent( TEvent& event ) noexcept;
    static void clearPendingEvent() noexcept;
    static void waitForEvent( int timeoutMs ) noexcept;
    static void interruptEventWait() noexcept;
    static BOOL setClipboardText( TStringView text ) noexcept;
    static BOOL requestClipboardText( void (&accept)( TStringView ) ) noexcept;

// System functions.

    static BOOL setCtrlBrkHandler( BOOL install ) noexcept;
    static BOOL setCritErrorHandler( BOOL install ) noexcept;

    static const ushort NormalCvt[89];
    static const ushort ShiftCvt[89];
    static const ushort CtrlCvt[89];
    static const ushort AltCvt[89];

private:

    static BOOL __stdcall ctrlBreakHandler( DWORD dwCtrlType ) noexcept;

    static BOOL insertState;
    static PlatformType platform;
    static HANDLE consoleHandle[3];
    static DWORD consoleMode;
    static DWORD pendingEvent;
    static INPUT_RECORD irBuffer;
    static CONSOLE_CURSOR_INFO crInfo;
    static CONSOLE_SCREEN_BUFFER_INFO sbInfo;

#ifndef __BORLANDC__
    enum { eventQSize = 24 };
    static TEvent eventQ[eventQSize];
    static size_t eventCount;
    static BOOL getPendingEvent(TEvent &event, Boolean mouse) noexcept;
    static void readEvents() noexcept;
#endif

#else

    static ushort *getColorAddr( ushort offset = 0 );
    static ushort *getMonoAddr( ushort offset = 0 );
    static uchar getShiftState();
    static uchar getBiosScreenRows();
    static uchar getBiosVideoInfo();
    static void setBiosVideoInfo( uchar info );
    static ushort getBiosEquipmentFlag();
    static ushort huge getBiosEquipmentFlag(int);   // Non-inline version.
    static void setBiosEquipmentFlag( ushort flag );
    static Boolean getDPMIFlag();

private:

    static ushort huge getBiosSelector();   // For SYSINT.ASM.

    static Boolean dpmiFlag;
    static ushort colorSel;
    static ushort monoSel;
    static ushort biosSel;

#endif

};
```


## struct InputState

tvision/include/tvision/internal/terminal.h:20
```cpp
struct InputState
{
    uchar buttons {0};
#ifdef _WIN32
    wchar_t surrogate {0};
#endif
    Far2lState far2l;
    bool hasFullOsc52 {false};
    bool bracketedPaste {false};
    bool gotDsrResponse {false};
    void (*putPaste)(TStringView) {nullptr};
};
```


## class InputGetter

tvision/include/tvision/internal/terminal.h:33
```cpp
class InputGetter
{
public:

    virtual int get() noexcept = 0;
    virtual void unget(int) noexcept = 0;
};
```


## struct CSIData

tvision/include/tvision/internal/terminal.h:100
```cpp
struct CSIData
{
    // Represents the data stored in a CSI escape sequence:
    // \x1B [ _val[0] ; _val[1] ; ... terminator

    // CSIs can be longer, but this is the largest we need for now.
    enum { maxLength = 6 };

    uint _val[maxLength];
    uint terminator {0};
    uint length {0};

    bool readFrom(GetChBuf &buf) noexcept;
    inline uint getValue(uint i, uint defaultValue = 1) const noexcept;
};
```


## namespace TermIO

tvision/include/tvision/internal/terminal.h:121
```cpp
namespace TermIO
{
    void mouseOn(StdioCtl &) noexcept;
    void mouseOff(StdioCtl &) noexcept;
    void keyModsOn(StdioCtl &) noexcept;
    void keyModsOff(StdioCtl &) noexcept;

    void normalizeKey(KeyDownEvent &keyDown) noexcept;

    bool setClipboardText(StdioCtl &, TStringView, InputState &) noexcept;
    bool requestClipboardText(StdioCtl &, void (&)(TStringView), InputState &) noexcept;

    ParseResult parseEvent(GetChBuf&, TEvent&, InputState&) noexcept;
    ParseResult parseEscapeSeq(GetChBuf&, TEvent&, InputState&) noexcept;
    ParseResult parseX10Mouse(GetChBuf&, TEvent&, InputState&) noexcept;
    ParseResult parseSGRMouse(GetChBuf&, TEvent&, InputState&) noexcept;
    ParseResult parseCSIKey(const CSIData &csi, TEvent&, InputState&) noexcept;
    ParseResult parseFKeyA(GetChBuf&, TEvent&) noexcept;
    ParseResult parseSS3Key(GetChBuf&, TEvent&) noexcept;
    ParseResult parseArrowKeyA(GetChBuf&, TEvent&) noexcept;
    ParseResult parseFixTermKey(const CSIData &csi, TEvent&) noexcept;
    ParseResult parseDCS(GetChBuf&, InputState&) noexcept;
    ParseResult parseOSC(GetChBuf&, InputState&) noexcept;
    ParseResult parseCPR(const CSIData &csi, InputState&) noexcept;
    ParseResult parseWin32InputModeKeyOrEscapeSeq(const CSIData &, InputGetter&, TEvent&, InputState&) noexcept;

    char *readUntilBelOrSt(GetChBuf &) noexcept;
    void consumeUnprocessedInput(StdioCtl &, InputGetter &, InputState &) noexcept;
}
```


TVISION_MAX_FPS

# Ввод

## TVISION_ESCDELAY




# Резюме по DisplayStrategy

Базовый класс, он же "интерфейс" - `DisplayStrategy`. От него наследуется `TerminalDisplay`.
От `TerminalDisplay` наследуются `NcursesDisplay` и `Win32Display`.
`NcursesDisplay` и `Win32Display` используются или непосредственно, или в качестве базового для `AnsiDisplay` 
(`AnsiDisplay<Win32Display>` / `AnsiDisplay<NcursesDisplay>`). Для `BareMetall` нужно делать `BareMetallDisplay`, 
который ведёт себя аналогично - `BareMetallDisplay` умеет, например, отображать себя на графическом дисплее 
(через предоставляемые пользователем внешние функции сброса изображения на дисплей), а обёрнутый в `AnsiDisplay`
(`AnsiDisplay<BareMetallDisplay>`) - умеет отображать себя на ANSI терминале (также через предоставляемую 
пользователем функцию записы в последовательный порт).


## Методы DisplayStrategy

 - `TPoint getScreenSize()` - возвращает размер экрана; 
 - `int getCaretSize()` - возвращает размер каретки, от 0 до 100, где 0 - каретка выключена. По факту используются значения 0, 1, 100.
 - `void clearScreen()` - AnsiDisplay переопределяет.
 - `ushort getScreenMode()` - 
 - `void reloadScreenInfo()` - 
 - `void lowlevelWriteChars(TStringView /*chars*/, TColorAttr /*attr*/)` - AnsiDisplay переопределяет.
 - `void lowlevelMoveCursor(uint /*x*/, uint /*y*/)` - AnsiDisplay переопределяет.
 - `void lowlevelMoveCursorX(uint x, uint y)` - AnsiDisplay переопределяет.
 - `void lowlevelCursorSize(int /*size*/)` - 
 - `void lowlevelFlush()` - AnsiDisplay переопределяет.
 - `bool screenChanged()` - 


## getScreenSize


Базовая реализация возвращает нулевой размер экрана.

tvision/include/tvision/internal/platform.h:24
```cpp
    virtual TPoint getScreenSize() noexcept { return {}; }
    virtual int getCaretSize() noexcept { return 0; } // Range [0, 100].
```

Реализация для `NcursesDisplay` возвращает размер экрана, получаемый через функцию [getmaxyx](https://www.opennet.ru/man.shtml?topic=getmaxyx&category=3&russian=1).

tvision/source/platform/ncurdisp.cpp:60
```cpp
TPoint NcursesDisplay::getScreenSize() noexcept
{
    int y, x;
    getmaxyx(stdscr, y, x);
    return {max(x, 0), max(y, 0)};
}
```

Реализация для `Win32Display` возвращает полученный и сохранённый в методе `reloadScreenInfo()` размер. `io` - член класса `TerminalDisplay` типа `StdioCtl` - хранящего хэндлы IO-потоков консоли.

tvision/source/platform/win32con.cpp:287
```cpp
TPoint Win32Display::getScreenSize() noexcept
{
    return size;
}
```

tvision/source/platform/win32con.cpp:257
```cpp
void Win32Display::reloadScreenInfo() noexcept
{
    size = io.getSize();
```



## getCaretSize

Базовая реализация возвращает нулевой размер каретки (каретка не отображается).

tvision/include/tvision/internal/platform.h:25
```cpp
    virtual int getCaretSize() noexcept { return 0; } // Range [0, 100].
    virtual void clearScreen() noexcept {}
```

Реализация для `NcursesDisplay` возвращает размер курсора при помощи функции [curs_set](https://pubs.opengroup.org/onlinepubs/7908799/xcurses/curs_set.html).

|Value of visibility|Appearance of Cursor|Рез-т getCaretSize|
|:---:|---|---|
|0|Invisible|0|
|1|Terminal-specific normal mode|1|
|2|Terminal-specific high visibility mode|100|

tvision/source/platform/ncurdisp.cpp:67
```cpp
int NcursesDisplay::getCaretSize() noexcept
{
    int size = curs_set(0);
    curs_set(size);
    return size <= 0 ? 0 : size == 1 ? 1 : 100;
}
```

Реализация для `Win32Display` возвращает размер курсора от 0 до 100 (где 0 - курсор выключен):
> Процент ячейки символа, заполненной курсором. Это значение составляет от 1 до 100. Внешний вид курсора зависит от полного заполнения ячейки до отображения в виде горизонтальной линии в нижней части ячейки.
> Хотя значение dwSize обычно составляет от 1 до 100, при некоторых обстоятельствах может быть возвращено значение за пределами этого диапазона. Например, если CursorSize имеет значение 0 в реестре, возвращаемое значение dwSize будет равно 0.

См. описание структуры [CONSOLE_CURSOR_INFO](https://learn.microsoft.com/ru-ru/windows/console/console-cursor-info-str).

tvision/source/platform/win32con.cpp:292
```cpp
int Win32Display::getCaretSize() noexcept
{
    CONSOLE_CURSOR_INFO crInfo {};
    GetConsoleCursorInfo(io.out(), &crInfo);
    return crInfo.bVisible ? crInfo.dwSize : 0;
}
```



## clearScreen

Базовая реализация ничего не делает.

tvision/include/tvision/internal/platform.h:26
```cpp
    virtual void clearScreen() noexcept {}
    virtual ushort getScreenMode() noexcept { return 0; }
```

Реализация для `NcursesDisplay` очищает экран при помощи вызова [wclear](https://www.opennet.ru/man.shtml?topic=wclear&category=3&russian=1).

tvision/source/platform/ncurdisp.cpp:79
```cpp
void NcursesDisplay::clearScreen() noexcept { wclear(stdscr); }
void NcursesDisplay::lowlevelMoveCursor(uint x, uint y) noexcept { wmove(stdscr, y, x); }
```

Реализация для `Win32Display` очищает экран заполнением атрибутом 0x07 (белый на черном фоне) и символом пробела при помощи функций
[FillConsoleOutputAttribute](https://learn.microsoft.com/ru-ru/windows/console/fillconsoleoutputattribute) / [FillConsoleOutputCharacterA](https://learn.microsoft.com/ru-ru/windows/console/fillconsoleoutputcharacter).

tvision/source/platform/win32con.cpp:323
```cpp
void Win32Display::clearScreen() noexcept
{
    COORD coord = {0, 0};
    DWORD length = size.x * size.y;
    BYTE attr = 0x07;
    DWORD read;
    FillConsoleOutputAttribute(io.out(), attr, length, coord, &read);
    FillConsoleOutputCharacterA(io.out(), ' ', length, coord, &read);
    lastAttr = attr;
}
```



## getScreenMode

Базовая реализация возвращает "пустой" нулевой режим.

tvision/include/tvision/internal/platform.h:27
```cpp
    virtual ushort getScreenMode() noexcept { return 0; }
    virtual void reloadScreenInfo() noexcept {}
```

Реализации для `NcursesDisplay` и для `Win32Display` не реализуют данный метод самостоятельно, а наследуют реализацию из класса `TerminalDisplay`:

tvision/source/platform/termdisp.cpp:134
```cpp
ushort TerminalDisplay::getScreenMode() noexcept
{
    ushort mode;
    if (termcap.colors == NoColor)
        mode = TDisplay::smMono;
    else
        mode = TDisplay::smCO80;

    if (termcap.colors == Direct)
        mode |= TDisplay::smColor256 | TDisplay::smColorHigh;
    else if (termcap.colors == Indexed256)
        mode |= TDisplay::smColor256;

    TPoint fontSize = io.getFontSize();
    if (fontSize.x > 0 && fontSize.y > 0 && fontSize.x >= fontSize.y)
        mode |= TDisplay::smFont8x8;

    return mode;
}
```

Возможные флаги:

tvision/include/tvision/system.h:415
```cpp
    enum videoModes
        {
        smBW80      = 0x0002,
        smCO80      = 0x0003,
        smMono      = 0x0007,
        smFont8x8   = 0x0100,
        smColor256  = 0x0200,
        smColorHigh = 0x0400,
        smUpdate    = 0x8000,
        };
```


tvision/include/tvision/internal/termdisp.h:46
```cpp
    void initCapabilities() noexcept
    {
        termcap = getCapabilities();
    }
```

tvision/source/platform/termdisp.cpp:11
```cpp
TermCap TerminalDisplay::getCapabilities() noexcept
{
    TermCap termcap {};

    bool termXterm = false;

#if !defined(TV_BARE_METAL)

    // Allow env var detection if not a bare metal

#if defined( __BORLANDC__ )
    // TStringView is only for BCC
    auto colorterm = getEnv<TStringView>("COLORTERM");
    if (colorterm == "truecolor" || colorterm == "24bit")
    {
        termcap.colors = Direct;
    }
#else
    // Use normal std::string_view
    // TERM in Debian displays "xterm-256color"
    // COLORTERM in Debian is empty

    // Changing detection of terminal capabilities by environment vars
    auto termStr      = getEnv<TStringView>("TERM");
    auto colortermStr = getEnv<TStringView>("COLORTERM");

    bool term256c  = false;
    bool term24bit = false;
    if (!termStr.empty())
    {
        auto pos = termStr.find("xterm", 0);
        if (pos==0)
        {
            // found at start
            termXterm = true;
        }

        pos = termStr.find("256color", 0);
        if (pos!=termStr.npos)
        {
            term256c = true;
        }

        pos = termStr.find("truecolor", 0);
        if (pos!=termStr.npos)
        {
            term24bit = true;
        }

        pos = termStr.find("24bit", 0);
        if (pos!=termStr.npos)
        {
            term24bit = true;
        }
    }

    if (!colortermStr.empty())
    {
        auto pos = colortermStr.find("256color", 0);
        if (pos!=colortermStr.npos)
        {
            term256c = true;
        }

        pos = colortermStr.find("truecolor", 0);
        if (pos!=colortermStr.npos)
        {
            term24bit = true;
        }

        pos = colortermStr.find("24bit", 0);
        if (pos!=colortermStr.npos)
        {
            term24bit = true;
        }

    }


    if (term24bit)
    {
        termcap.colors = Direct;
    }
    else if (term256c)
    {
        termcap.colors = Indexed256;
    }
#endif

    else

#endif // !defined(TV_BARE_METAL)

    // Common term capabilities detection
    {
        int colors = getColorCount();
        if (colors >= 256*256*256)
            termcap.colors = Direct;
        else if (colors >= 256)
            termcap.colors = Indexed256;
        else if (colors >= 16)
            termcap.colors = Indexed16;
        else if (colors >= 8)
        {
            termcap.colors = Indexed8;
            termcap.quirks |= qfBoldIsBright;
#if !defined(TV_BARE_METAL)
#ifdef __linux__
            if (io.isLinuxConsole())
                termcap.quirks |= qfBlinkIsBright | qfNoItalic | qfNoUnderline;
            else
#endif // __linux__
#endif // !defined(TV_BARE_METAL)
            //if (getEnv<TStringView>("TERM") == "xterm")
            if (termXterm)
                // Let's assume all terminals disguising themselves as 'xterm'
                // support at least 16 colors.
                termcap.colors = Indexed16;
        }
    }
    return termcap;
}
```






Классы, производные от `TerminalDisplay` должны вызывать `initCapabilities()` в конструкторе. `initCapabilities()`, в свою очередь, вызывает `getCapabilities()`, которая на основании
переменных окружения настраивает количество цветов терминала (**тут надо переделать** - добавить условную компиляцию для `TV_BARE_METAL` и избавится от переменных окружения, 
также надо сделать не прямое сравнение, а поиск подстрок - так, переменная `TERM` в Debian консоли возвращает строку "xterm-256color", а переменная `COLORTERM` - пуста).

Если не получается настроить при помощи переменных окружения, вызывается метод `virtual int TerminalDisplay::getColorCount() noexcept` - `BareMetallDisplay` должен его переопределять. 
Версия для `TV_BARE_METAL` - 16 цветов, если не задано `TV_BARE_METAL_TRUE_COLOR`.

Также в `TerminalDisplay::getScreenMode()` вызывается `io.getFontSize()` - не очень пока понятно, зачем.



## reloadScreenInfo

tvision/include/tvision/internal/platform.h:28
```cpp
    virtual void reloadScreenInfo() noexcept {}
    virtual void lowlevelWriteChars(TStringView /*chars*/, TColorAttr /*attr*/) noexcept {}
```

tvision/source/platform/ncurdisp.cpp:47
```cpp
void NcursesDisplay::reloadScreenInfo() noexcept
{
    TPoint size = io.getSize();
    // When Ncurses is not used for drawing (e.g. AnsiDisplay<NcursesDisplay>),
    // 'resizeterm' causes terrible flickering, so we better use 'resize_term'.
    // However, when Ncurses is used for drawing, 'resizeterm' is necessary, as
    // the screen becomes garbled otherwise.
    if (usesNcursesDraw)
        resizeterm(size.y, size.x);
    else
        resize_term(size.y, size.x);
}
```

tvision/source/platform/win32con.cpp:257
```cpp
void Win32Display::reloadScreenInfo() noexcept
{
    size = io.getSize();
    CONSOLE_SCREEN_BUFFER_INFO sbInfo {};
    GetConsoleScreenBufferInfo(io.out(), &sbInfo);
    // Set the cursor temporally to (0, 0) to prevent the console from crashing
    // due to https://github.com/microsoft/terminal/issues/7511.
    auto curPos = sbInfo.dwCursorPosition;
    SetConsoleCursorPosition(io.out(), {0, 0});
    // Make sure the buffer size matches the viewport size so that the
    // scrollbars are not shown.
    SetConsoleScreenBufferSize(io.out(), {(short) size.x, (short) size.y});
    // Restore the cursor position (it does not matter if it is out of bounds).
    SetConsoleCursorPosition(io.out(), curPos);
}
```

Для `BareMetallDisplay` сделаем примерно как для `Win32Display`. `reloadScreenInfo()` будет возвращать фикс. значения, если задано `TV_BARE_METAL_DISPLAY_SIZE_80_25` или `TV_BARE_METAL_DISPLAY_SIZE_132_40`,
или надо самому реализовать метод `reloadScreenInfo()`, если задано `TV_BARE_METAL_DISPLAY_SIZE_CUSTOM`.



## lowlevelWriteChars

tvision/include/tvision/internal/platform.h:29
```cpp
    virtual void lowlevelWriteChars(TStringView /*chars*/, TColorAttr /*attr*/) noexcept {}
    virtual void lowlevelMoveCursor(uint /*x*/, uint /*y*/) noexcept {};
```

tvision/source/platform/ncurdisp.cpp:128
```cpp
void NcursesDisplay::lowlevelWriteChars(TStringView chars, TColorAttr attr) noexcept
{
    usesNcursesDraw = true;
    // Translate and apply text attributes.
    uint curses_attr = translateAttributes(attr);
    wattron(stdscr, curses_attr);
    // Print characters.
    waddnstr(stdscr, chars.data(), chars.size());
    wattroff(stdscr, curses_attr);
}
```

tvision/source/platform/win32con.cpp:336
```cpp
void Win32Display::lowlevelWriteChars(TStringView chars, TColorAttr attr) noexcept
{
    uchar bios = attr.toBIOS();
    if (bios != lastAttr)
    {
        lowlevelFlush();
        SetConsoleTextAttribute(io.out(), bios);
        lastAttr = bios;
    }
    buf.insert(buf.end(), chars.data(), chars.data()+chars.size());
}
```

tvision/source/platform/ansidisp.cpp:95
```cpp
void AnsiDisplayBase::lowlevelWriteChars( TStringView chars, TColorAttr attr,
                                          const TermCap &termcap ) noexcept
{
    buf.reserve(256);
    buf.tail = convertAttributes(attr, lastAttr, termcap, buf.tail);
    buf.push(chars);
}
```

tvision/include/tvision/internal/ansidisp.h:142
```cpp
    void lowlevelWriteChars(TStringView chars, TColorAttr attr) noexcept override
        { AnsiDisplayBase::lowlevelWriteChars(chars, attr, TerminalDisplay::termcap); }
```




## lowlevelMoveCursor

tvision/include/tvision/internal/platform.h:30
```cpp
    virtual void lowlevelMoveCursor(uint /*x*/, uint /*y*/) noexcept {};
    virtual void lowlevelMoveCursorX(uint x, uint y) noexcept { lowlevelMoveCursor(x, y); }
```

tvision/source/platform/ncurdisp.cpp:80
```cpp
void NcursesDisplay::lowlevelMoveCursor(uint x, uint y) noexcept { wmove(stdscr, y, x); }
void NcursesDisplay::lowlevelFlush() noexcept { wrefresh(stdscr); }
```

tvision/source/platform/win32con.cpp:348
```cpp
void Win32Display::lowlevelMoveCursor(uint x, uint y) noexcept
{
    lowlevelFlush();
    SetConsoleCursorPosition(io.out(), {(short) x, (short) y});
}
```

tvision/source/platform/ansidisp.cpp:109
```cpp
void AnsiDisplayBase::lowlevelMoveCursor(uint x, uint y) noexcept
{
    buf.reserve(32);
//     buf.push('\r'); // Make dumps readable.
    bufWriteCSI2(y + 1, x + 1, 'H');
}
```

tvision/include/tvision/internal/ansidisp.h:144
```cpp
    void lowlevelMoveCursor(uint x, uint y) noexcept override
        { AnsiDisplayBase::lowlevelMoveCursor(x, y); }
```


## lowlevelMoveCursorX

По умолчанию вызывается реализация `lowlevelMoveCursor`:

tvision/include/tvision/internal/platform.h:31
```cpp
    virtual void lowlevelMoveCursorX(uint x, uint y) noexcept { lowlevelMoveCursor(x, y); }
    virtual void lowlevelCursorSize(int /*size*/) noexcept {};
```

Для `AnsiDisplay` производится оптимизация:

tvision/source/platform/ansidisp.cpp:103
```cpp
void AnsiDisplayBase::lowlevelMoveCursorX(uint x, uint) noexcept
{
    // Optimized case where the cursor only moves horizontally.
    bufWriteCSI1(x + 1, 'G');
}
```

tvision/include/tvision/internal/ansidisp.h:146
```cpp
    void lowlevelMoveCursorX(uint x, uint y) noexcept override
        { AnsiDisplayBase::lowlevelMoveCursorX(x, y); }
```


## lowlevelCursorSize

Базовая реализация ничего не делает.

tvision/include/tvision/internal/platform.h:32
```cpp
    virtual void lowlevelCursorSize(int /*size*/) noexcept {};
    virtual void lowlevelFlush() noexcept {};
```

tvision/source/platform/ncurdisp.cpp:83
```cpp
void NcursesDisplay::lowlevelCursorSize(int size) noexcept
{
/* The caret is the keyboard cursor. If size is 0, the caret is hidden. The
 * other possible values are from 1 to 100, theoretically, and represent the
 * percentage of the character cell the caret fills.
 * https://docs.microsoft.com/en-us/windows/console/console-cursor-info-str
 *
 * ncurses supports only three levels: invisible (0), normal (1) and
 * very visible (2). They don't make a difference in all terminals, but
 * we can try mapping them to the values requested by Turbo Vision. */
    curs_set(size > 0 ? size == 100 ? 2 : 1 : 0); // Implies refresh().
}
```

tvision/source/platform/win32con.cpp:310
```cpp
void Win32Display::lowlevelCursorSize(int size) noexcept
{
    CONSOLE_CURSOR_INFO crInfo;
    if (size) {
        crInfo.bVisible = TRUE;
        crInfo.dwSize = size;
    } else {
        crInfo.bVisible = FALSE;
        crInfo.dwSize = 1;
    }
    SetConsoleCursorInfo(io.out(), &crInfo);
}
```

Для `AnsiDisplay` ничего не делается, но вообще-то команды такие есть в протоколе, как минимум, показать и спрятать 
каретку - **надо доделать** ([CSI ?25l/CSI ?25h](https://ru.wikipedia.org/wiki/%D0%A3%D0%BF%D1%80%D0%B0%D0%B2%D0%BB%D1%8F%D1%8E%D1%89%D0%B8%D0%B5_%D0%BF%D0%BE%D1%81%D0%BB%D0%B5%D0%B4%D0%BE%D0%B2%D0%B0%D1%82%D0%B5%D0%BB%D1%8C%D0%BD%D0%BE%D1%81%D1%82%D0%B8_ANSI)).


## lowlevelFlush

tvision/include/tvision/internal/platform.h:33
```cpp
    virtual void lowlevelFlush() noexcept {};
    virtual bool screenChanged() noexcept { return false; }
```

tvision/source/platform/ncurdisp.cpp:81
```cpp
void NcursesDisplay::lowlevelFlush() noexcept { wrefresh(stdscr); }

void NcursesDisplay::lowlevelCursorSize(int size) noexcept
{
/* The caret is the keyboard cursor. If size is 0, the caret is hidden. The
 * other possible values are from 1 to 100, theoretically, and represent the
 * percentage of the character cell the caret fills.
 * https://docs.microsoft.com/en-us/windows/console/console-cursor-info-str
 *
 * ncurses supports only three levels: invisible (0), normal (1) and
 * very visible (2). They don't make a difference in all terminals, but
 * we can try mapping them to the values requested by Turbo Vision. */
    curs_set(size > 0 ? size == 100 ? 2 : 1 : 0); // Implies refresh().
}
```

tvision/source/platform/win32con.cpp:354
```cpp
void Win32Display::lowlevelFlush() noexcept
{
    io.write(buf.data(), buf.size());
    buf.resize(0);
}
```

tvision/source/platform/ansidisp.cpp:116
```cpp
void AnsiDisplayBase::lowlevelFlush() noexcept
{
    io.write(buf.data(), buf.size());
    buf.clear();
}
```

tvision/include/tvision/internal/ansidisp.h:148
```cpp
    void lowlevelFlush() noexcept override
        { AnsiDisplayBase::lowlevelFlush(); }
```


## screenChanged

tvision/include/tvision/internal/platform.h:34
```cpp
    virtual bool screenChanged() noexcept { return false; }
};

class InputStrategy : public EventSource
{
public:

    InputStrategy(SysHandle aHandle) noexcept :
        EventSource(aHandle)
    {
    }
```

tvision/source/platform/termdisp.cpp:154
```cpp
bool TerminalDisplay::screenChanged() noexcept
{
    TPoint size = io.getSize();
    bool changed = (size != lastSize);
    lastSize = size;
    return changed;
}
```

tvision/source/platform/win32con.cpp:273
```cpp
bool Win32Display::screenChanged() noexcept
{
    bool changed = TerminalDisplay::screenChanged();
    CONSOLE_FONT_INFO fontInfo;
    if ( GetCurrentConsoleFont(io.out(), FALSE, &fontInfo)
         && memcmp(&fontInfo, &lastFontInfo, sizeof(fontInfo)) != 0 )
    {
        changed = true;
        WinWidth::reset();
        lastFontInfo = fontInfo;
    }
    return changed;
}
```

Для `NcursesDisplay` реализации нет, там каким-то образом (не разбирался детально) используется сигнал `SIGWINCH`.




