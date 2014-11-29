#include "edline.xh"
#include "edtext.xh"
#include "edserv.xh"

#ifdef __IBMC__
  #pragma linkage(SOMInitModule,system)
#endif

SOMEXTERN void SOMLINK SOMInitModule(long majorVersion,
                                     long minorVersion,
                                     string className)
{
   SOM_IgnoreWarning(majorVersion);
   SOM_IgnoreWarning(minorVersion);
   SOM_IgnoreWarning(className);

   EDLineNewClass(EDLine_MajorVersion, EDLine_MinorVersion);
   EDTextNewClass(EDText_MajorVersion, EDText_MinorVersion);
   EDServiceNewClass(EDService_MajorVersion,
                       EDService_MinorVersion);

}
