@echo off

pushd build
cl -nologo ..\src\main.cpp
xcopy *.exe ..\bin /q /y
popd