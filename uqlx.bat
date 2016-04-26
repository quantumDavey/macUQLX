REM example script to fire up Xserver and UQLX in Cygwin

@echo off
     
SET DISPLAY=127.0.0.1:0.0
     
echo uqlx.bat - Starting uQLx on Windows

REM Startup the X Server with the integrated Windows-based window manager

start XWin -multiwindow -trayicon

REM Try X Server in fullscreen mode for speedup or if you have color problems
REM start XWin -fullscreen -depth 16

REM Startup uQLx
start qm.exe -f .uqlxrc
