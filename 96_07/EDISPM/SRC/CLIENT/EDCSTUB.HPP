/* EDCStub.hpp: EDClientStub:   EDisPM.EXE V 1.1 */
/* Editorclient - DSOM-Anbindungsverwaltung      */
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#ifndef EDCSTUB_HPP
#define EDCSTUB_HPP


/* ICUIL-Headers */
#include <istring.hpp>
#include <iinfoa.hpp>


/* SOM/DSOM Headers */
#include <somd.xh>


/* SOM-IDL-Klassen Headers */
#include "EDLINE.XH"
#include "EDTEXT.XH"
#include "EDSERV.XH"

void weiter(IString text);


class EDClientStub {
   private:
      IString userName;
      IString fileName;
      IString dokuName;

      // SOM/DSOM: variables
      IString serverName;
      EDLine *eline;
      EDText *etext;
      short createFile;

   public:
      EDClientStub (IWindow *, IString);
      ~EDClientStub();
      Environment  *ev;
      EDService    *ed;
      long         eventCnt;
      short        SOMOpOK(Environment *);
      IInfoArea    *infoA;
      short        edClientStatus;

      short        isClientStubReady();
      short        logonEDSObj(IString*, IString*, IString*, IString);
      short        logoffEDSObj();
      IString      getLogonFileNameStr();
      IString      getLogonUserNameStr();
      Boolean      changeLogonUserNameStr(IString);

      void         errorMsgBox(IString, IString);

     /* SOM/DSOM transfer-Methoden */
      short loadtext(IString);
      short savetext(IString);
      short gettext(_IDL_SEQUENCE_long *);
      short getline(long, EDLine **, char *);
      short putline(long, IString, long, char *);
      short insline(long, EDLine **, char *);
      short delline(long, long, char *);
      short getlock(long, long, char *);
      short rellock(long, char *);
      short appendline(long, long, char *);
      short getinfo(long *, long*, char *);
      short getevents(_IDL_SEQUENCE_long *, char *);
      short joinsession(char *);
      short leavesession(char *);
      short getusers(_IDL_SEQUENCE_string *);

};

#endif
