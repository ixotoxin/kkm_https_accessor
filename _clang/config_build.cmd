@ECHO OFF

REM Сборка приложения с поддержкой сокращенных команд
SET SHORTCUTS=OFF

REM Сборка приложения как отдельных исполняемых файлов
SET SEPARATED=OFF

REM Статическая сборка
SET STATIC=OFF

REM Использовать относительные пути исходных файлов в приложении
SET RELSL=ON

REM Разрешить инвазивный доступ к буферу std::string (ересь)
SET SBIAC=OFF

REM Создание утечек памяти в отладочной сборке
SET LEAKS=ON

SET COMMON_OPTS=-D WITH_SHORTCUTS=%SHORTCUTS% -D BUILD_SEPARATED=%SEPARATED% -D BUILD_STATIC=%STATIC% -D WITH_RELSL=%RELSL% -D WITH_SBIAC=%SBIAC%
SET INSTALL_DIR=../_build/
