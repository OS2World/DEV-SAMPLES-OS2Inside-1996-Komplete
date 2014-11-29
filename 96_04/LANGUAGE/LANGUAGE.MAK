.all: language.exe reseng.dll resger.dll 

.SUFFIXES:
.SUFFIXES: .cpp .cx .rc

CFLAGS  = /Tdp /Q /O /G5 /Gm /C
CXFLAGS = /Tdc /Ge- /Tdc /Ss /Q /O /G5 /C
LFLAGS  = /Q /Gm /O /G5 /B" /do /nologo /pmtype:pm /e:2 /packd /packc"
LXFLAGS = /Ge- /Q /O /G5 /B" /do /nologo /pmtype:pm /e:2 /packd /packc"
MAKEDEP = language.mak
RCPACK  = -X:2


.rc.res:
   rc -r %s %|fF.RES 

.cpp.obj:
   icc $(CFLAGS) %s

.cx.obj:
   icc $(CXFLAGS) %s

language.exe: language.obj language.res {$(LIB)}OCLVAI.LIB $(MAKEDEP)
   icc @<<
$(LFLAGS) /Fe language.exe language.obj OCLVAI.LIB
<<
   rc $(RCPACK) language.res language.exe

reseng.dll: res.obj reseng.res $(MAKEDEP)
   @echo LIBRARY RESENG INITINSTANCE TERMINSTANCE > res.def
   @echo DESCRIPTION 'LANGUAGE Resource Library' >> res.def
   @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> res.def
   @echo CODE LOADONCALL >> res.def
   icc @<<
$(LXFLAGS) /Fe reseng.dll res.obj res.def
<<
   rc $(RCPACK) reseng.res reseng.dll
   @if exist res.def erase res.def

resger.dll: res.obj resger.res $(MAKEDEP)
   @echo LIBRARY RESGER INITINSTANCE TERMINSTANCE > res.def
   @echo DESCRIPTION 'LANGUAGE Resource Library' >> res.def
   @echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >> res.def
   @echo CODE LOADONCALL >> res.def
   icc @<<
$(LXFLAGS) /Fe resger.dll res.obj res.def
<<
   rc $(RCPACK) resger.res resger.dll
   @if exist res.def erase res.def

# dependencies

language.obj : language.cpp language.hpp res.h $(MAKEDEP)
res.obj      : res.cx $(MAKEDEP)
reseng.res   : reseng.rc res.h $(MAKEDEP)
resger.res   : resger.rc res.h $(MAKEDEP)


 
