// This is an example of a progress bar.
//
// Modified THERMO.ZIP in the C++ lib to work with TURBO VISION
// and fixed a small bug.
//
// Author: Jay Perez. Modified by Barnaby Falls
//
// Added: Code page remap, busy state feedback and window title.
// By Salvador E. Tropea (SET)
//

#include "example.h"

__link( RView )
__link( RDialog )
__link( RButton )


const int cmAboutCmd   = 100;  // User selected menu item 'About'
const int cmStatusCmd  = 101;  // User selected menu item 'Progress Bar'

static char cBackChar='²';
static char oBackChar='²';

//========================================================================

class TMyApplication : public TApplication // TEmbedApplication // public TApplication
{
public:
   TMyApplication();
   static TMenuBar *initMenuBar(TRect);
   void handleEvent(TEvent &);
   
   static void cpCallBack(ushort *map);
   
private:
   void aboutDlg();
   void statusDlg();
   Boolean isCancel(TDialog *pd);
};

//========================================================================

TMyApplication::TMyApplication()
: TProgInit( &TApplication::initStatusLine // TEmbedApplication
           , &TMyApplication::initMenuBar
           , &TApplication::initDeskTop // TEmbedApplication
           )
{
}

//========================================================================

TMenuBar *TMyApplication::initMenuBar(TRect bounds)
{
	 bounds.b.y = bounds.a.y + 1;
	 return(new TMenuBar(bounds,
				new TMenu(
					 *new TMenuItem("~A~bout",cmAboutCmd,kbAltA,hcNoContext,0,
						  new TMenuItem("~P~rogress Bar",cmStatusCmd,kbAltL,hcNoContext,0)))));
}

//========================================================================

void TMyApplication::handleEvent(TEvent &event)
{
	 //TEmbedApplication::handleEvent(event);
     TApplication::handleEvent(event);

	 if (event.what == evCommand)
	 {
		  switch (event.message.command)
		  {
		  case cmAboutCmd:
		  {
				aboutDlg();
				clearEvent(event);
				break;
		  }
		  case cmStatusCmd:
		  {
				statusDlg();
				clearEvent(event);
				break;
		  }
		  }
	 }
}

//========================================================================

void TMyApplication::aboutDlg()
{
	 TDialog *pd = new TDialog(TRect(0,0,35,12),"About");
	 if (pd)
	 {
	pd->options |= ofCentered;
	pd->insert(new TStaticText(TRect(1,2,34,7),
		"\003Turbo Vision Example\n\003\n"
					 "\003Using a Progress Bar\n\003\n"));
		  pd->insert(new TButton(TRect(3,9,32,11),"~O~k",cmOK,bfDefault));

	if (validView(pd) != 0)
	{
				deskTop->execView(pd);

	    //CLY_
        destroy(pd);
	}
	 }
}

//========================================================================

Boolean TMyApplication::isCancel(TDialog *pd)
{
   TEvent event;
   pd->getEvent(event);
   pd->handleEvent(event);
   if(event.what==evCommand && event.message.command==cmCancel) {
      Boolean ret=messageBox("Are you sure you want to Cancel",mfConfirmation|mfYesButton|mfNoButton)==cmYes ? True : False;
      return ret;
   }
   else
      return False;
}

void TMyApplication::statusDlg()
{
   TDialog *pd = new TDialog(TRect(0,0,60,15),"Example Progress Bar");
   pd->flags &= ~wfClose;
   pd->options |= ofCentered;
   TProgressBar *pbar = new TProgressBar(TRect(2,2,pd->size.x-2,3),300,cBackChar);
   pd->insert(pbar);
   pd->insert(new TButton(TRect(10,pd->size.y-3,pd->size.x-10,pd->size.y-1),"~C~ancel",cmCancel,bfDefault));

   //Boolean oldBusyState=TScreen::showBusyState(True);
   
   TProgram::deskTop->insert(pd);     // Modeless !!!!

   int i=0;
   Boolean keepOnGoing=True;

   TRect r(5,5,pd->size.x-5,pd->size.y-5);
   TStaticText *theMessage;

   // The first 3rd.
   theMessage = new TStaticText( r,
      "This is a MODELESS dialog box. You can drag this box around the desktop." );
   pd->insert(theMessage);
   for(;i<=100;i++) {
      pbar->update(i);
      idle();
      if(isCancel(pd)) {
	 keepOnGoing=False;
	 break;
	 }
      //usleep(50000);
      }
   //CLY_
   destroy(theMessage);

   if(keepOnGoing) {
      // The second 3rd
      theMessage = new TStaticText( r,
	 "Notice that only the attribute is changed to show progress" );
      pd->insert(theMessage);
      for(;i<=200;i++) {
	 pbar->update(i);
	 idle();
	 if(isCancel(pd)) {
	    keepOnGoing=False;
	    break;
	    }
	 //usleep(50000);
	 }
      //CLY_
      destroy(theMessage);
      }

   if(keepOnGoing) {
      // The last 3rd
      theMessage = new TStaticText( r,
	 "Syntax: TProgressBar(TRect &r, double total, char bar);" );
      pd->insert(theMessage);
      for(;i<=300;i++) {
	 pbar->update(i);
	 idle();
	 if(isCancel(pd)) {
	    keepOnGoing=False;
	    break;
	    }
	 //usleep(50000);
	 }
      //CLY_
      destroy(theMessage); // not necessary since we destroy
			   // the TDialog also
      }
   //CLY_
   destroy(pd);

   //TScreen::showBusyState(oldBusyState);
}


void TMyApplication::cpCallBack(ushort *map)
{
// cBackChar=TVCodePage::RemapChar(oBackChar,map);
}

//========================================================================

//int main(int argc, char **argv, char **envir)
//{

TMyApplication myApplication;

const char *title = 0;

#if 0
void tvInitEmbedApp()
{
	 //TDisplay::setArgv(argc,argv,envir);
	 TVCodePage::SetCallBack(TMyApplication::cpCallBack);

     title=TScreen::getWindowTitle();
	 TScreen::setWindowTitle("Progress bar example");

     myApplication.executeInit();
}

ushort tvPollAppExecute()
{
    return myApplication.executePoll();
}
#endif

//int main(int argc, char **argv, char **envir)
int main()
{
     //TDisplay::setArgv(argc,argv,envir);  

     //tvInitEmbedApp();

 	 // endState = 0;

     //while(tvPollAppExecute()==0) {}
     myApplication.run();

 	 // if(title) {
 		// TScreen::setWindowTitle(title);
 		// delete[] title;
 	 // }
  
	 return 0;
}
  
