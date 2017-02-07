@echo off

pushd build

cl /W4 /wd4201 /wd4100 /EHa /Zi /nologo ..\src\odin_win32.cpp ^
/link user32.lib opengl32.lib gdi32.lib

cl /Zi /LD ..\src\odin.cpp /link glad.lib

xcopy *.exe ..\bin /q /y
xcopy *.dll ..\bin /q /y

popd