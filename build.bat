@echo off

pushd build
cl /Zi /nologo ..\src\odin_win32.cpp ^
/link user32.lib opengl32.lib gdi32.lib glad.lib
REM compile and generate glad.lib
REM cl /Zi  ..\src\glad.c
REM lib /out:glad.lib glad.obj
xcopy *.exe ..\bin /q /y
xcopy *.dll ..\bin /q /y
popd