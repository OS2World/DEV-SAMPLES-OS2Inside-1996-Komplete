#
#	CT.NET++ "Come Together"
#	Windows NT 3.51, Windows 95
#
#	DEMO\SMTP\WIN32\SMTPD.MAK

.AUTODEPEND

#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg
TLINK32 = TLink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32
#
# IDE macros
#


#
# Options
#
IDE_LFLAGS32 =  -LD:\BC45\LIB; -c -wdpl -went -wdup -wdef -wimt -wbdl -wsrf
LLATC32_smtpddexe =  -Tpe -ap -c
RLATC32_smtpddexe =  -w32
BLATC32_smtpddexe = 
CNIEAT_smtpddexe = -I\CTNET\INCLUDE;D:\BC45\INCLUDE; -D_RTLDLL;
LNIEAT_smtpddexe = -x
LEAT_smtpddexe = $(LLATC32_smtpddexe)
REAT_smtpddexe = $(RLATC32_smtpddexe)
BEAT_smtpddexe = $(BLATC32_smtpddexe)
CLATW16_ddbddbddblibbwin32btcpw32dlib = 
LLATW16_ddbddbddblibbwin32btcpw32dlib = 
RLATW16_ddbddbddblibbwin32btcpw32dlib = 
BLATW16_ddbddbddblibbwin32btcpw32dlib = 
CEAT_ddbddbddblibbwin32btcpw32dlib = $(CEAT_smtpddexe) $(CLATW16_ddbddbddblibbwin32btcpw32dlib)
CNIEAT_ddbddbddblibbwin32btcpw32dlib = -I\CTNET\INCLUDE;D:\BC45\INCLUDE; -D_RTLDLL;
LNIEAT_ddbddbddblibbwin32btcpw32dlib = -x
LEAT_ddbddbddblibbwin32btcpw32dlib = $(LEAT_smtpddexe) $(LLATW16_ddbddbddblibbwin32btcpw32dlib)
REAT_ddbddbddblibbwin32btcpw32dlib = $(REAT_smtpddexe) $(RLATW16_ddbddbddblibbwin32btcpw32dlib)
BEAT_ddbddbddblibbwin32btcpw32dlib = $(BEAT_smtpddexe) $(BLATW16_ddbddbddblibbwin32btcpw32dlib)
CLATW16_ddbddbddblibbwin32bbasew32dlib = 
LLATW16_ddbddbddblibbwin32bbasew32dlib = 
RLATW16_ddbddbddblibbwin32bbasew32dlib = 
BLATW16_ddbddbddblibbwin32bbasew32dlib = 
CEAT_ddbddbddblibbwin32bbasew32dlib = $(CEAT_smtpddexe) $(CLATW16_ddbddbddblibbwin32bbasew32dlib)
CNIEAT_ddbddbddblibbwin32bbasew32dlib = -I\CTNET\INCLUDE;D:\BC45\INCLUDE; -D_RTLDLL;
LNIEAT_ddbddbddblibbwin32bbasew32dlib = -x
LEAT_ddbddbddblibbwin32bbasew32dlib = $(LEAT_smtpddexe) $(LLATW16_ddbddbddblibbwin32bbasew32dlib)
REAT_ddbddbddblibbwin32bbasew32dlib = $(REAT_smtpddexe) $(RLATW16_ddbddbddblibbwin32bbasew32dlib)
BEAT_ddbddbddblibbwin32bbasew32dlib = $(BEAT_smtpddexe) $(BLATW16_ddbddbddblibbwin32bbasew32dlib)

#
# Dependency List
#
Dep_smtpd = \
   smtpd.exe

smtpd : BccW32.cfg $(Dep_smtpd)
  echo MakeNode 

Dep_smtpddexe = \
   smtpd.obj\
   ..\..\..\lib\win32\tcpw32.lib\
   ..\..\..\lib\win32\basew32.lib

smtpd.exe : $(Dep_smtpddexe)
  $(TLINK32) @&&|
 /v $(IDE_LFLAGS32) $(LEAT_smtpddexe) $(LNIEAT_smtpddexe) +
D:\BC45\LIB\c0x32.obj+
smtpd.obj
$<,$*
..\..\..\lib\win32\tcpw32.lib+
..\..\..\lib\win32\basew32.lib+
D:\BC45\LIB\import32.lib+
D:\BC45\LIB\cw32mti.lib

|

smtpd.obj :  ..\smtpd.cpp
  $(BCC32) -c @&&|
 $(CEAT_smtpddexe) $(CNIEAT_smtpddexe) -o$@ ..\smtpd.cpp
|

#Compiler configuration file
BccW32.cfg : 
   Copy &&|
-R
-v
-vi
-H
-H=smtpd.csm
-v-
-R-
-N-
-RT
-wcln
-wsig
-wucp
-wbbf
-wpin
-w-nak
-wpre
-wdef
-wnod
-wamb
-wuse
-wstv
-wasm
-wamp
-wobs
-wpch
-r-
-a2
-Od
-WM
-WC
| $@


