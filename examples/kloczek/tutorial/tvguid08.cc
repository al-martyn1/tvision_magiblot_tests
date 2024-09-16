/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision 1.0                                      */
/*! \file \brief TVGUID08 Demo Source File                 */
/*   Copyright (c) 1991 by Borland International           */
/*                                                         */
/*---------------------------------------------------------*/

// same as tvguid07 except for scrolling interior
// add TDemoWindow::makeInterior

#define Uses_TEventQueue
#define Uses_TEvent
#define Uses_TProgram
#define Uses_TApplication
#define Uses_TKeys
#define Uses_TRect
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TView
#define Uses_TWindow
#define Uses_TScroller
#define Uses_TScrollBar
#include <tv.h>

#include <stdlib.h>		// for exit(), random()
#include <iostream>
#include <fstream>		// for ifstream
#include <stdio.h>		// for puts() etc
#include <string.h>		// for strlen etc
#include <ctype.h>

#include "tvgui.h"

const int cmMyFileOpen = 200;	// assign new command values
const int cmMyNewWin = 201;

/* SS: micro change here */

const char *fileToRead = "tvguid08.cc";
const int maxLineLength = maxViewWidth + 1;
const int maxLines = 100;
char *lines[maxLines];
int lineCount = 0;

class TMyApp:public TApplication {
      public:
	TMyApp();
	static TStatusLine *initStatusLine(TRect r);
	static TMenuBar *initMenuBar(TRect r);
	virtual void handleEvent(TEvent & event);
	void myNewWindow();
};

class TInterior:public TScroller {
      public:
	TInterior(const TRect & bounds, TScrollBar * aHScrollBar, TScrollBar * aVScrollBar);	// constructor
	virtual void draw();	// override TView::draw
};

static short winNumber = 0;	// initialize window number

class TDemoWindow:public TWindow	// define a new window class
{
      public:
	TDemoWindow(const TRect & bounds, const char *aTitle, short aNumber);
	void makeInterior();
};

TMyApp::TMyApp():
TProgInit(&TMyApp::initStatusLine, &TMyApp::initMenuBar, &TMyApp::initDeskTop)
{
}

TStatusLine *TMyApp::initStatusLine(TRect r)
{
	r.a.y = r.b.y - 1;	// move top to 1 line above bottom
	return new TStatusLine(r, *new TStatusDef(0, 0xFFFF) +
			       *new TStatusItem(nullptr, kbF10, cmMenu) +
			       *new TStatusItem("~Alt-X~ Exit", kbAltX,
						cmQuit) +
			       *new TStatusItem("~Alt-F3~ Close", kbAltF3,
						cmClose)
	    );
}

TMenuBar *TMyApp::initMenuBar(TRect r)
{
	r.b.y = r.a.y + 1;	// set bottom line 1 line below top line
	return new TMenuBar(r,
			    *new TSubMenu("~F~ile", kbAltF) +
			    *new TMenuItem("~O~pen", cmMyFileOpen, kbF3,
					   hcNoContext,
					   "F3") + *new TMenuItem("~N~ew",
								  cmMyNewWin,
								  kbF4,
								  hcNoContext,
								  "F4") +
			    newLine() + *new TMenuItem("E~x~it", cmQuit, cmQuit,
						       hcNoContext,
						       "Alt-X") +
			    *new TSubMenu("~W~indow",
					  kbAltW) + *new TMenuItem("~N~ext",
								   cmNext, kbF6,
								   hcNoContext,
								   "F6") +
			    *new TMenuItem("~Z~oom", cmZoom, kbF5, hcNoContext,
					   "F5")
	    );
}

void TMyApp::handleEvent(TEvent & event)
{
	TApplication::handleEvent(event);	// act like base!
	if (event.what == evCommand) {
		switch (event.message.command) {
		case cmMyNewWin:	// but respond to additional commands
			myNewWindow();	// define action for cmMyNewWin command
			break;
		default:
			return;
		}
		clearEvent(event);	// clear event after handling
	}
}

void TMyApp::myNewWindow()
{
	TRect r(0, 0, 26, 7);	// set initial size and position

	/* SS: micro change here */

	//r.move( random(53), random(16) );     // randomly move around screen
	r.move(rand() % 53, rand() % 16);	// randomly move around screen
	TDemoWindow *window = new TDemoWindow(r, "Demo Window", ++winNumber);
	deskTop->insert(window);	// put window into desktop and draw it
}

TInterior::TInterior(const TRect & bounds, TScrollBar *aHScrollBar,
		     TScrollBar *aVScrollBar):TScroller(bounds, aHScrollBar,
							aVScrollBar)
{
	growMode = gfGrowHiX | gfGrowHiY;
	options = options | ofFramed;
	setLimit(maxLineLength, maxLines);
}

void TInterior::draw()		// modified for scroller
{
	ushort color = getColor(0x0301);
	for (int i = 0; i < size.y; i++)
		// for each line:
	{
		TDrawBuffer b;
		b.moveChar(0, ' ', color, size.x);
		// fill line buffer with spaces
		int j = delta.y + i;	// delta is scroller offset
		if (lines[j]) {
			char s[maxLineLength];
			if (delta.x > (int)strlen(lines[j]))
				s[0] = EOS;
			else {
				strncpy(s, lines[j] + delta.x, size.x);
				s[size.x] = EOS;
			}
			b.moveStr(0, s, color);
		}
		writeLine(0, i, size.x, 1, b);
	}
}

TDemoWindow::TDemoWindow(const TRect & bounds, const char *aTitle,
			 short aNumber):TWindowInit(&TDemoWindow::initFrame),
TWindow(bounds, aTitle, aNumber)
{
	makeInterior();		// creates scrollable interior and inserts into window
}

void TDemoWindow::makeInterior()
{
	TScrollBar *vScrollBar =
	    standardScrollBar(sbVertical | sbHandleKeyboard);
	TScrollBar *hScrollBar =
	    standardScrollBar(sbHorizontal | sbHandleKeyboard);
	TRect r = getClipRect();	// get exposed view bounds
	r.grow(-1, -1);		// shrink to fit inside window frame
	insert(new TInterior(r, hScrollBar, vScrollBar));
}

void readFile(const char *fileName)
{
	std::ifstream fileToView(fileName);
	if (!fileToView) {
		std::cout << "Invalid file name..." << std::endl;
		exit(1);
	} else {
		char buf[maxLineLength];
		while (lineCount < maxLines &&
		       fileToView.getline(buf, maxLineLength)) {
			lines[lineCount] = newStr(buf);
			lineCount++;
		}
	}
}

void deleteFile()
{
	for (int i = 0; i < lineCount; i++)
		delete lines[i];
}

int main()
{
	readFile(fileToRead);
	TMyApp myApp;
	myApp.run();
	deleteFile();
	return 0;
}
