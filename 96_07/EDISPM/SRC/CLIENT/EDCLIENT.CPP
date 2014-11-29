/* EDClient.cpp: EDClient:      EDisPM.EXE V 1.1 */
/* Editorclient - Client-(Edit-und User-) fenster*/
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#include "EDISPM.H"
#include "EDCLIENT.HPP"

#include <ititle.hpp>
#include <iexcept.hpp>

/* Update-Thread-Klasse-Implementation */
Update::Update (IFrameWindow *mainWin) :
   mainWin(mainWin),
   t()
{
   t.start(new IThreadMemberFn<Update>(*this, update));
}

Update::~Update()
{
  t.stop();
}

void Update::update()
{
   IWindowHandle handle;
   handle = mainWin->handle();
   for (; ; ) {
      IThread::current().sleep(15000);
      handle.postEvent(WM_COMMAND, IDMM_UPDATE, 0);
   } /* endfor */
}


/* Editor-Client-Fenster-Klasse-Impl. */
EDClient::EDClient (IFrameWindow *mW, IString uName) :
    EDClientStub(mW, uName)
{
   mainWin = mW;
}

EDClient::~EDClient()
{
}

short EDClient::openEditWindow(IMenuBar *mainWinMenu)
{
   short rc = 0;
   long i;

   infoA->setInactiveText("");

  /* ClientAreaFenster erzeugen und im Hauptfenster anmelden */
   EDMainSplitWin = new ISplitCanvas(0x8008, mainWin, mainWin);

  /* Text & Userfenster erzeugen und anordnen */
   EDTextWin = new IMultiLineEdit(1, EDMainSplitWin, EDMainSplitWin,
                                  IRectangle(),
                                  IMultiLineEdit::classDefaultStyle
                                  & ~IMultiLineEdit::wordWrap
                                  | IMultiLineEdit::verticalScroll
                                  | IMultiLineEdit::horizontalScroll
                                  & ~IWindow::visible);
   EDUserWin = new IContainerControl(2, EDMainSplitWin, EDMainSplitWin);

   EDMainSplitWin->setSplitWindowPercentage(EDTextWin, 75);
   EDMainSplitWin->setSplitWindowPercentage(EDUserWin, 25);

   EDTextWin->setText("");

  /* Info von EDService holen: Zeilenanzahl, Eventcounter */
   long lCount, lEventCnt;   getinfo(&lCount, &lEventCnt, getLogonUserNameStr());
   eventCnt = lEventCnt;
   nLines   = lCount;

  /* Text von EDService holen */
   IString ltext("");
   _IDL_SEQUENCE_long   llist;

  /* ZeilenInfoListe (EDTextInf-Sequence) leeren */
   EDTextInf.removeAll();
   if (!gettext(&llist)) {
      EDLine*    lline;

      for (i=0; i<sequenceLength(llist); i++ ) {
        /* Zeile anhand der ID holen */
         getline(sequenceElement(llist, i), &lline, getLogonUserNameStr());

        /* MLE-Text aufbauen */
         EDTextWin->addLineAsLast(lline->_get_line(ev));
        /* Neue LineInfo erzeugen und fÅllen */
         LineInfo linfo(  sequenceElement(llist, i),
                          lline->_get_vnr(ev),
                          lline->_get_lock(ev),
                          lline->_get_ltime(ev) );
        /* LineInfo in ZeilenInfoListe einfÅgen */
         EDTextInf.addAsLast(linfo);
      } /* endfor */
   } /* endif */

  /* Client-Fenster in Hauptfenster einbinden */
   mainWin->setClient(EDMainSplitWin);

  /* Cursor setzten */
   lCursorLine = 0;
   lCursorCol  = 0;
   EDTextWin->setCursorAtLine(lCursorLine);
   EDTextWin->setTop(lCursorLine);
   EDTextWin->setFocus();
   EDTextWin->show();

  /* Weise dem MLE den eigenen Keyboard-Handler zu */

   EDTextWinKHandler = new EDClient::EDKeyboardHandler(this);
   EDTextWinKHandler->handleEventsFor(EDTextWin);
   EDTextWinEHandler = new EDClient::EDEditHandler(this);
   EDTextWinEHandler->handleEventsFor(EDTextWin);
   EDTextWinMHandler = new EDClient::EDMouseClickHandler(this);
   EDTextWinMHandler->handleEventsFor(EDTextWin);

  /* Userliste holen und in Userfenster eintragen */
   _IDL_SEQUENCE_string ulist;

   if (!getusers(&ulist)) {
      EDUserObject* user;
      for (i=0; i<ulist._length; i++ ) {
         IString name(ulist._buffer[i]);
         if (name == getLogonUserNameStr())
            name = name + "\n(myself)";
         user = new EDUserObject (name,
                          IDIC_EDISPM,
                          "12345,6789",
                          38);

         EDUserWin->addObject(user);
      } /* endfor */
   } /* endif */

   EDUserWin->arrangeIconView();
   EDUserWin->showIconView();
   mainWinMenu->setMenu(EDCLIENTMENU);

   mainWin->update();

  /* Neue Zeileninfo anzeigen */
   setLineStatusInfo();

  /* Update-Thread starten */
   upThread = new Update(mainWin);

   edClientStatus = 3;

   return 0;
}



short EDClient::closeEditWindow(IStaticText *emptyClient, IMenuBar *mainWinMenu)
{
   short rc = 0;

   if (edClientStatus == 0)
      return rc;

   if (edClientStatus == 3) {
     /* Gesperrte Zeile entsperren */
      ISequence<LineInfo>::Cursor lCursor(EDTextInf);

      EDTextInf.setToPosition(lCursorLine+1, lCursor);

      if ((EDTextInf.elementAt(lCursor)).lock == getLogonUserNameStr()) {
         rc = putline((EDTextInf.elementAt(lCursor)).lid, getCursorLine(lCursorLine), (EDTextInf.elementAt(lCursor)).vnr + 1, getLogonUserNameStr());
         if (rc >= 0) {
            (EDTextInf.elementAt(lCursor)).vnr++;
         } /* endif */
         rc = rellock((EDTextInf.elementAt(lCursor)).lid, getLogonUserNameStr());
         if (rc >= 0) {
            (EDTextInf.elementAt(lCursor)).lock = "";
         } /* endif */
      } /* endif */

     /* Update-Thread stoppen */
      upThread->~Update();

      delete EDUserWin;
      delete EDTextWinKHandler;
      delete EDTextWinEHandler;
      delete EDTextWinMHandler;
      delete EDTextWin;
      delete EDMainSplitWin;

      infoA->setInactiveText("");

     /* Hauptfenster mit leerem Hintergrund und einfachen MenÅ besetzen */
     /* falls nicht Programmende */
      if ((emptyClient != NULL) && (mainWinMenu != NULL)) {
         mainWin->setClient(emptyClient);
         mainWinMenu->setMenu(EDMAINMENU);
         mainWin->update();
      } /* endif */
   } /* endif */

   edClientStatus = 0;

   return rc;
}

short EDClient::saveEditWinText()
{
   short rc;
  /* Gesperrte Zeile entsperren */
   ISequence<LineInfo>::Cursor lCursor(EDTextInf);

   EDTextInf.setToPosition(lCursorLine+1, lCursor);

   if ((EDTextInf.elementAt(lCursor)).lock == getLogonUserNameStr()) {
      rc = putline((EDTextInf.elementAt(lCursor)).lid, getCursorLine(lCursorLine), (EDTextInf.elementAt(lCursor)).vnr + 1, getLogonUserNameStr());
      if (rc >= 0) {
         (EDTextInf.elementAt(lCursor)).vnr++;
      } /* endif */
      rc = rellock((EDTextInf.elementAt(lCursor)).lid, getLogonUserNameStr());
      if (rc >= 0) {
         (EDTextInf.elementAt(lCursor)).lock = "";
      } /* endif */
   } /* endif */

  /* EDService: Text abspeichern */
   savetext(getLogonFileNameStr());

   return 0;
}

short EDClient::isEditWindowActive()
{
   if (edClientStatus == 3)
      return (short) true;

   return (short) false;
}


ULONG EDClient::getXCursorPos()
{
   return(EDTextWin->cursorLinePosition());
}

ULONG EDClient::getYCursorPos()
{
   ULONG tmpYPos, tmpYOrgPos, tmpXPos;

   tmpXPos = EDTextWin->cursorLinePosition();
   tmpYOrgPos = EDTextWin->cursorPosition();
   EDTextWin->setCursorLinePosition(tmpXPos);
   tmpYPos = tmpYOrgPos - EDTextWin->cursorPosition();
   EDTextWin->setCursorPosition(tmpYOrgPos);

   return(tmpYPos);
}

IString EDClient::getCursorLine(ULONG cPos)
{
   return(EDTextWin->text(cPos));
}

void EDClient::setLineStatusInfo()
{
  // show line info in DocShell statusline
   LineInfo lInfo = EDTextInf.elementAtPosition(lCursorLine+1);
   IString  lInfText;
   lInfText = "Zeile "+IString(lCursorLine+1)+" Spalte "+IString(lCursorCol+1);
   if (lInfo.lock == "") {
      lInfText += " Id "+ IString(lInfo.lid) +
              " Vnr "+IString(lInfo.vnr);
   } else {
      lInfText += " Id "+ IString(lInfo.lid) +
              " Vnr "+IString(lInfo.vnr)+" Gesperrt von "+lInfo.lock;
   }
   infoA->setInactiveText(lInfText);
}



short EDClient::updateTextWin()
{
   short rc = 0;
   long i, i2, lid, k1, k2, k3, k4, k1Tmp, k2Tmp;
   EDUserObject* user;
   _IDL_SEQUENCE_long   elist;   // Eventsliste
   _IDL_SEQUENCE_string ulist;   // Userliste
   _IDL_SEQUENCE_long   llist;   // Textliste
   ISequence<LineInfo>::Cursor lCursor(EDTextInf);

   long     curXPos = getXCursorPos();
   long     curYPos = getYCursorPos();
   long     tmpYPos;

  /* Hole Eventliste seit dem letzten Update vom Server */
   rc = getevents(&elist, getLogonUserNameStr());

   EDTextWin->disableUpdate();

  /* Wenn Text markiert->demarkieren */
//   if (EDTextWin->hasSelectedText()) {
//      EDTextWin->selectRange(IRange(curYPos, curYPos));
//   } /* endif */

try {

  /* PrÅfen, ob Cursor Zeile gewechselt hat */
   if (lCursorLine != curXPos) {

     /* Wenn vorherige Zeile gesperrt von mir gesperrt: entsperren */
      long curLine = lCursorLine;
      EDTextInf.setToPosition(curLine+1, lCursor);
      if ((EDTextInf.elementAt(lCursor)).lock == getLogonUserNameStr()) {
         rc = putline((EDTextInf.elementAt(lCursor)).lid, getCursorLine(curLine), (EDTextInf.elementAt(lCursor)).vnr + 1, getLogonUserNameStr());
         if (rc >= 0) {
            (EDTextInf.elementAt(lCursor)).vnr++;
         } /* endif */
         rc = rellock((EDTextInf.elementAt(lCursor)).lid, getLogonUserNameStr());
         if (rc >= 0) {
            (EDTextInf.elementAt(lCursor)).lock = "";
         } /* endif */
      } /* endif */
   } /* endif */

  /* Eventliste bearbeiten */
   for (i=0; i<elist._length; i++ ) {

      switch (elist._buffer[i]) {
      case -1:      // User-update

        /* Userliste holen und in Userfenster eintragen */
         rc = getusers(&ulist);
         EDUserWin->removeAllObjects();
         for (i2=0; i2<ulist._length; i2++ ) {
            IString name(ulist._buffer[i2]);
            if (name == getLogonUserNameStr())
               name = name + "\n(myself)";
            user = new EDUserObject (name,
                   IDIC_EDISPM,
                   "12345,6789",
                   38);

            EDUserWin->addObject(user);
         } /* endfor */
         EDUserWin->arrangeIconView();

         break;
      case -2:      // Textreihenfolge verÑndert

        /* Text von EDService holen */
         if (!gettext(&llist)) {
            for (k1=0, k2=1; k1<sequenceLength(llist); k1++, k2++ ) {
               if (k2 > EDTextInf.numberOfElements()) {

                 /* Neue Zeilen am Ende anhÑngen */
                  LineInfo lInfo(sequenceElement(llist, k1), 0, "", 0);
                  EDTextInf.addAsLast(lInfo);
                 /* Erste Zeile */
                  if ((k2 == 2) && (getCursorLine(0) == "")) {
                     EDTextWin->addAsLast("^");
                  } /* endif */
                  EDTextWin->addLine("^", k2-1);

               } else if (sequenceElement(llist, k1) != (EDTextInf.elementAtPosition(k2)).lid) {

                  k1Tmp = sequenceElement(llist, k1);
                  k2Tmp = (EDTextInf.elementAtPosition(k2)).lid;

                  for ( k3 = k1+1, k4 = k2+1;
                           (k3 < sequenceLength(llist)-1)
                        || (k4 < EDTextInf.numberOfElements());
                        k3++, k4++)
                  {
                     if (k3 < sequenceLength(llist)-1) {
                        if (sequenceElement(llist, k3) == k2Tmp) {
                           break;
                        } /* endif */
                     } /* endif */
                     if (k4 < EDTextInf.numberOfElements()) {
                        if (k1Tmp == (EDTextInf.elementAtPosition(k4)).lid) {
                           break;
                        } /* endif */
                     } /* endif */
                  } /* endfor */

                  if (k4 <= EDTextInf.numberOfElements()) {
                     if (k1Tmp == (EDTextInf.elementAtPosition(k4)).lid) {
                       /* k4-k2-1 Elemente lîschen */
                        for (k3 = k2; k3<k4 ; k3++ ) {
                           EDTextInf.removeAtPosition(k2);
                           EDTextWin->removeLine(k2-1);
                           if (curXPos > k2-1 ) {
                              curXPos--;
                           } /* endif */
                        } /* endfor */
                        goto ende_k3k4_behandlung;
                     } /* endif */
                  } /* endif */
                  if (k3 < sequenceLength(llist)) {
                     if (sequenceElement(llist, k3) == k2Tmp) {
                       /* k3-k1 Elemente einfÅgen */
                        for ( ; k1 < k3 ; k1++,k2++ ) {
                           LineInfo lInfo(sequenceElement(llist, k1), 0, "", 0);
                           EDTextWin->addLine("^", k2-1);
                           EDTextInf.addAtPosition(k2, lInfo);
                           if (curXPos >= k2-1 ) {
                              curXPos++;
                           } /* endif */
                        } /* endfor */
                        goto ende_k3k4_behandlung;
                     } /* endif */
                  } /* endif */
                 /* Sonst: Element einfÅgen */
                  {
                     LineInfo lInfo(k1Tmp, 0, "", 0);
                     EDTextWin->addLine("^", k2-1);
                     EDTextInf.addAtPosition(k2, lInfo);
                     if (curXPos >= k2-1 ) {
                        curXPos++;
                     } /* endif */
                     goto ende_k3k4_behandlung;
                  }

                  ende_k3k4_behandlung:;
               } /* endif */
            } /* endfor */
            for (k4=k2; k4 <= EDTextInf.numberOfElements() ; k4++) {
              /* Zeile am Ende lîschen */
               EDTextInf.removeAtPosition(k2);
               EDTextWin->removeLine(k2-1);
            } /* endfor */
         } /* endif */
         break;
      default: // IDs geÑnderter Zeilen
         if ((lid = sequenceElement(elist, i)) >= 0) {
            EDLine *eLine;
            rc = getline(lid, &eLine, getLogonUserNameStr());
            if (rc >= 0) {
               for ( i2 = 1; i2 < EDTextInf.numberOfElements()
                             && (EDTextInf.elementAtPosition(i2)).lid != lid ;
                     i2++);
               if ((EDTextInf.elementAtPosition(i2)).lid == lid) {
                  EDTextInf.setToPosition(i2, lCursor);
                  if ((EDTextInf.elementAt(lCursor)).lock != getLogonUserNameStr()) {
                     if (getCursorLine(i2-1) != eLine->_get_line(ev)) {
                       /* Letzte Zeile ? */
                        if (i2 >= EDTextWin->numberOfLines()) {
                           EDTextWin->removeLine(i2-1);
                           EDTextWin->addAsLast(eLine->_get_line(ev));
                       /* Standard */
                        } else {
                           EDTextWin->removeLine(i2-1);
                           EDTextWin->addLine(eLine->_get_line(ev), i2-1);
                        } /* endif */
                     } /* endif */
                  } /* endif */

                  (EDTextInf.elementAt(lCursor)).lock  = eLine->_get_lock(ev);
                  (EDTextInf.elementAt(lCursor)).vnr   = eLine->_get_vnr(ev);
                  (EDTextInf.elementAt(lCursor)).ltime = 0;
               } /* endif */
            } /* endif */
         } /* endif */

         break;
      } /* endswitch */
   } /* endfor */

   if (curXPos < 0)
      curXPos = 0;
   if (curXPos >= EDTextInf.numberOfElements() )
      curXPos = EDTextInf.numberOfElements()-1;

} /* end try */
catch ( IException& exc) {
   weiter("Update has failed: k1:"+IString(k1)+" k2:"+IString(k2)+" curXPos:"+IString(curXPos)+"--"+exc.text());
} /* end catch */

   EDTextWin->setCursorLinePosition(curXPos);
   tmpYPos = EDTextWin->cursorPosition();
   if (curYPos < 0)
      curYPos = 0;
   if (curYPos >= getCursorLine(curXPos).length()-1)
      curYPos = getCursorLine(curXPos).length()-1;
   EDTextWin->setCursorPosition(tmpYPos + curYPos);
   EDTextWin->enableUpdate();

   lCursorLine = curXPos;
   lCursorCol  = curYPos;

  /* Neue Zeileninfo anzeigen */
   setLineStatusInfo();
   return 0;
}

short EDClient::setWindowView(long horzVert)
{
   if (horzVert == IDMM_HORZ) {
      EDMainSplitWin->setOrientation(ISplitCanvas::horizontalSplit);
   } else {
      EDMainSplitWin->setOrientation(ISplitCanvas::verticalSplit);
   } /* endif */
   EDUserWin->arrangeIconView();
   return 0;
}


/* EDTextWinKHandler: in EDClient eingelegte Klasse */
EDClient::EDKeyboardHandler :: EDKeyboardHandler ( EDClient* client ) :
  clClient( client )
{ }

Boolean EDClient::EDKeyboardHandler::characterKeyPress (IKeyboardEvent& event)
{
   Boolean dontPassOn = false;  // Standard: Zeichen weitergeben
   long     curXPos = clClient->getXCursorPos();
   long     curYPos = clClient->getYCursorPos();
   IString  lText;
   short    rc;
   ISequence<LineInfo>::Cursor lCursor(clClient->EDTextInf);

  /* Wenn Text markiert->demarkieren */
//   if (clClient->EDTextWin->hasSelectedText()) {
//      clClient->EDTextWin->selectRange(IRange(curYPos, curYPos));
//   } /* endif */

  /* PrÅfen ob Zeile gesperrt, wenn nicht->sperren */
   clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
   if ((clClient->EDTextInf.elementAt(lCursor)).lock != clClient->getLogonUserNameStr()) {
      rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
      if (rc >= 0) {
         (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();
      } else {
         dontPassOn = true;
      } /* endif */
   } /* endif */

   return dontPassOn;
}

Boolean EDClient::EDKeyboardHandler :: virtualKeyPress ( IKeyboardEvent& event )
{
   Boolean  dontPassOn = true;  // Standard: Eingabe nicht gÅltig
   long     curXPos = clClient->getXCursorPos();
   long     curYPos = clClient->getYCursorPos();
   LineInfo lInfo(0, 0, "",0);
   IString  lText;
   short    rc;
   ISequence<LineInfo>::Cursor lCursor(clClient->EDTextInf);

  /* Wenn Text markiert->demarkieren */
//   if (clClient->EDTextWin->hasSelectedText()) {
//      clClient->EDTextWin->selectRange(IRange(curYPos, curYPos));
//   } /* endif */

  /* Cursor runter */
   switch (event.virtualKey()) {
   case IKeyboardEvent::insert:
      if (event.isAltDown()) {
         clClient->EDTextWin->setChangedFlag();
         dontPassOn = true;
      } else if (!event.isShiftDown() && !event.isCtrlDown())
         dontPassOn = false;
      break;
   case IKeyboardEvent::home:
      if (!event.isAltDown() && !event.isShiftDown()) {
         if (event.isCtrlDown()) {  /* Cursor in erste Zeile */
            clClient->EDTextWin->setCursorLinePosition(0);
            clClient->EDTextWin->setTop(0);
            clClient->EDTextWin->setChangedFlag();
            dontPassOn = true;
         } else {
            clClient->EDTextWin->setChangedFlag();
            dontPassOn = false;
         } /* endif */
      } /* endif */
      break;
   case IKeyboardEvent::end:
      if (!event.isAltDown() && !event.isShiftDown()) {
         if (event.isCtrlDown()) {  /* Cursor in letzte Zeile */
            clClient->EDTextWin->setCursorLinePosition(clClient->EDTextInf.numberOfElements()-1);
            clClient->EDTextWin->setTop(clClient->EDTextInf.numberOfElements()-1);
            clClient->EDTextWin->setChangedFlag();
            dontPassOn = true;
         } else {
            clClient->EDTextWin->setChangedFlag();
            dontPassOn = false;
         } /* endif */
      } /* endif */
      break;
   case IKeyboardEvent::up:         /* Cursor hoch */
   case IKeyboardEvent::down:       /* Cursor runter */
   case IKeyboardEvent::left:       /* Cursor links */
   case IKeyboardEvent::right:      /* Cursor rechts */
     /* Pure Cursor-Taste ? */
      if (!event.isAltDown() && !event.isShiftDown()) {
         IEventParameter1 param1(event.parameter1().number1() | 0x0020,
                                 event.parameter1().char3(), 0);
         (*event.window()).postEvent(IWindow::character, param1, IEventParameter2(0, 0x1A));
         dontPassOn = false;
      } /* endif */
      break;
   case IKeyboardEvent::space:
      event.setResult(false);
      dontPassOn = false;
      break;
   case IKeyboardEvent::enter:      /* Neue Zeile einfÅgen */
   case IKeyboardEvent::newLine:
      {
         clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
         EDLine *eLine;
         long prelid = (clClient->EDTextInf.elementAt(lCursor)).lid;
         lCursor.setToNext();

         clClient->lCursorLine = curXPos;
        /* PrÅfen ob Zeile gesperrt, wenn nicht->sperren */
         clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
         rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
         if (rc >= 0) {
            (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();

            rc = clClient->insline(prelid, &eLine, clClient->getLogonUserNameStr());
            if (rc >= 0) {
               lInfo.lid = eLine->_get_lnr(clClient->ev);
               lInfo.lock = eLine->_get_lock(clClient->ev);
               lInfo.vnr = eLine->_get_vnr(clClient->ev);
               clClient->EDTextInf.addAtPosition(curXPos+2, lInfo);

//                  clClient->EDTextWin->disableRefresh();
//                  clClient->EDTextWin->addLine(eLine->_get_line(clClient->ev), curXPos+1);
//
//                  clClient->EDTextWin->setCursorAtLine(curXPos);
//                  curY1Pos = clClient->EDTextWin->selectedRange().lowerBound();
//                  clClient->EDTextWin->setCursorAt(curYPos);
//
//                  IString zeile1((clClient->getCursorLine(curXPos)).subString(1,curYPos-curY1Pos));
//                  IString zeile2((clClient->getCursorLine(curXPos)).subString(curYPos-curY1Pos+1));
//
//                  rc = clClient->putline((clClient->EDTextInf.elementAt(lCursor)).lid, zeile1, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
//                  if (rc >= 0) {
//                     (clClient->EDTextInf.elementAt(lCursor)).vnr++;
//                     clClient->EDTextWin->removeLine(curXPos);
//                     clClient->EDTextWin->addLine(zeile1, curXPos);
//                  } /* endif */
//                  lCursor.setToNext();
//                  rc = clClient->putline((clClient->EDTextInf.elementAt(lCursor)).lid, zeile2, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
//                  if (rc >= 0) {
//                     (clClient->EDTextInf.elementAt(lCursor)).vnr++;
//                     clClient->EDTextWin->removeLine(curXPos+1);
//                     clClient->EDTextWin->addLine(zeile2, curXPos+1);
//                  } /* endif */
//                  clClient->lCursorLine = curXPos+1;
//                  clClient->EDTextWin->setCursorAtLine(clClient->lCursorLine);
//                  clClient->EDTextWin->enableRefresh();
               dontPassOn = false;
           } /* endif */
         } /* endif */
      }
      break;
   case IKeyboardEvent::backSpace:
      if (event.isCtrlDown()) {      /* Zeile lîschen */

        /* PrÅfen ob Zeile gesperrt, wenn nicht->sperren */
         clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
         if ((clClient->EDTextInf.elementAt(lCursor)).lock != clClient->getLogonUserNameStr()) {
            rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
            if (rc >= 0) {
               (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();
               clClient->EDTextWin->disableUpdate();
              /* Letzte Zeile ? -> Zeile leeren */
               if (curXPos+1 == clClient->EDTextInf.numberOfElements()) {
                  clClient->EDTextWin->removeLine(curXPos);
              /* Standard-Behandlung */
               } else {
                  long lid    = (clClient->EDTextInf.elementAt(lCursor)).lid;
                  long vnr    = (clClient->EDTextInf.elementAt(lCursor)).vnr;
                  rc = clClient->delline(lid, vnr, clClient->getLogonUserNameStr());
                  if (rc >= 0) {
                     long lc = clClient->getXCursorPos();
                     clClient->EDTextWin->removeLine(curXPos);
                     clClient->EDTextWin->setCursorLinePosition(lc);
                     clClient->EDTextInf.removeAtPosition(curXPos+1);
                  } /* endif */
               } /* endif */
               clClient->EDTextWin->enableUpdate();
            } /* endif */
         } /* endif */
     /* Zeichen vor Cursor lîschen */
      } else if (!event.isAltDown() && !event.isShiftDown()) {

        /* PrÅfen ob Cursor != 1. Zeichen in Zeile (Zeilenwechsel) */
         if (curYPos > 0) {
           /* PrÅfen ob Zeile gesperrt, wenn nicht->sperren */
            clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
            if ((clClient->EDTextInf.elementAt(lCursor)).lock != clClient->getLogonUserNameStr()) {
               rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
               if (rc >= 0) {
                  (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();
               } else {
                  dontPassOn = true;
                  break;
               } /* endif */
            } /* endif */

           /* BackSpace-Taste gÅltig */
            dontPassOn = false;
         } /* endif */
      } /* endif */
      break;

  /* Zeichen nach Cursor lîschen */
   case IKeyboardEvent::deleteKey:
      if (!event.isAltDown() && !event.isShiftDown() && !event.isCtrlDown()) {
        /* PrÅfen ob Cursor != letztes Zeichen in Zeile (Zeilenwechsel) */
         if (curYPos >= clClient->getCursorLine(curXPos).length()-1) {
           /* Zeile lîschen und zusammenhÑngen */
            if (curXPos+1 < clClient->EDTextInf.numberOfElements()) {
               clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
               rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
               if (rc >= 0) {
                  (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();
                  lCursor.setToNext();
                  rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
                  if (rc >=0) {
                     (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();
                     long lid    = (clClient->EDTextInf.elementAt(lCursor)).lid;
                     long vnr    = (clClient->EDTextInf.elementAt(lCursor)).vnr;
                     rc = clClient->delline(lid, vnr, clClient->getLogonUserNameStr());
                     if (rc >= 0) {
                        clClient->EDTextInf.removeAtPosition(curXPos+2);
                        dontPassOn = false;
                        break;
                     } else {
                        rc = clClient->rellock((clClient->EDTextInf.elementAt(lCursor)).lid, clClient->getLogonUserNameStr());
                        if (rc >= 0) {
                           (clClient->EDTextInf.elementAt(lCursor)).lock = "";
                        } /* endif */
                     } /* endif */
                  } /* endif */
               } /* endif */
            } /* endif */
         } else {
           /* Zeichen lîschen */
           /* PrÅfen ob Zeile gesperrt, wenn nicht->sperren */
            clClient->EDTextInf.setToPosition(curXPos+1, lCursor);
            if ((clClient->EDTextInf.elementAt(lCursor)).lock != clClient->getLogonUserNameStr()) {
               rc = clClient->getlock((clClient->EDTextInf.elementAt(lCursor)).lid, (clClient->EDTextInf.elementAt(lCursor)).vnr, clClient->getLogonUserNameStr());
               if (rc >= 0) {
                  (clClient->EDTextInf.elementAt(lCursor)).lock = clClient->getLogonUserNameStr();
               } else {
                  dontPassOn = true;
                  break;
               } /* endif */
            } /* endif */

           /* Delete-Taste gÅltig */
            dontPassOn = false;
         } /* endif */
      } /* endif */
      break;
   default:
     break;
   } /* endswitch */
   event.setResult(true);
   return dontPassOn;
}




/* EDEditHandler: in EDClient eingelegte Klasse */
EDClient::EDEditHandler :: EDEditHandler ( EDClient* client ) :
  clClient( client )
{ }

Boolean EDClient::EDEditHandler::edit (IControlEvent& event)
{
   long     curLine;
   LineInfo lInfo(0, 0, "", 0);
   IString  lInfText;
   ISequence<LineInfo>::Cursor lCursor(clClient->EDTextInf);
   short    rc;
   if ((clClient->getXCursorPos() < clClient->EDTextWin->numberOfLines())
       && (clClient->getXCursorPos()+1 <= clClient->EDTextInf.numberOfElements())) {

     /* PrÅfen, ob Cursor Zeile gewechselt hat */
      curLine = clClient->lCursorLine;
      clClient->EDTextInf.setToPosition(curLine+1, lCursor);

      if (clClient->lCursorLine != clClient->getXCursorPos()) {
        /* Wenn vorherige Zeile gesperrt von mir gesperrt: entsperren */
         if ((clClient->EDTextInf.elementAt(lCursor)).lock == clClient->getLogonUserNameStr()) {
            rc = clClient->putline((clClient->EDTextInf.elementAt(lCursor)).lid, clClient->getCursorLine(curLine), (clClient->EDTextInf.elementAt(lCursor)).vnr + 1, clClient->getLogonUserNameStr());
            if (rc >= 0) {
               (clClient->EDTextInf.elementAt(lCursor)).vnr++;
            } /* endif */
            rc = clClient->rellock((clClient->EDTextInf.elementAt(lCursor)).lid, clClient->getLogonUserNameStr());
            if (rc >= 0) {
               (clClient->EDTextInf.elementAt(lCursor)).lock = "";
            } /* endif */
         } /* endif */
      } /* endif */

     /* Neue Zeileninfo anzeigen */
      clClient->lCursorLine = clClient->getXCursorPos();
      clClient->lCursorCol  = clClient->getYCursorPos();
      clClient->setLineStatusInfo();
   } /* endif */

   return false;
}


/* EDMouseClickHandler: in EDClient eingelegte Klasse */
EDClient::EDMouseClickHandler :: EDMouseClickHandler ( EDClient* client ) :
  clClient( client )
{ }

Boolean EDClient::EDMouseClickHandler::mouseClicked (IMouseClickEvent& event)
{
   Boolean dontPassOn = true;   // Standard: Mouseclick verhindern
   switch (event.mouseAction()) {
   case IMouseClickEvent::click:
      dontPassOn = false;
      break;
   case IMouseClickEvent::down:
      clClient->EDTextWin->setFocus();
      break;
   default:
     break;
   } /* endswitch */
   return dontPassOn;
}

