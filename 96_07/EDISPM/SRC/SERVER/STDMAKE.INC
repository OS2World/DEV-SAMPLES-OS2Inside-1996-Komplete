# ----Macro Definitions----Macro Definitions-----Macro Definitions----

SOMLIBPATH  = $(SOMBASE)\LIB

INCLUDEPATH = -I. -I$(INCLUDE)
IDLPATH     = -I. -I$(SMINCLUDE)

SC          = sc
SCFLAGSXH   = -s"xh"     $(IDLPATH) -p -D _PLATFORM_OS2_ -S 98304
SCFLAGSXIH  = -s"xh;xih" $(IDLPATH) -p -D _PLATFORM_OS2_ -S 98304
SCFLAGSH    = -s"h"      $(IDLPATH) -p -D _PLATFORM_OS2_ -S 98304
SCFLAGSIH   = -s"h;ih"   $(IDLPATH) -p -D _PLATFORM_OS2_ -S 98304
SCDEF       = -sdef      $(IDLPATH) -p -D _PLATFORM_OS2_ -S 98304
SCCPP       = -sxc       $(IDLPATH) -p -D _PLATFORM_OS2_ -S 120000

IMPLIB     = implib

# ----Inference Rules-----Inference Rules----Inference Rules-----Inference Rules----

.SUFFIXES: .c .cpp .idl .pdl .h .ih .xh .xih .rc .res .def .lib .exe .dll

.idl.xh:
        $(SC) $(SCFLAGSXH) $*.idl

.idl.xih:
        $(SC) $(SCFLAGSXIH) $*.idl

.idl.h:
        $(SC) $(SCFLAGSH) $*.idl

.idl.ih:
        $(SC) $(SCFLAGSIH) $*.idl

#.idl.cpp:
        $(SC) $(SCCPP) $*.idl

.idl.def:
        $(SC) $(SCDEF) $*.idl

.def{$(PUBLIC)}.lib:
       implib $(PUBLIC)\$(@B).lib $(@B).def


.rc.res:
        rc -r $*.rc


#.cpp.obj:
#        $(CC) $(CFLAGS) $*.cpp

#.c.obj:
#        $(CC) $(CFLAGS) $*.c

