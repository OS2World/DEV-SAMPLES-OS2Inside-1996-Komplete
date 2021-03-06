/*******************************************************************************
* FILE NAME: Blckbard.hpp                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     Blackboard                                                               *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _BLACKBOARD_
#define _BLACKBOARD_  

class Blackboard;

#ifndef _IFRAME_
#include <iframe.hpp>
#endif

class ICanvas;
class IPushButton;
class IGroupBox;
class IStaticText;
class IEntryField;
#ifndef _IRECT_
#include <irect.hpp>
#endif

#ifndef _ISTRING_
#include <istring.hpp>
#endif

class BlackboardConn0;
class BlackboardConn3;
class BlackboardConn4;
class BlackboardConn5;
class BlackboardConn7;
class BlackboardConn17;
#include "Blckbard.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for Blackboard
//*****************************************************************************
class Blackboard : public IFrameWindow {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   Blackboard(
      unsigned long id = WND_Blackboard,
      IWindow * parent = IWindow::desktopWindow(),
      IWindow * owner = 0,
      const IRectangle & rect = defaultFramingSpec(),
      const IFrameWindow::Style & style = IFrameWindow::defaultStyle ( ),
      const char * title = defaultTitle());

   virtual  ~Blackboard();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   static const IRectangle defaultFramingSpec();
   static IString defaultTitle();
   virtual Blackboard & initializePart();
   Blackboard * getBlackBoardFrame() { return this; };

   //---------------------------------------------------------------------------
   // public member data
   //---------------------------------------------------------------------------
   static const INotificationId readyId;

protected:
   //---------------------------------------------------------------------------
   // protected member functions
   //---------------------------------------------------------------------------
   Boolean makeConnections();

private:
   //---------------------------------------------------------------------------
   // private member data
   //---------------------------------------------------------------------------
   ICanvas * iCanvas;
   IPushButton * iWriteBoard;
   IPushButton * iClearButton;
   IPushButton * iExitButton;
   IGroupBox * iOutputGroup;
   IGroupBox * iInputGroup;
   IStaticText * iNameTxt;
   IStaticText * iCompanyTxt;
   IStaticText * iNameTxt1;
   IStaticText * iCompanyTxt3;
   IEntryField * iCompanyEntry;
   IEntryField * iNameEntry;
   IEntryField * iCompanyBoard;
   IEntryField * iNameBoard;
   BlackboardConn0 * conn0;
   BlackboardConn3 * conn3;
   BlackboardConn4 * conn4;
   BlackboardConn5 * conn5;
   BlackboardConn7 * conn7;
   BlackboardConn17 * conn17;
   static unsigned long partWindowId;


};   //Blackboard


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif
