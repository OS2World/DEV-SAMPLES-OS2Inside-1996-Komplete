/* EDClInf.hpp: LineInfo:       EDisPM.EXE V 1.1 */
/* Editorclient - LineInfo Klasse fÅr ISequence<>*/
/*                                               */
/* (C) 1995 M.Schwarz, FoPra TU-Muenchen SS1995  */
/* (C) 1996 M.Schwarz, SOMObjects-Programmierung */
/*                     OS/2 Inside 7/96          */
/*                                               */
/* latest changes: 11.5.1996                     */
/*-----------------------------------------------*/

#ifndef EDCLINF_HPP
#define EDCLINF_HPP

#include <istring.hpp>

class LineInfo {
   public:
      LineInfo(long l1, long l2, char *l3, long l4) {
         lid = l1; vnr = l2; lock = l3; ltime = l4;
      }

      long lid;
      long vnr;
      IString lock;
      long ltime;
};

#endif
