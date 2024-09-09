#pragma once


#define Uses_stdio
#define Uses_string
#define Uses_stdlib
#define Uses_ctype
#define Uses_unistd

#define Uses_TBackground
#define Uses_TListBox
#define Uses_TMenu
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TScrollBar
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TStringCollection
#define Uses_MsgBox
#define Uses_TEventQueue
#define Uses_TApplication
#define Uses_TRect
#define Uses_TDeskTop
#define Uses_TView
#define Uses_TWindow
#define Uses_TDialog
#define Uses_TButton
#define Uses_StaticText
#define Uses_TSItem
#define Uses_TLabel
#define Uses_TInputLine
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TDrawBuffer
#define Uses_TStreamableClass
#define Uses_TStreamable
#define Uses_TScreen
#define Uses_TVCodePage
#define Uses_TEmbedApplication

#include <tvision/tv.h>

#include "tprogbar.h"

//#include <tv/embedapp.h>


extern void tvInitEmbedApp();
extern ushort tvPollAppExecute();