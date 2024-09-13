/*---------------------------------------------------------*/
/*                                                         */
/*   Gadgets.h : Header file for gadgets.cpp               */
/*                                                         */
/*---------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#if !defined( TV_INC_GADGETS_H )
#define TV_INC_GADGETS_H

class THeapView:public TView {
      public:
	THeapView(TRect & r);
	virtual void update();
	virtual void draw();
	virtual long heapSize();

      private:
	long oldMem, newMem;
	char heapStr[16];
};

class TClockView:public TView {
      public:
	TClockView(TRect & r);
	virtual void draw();
	virtual void update();

      private:
	char lastTime[9];
	char curTime[9];
};

#endif // TV_INC_GADGETS_H