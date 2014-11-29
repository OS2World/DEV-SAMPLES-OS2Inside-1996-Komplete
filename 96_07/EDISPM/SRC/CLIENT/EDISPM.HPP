/* EDisPM.hpp: EDMainWin:       EDisPM.EXE V 1.1 */
/* Editorclient - Hauptprogrammrahmenfenster     */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#ifndef EDISPM_HPP
#define EDISPM_HPP

/* IUICL Headers */
#include <iframe.hpp>
#include <imenubar.hpp>
#include <iinfoa.hpp>
#include <icmdhdr.hpp>
#include <istring.hpp>
#include <istattxt.hpp>


/* Application Headers */
#include "EDClient.HPP"

/* Hauptfenster-Klasse */
class EDMainWin  : public EDClient,
                   public IFrameWindow,
                   public ICommandHandler
{
   private:
      IMenuBar Menu;
      IInfoArea Info;
      IStaticText *EmptyClient;
   protected:
      Boolean command       (ICommandEvent &);
      Boolean systemCommand (ICommandEvent &);
   public:
      EDMainWin (char *, char *);

      IString userNameStr;
      IString textNameStr;

      Boolean commandTextLogon(IString *);
      Boolean commandTextLogoff(IString *);
      Boolean commandTextUsername(IString *);
      Boolean displayAboutBox(HWND);
};
#endif
