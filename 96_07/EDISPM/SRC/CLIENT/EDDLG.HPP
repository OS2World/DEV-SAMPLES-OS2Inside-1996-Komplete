/* EDDlg.hpp: EDUserNameDlg:    EDisPM.EXE V 1.1 */
/* Editorclient - Allgemeine Dialogfenster       */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#ifndef EDDLG_HPP
#define EDDLG_HPP

/* IUICL Headers */
#include <iframe.hpp>
#include <icmdhdr.hpp>
#include <iframhdr.hpp>
#include <istring.hpp>
#include <ientryfd.hpp>


class UserNameDlg : public IFrameWindow, public ICommandHandler, public IFrameHandler {
   private:
      IString* uName;
   protected:
      Boolean command (ICommandEvent&);
   public:
      UserNameDlg(IFrameWindow*, IString*);
      IEntryField entryName;
};

MRESULT EXPENTRY AboutBoxDlgProc(HWND, ULONG, MPARAM, MPARAM);
#endif
