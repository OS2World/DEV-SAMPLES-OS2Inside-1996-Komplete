/* EDAdm.hpp: EDDocAdmin:       EDisPM.EXE V 1.1 */
/* Editorclient - Text Server Verwaltung         */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#ifndef EDADM_HPP
#define EDADM_HPP

/* IUICL Headers */
#include <iframe.hpp>
#include <icmdhdr.hpp>
#include <iframhdr.hpp>
#include <istring.hpp>
#include <iprofile.hpp>
#include <imcelcv.hpp>
#include <icnrctl.hpp>
#include <ientryfd.hpp>

/* Application Headers */
#include "EDISPM.H"


/* Text-™ffnen-Neu-Fenster-Klasse */
class EDDocNewDlg : public IFrameWindow, public ICommandHandler, public IFrameHandler {
   private:
      IString* docName;
      IString* docFilePath;
   protected:
      Boolean command (ICommandEvent&);
   public:
      EDDocNewDlg(IFrameWindow*, IString*, IString*);
      IEntryField entryName;
      IEntryField entryFilePath;
};


/* Text-™ffnen-Fenster-Klasse */
class EDDocOpenDlg : public IFrameWindow, public ICommandHandler, public IFrameHandler {
   private:
      IProfile *profile;
      IString  *docName;
   protected:
      Boolean command (ICommandEvent&);
   public:
      EDDocOpenDlg(IString*, IWindow*, IProfile *);
      IMultiCellCanvas  DocOpenClient;
      IContainerControl DocOpenCnr;
};


/* Hauptfenster-Klasse */
class EDDocAdmin
{
   private:
      IString profPath;
      IProfile *EDProfile;
      IWindow  *parent;
   protected:
 /*     static IString validate ( const IString &name); */
   public:
      EDDocAdmin (IWindow *);
      ~EDDocAdmin();
      short openProfile();
      short getDocFromID(IString *, IString *, IString* );
      short chooseDoc(IString *, IString *, IString *);
      short setServer(IString *, IString *);
};
#endif
