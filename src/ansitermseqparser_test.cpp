
#include "umba/umba.h"

#define Uses_TKeys

#include <tvision/tv.h>
//
#include <tvision/util.h>
//
#include <tvision/umba/ansiterm/seqparser.h>

#include "umba/simple_formatter.h"

#include <iostream>
#include <string>

using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{

    auto validKeyHandler = [&](ushort keyCode, ushort controlKeyState)
    {
        // std::cout << "KeyCode: ";
        //  /* tvision:: */ printKeyCode(std::cout, keyCode);
        // std::cout << ";  ControlKeyState: ";
        //  /* tvision:: */ printControlKeyState(std::cout, controlKeyState);
        // std::cout << "\n";
    };

    auto invalidKeyHandler = [&](const uchar* pData, size_t size)
    {
        for(std::size_t i=0; i!=size; ++i)
        {
            if (pData[i]==27)
                umba::lout << "ESC ";
            else if (pData[i]<33 || pData[i]>=127)
                umba::lout << umba::omanip::hex << pData[i] << " ";
            else
                umba::lout << (char)pData[i] << " ";
        }
        umba::lout << "\n";
        umba::lout << umba::omanip::flush;
    };

    auto parser = tvision::ansiterm::AnsiTerminalKeySequenceParser(validKeyHandler, invalidKeyHandler);

    std::string str = "\x1B[24~\x1B\x1B[19~";

    parser.putData((const uchar*)str.data(), str.size());
    parser.putTimeout();


    // tvision::ansiterm


        //     << "    .keyCode = ";
        // printConstants(out, ev.keyDown.keyCode, printKeyCode);
        // out << ",\n"
        //     << "    .charScan = CharScanType {\n"
        //     << "      .charCode = " << (int) (uchar) charCode;
        // if (charCode)
        //     out << " ('" << charCode << "')";
        // out << ",\n"
        //     << "      .scanCode = " << (int) (uchar) ev.keyDown.charScan.scanCode << "\n"
        //     << "    },\n"
        //     << "    .controlKeyState = ";
        // printConstants(out, ev.keyDown.controlKeyState, printControlKeyState);

    

    return 0;
}