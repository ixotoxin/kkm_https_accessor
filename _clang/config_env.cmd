@ECHO OFF

IF NOT "%CLANG_DIR%" == "" GOTO CLANG_DIR_IS_SET
@REM --- Standalone Clang -------------------------------------------------------------
SET CLANG_DIR=C:\Devel\Platform\Clang\22.1.2-x86_64
SET CLANG_SANLIB_DIR=%CLANG_DIR%\lib\clang\22\lib\windows
SET CMAKE_DIR=C:\Devel\Platform\CMake\4.2.3-x86_64
SET NINJA_DIR=C:\Devel\Platform\Ninja\1.13.2
@REM --- MSVS bundle Clang ------------------------------------------------------------
@REM SET MSVC_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community
@REM SET CLANG_DIR=%MSVC_DIR%\VC\Tools\Llvm\x64\bin
@REM SET CLANG_SANLIB_DIR=%MSVC_DIR%\VC\Tools\Llvm\x64\lib\clang\19\lib\windows
@REM SET CMAKE_DIR=%MSVC_DIR%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake
@REM SET NINJA_DIR=%MSVC_DIR%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja
@REM CALL "%MSVC_DIR%\VC\Auxiliary\Build\vcvars64.bat"
@REM ----------------------------------------------------------------------------------
SET CLANG_CXX_FRONTEND=clang++
@REM SET CLANG_CXX_FRONTEND=clang-cl
SET VCPKG_DIR=C:\Devel\Platform\vcpkg
SET VCPKG_BLD=C:\Devel\Platform\vcpkg\buildtrees
SET VCPKG_TCS=%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake
SET PATH=%NINJA_DIR%;%CMAKE_DIR%\bin;%CLANG_DIR%\bin;%PATH%
:CLANG_DIR_IS_SET

IF NOT EXIST "%~dp0../__temp" MKDIR "%~dp0../__temp"
IF NOT EXIST "%~dp0../_build" MKDIR "%~dp0../_build"
