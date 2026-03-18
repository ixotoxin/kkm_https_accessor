@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0config_build.cmd"
SET GENERATE_OPTS=-D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% -D CMAKE_BUILD_TYPE=RelWithDebInfo %COMMON_OPTS% -D WITH_SNTZ=ON -D WITH_LEAKS=%LEAKS%
SET BUILD_OPTS=--config RelWithDebInfo --verbose
SET BUILD_DIR=../__temp/sntz-clang
cmake -G Ninja %GENERATE_OPTS% -B %BUILD_DIR% -S ..
cmake --build %BUILD_DIR% %BUILD_OPTS%
cmake --install %BUILD_DIR% --prefix %INSTALL_DIR% %BUILD_OPTS%
