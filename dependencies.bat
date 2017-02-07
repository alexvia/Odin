@echo off

pushd build

REM compile and generate glad.lib
cl /c /Zi  ..\src\glad\glad.c
lib /out:glad.lib glad.obj

popd