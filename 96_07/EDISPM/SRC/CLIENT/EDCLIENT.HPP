/* EDClient.hpp: EDClient:      EDisPM.EXE V 1.1 */
/* Editorclient - Client-(Edit-und User-) fenster*/
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#ifndef EDCLIENT_HPP
#define EDCLIENT_HPP


/* ICUIL-Headers */
#include <istring.hpp>
#include <iframe.hpp>
#include <imenubar.hpp>
#include <imsgbox.hpp>
#include <isplitcv.hpp>
#include <imle.hpp>
#include <icnrctl.hpp>
#include <icnrobj.hpp>
#include <istattxt.hpp>
#include <ithread.hpp>
#include <icmdhdr.hpp>
#include <ikeyhdr.hpp>
#include <iedithdr.hpp>
#include <imchdr.hpp>

/* CollectionClass-Headers */
#include <iseq.h>

/* Application Headers */
#include "EDCStub.HPP"
#include "EDCLInf.HPP"

/* UserIcon-Klasse */
class EDUserObject : public IContainerObject
{
  public:
    EDUserObject( IString       name,
                unsigned long iconId,
                IString       compuServeId,
                unsigned long age)
     : IContainerObject(name, iconId),
       strCompuServeId(compuServeId),
       ulAge(age)
     {}

     static unsigned long compuServeOffset ( )
     { return offsetof(EDUserObject, strCompuServeId); }

     static unsigned long ageOffset        ( )
     {   return offsetof(EDUserObject, ulAge); }

   private:
     IString  strCompuServeId;
     unsigned long ulAge;
};



/* Update-Thread-Klasse */
class Update {
   public:
      Update (IFrameWindow *);
      ~Update();
      virtual void update();

   private:
      IFrameWindow *mainWin;
      IThread t;

};


class EDClient : public EDClientStub{
   public:
      EDClient (IFrameWindow *, IString);
      ~EDClient();

      short    openEditWindow(IMenuBar *);
      short    closeEditWindow(IStaticText *, IMenuBar *);
      short    saveEditWinText();
      unsigned long getXCursorPos();
      unsigned long getYCursorPos();
      IString  getCursorLine(unsigned long);
      void     setLineStatusInfo();
      short    updateTextWin();
      short    setWindowView(long);
      short    isEditWindowActive();

      long     nLines;
      long     lCursorLine;
      long     lCursorCol;

      ISequence<LineInfo> EDTextInf;
      IMultiLineEdit    *EDTextWin;

   private:
      IFrameWindow* mainWin;
      Update *upThread;

      class EDKeyboardHandler : public IKeyboardHandler {
         public:
            EDKeyboardHandler (EDClient *);
         protected:
            virtual Boolean virtualKeyPress ( IKeyboardEvent& event );
            virtual Boolean characterKeyPress (IKeyboardEvent& event);
         private:
            EDClient *clClient;
      }; /* class EDClient::EDKeyboardHandler */
      class EDEditHandler : public IEditHandler {
         public:
            EDEditHandler (EDClient *);
         protected:
            virtual Boolean edit(IControlEvent& event);
         private:
            EDClient *clClient;
      }; /* class EDClient::EDEditHandler */
      class EDMouseClickHandler : public IMouseClickHandler {
         public:
            EDMouseClickHandler (EDClient *);
         protected:
            virtual Boolean mouseClicked ( IMouseClickEvent& event );
         private:
            EDClient *clClient;
      }; /* class EDClient::EDMouseClickHandler */


      ISplitCanvas        *EDMainSplitWin;
//      IMultiLineEdit    *EDTextWin;
      EDKeyboardHandler   *EDTextWinKHandler;
      EDEditHandler       *EDTextWinEHandler;
      EDMouseClickHandler *EDTextWinMHandler;
      IContainerControl   *EDUserWin;

};

#endif
