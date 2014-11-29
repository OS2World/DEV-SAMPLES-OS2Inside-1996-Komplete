/* EDDlg.cpp: EDUserNameDlg:    EDisPM.EXE V 1.1 */
/* Editorclient - Allgemeine Dialogfenster       */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

/* IUICL Headers */
#include <imsgbox.hpp>
#include <ititle.hpp>

/* OS/2-API-Headers */
#ifndef OS2_INCLUDED
   #define INCL_WIN
   #include <os2.h>
#endif

/* Application Headers */
#include "EDISPM.H"
#include "EDDLG.HPP"

Boolean UserNameDlg::command (ICommandEvent& event)
{
   Boolean dontPassOn = false;
   switch (event.commandId()) {
   case DID_OK:
      *uName = entryName.text();
   case DID_CANCEL:
      ((IFrameWindow*) (event.window()))->dismiss(event.commandId());
      dontPassOn = true;
   } /* endswitch */
   return dontPassOn;
}


UserNameDlg::UserNameDlg(IFrameWindow* parent, IString *userName) :
   IFrameWindow(IDD_UNAMEBOX, parent),
   entryName(IDD_UENTRY, this)
{
   uName = userName;
   IFrameHandler::handleEventsFor(this);
   setFocus();
   entryName.setText(*uName);
   entryName.setFocus();
   showModally();
}


/* Dialog-Procedure fÅr die AboutBox (OS/2-Standard) */
MRESULT EXPENTRY AboutBoxDlgProc(HWND hwnd, ULONG msg, MPARAM mp1,MPARAM mp2)
{
   HWND  TextPos;
   IString textstr;
   SHORT i;
   ULONG   ulScrWidth, ulScrHeight;
   RECTL   Rectl;
   SWP     Swp;
   HPS     hps;

   switch(msg) {
   case WM_PAINT:
      hps = WinBeginPaint (hwnd,0,0);
      WinQueryWindowRect (hwnd, &Rectl);
      WinFillRect (hps, &Rectl, SYSCLR_DIALOGBACKGROUND);
      Rectl.yTop -= WinQuerySysValue(HWND_DESKTOP,SV_CYTITLEBAR),

      WinDrawBorder (hps, &Rectl,
          WinQuerySysValue(HWND_DESKTOP,SV_CXDLGFRAME),
          WinQuerySysValue(HWND_DESKTOP,SV_CYDLGFRAME),
          CLR_DARKGRAY, CLR_WHITE, DB_RAISED);
      GpiMove (hps, (PPOINTL)&Rectl);
      Rectl.xRight--;
      Rectl.yTop--;
      WinQueryWindowPos (WinWindowFromID (hwnd, IDD_ACOPYRIGHTFRAME), &Swp);
      Rectl.xLeft   = Swp.x-1;
      Rectl.yBottom = Swp.y-1;
      Rectl.xRight  = Swp.x + Swp.cx + 1;
      Rectl.yTop    = Swp.y + Swp.cy + 1;
      WinDrawBorder (hps, &Rectl, 1L, 1L,
          CLR_DARKGRAY, CLR_WHITE, DB_DEPRESSED);
      WinQueryWindowPos (WinWindowFromID (hwnd, IDD_APROGRAMFRAME), &Swp);
      Rectl.xLeft   = Swp.x-1;
      Rectl.yBottom = Swp.y-1;
      Rectl.xRight  = Swp.x + Swp.cx + 1;
      Rectl.yTop    = Swp.y + Swp.cy + 1;
      WinDrawBorder (hps, &Rectl, 1L, 1L,
          CLR_DARKGRAY, CLR_WHITE, DB_DEPRESSED);
      WinEndPaint (hps);
      break;
   case WM_COMMAND:
     /* no matter what the command, close the dialog */
      WinDismissDlg(hwnd, TRUE);
      break;

   default:
      return(WinDefDlgProc(hwnd, msg, mp1, mp2));
   }
   return(MPVOID);
}  /* AboutBoxWndProc() */

