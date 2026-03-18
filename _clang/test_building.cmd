@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0config_build.cmd"

ECHO Build and test:
SET TEST_OPTS0=-D CMAKE_BUILD_TYPE=Debug -D BUILD_TESTS=ON -D BUILD_STATIC=%STATIC%

ECHO   debug config #1...
SET TEST_OPTS=%TEST_OPTS0%
SET TEST_PATH=../__temp/test1-clang
cmake -G Ninja -D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% %TEST_OPTS% -B %TEST_PATH% -S .. > %TEST_PATH%.log 2>&1
cmake --build %TEST_PATH% --config Debug --verbose >> %TEST_PATH%.log 2>&1
ctest --test-dir %TEST_PATH% --build-config Debug --rerun-failed --output-on-failure >> %TEST_PATH%.log 2>&1

ECHO   debug config #2...
SET TEST_OPTS=%TEST_OPTS0% -D WITH_SHORTCUTS=ON -D BUILD_SEPARATED=OFF -D WITH_RELSL=ON -D WITH_SBIAC=OFF
SET TEST_PATH=../__temp/test2-clang
cmake -G Ninja -D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% %TEST_OPTS% -B %TEST_PATH% -S .. > %TEST_PATH%.log 2>&1
cmake --build %TEST_PATH% --config Debug --verbose >> %TEST_PATH%.log 2>&1
ctest --test-dir %TEST_PATH% --build-config Debug --rerun-failed --output-on-failure >> %TEST_PATH%.log 2>&1

ECHO   debug config #3...
SET TEST_OPTS=%TEST_OPTS0% -D WITH_SHORTCUTS=OFF -D BUILD_SEPARATED=ON -D WITH_RELSL=OFF -D WITH_SBIAC=ON
SET TEST_PATH=../__temp/test3-clang
cmake -G Ninja -D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% %TEST_OPTS% -B %TEST_PATH% -S .. > %TEST_PATH%.log 2>&1
cmake --build %TEST_PATH% --config Debug --verbose >> %TEST_PATH%.log 2>&1
ctest --test-dir %TEST_PATH% --build-config Debug --rerun-failed --output-on-failure >> %TEST_PATH%.log 2>&1

ECHO   debug config #4...
SET TEST_OPTS=%TEST_OPTS0% -D WITH_SHORTCUTS=ON -D BUILD_SEPARATED=ON -D WITH_RELSL=ON -D WITH_SBIAC=ON
SET TEST_PATH=../__temp/test4-clang
cmake -G Ninja -D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% %TEST_OPTS% -B %TEST_PATH% -S .. > %TEST_PATH%.log 2>&1
cmake --build %TEST_PATH% --config Debug --verbose >> %TEST_PATH%.log 2>&1
ctest --test-dir %TEST_PATH% --build-config Debug --rerun-failed --output-on-failure >> %TEST_PATH%.log 2>&1

ECHO   configured debug...
SET TEST_OPTS=-D CMAKE_BUILD_TYPE=Debug %COMMON_OPTS% -D BUILD_TESTS=ON
SET TEST_PATH=../__temp/test5-clang
cmake -G Ninja -D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% %TEST_OPTS% -B %TEST_PATH% -S .. > %TEST_PATH%.log 2>&1
cmake --build %TEST_PATH% --config Debug --verbose >> %TEST_PATH%.log 2>&1
ctest --test-dir %TEST_PATH% --build-config Debug --rerun-failed --output-on-failure >> %TEST_PATH%.log 2>&1

ECHO   configured release...
SET TEST_OPTS=-D CMAKE_BUILD_TYPE=Release %COMMON_OPTS% -D BUILD_TESTS=ON
SET TEST_PATH=../__temp/test6-clang
cmake -G Ninja -D CMAKE_CXX_COMPILER=%CLANG_CXX_FRONTEND% %TEST_OPTS% -B %TEST_PATH% -S .. > %TEST_PATH%.log 2>&1
cmake --build %TEST_PATH% --config Release --verbose >> %TEST_PATH%.log 2>&1
ctest --test-dir %TEST_PATH% --build-config Release --rerun-failed --output-on-failure >> %TEST_PATH%.log 2>&1

ECHO See the results in the files ../__temp/test?-clang.log
