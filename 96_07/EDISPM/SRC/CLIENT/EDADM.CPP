/* EDAdm.cpp: EDDocAdmin:       EDisPM.EXE V 1.1 */
/* Editorclient - Text Server Verwaltung         */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#include "EDADM.HPP"

#include <ifiledlg.hpp>
#include <fstream.h>
#include <imsgbox.hpp>
#include <icnrcol.hpp>
#include <icnrobj.hpp>
#include <icnrhdr.hpp>
#include <ipushbut.hpp>
#include <isetcv.hpp>
#include <ipushbut.hpp>
#include <isetcv.hpp>

#define MARGIN 10
#define IDD_DOCNEW 14001
#define IDD_DOCDEL 14002

void weiter(IString text)
{
   IMessageBox imb(IWindow::objectWindow());
   imb.setTitle("Profilebearbeitung-Info");
   imb.show(text, IMessageBox::okButton);
}

/* Implementierung der Klasse EDDocAdmin */

EDDocAdmin::EDDocAdmin (IWindow* p)
{
  /* Parent/Owner-Window speichern */
   parent = p;
   EDProfile = (IProfile *) 0;
}


EDDocAdmin::~EDDocAdmin()
{
  /* Profile (INI-Datei) schlieáen */
   if (EDProfile) {
      delete EDProfile;
   } /* endif */
}


short EDDocAdmin::openProfile()
{
   short rc = 1;

  /* Hole INI-Pfad aus der OS/2-UserIni */
   try {
      profPath = IProfile::userProfile().elementWithKey("Profile-Pfad", "EDisPM");
   } /* end try */

  /* Fehlerbehandlung -> Pfad nicht in OS/2 Userprofile */
   catch ( const IAccessError &exception ) {
      IFileDialog::Settings dlgSettings;
      dlgSettings.setTitle("Textzuordnugsdatei festlegen");
      dlgSettings.setFileName("EDisPM.INI");
      IFileDialog dlg(0, parent, IFileDialog::defaultStyle(), dlgSettings);
      if (dlg.pressedOK()) {
        /* Profile-Pfad einlesen */
         profPath = dlg.fileName();
        /* Speichere Profile-Pfad in der OS/2-Ini */
         IProfile::userProfile().addOrReplaceElementWithKey("Profile-Pfad", profPath, "EDisPM");
      } else {
         rc = 0;
      } /* endif */
   } /* end catch */

  /* Profile ”ffnen */
   try {
      if (rc) {
         EDProfile = new IProfile(profPath);
      } /* endif */
   } /* end try */

  /* Fehlerbehandlung -> Profile ”ffnen */
   catch ( const IString &msg ) {
     /* Display Error Message */
      IMessageBox msgBox(IWindow::objectWindow());
      msgBox.show(msg, msgBox.cancelButton);
      IProfile::userProfile().deleteElementWithKey("Profile-Pfad", "EDisPM");
      rc = 0;
   } /* end catch */

  /* Erfolgreich zurck */
   return rc;
}

/*
IString EDDocAdmin::validate ( const IString &name )
{
   int      option = 0;
   IString  outName( name );
   ifstream test( outName, option );

   if ( !test ) {
      outName += ".ini";
      ifstream test( outName, option );
      if ( !test ) {
         IString msg( "Profile-Datei kann nicht ge”ffnet werden " );
         msg += outName;
         throw msg;
      }
   }
   return outName;
}
*/

short EDDocAdmin::getDocFromID(IString *DocID, IString *DocPath, IString *DocServer) {
   short rc = 0;

   if (*DocID != "") {
      try {
         *DocPath   = EDProfile->elementWithKey("FilePath", *DocID);
         *DocServer = EDProfile->elementWithKey("DSomServer", *DocID);
         rc = 1;
      } /* end try */
      catch ( const IAccessError &exception  ) {
         weiter("Der Texteintrag "+*DocID+" kann nicht ge”ffnet werden.");
      } /* end catch */
   } /* endif */
   if ((*DocPath == "") || (*DocServer == "") ) {
      rc = 0;
   } /* endif */

   return rc;
}

short EDDocAdmin::chooseDoc(IString *DocID, IString *DocPath, IString *DocServer) {
   short rc = 0;

   EDDocOpenDlg dlg(DocID, parent, EDProfile);
   if (*DocID != "") {
      try {
         *DocPath   = EDProfile->elementWithKey("FilePath", *DocID);
         *DocServer = EDProfile->elementWithKey("DSomServer", *DocID);
         rc = 1;
      } /* end try */
      catch ( const IAccessError &exception  ) {
         weiter("Der Texteintrag "+*DocID+" kann nicht ge”ffnet werden.");
         rc = 0;
      } /* end catch */
   } /* endif */
   return rc;
}



short EDDocAdmin::setServer(IString *DocID, IString *DocServer) {
   short rc = 0;
   if (*DocID != "") {
      try {
         EDProfile->addOrReplaceElementWithKey("DSomServer", *DocServer, *DocID);
         rc = 1;
      } /* end try */
      catch ( const IAccessError &exception  ) {
         weiter("Der Servereintrag zu "+*DocID+" kann nicht gesetzt werden.");
         rc = 0;
      } /* end catch */
   } /* endif */
   return rc;
}



/* Implementierung der Containerobjekt-Klasse */

class DocCnrObj : public IContainerObject {
   public:
      DocCnrObj( IString name, IProfile profile) :
         IContainerObject(name, 0)
      {
         strFilePath   = profile.elementWithKey("FilePath", name);
         strDSomServer = profile.elementWithKey("DSomServer", name);
         if (strDSomServer == "NO") {
            strStatus = "-----";
         } else {
            strStatus = "l„uft";
         } /* endif */
      }

      static unsigned long statusOffset() {
         return offsetof(DocCnrObj, strStatus);
      }

      static unsigned long filepathOffset() {
         return offsetof(DocCnrObj, strFilePath);
      }

      static unsigned long dsomserverOffset() {
         return offsetof(DocCnrObj, strDSomServer);
      }

   private:
      IString strFilePath;
      IString strStatus;
      IString strDSomServer;
};


/* Implementierung der Text-™ffnen-Neu-Fenster-Klasse */

Boolean EDDocNewDlg::command (ICommandEvent& event)
{
   Boolean dontPassOn = false;
   switch (event.commandId()) {
   case IDD_NEUFILE: {
         IFileDialog dlg(0, event.window(), IFileDialog::defaultStyle(), IFileDialog::Settings());
         if (dlg.pressedOK()) {
            entryFilePath.setText(dlg.fileName());
         } /* endif */
      }
      break;
   case DID_OK:
      *docName = entryName.text();
      *docFilePath = entryFilePath.text();
      if (((*docName).length() > 0) && ((*docFilePath).length() > 0)) {
         ((IFrameWindow*) (event.window()))->dismiss(event.commandId());
      } /* endif */
      dontPassOn = true;
      break;
   case DID_CANCEL:
      *docName = "";
      ((IFrameWindow*) (event.window()))->dismiss(event.commandId());
      dontPassOn = true;
      break;
   } /* endswitch */
   return dontPassOn;
}


EDDocNewDlg::EDDocNewDlg(IFrameWindow* parent, IString *dN, IString *dFP) :
   IFrameWindow(IDD_OPENNEW, parent),
   entryName(IDD_NDOCID, this),
   entryFilePath(IDD_NDOCFILENAME, this)
{
   docName = dN;
   docFilePath = dFP;

   IFrameHandler::handleEventsFor(this);
   setFocus();
   entryName.setFocus();
   entryFilePath.setLimit(255);
   showModally();
}



/* Implementierung der Editor-Open-Dialog-Klasse */

Boolean EDDocOpenDlg::command (ICommandEvent& event)
{
   Boolean dontPassOn = false;
   switch (event.commandId()) {
   case IDD_DOCNEW: {
         IString strName("");
         IString strFilePath("");
         EDDocNewDlg dlg(this, &strName, &strFilePath);

         if (strName != "") {
           /* Neuer Eintrag in Profile schreiben */
            try {
               profile->addOrReplaceElementWithKey("DSomServer", "NO", strName);
               profile->addOrReplaceElementWithKey("FilePath", strFilePath, strName);
              /* Neuer Eintrag in den Container eintragen */
               DocCnrObj *docObj = new DocCnrObj(strName, *profile);
               DocOpenCnr.addObject(docObj)
                         .sortByIconText();
            } /* end try */
            catch ( const IAccessError &exception  ) {
               weiter("Der Eintrag "+strName+" konnte nicht hinzugefgt werden.");
            } /* end catch */
         } /* endif */
      }
      break;
   case IDD_DOCDEL: {
         DocCnrObj *docObj = (DocCnrObj *) DocOpenCnr.cursoredObject();
         IString strName = docObj->iconText();

         if (strName!="") {
            try {
               if (profile->elementWithKey("DSomServer",strName) != "NO") {
                 /* Server l„uft gerade, trotzdem l”schen ? */
                  IMessageBox imb(IWindow::objectWindow());
                  imb.setTitle("Profilebearbeitung-Info");
                  if (IMessageBox::ok != imb.show("Der Text wird noch von einem Server bearbeitet, Eintrag trotzdem l”schen? ", IMessageBox::okCancelButton))
                     break;
               } /* endif */
              /* Server-Eintrag l”schen */
               profile->deleteElementWithApplication(strName);
               DocOpenCnr.removeObject(docObj)
                         .sortByIconText();
            } /* end try */
            catch ( const IAccessError &exception  ) {
               weiter("Der Texteintag "+strName+" konnte nicht gel”scht werden.");
            } /* end catch */
         } /* endif */
      }
      break;
   case DID_OK: {
         DocCnrObj *docObj = (DocCnrObj *) DocOpenCnr.cursoredObject();
         *docName =  docObj->iconText();
         ((IFrameWindow*) (event.window()))->dismiss(event.commandId());
         dontPassOn = true;
      }
      break;
   case DID_CANCEL:
      ((IFrameWindow*) (event.window()))->dismiss(event.commandId());
      dontPassOn = true;
      break;
   } /* endswitch */
   return dontPassOn;
}


EDDocOpenDlg::EDDocOpenDlg(IString* dN, IWindow* parent, IProfile *p) :
   IFrameWindow( 0,0,parent, IFrameWindow::nextShellRect().scaleBy(0.6, 0.5),
                  IFrameWindow::classDefaultStyle
                & ~IFrameWindow::maximizeButton
                & ~IFrameWindow::minimizeButton
                | IFrameWindow::dialogBackground, "™ffnen"),
   DocOpenClient(0, this, this),
   DocOpenCnr   (0, &DocOpenClient, &DocOpenClient)
{
   profile = p;
   docName = dN;
   *docName = "";

  /* Erstelle Dateiauswahl-Container */
   IContainerColumn* pcolumn;

   pcolumn = new IContainerColumn(IContainerColumn::isIconViewText);
   pcolumn->setHeadingText("Name");
   pcolumn->showSeparators(IContainerColumn::horizontalSeparator);
   DocOpenCnr.addColumn(pcolumn);
   DocOpenCnr.setDetailsViewSplit(pcolumn, 100);

   pcolumn = new IContainerColumn(DocCnrObj::statusOffset());
   pcolumn->setHeadingText("Status");
   pcolumn->showSeparators();
   DocOpenCnr.addColumn(pcolumn);

   pcolumn = new IContainerColumn(DocCnrObj::filepathOffset());
   pcolumn->setHeadingText("Dateipfad");
   pcolumn->showSeparators(IContainerColumn::horizontalSeparator);
   DocOpenCnr.addColumn(pcolumn);

   DocOpenCnr
      .setSingleSelection()
      .setDeleteColumnsOnClose()
      .setDeleteObjectsOnClose()
      .showSplitBar()
      .showTitleSeparator()
      .showDetailsView()
      .enableTabStop();

  /* Erstelle Buttons */
   ISetCanvas  BSpace(0x800C, &DocOpenClient, &DocOpenClient);
   IPushButton BOk(DID_OK, &BSpace, &BSpace);
   IPushButton BCancel(DID_CANCEL, &BSpace, &BSpace);
   IPushButton BNew(IDD_DOCNEW, &BSpace, &BSpace);
   IPushButton BDelete(IDD_DOCDEL, &BSpace, &BSpace);
   BOk.enableDefault()
      .setText( "~OK" )
      .enableTabStop()
      .enableGroup();
   BCancel
      .setText( "A~bbruch" )
      .enableTabStop();
   BNew
      .setText("~Neu")
      .enableTabStop();
   BDelete
      .setText("~L”schen")
      .enableTabStop();
   BSpace
     .setMargin( ISize() )
     .setPackType( ISetCanvas::expanded );

  /* Baue Fenster auf */
   ISize defaultCell = IMultiCellCanvas::defaultCell();
   DocOpenClient
      .addToCell(&DocOpenCnr,  2, 2, 3)
      .addToCell(&BSpace,      2, 4);

   DocOpenClient
      .setColumnWidth(1, MARGIN)
      .setColumnWidth(4, MARGIN)
      .setColumnWidth(3, defaultCell.width(), true)
      .setRowHeight  (1, MARGIN)
      .setRowHeight  (2, defaultCell.height(), true)
      .setRowHeight  (3, MARGIN)
      .setRowHeight  (5, MARGIN);

  /* Text-Eintr„ge aus Profile einlesen */
  /* Profile-Cursor, um Text-Eintr„ge auszulesen */
   IProfile::Cursor cursor(*profile);
   for (cursor.setToFirst(); cursor.isValid(); cursor.setToNext() ) {
      IString   strDocName = profile->applicationOrKeyAt(cursor);
      DocCnrObj *docObj = new DocCnrObj(strDocName, *profile);
      DocOpenCnr.addObject(docObj);
   } /* endfor */

   DocOpenCnr.sortByIconText();

  /* Fenster anzeigen und abfragen */
   IFrameHandler::handleEventsFor(this);
   setClient(&DocOpenClient);
   setFocus();
   showModally();
}

