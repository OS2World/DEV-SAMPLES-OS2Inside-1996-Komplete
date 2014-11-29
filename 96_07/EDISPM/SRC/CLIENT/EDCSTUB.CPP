/* EDCStub.cpp: EDClientStub:   EDisPM.EXE V 1.1 */
/* Editorclient - DSOM-Anbindungsverwaltung      */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#include "EDCStub.HPP"
#include "EDADM.HPP"

/* IUICL Headers */
#include <imsgbox.hpp>
#include <iwindow.hpp>


EDClientStub::EDClientStub (IWindow *fw, IString uname) :
   userName(uname)
{
   edClientStatus = 0;

   ev = new Environment;

  // Initialisierung von SOM/DSOM
   SOM_InitEnvironment(ev);
   SOMD_Init(ev);

   /*
      Prfung, ob Server "EDisServ" im Implementation Repository
      eingetragen ist, wenn nein, wird ein Eintrag erstellt
   */

   sequence(ImplementationDef) impldefs;
   ImplementationDef *impldef;

   impldefs = SOMD_ImplRepObject->find_impldef_by_class(ev, "EDService");
   if ((sequenceLength(impldefs)<=0) || (ev->_major == SYSTEM_EXCEPTION)) {
     /* Fehler im ImplRep, Eintrag neu einfgen */
     /* Eintrag: Server EDService, Klassen: EDisServ, EDText, EDLine */

     // get hostname from environment
      IString hostname(getenv("HOSTNAME"));
      if ((hostname == "") || (hostname == "0"))
         hostname = "localhost";

      impldef = new ImplementationDef();
      impldef->_set_impl_alias(ev, "EDisServ");
      impldef->_set_impl_program(ev, "SOMDSVR.EXE");
      impldef->_set_impl_flags(ev, 0);
      impldef->_set_impl_server_class(ev, "SOMDServer");
      impldef->_set_impl_hostname(ev, hostname);

      SOMD_ImplRepObject->add_impldef(ev, impldef);
      impldef = SOMD_ImplRepObject->find_impldef_by_alias(ev, "EDisServ");
      if (!SOMOpOK(ev)) {
         return;
      } /* endif */

      ImplId implid;

      implid = impldef->_get_impl_id(ev);
      SOMD_ImplRepObject->add_class_to_impldef(ev, implid, "EDLine");
      SOMD_ImplRepObject->add_class_to_impldef(ev, implid, "EDText");
      SOMD_ImplRepObject->add_class_to_impldef(ev, implid, "EDService");

   } /* endif */

}



EDClientStub::~EDClientStub()
{
   /* SOM/DSOM: Entinitialisieren von SOM/DSOM */
    SOMD_Uninit(ev);
    SOM_UninitEnvironment(ev);
}



short EDClientStub::isClientStubReady()
{
   short rc = 0;

   return rc;
}



short EDClientStub::logonEDSObj(IString *docId, IString *docFilePath, IString *docDSomServer, IString uName)
{
   short rc = 0;
   long lines = 0;

   userName = uName;
   fileName = *docFilePath;

   edClientStatus = 0;

   if (*docDSomServer == "NO") {

     /* Neues EDService-Object erstellen */
      ed = (EDService *) SOMD_ObjectMgr->somdNewObject(ev, "EDService", NULL);
      if (!SOMOpOK(ev)) return -1;

      edClientStatus = 1;

     /* Object-Reference als docDSomServer zurckgeben (CORBA) */
      *docDSomServer = SOMD_ORBObject->object_to_string(ev, (SOMDObject*)ed);

     /* EDService-Object: Laden der Datei *docFilePath */
      rc = loadtext(fileName);
      if (rc < 0) {
        /* L”sche EDService-Object (auf lokaler und Server-Seite) */
         SOMD_ObjectMgr->somdDestroyObject(ev, ed);
         edClientStatus = 0;
         return -1;
      } /* endif */

      rc = 1;
   } else {

     /* Verbindung zu EDService-Object aufbauen (CORBA) */
      ed = (EDService*) SOMD_ORBObject->string_to_object(ev, *docDSomServer);
      if (!SOMOpOK(ev)) return -1;

      edClientStatus = 1;

      rc = 0;
   } /* endif */

   edClientStatus = 2;
   eventCnt = 0;
   dokuName = *docId;

  // Registerung beim EDService text server
   short nameCount = joinsession(userName);
   if (nameCount < 0) {         // error
      edClientStatus = 0;
      SOMD_ObjectMgr->somdReleaseObject(ev, ed);
      return -1;
   } else if (nameCount > 0) {  // username already in userlist
     userName = userName + ":" + IString(nameCount);
   } /* endif */
   return rc;
}


short EDClientStub::logoffEDSObj()
{
   short rc;
   _IDL_SEQUENCE_string ulist;

  // Unregistriere dich beim EDService text server
   leavesession(userName);

  /* Prfe, ob noch User den Text bearbeiten */
   rc = getusers(&ulist);

   if (ulist._length == 0) {
     /* Kein User mehr, beende EDService-Object */
      EDDocAdmin DocVerw(IWindow::objectWindow());
      IString    noEDServiceRef("NO");

     /* L”sche EDService-Object-Reference aus Profile */
      DocVerw.openProfile();
      DocVerw.setServer(&dokuName, &noEDServiceRef);
     /* EDService-Object:Text speichern */
      savetext(fileName);
     /* L”sche EDService-Object (auf lokaler und Server-Seite) */
      SOMD_ObjectMgr->somdDestroyObject(ev, ed);

   } else {

     /* EDService-Object:Text speichern */
      savetext(fileName);
     /* L”sche lokalen Vertreter des EDService-Objects */
      SOMD_ObjectMgr->somdReleaseObject(ev, ed);

   } /* endif */

   dokuName = "";
   fileName = "";

   return 0;
}


IString EDClientStub::getLogonFileNameStr()
{
   return fileName;
}


IString EDClientStub::getLogonUserNameStr()
{
   return userName;
}

Boolean EDClientStub::changeLogonUserNameStr(IString uName)
{
   IString oldUserName = userName;

   userName = uName;

  // Registerung beim EDService text server
   short nameCount = joinsession(userName);
   if (nameCount < 0) {         // error
      userName = oldUserName;
      return false;
   } else if (nameCount > 0) {  // username already in userlist
     userName = userName + ":" + IString(nameCount);
   } /* endif */

  // Jetzt alten Benutzernamen unregistrieren
   leavesession(oldUserName);

   return true;
}


short EDClientStub::SOMOpOK(Environment *ev)
{
    char *exID;

    switch (ev->_major)
    {
      case SYSTEM_EXCEPTION:
        {
           exID = somExceptionId(ev);
           errorMsgBox("SOM / DSOM", "System exception: " + IString(exID) + ".");
           exception_free(ev);
        }
        return (FALSE);

      case USER_EXCEPTION:
        {
           exID = somExceptionId(ev);
           errorMsgBox("SOM / DSOM", "User exception: " + IString(exID) + ".");
           exception_free(ev);
        }
        return (FALSE);

      case NO_EXCEPTION:
        return (TRUE);

      default:
        {
           errorMsgBox("SOM / DSOM", "Ungltige SOM-Fehlerbehandlungsangabe.");
           exception_free(ev);
        }
        return (FALSE);
   }
}



void EDClientStub::errorMsgBox(IString title, IString text) {
   IMessageBox imb(IWindow::objectWindow());
   imb.setTitle("Fehler: "+title);
   imb.show(text, IMessageBox::okButton);
}



/* EDClient -> EDService Funktionen */

short EDClientStub::loadtext(IString filePath) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: loadtext");
   rc = ed->loadtext(ev, filePath);
   SOMOpOK(ev);
   switch (rc) {
   case -1:
   {  /* fileName ungltig bzw. sonstiger Ladefehler! */
      IMessageBox imb(IWindow::objectWindow());
      imb.setTitle("Benutzerfehler");
      imb.show("Auf die Datei "+filePath+" konnte vom EDisPM-Server nicht zugegriffen werden.", IMessageBox::cancelButton);
   }   break;
   case -2:
   case -3:
   {   IMessageBox imb(IWindow::objectWindow());
      imb.setTitle("EDisPM-Textverwaltungsfehler");
      imb.show("Beim Laden des Dokuments ist ein (Speicher-)Fehler aufgetreten.", IMessageBox::cancelButton);
   }   break;
   default:
     break;
   } /* endswitch */
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::savetext(IString filePath) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: savetext");
   rc = ed->savetext(ev, filePath);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::gettext(_IDL_SEQUENCE_long *llist) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: gettext");
   rc = ed->getText(ev, llist);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
};

short EDClientStub::getline(long lnr, EDLine **edLine, char* uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: getline");
   rc = ed->get_edline(ev, lnr, edLine, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::putline(long lnr, IString line, long vnr, char* uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: putline");
   rc = ed->put_edline(ev, lnr, line, vnr, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::insline(long prelnr, EDLine **edLine, char *uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: insline");
   rc = ed->ins_edline(ev, prelnr, edLine, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::delline(long lnr, long vnr, char *uName) {
   short rc;
    IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: delline");
   rc = ed->del_edline(ev, lnr, vnr, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::getlock(long lnr, long vnr, char* uName) {
   short rc;
   EDLine *eline;
   IString inActTxt = infoA->inactiveText();
   infoA->setInactiveText("SOM/DSOM: getlock");
   rc = ed->getlock(ev, lnr, vnr, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::rellock(long lnr, char* uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: rellock");
   rc = ed->rellock(ev, lnr, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
}

short EDClientStub::appendline(long lnr, long vnr, char* uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: appendline");
   rc = ed->appendline(ev, lnr, vnr, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
};


short EDClientStub::getinfo(long * nlines, long* ecnt, char* uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: getinfo");
   rc = ed->getinfo(ev, nlines, ecnt, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
};

short EDClientStub::joinsession(char *uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: joinsession");
   rc = ed->joinsession(ev, uName);
   if (!SOMOpOK(ev))
      rc = -10;
   infoA->setInactiveText(inActTxt);
   return(rc);
};

short EDClientStub::leavesession(char *uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: leavesession");
   rc = ed->leavesession(ev, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
};

short EDClientStub::getusers(_IDL_SEQUENCE_string *ulist) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: getusers");
   rc = ed->getusers(ev, ulist);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
};

short EDClientStub::getevents(_IDL_SEQUENCE_long *elist, char *uName) {
   short rc;
   IString inActTxt = infoA->inactiveText();

   infoA->setInactiveText("SOM/DSOM: getevents");
   rc = ed->getevents(ev, &eventCnt, elist, uName);
   SOMOpOK(ev);
   infoA->setInactiveText(inActTxt);
   return(rc);
};

