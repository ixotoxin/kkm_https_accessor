@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cd "%~dp0..\\src\\regmsg"
mc.exe -u -U -r .\\resources -h .\\resources messages.mc
rc.exe /i . /fo .\\resources\\messages.res .\\resources\\messages.rc
cd "%~dp0"
