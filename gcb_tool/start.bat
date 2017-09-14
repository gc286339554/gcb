lua.exe start.lua

rd /s /q  ..\..\src\commom\op\details\
xcopy generate_cpp\* ..\gcb_example\src\generate\ /s /y
xcopy generate_lua\* ..\gcb_example\src\generate\ /s /y
pause