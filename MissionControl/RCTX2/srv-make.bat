@ECHO OFF

SETLOCAL
SET CFLAGS=-g
SET LFLAGS=-g
SET STRIP=NO

SET SPEECH=ESPEAK
IF "%SPEECH%"=="SAM" SET SPEECH_LIBS=-lmsvcrt -lsam
IF "%SPEECH%"=="SAM" SET CFLAGS=%CFLAGS% -DUSE_SAM
IF "%SPEECH%"=="ESPEAK" SET SPEECH_LIBS=-lespeak

IF "%1"=="-service" GOTO SERVICE_STOP

:BEGIN

ECHO Compiling capture.cpp...
g++ capture.cpp -c %CFLAGS% -I./include
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling srv.c...
gcc srv.c -c %CFLAGS% -I./include -I./include/ffmpeg
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling oswrap.c...
gcc oswrap.c -c %CFLAGS% -I./include
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling speech.c...
gcc speech.c -c %CFLAGS% -I./include -I./include/ffmpeg
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling utils.c...
gcc utils.c -c %CFLAGS% -I./include

ECHO Linking...
g++ oswrap.o capture.o srv.o speech.o utils.o %LFLAGS% -L ./lib-w32                               -lsdl -lkernel32 -lwsock32 -lx264 -lmsvcrt -lswscale -lavutil -lvideoinput -lddraw -ldxguid -lole32 -loleaut32 -lstrmiids -luuid -lrcplug_srv %SPEECH_LIBS% -o bin/srv.exe
g++ oswrap.o capture.o srv.o speech.o utils.o %LFLAGS% -L ./lib-w32 -mwindows -lmingw32 -lsdlmain -lsdl -lkernel32 -lwsock32 -lx264 -lmsvcrt -lswscale -lavutil -lvideoinput -lddraw -ldxguid -lole32 -loleaut32 -lstrmiids -luuid -lrcplug_srv %SPEECH_LIBS% -o bin/srv_sdl.exe
IF ERRORLEVEL 1 GOTO ERROR

ECHO Cleaning up...
del *.o
IF %STRIP%==YES strip bin/srv.exe
IF %STRIP%==YES strip bin/srv_sdl.exe

ECHO Done!

:ERROR

ENDLOCAL

IF "%1"=="-run" GOTO RUN
IF "%1"=="-service" GOTO SERVICE_START

GOTO FINAL

:SERVICE_STOP
ECHO Stopping KiwiRayServer...
sc stop KiwiRayServer
GOTO BEGIN

:SERVICE_START
ECHO Starting KiwiRayServer...
sc start KiwiRayServer
GOTO FINAL

:RUN
ECHO Running application...
cd bin
srv
cd ..
GOTO FINAL

:FINAL