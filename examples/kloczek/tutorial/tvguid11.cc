/*! \file
    \brief Same as tvguid10 except for added dialog box
*/
/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision 1.0                                      */
/*   TVGUID11 Demo Source File                             */
/*   Copyright (c) 1991 by Borland International           */
/*                                                         */
/*---------------------------------------------------------*/

// same as tvguid10 except for added dialog box
// modify TMyApp

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
#define Uses_TDialog
#include <tv.h>

#include <stdlib.h>		// for exit(), random()
#include <iostream>
#include <fstream>		// for ifstream
#include <stdio.h>		// for puts() etc
#include <string.h>		// for strlen etc
#include <ctype.h>

#include "tvgui.h"

// note the extra #define above

const int cmMyFileOpen = 200;	// assign new command values
const int cmMyNewWin = 201;

// added for dialog menu
const int cmNewDialog = 202;

/* SS: micro change here */

const char *fileToRead = "tvguid11.cc";
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
	void newWindow();
	void newDialog();
	// added member
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
	TInterior *makeInterior(const TRect & r, Boolean left);
	virtual void sizeLimits(TPoint & minP, TPoint & maxP);
	// override TWindow::sizeLimits
      private:
	 TInterior * lInterior, *rInterior;
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
					   "F5") + *new TMenuItem("~D~ialog",
								  cmNewDialog,
								  kbF2,
								  hcNoContext,
								  "F2")
			    // new dialog menu added here
	    );
}

void TMyApp::handleEvent(TEvent & event)
{
	TApplication::handleEvent(event);
	if (event.what == evCommand) {
		switch (event.message.command) {
		case cmMyNewWin:
			newWindow();
			break;
		case cmNewDialog:
			newDialog();
			break;
		default:
			return;
		}
		clearEvent(event);	// clear event after handling
	}
}

void TMyApp::newWindow()
{
	TRect r(0, 0, 45, 13);	// set initial size and position

	/* SS: micro change here */

	//r.move( random(34), random(11) );     // randomly move around screen
	r.move(rand() % 34, rand() % 11);	// randomly move around screen
	TDemoWindow *window = new TDemoWindow(r, "Demo Window", ++winNumber);
	deskTop->insert(window);	// put window into desktop and draw it
}

// TMyApp needs newDialog member

void TMyApp::newDialog()
{
	TRect r(0, 0, 40, 13);

	/* SS: micro change here */

	//r.move( random(39), random(10) );
	r.move(rand() % 39, rand() % 10);
	deskTop->insert(new TDialog(r, "Demo Dialog"));
}

TInterior::TInterior(const TRect & bounds, TScrollBar *aHScrollBar,
		     TScrollBar *aVScrollBar):TScroller(bounds, aHScrollBar,
							aVScrollBar)
{
	options = options | ofFramed;
	setLimit(maxLineLength, lineCount);
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
		if (j < lineCount && lines[j] != nullptr) {
			char s[maxLineLength];
			if (delta.x > (int)strlen(lines[j]))
				s[0] = EOS;
			else {
				strncpy(s, lines[j] + delta.x, size.x);
				s[size.x] = EOS;
			}
			b.moveCStr(0, s, color);
		}
		writeLine(0, i, size.x, 1, b);
	}
}

TDemoWindow::TDemoWindow(const TRect & bounds, const char *aTitle,
			 short aNumber):TWindowInit(&TDemoWindow::initFrame),
TWindow(bounds, aTitle, aNumber)
{
	TRect lbounds = getExtent();
	TRect r(lbounds.a.x, lbounds.a.y, lbounds.b.x / 2 + 1, lbounds.b.y);
	lInterior = makeInterior(r, True);
	lInterior->growMode = gfGrowHiY;
	insert(lInterior);
	// creates left-side scrollable interior and inserts into window
	r = TRect(lbounds.b.x / 2, lbounds.a.y, lbounds.b.x, lbounds.b.y);
	rInterior = makeInterior(r, False);
	rInterior->growMode = gfGrowHiX | gfGrowHiY;
	insert(rInterior);
	// likewise for right-side scroller
}

TInterior *TDemoWindow::makeInterior(const TRect & bounds, Boolean left)
{
	TRect r =
	    TRect(bounds.b.x - 1, bounds.a.y + 1, bounds.b.x, bounds.b.y - 1);
	TScrollBar *vScrollBar = new TScrollBar(r);
	if (vScrollBar == nullptr) {
		std::cout << "vScrollbar init error" << std::endl;
		exit(1);
	}
	// production code would display error dialog box
	vScrollBar->options |= ofPostProcess;
	if (left)
		vScrollBar->growMode = gfGrowHiY;
	insert(vScrollBar);

	r = TRect(bounds.a.x + 2, bounds.b.y - 1, bounds.b.x - 2, bounds.b.y);
	TScrollBar *hScrollBar = new TScrollBar(r);
	if (hScrollBar == nullptr) {
		std::cout << "hScrollbar init error" << std::endl;
		exit(1);
	}
	hScrollBar->options |= ofPostProcess;
	if (left)
		hScrollBar->growMode = (gfGrowHiY | gfGrowLoY);
	insert(hScrollBar);

	r = bounds;
	r.grow(-1, -1);
	return new TInterior(r, hScrollBar, vScrollBar);
}

void TDemoWindow::sizeLimits(TPoint & minP, TPoint & maxP)
{
	TWindow::sizeLimits(minP, maxP);
	minP.x = lInterior->size.x + 9;
}

void readFile(const char *fileName)
{
	std::ifstream fileToView(fileName);
	if (!fileToView) {
		std::cout << "Invalid file name..." << std::endl;
		//exit(1);
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
