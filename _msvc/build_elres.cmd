@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cd "%~dp0..\\src\\resources"
mc.exe -u -U -r .\\compiled -h .\\compiled messages.mc
rc.exe /i . /fo .\\compiled\\messages.res .\\compiled\\messages.rc
cd "%~dp0"
