@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0config_build.cmd"
SET GENERATE_OPTS=-D CMAKE_BUILD_TYPE=Debug %COMMON_OPTS% -D WITH_CRTD=ON -D WITH_LEAKS=%LEAKS%
SET BUILD_OPTS=--config Debug --verbose
SET BUILD_DIR=../__temp/crtd-msvc
cmake -G "Visual Studio 17 2022" %GENERATE_OPTS% -B %BUILD_DIR% -S ..
cmake --build %BUILD_DIR% %BUILD_OPTS%
cmake --install %BUILD_DIR% --prefix %INSTALL_DIR% %BUILD_OPTS%
