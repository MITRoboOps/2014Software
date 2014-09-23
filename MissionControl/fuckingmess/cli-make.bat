@ECHO OFF

SETLOCAL
SET CFLAGS=
SET LFLAGS=
SET STRIP=YES

SET SPEECH=ESPEAK
IF "%SPEECH%"=="SAM" SET SPEECH_LIBS=-lmsvcrt -lsam
IF "%SPEECH%"=="SAM" SET CFLAGS=%CFLAGS% -DUSE_SAM
IF "%SPEECH%"=="ESPEAK" SET SPEECH_LIBS=-lespeak

ECHO Compiling cli.c...
gcc cli.c %CFLAGS% -I ./include -I ./include/ffmpeg -c
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling cli_term.c...
gcc cli_term.c %CFLAGS% -I ./include -c
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling oswrap.c...
gcc oswrap.c %CFLAGS% -I ./include -c
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling speech.c...
gcc speech.c %CFLAGS% -I ./include -I ./include/ffmpeg -c
IF ERRORLEVEL 1 GOTO ERROR

ECHO Compiling utils.c...
gcc utils.c -c %CFLAGS% -I./include

ECHO Resources...
windres cli-w32.rc -O coff -o cli.res

ECHO Linking...
g++ oswrap.o cli_term.o cli.o speech.o utils.o cli.res %LFLAGS% -I ./include -L ./lib-w32 -mwindows -lmingw32 -lsdlmain -lsdl -lavcodec -lavutil -lwsock32 -lswscale -lrcplug_cli %SPEECH_LIBS% -o bin/cli.exe
g++ oswrap.o cli_term.o cli.o speech.o utils.o cli.res %LFLAGS% -I ./include -L ./lib-w32                               -lsdl -lavcodec -lavutil -lwsock32 -lswscale -lrcplug_cli %SPEECH_LIBS% -o bin/cli_nosdl.exe
IF ERRORLEVEL 1 GOTO ERROR

ECHO Cleaning up...
del *.o
del *.res
IF %STRIP%==YES strip bin/cli.exe
IF %STRIP%==YES strip bin/cli_nosdl.exe

ECHO Done!

:ERROR

ENDLOCAL

IF "%1"=="-run" GOTO RUN

GOTO FINAL

:RUN
ECHO Running application...
cd bin
cli
cd ..
GOTO FINAL

:FINAL