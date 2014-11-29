/* EDisPM.cpp: EDMainWin:       EDisPM.EXE V 1.1 */
/* Editorclient - Hauptprogrammrahmenfenster     */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

/* IUICL Headers */
#include <iapp.hpp>
#include <imsgbox.hpp>
#include <ititle.hpp>

/* OS/2-API Headers */
#define INCL_WIN
#include <os2.h>


/* Application Headers */
#include "EDISPM.H"

#include "EDISPM.HPP"
#include "EDDLG.HPP"
#include "EDADM.HPP"


/* Implementierung der Klasse EDMainWin */

EDMainWin::EDMainWin (char *fname, char *uname) :
   IFrameWindow ("EDisPM"),
   Menu (IResourceId (EDMAINMENU), this),
   Info(this),
   EDClient(this, *uname),
   textNameStr(fname),
   userNameStr(uname)

{
  // Infozeile fÅr EDClientStub zugÑnglich machen
   infoA = &Info;

   IString titel("EDisPM");
   EmptyClient = new IStaticText(0, this, this);

  /* Hauptfenster aufbauen und anzeigen */
   setClient(EmptyClient);
   setIcon(IDIC_EDISPM);
   ICommandHandler::handleEventsFor(this);
   show();
   setFocus();

  /* Wenn Username nicht gesetzt, mit Dialog einlesen */
   if (userNameStr == "") {
      commandTextUsername(&userNameStr);
   }
   ITitle(this, titel+" ("+userNameStr+")");

   if (isClientStubReady()) {
      IEvent closeEvent(this, WM_CLOSE, 0, 0);
      dispatch (closeEvent);
   } /* endif */

  /* Wenn Doc-kÅrzel angegeben: EDClient starten */
   if (textNameStr != "NO")
      commandTextLogon(&textNameStr);
}



Boolean EDMainWin::command (ICommandEvent &e)
{
   short rc;
   switch (e.commandId()) {
   case IDMM_OPEN:
      textNameStr = "NO";
      commandTextLogon(&textNameStr);
      break;
   case IDMM_CLOSE:
      commandTextLogoff(&textNameStr);
      break;
   case IDMM_SAVE:
      saveEditWinText();
      break;
   case IDMM_TEXT_USERNAME:
      commandTextUsername(&userNameStr);
      break;
   case IDMM_ABOUT:
      displayAboutBox((HWND) e.handle());
      break;
   case IDMM_HORZ:
      setWindowView(IDMM_HORZ);
     break;
   case IDMM_VERT:
      setWindowView(IDMM_VERT);
      break;
   case IDMM_UPDATE:
      updateTextWin();
      break;
   case IDMM_EXIT:
      {
         commandTextLogoff(&textNameStr);
         IEvent closeEvent(this, WM_CLOSE, 0, 0);
         dispatch (closeEvent);
      }
      return true;
   }
   return false;
}


Boolean EDMainWin::systemCommand (ICommandEvent &e)
{
   Boolean result = false;

   if (e.commandId() == SC_CLOSE) {
      commandTextLogoff(&textNameStr);
   } /* endif */

   return result;
}

Boolean EDMainWin::commandTextLogon(IString *textNameStr) {


   EDDocAdmin DocVerw(this);
   IString    strDocID("");
   IString    strDocPfad("");
   IString    strDocServer("");
   short      rc;

  // if text currently active then output error message
   if (isEditWindowActive()) {
      errorMsgBox("Text-Logon", "Vor dem Einloggen mu· der derzeitig bearbeitete Text geschlossen werden.");
      return false;
   }

   if (DocVerw.openProfile()) {
     // choose or retrieve textName?
      if (*textNameStr == "NO") {
        // choose text with dialog
         if (!DocVerw.chooseDoc(&strDocID, &strDocPfad, &strDocServer)) {
            *textNameStr = "NO";
            return false;
         }
      } else {
        // retrieve text with textNameStr
         strDocID = *textNameStr;
         if (!DocVerw.getDocFromID(&strDocID, &strDocPfad, &strDocServer)) {
            *textNameStr =  "NO";
            return false;
         }
      }
      *textNameStr = strDocID;

     // build DSOM-connection to text server
      rc = logonEDSObj(&strDocID, &strDocPfad, &strDocServer, userNameStr);
      if (rc == 1) {
       // enter new text server in profile
         DocVerw.setServer(&strDocID, &strDocServer);
      } else if (rc < 0) {
         *textNameStr = "NO";
         return false;
      } /* endif */

     // open edit cient window
      openEditWindow(&Menu);

   } else {
      *textNameStr = "NO";
      edClientStatus = 0;
      return false;
   } /* endif */

  ITitle(this, "EDisPM ("+getLogonUserNameStr()+") - "+*textNameStr);

  return true;
}

Boolean EDMainWin::commandTextLogoff(IString* textNameStr) {

   if (!isEditWindowActive())
      return true;

  // close edit client window
   closeEditWindow(EmptyClient, &Menu);

   // stop DSOM connection to text server
   logoffEDSObj();

   *textNameStr = "NO";

   ITitle(this, "EDisPM ("+userNameStr+")");

   return true;
}




Boolean EDMainWin::commandTextUsername(IString *uNS) {
   IString uNameStr(*uNS);

   UserNameDlg UNameDlg(this, &uNameStr);
   if (uNameStr == "")
      uNameStr = "anonymous";

   if (*uNS == uNameStr)
      return false;  // No changes

  // Update of current textÔs username ?
   if (isEditWindowActive()) {
      IMessageBox imb(IWindow::objectWindow());
      imb.setTitle("Question: User - Change");
      short rc = imb.show("Soll der Benutzername fÅr den derzeitig bearbeiteten Text geÑndert werden?", IMessageBox::yesNoCancelButton);
      if (rc == IMessageBox::cancel) {
         return false;
      } else if (rc == IMessageBox::yes) {
         saveEditWinText();                // unlock lines
         changeLogonUserNameStr(uNameStr); // leave/joinsession
         updateTextWin();                  // update view
      }
   }

   *uNS = uNameStr;

   if (isEditWindowActive())
      ITitle(this, "EDisPM ("+*uNS+") - "+textNameStr);
   else
      ITitle(this, "EDisPM ("+*uNS+")");

   return true;
}


Boolean EDMainWin::displayAboutBox(HWND hwnd) {

  /* display my AboutBox dialog */
   WinDlgBox(HWND_DESKTOP,
             hwnd,
             AboutBoxDlgProc,
             0,
             IDD_ABOUTBOX,
             (PVOID)NULL);
   return 0;
}



/*****************/
/* Hauptprogramm */
/*****************/

main(int argc, char *argv[], char *envp[])
{
   IString argTextNameStr("NO");
   IString argUserNameStr("");

  /* Variablen initialisieren */
   if (argc >= 2) {
     /* Username Åbergeben */
      argUserNameStr = argv[1];
   } /* endif */
   if (argc == 3) {
     /* DateikÅrzel Åbergeben */
      argTextNameStr = argv[2];
   } /* endif */

  /* Programmfenster aufbauen */
   EDMainWin mainWindow(argTextNameStr, argUserNameStr);

  /* Progammschleife */
   IApplication::current().run();

}
