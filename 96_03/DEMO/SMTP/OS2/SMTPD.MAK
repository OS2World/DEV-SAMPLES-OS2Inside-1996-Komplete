# SMTPD.MAK
# Created by IBM WorkFrame/2 MakeMake at 17:06:40 on 27 Dec 1995
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES: .CPP .obj 

.all: \
    F:\CTNET\DEMO\SMTP\OS2\SMTPD.EXE

.CPP.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tl10 /IF:\CTNET\INCLUDE /Q /Ti /Gm /Gd /G4 /Fo"%|dpfF.obj" /C %s

F:\CTNET\DEMO\SMTP\OS2\SMTPD.EXE: \
    F:\CTNET\DEMO\SMTP\SMTPD.obj \
    F:\CTNET\LIB\OS2\VA\TCPOS2.LIB \
    F:\CTNET\LIB\OS2\VA\BASEOS2.LIB \
    SMTPD.MAK
    @echo " Link::Linker "
    icc.exe @<<
     /B" /de /exepack:2 /packd /optfunc"
     /FeF:\CTNET\DEMO\SMTP\OS2\SMTPD.EXE 
     F:\CTNET\LIB\OS2\VA\TCPOS2.LIB 
     F:\CTNET\LIB\OS2\VA\BASEOS2.LIB 
     F:\CTNET\DEMO\SMTP\SMTPD.obj
<<

F:\CTNET\DEMO\SMTP\SMTPD.obj: \
    F:\CTNET\DEMO\SMTP\SMTPD.CPP \
    SMTPD.MAK
