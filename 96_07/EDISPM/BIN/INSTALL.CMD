/* EDisPM - Installation mittels REXX */
'@Echo Off'
Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs
Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX
Call SysCls
say '-------------------------------'
say '---- EDisPM - Installation ----'
say '-------------------------------'
say ''
Result = SysFileTree( 'EDISPM.EXE', 'Files', 'F' )
If Files.0 = 0 Then
  Do
  Say 'Fehler: EDISPM.EXE nicht gefunden! Installation abgebrochen.'
  Signal DONE
  End
Result = SysFileTree( 'EDISPM.IR', 'Files', 'F' )
If Files.0 = 0 Then
  Do
  Say 'Fehler: EDISPM.IR nicht gefunden! Installation abgebrochen.'
  Signal DONE
  End
Result = SysFileTree( 'EDISSERV.DLL', 'Files', 'F' )
If Files.0 = 0 Then
  Do
  Say 'Fehler: EDISSERV.DLL nicht gefunden! Installation abgebrochen.'
  Signal DONE
  End
Result = SysFileTree( 'EDISPM.INF', 'Files', 'F' )
If Files.0 = 0 Then
  Do
  Say 'Fehler: EDISPM.INF nicht gefunden! Installation abgebrochen.'
  Signal DONE
  End
Result = SysFileTree( 'README.TXT', 'Files', 'F' )
If Files.0 = 0 Then
  Do
  Say 'Fehler: README.TXT nicht gefunden! Installation abgebrochen.'
  Signal DONE
  End
Result = SysFileTree( 'STARTEDS.CMD', 'Files', 'F' )
If Files.0 = 0 Then
  Do
  Say 'Fehler: STARTEDS.CMD nicht gefunden! Installation abgebrochen.'
  Signal DONE
  End
Say 'Bitte geben Sie den vollen Namen des Verzeichnisses an, in dem der'
Say 'verteilte Editor EDisPM installiert werden soll (default C:\EDISPM): '
Pull Directory
If Directory = "" Then Directory = 'C:\EDISPM'
Result = SysFileTree( Directory, 'Dirs', 'D' )
If Dirs.0 = 0 Then
  Do
  Result = SysMkDir( Directory )
  if Result == 0 Then
    Do
    End
  Else
    Do
    Say 'Fehler: Das Zielverzeichnis kann nicht erstellt werden. Installation abgebrochen.'
    Signal DONE
    End
  End
Say ''
Say 'Die Programmdateien werden kopiert...'

Say ''
Say 'Kopiere EDisPM.EXE nach' Directory '...'
Copy EDisPM.EXE Directory                  '1>NUL'
Say 'Kopiere EDisPM.IR nach' Directory '...'
Copy EDisPM.IR Directory                  '1>NUL'
Say 'Kopiere EDisServ.DLL nach' Directory '...'
Copy EDisServ.DLL Directory                  '1>NUL'
Say 'Kopiere EDisPM.INF nach' Directory '...'
Copy EDisPM.INF Directory                  '1>NUL'
Say 'Kopiere INSTALL.CMD nach' Directory '...'
Copy INSTALL.CMD Directory                  '1>NUL'
Say 'Kopiere README.TXT nach' Directory '...'
Copy README.TXT Directory                  '1>NUL'
Say 'Kopiere STARTEDS.CMD nach' Directory '...'
Copy STARTEDS.CMD Directory                  '1>NUL'
Say ''
Say 'Erstelle Programmobjekte im neuen Ordner EDisPM...'
Say ''
if SysCreateObject("WPFolder", "EDisPM", "<WP_DESKTOP>", "OBJECTID=<ED_ORDNER>") then
   do
   Folder = '<ED_ORDNER>'
   Type = 'WPProgram'
   Title = 'EDisPM Client'
   Parms = 'MINWIN=DESKTOP;PROGTYPE=PM;CCVIEW=YES;EXENAME='Directory'\EDisPM.EXE;STARTUPDIR='Directory';OBJECTID=<EDISPM>;NOPRINT=YES;'
   Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
   If Result = 1 Then
     do
     Type = 'WPShadow'
     Title = 'EDisPM Online'
     Parms = 'SHADOWID='Directory'\EDISPM.INF'
     Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
     If Result = 1 Then
       do
       Type = 'WPProgram'
       Title = 'EDisPM Server (somdd)'
       Parms = 'MINWIN=DESKTOP;PROGTYPE=WINDOWABLEVIO;MINIMIZED=YES;EXENAME='Directory'\STARTEDS.CMD;STARTUPDIR='Directory';OBJECTID=<STARTEDS>;NOPRINT=YES;'
       Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
       If Result = 1 Then
          do
          Say 'Der verteilte Editor EDisPM wurde erfolgreich installiert.'
          Say ''
          Say 'Die Datei >EDISPM.IR< wird an den DSOM Interface Repository Suchpfad'
          Say 'in der CONFIG.SYS angeh„ngt.'
          Say ''
          Call UpdateConfigSys
          Exit
          end
       end
     end
   end

Say 'Fehler: Objekte k”nnen nicht erstellt werden. Installation abgebrochen.'
Exit

FAILURE:
Say 'Installationsfehler.'
Signal DONE
HALT:
Say 'Installationsfehler.'
Signal DONE
SYNTAX:
Say 'Installationsfehler.'
Signal DONE
DONE:
Exit

/***************************/
/* Change the CONFIG.SYS   */
/***************************/

UpdateConfigSys:   /* change the config.sys file for IBMWorks */
/*
'@cls'
response=''

Say '    Do you want to update 'bootdrive'\CONFIG.SYS?(Y/N)'
  Parse Upper Pull response
  If response <> 'N' Then Do
     Say '    The old 'bootdrive'\CONFIG.SYS will be created called 'bootdrive'\CONFIG.IW'
     Say ' '
     Say '    Shutdown and restart your system for these changes to take affect.'
     Say ' '
  End
  Else
     return 0
*/


bootdrive = Substr(Translate(Value('PATH',,'OS2ENVIRONMENT')),Pos('\OS2\SYSTEM',Translate(Value('PATH',,'OS2ENVIRONMENT')))-2,2)

oldconfig=bootdrive'\CONFIG.SYS'
newconfig=bootdrive'\CONFIG.ED'
tmpconfig=bootdrive'\ED.SYS'

os2somir='SET SOMIR'

iniflag=0

/*
 * read the file one line at a time, and write it all out, making
 * changes accordingly - need to change path, libpath, help, IBMWORKS_INI
 */

'@del 'newconfig ' 2>nul >nul'

Do while lines(oldconfig)
    inline=linein(oldconfig)

    /******************************************/
    /* parse the line into variable and value */
    /******************************************/

    parse value inline with os2var '=' os2value

    /********************************************************/
    /* translate the variable into uppercase for comparison */
    /********************************************************/

    os2var=translate(os2var)
    Select

        /********************************************************/
        /*          For SOMIR                                   */
        /********************************************************/
        when os2var=os2somir Then Do
            ibmwline=inline

            /* see if path is already set */
            k = pos('EDISPM.IR', translate(ibmwline))
            If k = 0 Then Do
               /*
                * see if there is a ; at the end of the value
                */
               position=lastpos(';',os2value)
               lenvalue=length(os2value)

               If position=lenvalue Then do
                   /* ; at end of value */
                   ibmwline=os2var'='os2value''Directory'\EDISPM.IR;'
               End
               Else Do
                   /* ; NOT at end of value */
                   ibmwline=os2var'='os2value';'Directory'\EDISPM.IR;'
               End
            End
            /*
             * write the line out to the new config file
             */

            result=lineout(newconfig, ibmwline)

            If result=1 Then Do
                writeerror()
                return 1
            End
        End

        otherwise Do
            /*
             * write the line out to the new config file
             */

            result=lineout(newconfig, inline)
            If result=1 Then Do
                writeerror()
                return 1
            End
        End

    End  /* select */

End /* Do */

/*********************/
/* close the files   */
/*********************/

result=lineout(newconfig)
result=lineout(oldconfig)

/********************************************************************/
/* Copy created file(CONFIG.IW) to temporary file(IWTMP.SYS),       */
/* copy original file(CONFIG.SYS) to new file(CONFIG.IW),           */
/* and copy temporary file(IWTMP.SYS) to original file(CONFIG.SYS). */
/********************************************************************/

'@copy 'oldconfig' 'tmpconfig' 2>nul >nul'

'@copy 'newconfig' 'oldconfig' 2>nul >nul'

'@copy 'tmpconfig' 'newconfig' 2>nul >nul'

'@del 'tmpconfig' 2>nul >nul'

/******************************************************************/
/*  Finished the Update                                           */
/******************************************************************/

rem Say ' '
rem Say '               'bootdrive'\CONFIG.SYS erfolgreich upgedated.'
rem Say ' '

return 0


