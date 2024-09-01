#pragma once


#define TV_Disable_WinGr_Driver

#if defined(_WIN32) || defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif


#if 0

#ifdef __cplusplus

    #include <cstdint>
    #include <streambuf>
    #include <iostream>
    #include <iomanip>
    #include <cstring>

#else

    #include <stdint.h>
    #include <string.h>

#endif


#include <unistd.h>

#if defined(_WIN32) || defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

// #include <iostream>
// using namespace std;


// using CLY_ssize_t


#define CLY_DoNotDefineSizedTypes  1

#ifdef __cplusplus

    using uint64 = std::uint64_t;
    using int64  = std::int64_t ;
    
    using uint32 = std::uint32_t;
    using int32  = std::int32_t ;
    
    using uint16 = std::uint16_t;
    using int16  = std::int16_t ;
    
    using uint8  = std::uint8_t ;
    using int8   = std::int8_t  ;
    
    typedef bool Boolean;

    using std::memset;

    using std::cout;
    using std::cerr;
    using std::endl;

#else

    typedef uint64_t uint64;
    typedef int64_t  int64 ;
                    
    typedef uint32_t uint32;
    typedef int32_t  int32 ;
                    
    typedef uint16_t uint16;
    typedef int16_t  int16 ;
                    
    typedef uint8_t  uint8 ;
    typedef int8_t   int8  ;
    
#endif

// constexpr const static inline bool False = false;
// constexpr const static inline bool True  = true ;

#define False false
#define True  true



// https://habr.com/ru/articles/326578/

#ifdef __cplusplus

    #ifndef CLY_streambuf
        //#define CLY_streambuf      std::streambuf
        using CLY_streambuf   = std::streambuf;
    #endif
    
    #ifndef CLY_filebuf
        using CLY_filebuf     = std::filebuf;
    #endif
    
    #ifndef CLY_int_filebuf
        using CLY_int_filebuf = std::filebuf;
    #endif
    
    #ifndef CLY_StreamPosT
        using CLY_StreamPosT  = std::streambuf::pos_type;
    #endif
    
    #ifndef CLY_StreamOffT
        using CLY_StreamOffT  = std::streambuf::off_type;
    #endif
    
    #ifndef CLY_IOSSeekDir
        using CLY_IOSSeekDir  = std::ios::seekdir;
    #endif
    
    #ifndef CLY_OpenModeT
        using CLY_OpenModeT   = std::ios::openmode;
    #endif
    
    #ifndef CLY_IOSBin
      #define CLY_IOSBin         std::ios::binary
    #endif
    
    #ifndef CLY_IOSOut
      #define CLY_IOSOut         std::ios::out
    #endif
    
    #ifndef CLY_IOSIn
        #define CLY_IOSIn std::ios::in
    #endif
    
    #ifndef CLY_IOSApp
        #define CLY_IOSApp std::ios::app
    #endif
    
    #ifndef CLY_IOSAtE
        #define CLY_IOSAtE std::ios::ate
    #endif
    
    #ifndef CLY_IOSBeg
        #define CLY_IOSBeg std::ios::beg
    #endif
    
    #ifndef CLY_IOSCur
        #define CLY_IOSCur std::ios::cur
    #endif
    
    #ifndef CLY_IOSEnd
        #define CLY_IOSEnd std::ios::end
    #endif
    
    #ifndef CLY_IOSBadBit
        #define CLY_IOSBadBit std::ios::badbit
    #endif
    
    #ifndef CLY_IOSEOFBit
        #define CLY_IOSEOFBit std::ios::eofbit
    #endif
    
    #ifndef CLY_IOSFailBit
        #define CLY_IOSFailBit std::ios::failbit
    #endif
    
    #ifndef CLY_IOSGoodBit
        #define CLY_IOSGoodBit std::ios::goodbit
    #endif
    
    
    #ifndef DeleteArray
    
        template<typename DataType> inline
        void DeleteArray(const DataType *pA)
        {
            delete[] pA;
        }
    
    #endif

#endif



#ifndef CLY_Packed
    #ifdef __GNUC__
        //#error "AAA"
        #define CLY_Packed __attribute__((packed))
    #endif
#endif

#ifndef CLY_FBOpenProtDef
    #define CLY_FBOpenProtDef  0
#endif

#ifndef usleep
    #if defined(_WIN32) || defined(WIN32)
        //#define usleep(microseconds) // ::Sleep((microseconds)/1000)
        #define usleep(microseconds) usleep(microseconds)
    #endif
#endif




#endif


#ifdef __cplusplus
        inline 
        void delay(unsigned ms)
        {
#if defined(_WIN32) || defined(WIN32)
            ::Sleep(ms);
#endif
        }
#endif


