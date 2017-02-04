@echo off

pushd build
cl /Zi /nologo ..\src\main.cpp /link user32.lib
xcopy *.exe ..\bin /q /y
xcopy *.dll ..\bin /q /y
popd