# SOCKC.MAK
# Created by IBM WorkFrame/2 MakeMake at 13:54:27 on 27 Dec 1995
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES: .CPP .obj 

.all: \
    F:\CTNET\DEMO\MINI\OS2\SOCKC.EXE

.CPP.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /IF:\CTNET\INCLUDE /Ti /Gm /Gd /G4 /Fo"%|dpfF.obj" /C %s

F:\CTNET\DEMO\MINI\OS2\SOCKC.EXE: \
    F:\CTNET\DEMO\MINI\SOCKC.obj \
    F:\CTNET\LIB\OS2\VA\TCPOS2.LIB \
    SOCKC.MAK
    @echo " Link::Linker "
    icc.exe @<<
     /B" /de /exepack:2 /packd /optfunc"
     /FeF:\CTNET\DEMO\MINI\OS2\SOCKC.EXE 
     F:\CTNET\LIB\OS2\VA\TCPOS2.LIB 
     F:\CTNET\DEMO\MINI\SOCKC.obj
<<

F:\CTNET\DEMO\MINI\SOCKC.obj: \
    F:\CTNET\DEMO\MINI\SOCKC.CPP \
    SOCKC.MAK
