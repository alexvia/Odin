@echo off

pushd build

REM generate executable
cl /W4 /wd4201 /wd4100 /EHa /Zi /nologo ..\src\odin_win32.cpp ^
/link user32.lib opengl32.lib gdi32.lib

REM generate dll
cl /Zi /LD ..\src\odin.cpp /link glad.lib

REM copy output files to bin folder
xcopy *.exe ..\bin /q /y
xcopy *.dll ..\bin /q /y

popd