@echo off

pushd build
cl /Zi /nologo ..\src\odin_win32.cpp /link user32.lib opengl32.lib gdi32.lib
xcopy *.exe ..\bin /q /y
xcopy *.dll ..\bin /q /y
popd