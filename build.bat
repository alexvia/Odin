@echo off

pushd build
cl /W4 /wd4100 /EHa /Zi /nologo ..\src\odin_win32.cpp ^
/link user32.lib opengl32.lib gdi32.lib

cl /LD ..\src\odin.cpp /link glad.lib
REM compile and generate glad.lib
REM cl /Zi  ..\src\glad\glad.c
REM lib /out:glad.lib glad.obj
xcopy *.exe ..\bin /q /y
xcopy *.dll ..\bin /q /y
popd