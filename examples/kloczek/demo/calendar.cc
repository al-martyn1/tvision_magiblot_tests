/*---------------------------------------------------------*/
/*                                                         */
/*   Calendar.cpp:  TCalenderWindow member functions.      */
/*                                                         */
/*---------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */
/*
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TRect
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TDrawBuffer
#define Uses_TStreamableClass
#define Uses_TStreamable
#define Uses_TView
#define Uses_TWindow
#include <tv.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "calendar.h"

const char *monthNames[] = {
	"",
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

static unsigned char daysInMonth[] = {
	0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

//
// TCalendarView functions
//

const char *const TCalendarView::name = "TCalendarView";

void TCalendarView::write(opstream & os)
{
	TView::write(os);
	os << days << month << year << curDay << curMonth << curYear;
}

void *TCalendarView::read(ipstream & is)
{
	TView::read(is);
	is >> days >> month >> year >> curDay >> curMonth >> curYear;
	return this;
}

TStreamable *TCalendarView::build()
{
	return new TCalendarView(streamableInit);
}

TStreamableClass RCalendarView(TCalendarView::name,
			       TCalendarView::build, __DELTA(TCalendarView)
    );

TCalendarView::TCalendarView(TRect & r):TView(r)
{
	/* SS: little change */

//    struct date d;

	options |= ofSelectable;
	eventMask |= evMouseAuto;

	time_t now = time(nullptr);
	tm *broken = localtime(&now);

	year = curYear = broken->tm_year + 1900;
	month = curMonth = broken->tm_mon + 1;
	curDay = broken->tm_mday;

//    getdate( &d );
//    year = curYear = d.da_year;
//    month = curMonth = d.da_mon;
//    curDay = d.da_day;

	drawView();
}

int dayOfWeek(int day, int month, int year)
{
	int century, yr, dw;

	if (month < 3) {
		month += 10;
		--year;
	} else
		month -= 2;

	century = year / 100;
	yr = year % 100;
	dw = (((26 * (int)month - 2) / 10) + (int)day + yr + (yr / 4) +
	      (century / 4) - (2 * century)) % 7;

	if (dw < 0)
		dw += 7;

	return ((int)dw);
}

void TCalendarView::draw()
{
	char str[23];
	char current = (char)(1 - dayOfWeek(1, month, year));
	char days = (char)(daysInMonth[month] +
			   ((year % 4 == 0 && month == 2) ? 1 : 0));
	char color, boldColor;
	short i, j;
	TDrawBuffer buf;

	color = getColor(6);
	boldColor = getColor(7);

	buf.moveChar(0, ' ', color, 22);

	buf.moveStr(0, str, color);
	sprintf(str, "%9s %4u \032  \033 ", monthNames[month], year);
	buf.moveStr(0, str, color);
	writeLine(0, 0, 22, 1, buf);

	buf.moveChar(0, ' ', color, 22);
	buf.moveStr(0, "Su Mo Tu We Th Fr Sa", color);
	writeLine(0, 1, 22, 1, buf);

	for (i = 1; i <= 6; i++) {
		buf.moveChar(0, ' ', color, 22);
		for (j = 0; j <= 6; j++) {
			if (current < 1 || current > days)
				buf.moveStr((short)(j * 3), "   ", color);
			else {
				sprintf(str, "%2i", (int)current);
				if (year == curYear && month == curMonth
				    && current == (int)curDay)
					buf.moveStr((short)(j * 3), str,
						    boldColor);
				else
					buf.moveStr((short)(j * 3), str, color);
			}
			current++;
		}
		writeLine(0, (short)(i + 1), 22, 1, buf);
	}
}

void TCalendarView::handleEvent(TEvent & event)
{
	TPoint point;

	TView::handleEvent(event);
	if (state && sfSelected) {
		if ((event.what & evMouse) && (evMouseDown || evMouseAuto)) {
			point = makeLocal(event.mouse.where);
			if (point.x == 15 && point.y == 0) {
				++month;
				if (month > 12) {
					++year;
					month = 1;
				}
				drawView();
			} else if (point.x == 18 && point.y == 0) {
				--month;
				if (month < 1) {
					--year;
					month = 12;
				}
				drawView();
			}
		} else if (event.what == evKeyboard) {
			if ((loByte(event.keyDown.keyCode) == '+') ||
			    event.keyDown.keyCode == kbDown) {
				++month;
				if (month > 12) {
					++year;
					month = 1;
				}
			} else if ((loByte(event.keyDown.keyCode) == '-') ||
				   event.keyDown.keyCode == kbUp) {
				--month;
				if (month < 1) {
					--year;
					month = 12;
				}
			}
			drawView();
		}
	}
}

//
// TCalendarWindow functions
//

const char *const TCalendarWindow::name = "TCalendarWindow";

void TCalendarWindow::write(opstream & os)
{
	TWindow::write(os);
}

void *TCalendarWindow::read(ipstream & is)
{
	TWindow::read(is);
	return this;
}

TStreamable *TCalendarWindow::build()
{
	return new TCalendarWindow(streamableInit);
}

TStreamableClass RCalendarWindow(TCalendarWindow::name,
				 TCalendarWindow::build,
				 __DELTA(TCalendarWindow)
    );

TCalendarWindow::TCalendarWindow():
TWindowInit(&TCalendarWindow::initFrame),
TWindow(TRect(1, 1, 23, 11), "Calendar", wnNoNumber)
{
	TRect r(getExtent());

	flags &= ~(wfZoom | wfGrow);
	growMode = 0;

	palette = wpCyanWindow;

	r.grow(-1, -1);
	insert(new TCalendarView(r));
}
