/* Подавляем GCC-шные варнинги

[build] C:/PROGRA~2/ARMGNU~1/13C7F1~1.2RE/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe: C:/PROGRA~2/ARMGNU~1/13C7F1~1.2RE/bin/../lib/gcc/arm-none-eabi/13.2.1\libg.a(libc_a-closer.o): in function `_close_r':
[build] closer.c:(.text._close_r+0x18): warning: _close is not implemented and will always fail


Не очень понятно, с чего они лезут.

Рецепты в интернетах предлагают использовать ключик -ffreestanding.
Но если его использовать, то вылезает такая хрень:

build] In file included from C:/work/projects/firmware/UP_1023_CPU_Example/Lib/Framework/Hardware/STM32F4xx/exti.cpp:5:
[build] C:/work/projects/firmware/UP_1023_CPU_Example/Lib/Framework/Common/bitutils.h:296:37: error: 'allocator' in namespace 'std' does not name a template type
[build]   296 | template<typename TALLOCATOR = std::allocator<uint32_t>>
[build]       |                                     ^~~~~~~~~
[build] C:/work/projects/firmware/UP_1023_CPU_Example/Lib/Framework/Common/bitutils.h:9:1: note: 'std::allocator' is defined in header '<memory>'; did you forget to '#include <memory>'?

При этом 
#include <memory>

Явно было добавлено, и не помогло.

*/

void _close_r()
{}

void _fstat_r()
{}

void _getpid_r()
{}

void _isatty_r()
{}

void _kill_r()
{}

void _lseek_r()
{}

void _read_r()
{}

void _write_r()
{}

