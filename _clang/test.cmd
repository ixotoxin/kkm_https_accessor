@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0config_build.cmd"
SET GENERATE_OPTS=-D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% -D CMAKE_BUILD_TYPE=Debug %COMMON_OPTS% -D BUILD_TESTS=ON
SET BUILD_OPTS=--config Debug --verbose
SET BUILD_DIR=../__temp/tests-clang
SET TEST_OPTS=--build-config Debug --rerun-failed --output-on-failure
cmake -G Ninja %GENERATE_OPTS% -B %BUILD_DIR% -S ..
cmake --build %BUILD_DIR% %BUILD_OPTS%
ctest --test-dir %BUILD_DIR% %TEST_OPTS%
