@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0config_build.cmd"
SET GENERATE_OPTS=-D CMAKE_BUILD_TYPE=Release %COMMON_OPTS%
SET BUILD_OPTS=--config Release --verbose
SET BUILD_DIR=../__temp/release-msvc
cmake -G "Visual Studio 17 2022" %GENERATE_OPTS% -B %BUILD_DIR% -S ..
cmake --build %BUILD_DIR% %BUILD_OPTS%
cmake --install %BUILD_DIR% --prefix %INSTALL_DIR% %BUILD_OPTS%
