# SOCKS.MAK
# Created by IBM WorkFrame/2 MakeMake at 13:24:26 on 27 Dec 1995
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES: .CPP .obj

.all: \
    F:\CTNET\DEMO\MINI\OS2\SOCKS.EXE

.CPP.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tl10 /IF:\CTNET\INCLUDE /Q /Ti /Gm /Gd /G4 /Fo"%|dpfF.obj" /C %s

F:\CTNET\DEMO\MINI\OS2\SOCKS.EXE: \
    F:\CTNET\DEMO\MINI\SOCKS.obj \
    F:\CTNET\LIB\OS2\VA\TCPOS2.LIB \
    SOCKS.MAK
    @echo " Link::Linker "
    icc.exe @<<
     /B" /de /exepack:2 /packd /optfunc"
     /FeF:\CTNET\DEMO\MINI\OS2\SOCKS.EXE
     F:\CTNET\LIB\OS2\VA\TCPOS2.LIB
     F:\CTNET\DEMO\MINI\SOCKS.obj
<<

F:\CTNET\DEMO\MINI\SOCKS.obj: \
    F:\CTNET\DEMO\MINI\SOCKS.CPP \
    SOCKS.MAK
