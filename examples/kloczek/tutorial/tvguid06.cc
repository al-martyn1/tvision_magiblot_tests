/*! \file
    \brief A basic text scrolling window, with imperfect draw method
*/
/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision 1.0                                      */
/*   TVGUID06 Demo Source File                             */
/*   Copyright (c) 1991 by Borland International           */
/*                                                         */
/*---------------------------------------------------------*/

/* imperfect draw method--see tvguid07 for improvement */

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

const char *fileToRead = "tvguid06.cc";
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

class TInterior:public TView {
      public:
	TInterior(const TRect & bounds);	// constructor
	virtual void draw();	// override TView::draw
};

static short winNumber = 0;	// initialize window number

class TDemoWindow:public TWindow	// define a new window class
{
      public:
	TDemoWindow(const TRect & r, const char *aTitle, short aNumber);
	static const char *const name;
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

TInterior::TInterior(const TRect & bounds):TView(bounds)
{
	growMode = gfGrowHiX | gfGrowHiY;	//make size follow that of the window
	options = options | ofFramed;
}

void TInterior::draw()
{
	for (int i = 0; i < size.y; i++)
		writeStr(0, i, lines[i], 1);
}

// const char * const TDemoWindow::name = "TDemoWindow";

TDemoWindow::TDemoWindow(const TRect & bounds, const char *aTitle,
			 short aNumber):TWindowInit(&TDemoWindow::initFrame),
TWindow(bounds, aTitle, aNumber)
{
	TRect r = getClipRect();	// get exposed area
	r.grow(-1, -1);		// make interior fit inside window frame
	insert(new TInterior(r));	// add interior to window
}

void readFile(const char *fileName)
{
	std::ifstream fileToView(fileName);
	if (!fileToView) {
		std::cout << "Invalid file name..." << std::endl;
		// exit(1);
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
