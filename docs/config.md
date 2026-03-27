## Конфигурирование

После запуска `kkmha.exe` рабочей директорией устанавливается директория, в которой находится `kkmha.exe`. Все
относительные пути вычисляются относительно рабочей директории (далее `{work-dir}`). Если не установлена переменная
окружения `KKMHA_CONF_DIR` конфигурационный файл `kkmha.json` ищется в директории `{work-fir}\conf`. Все
относительные пути в конфигурационном файле вычисляются относительно директории с основным конфигурационным файлом,
кроме опции `log.file.directory`. Она вычисляется относительно рабочей директории. Пример конфигурационного файла:

```json
{
    "log": {
        "console": {
            "level": "debug",
            "terse": true,
            "output": "separated",
            "flushEveryWrite": "no"
        },
        "file": {
            "level": {
                "foreground": "none",
                "background": "debug"
            },
            "directory": "logs",
            "flushEveryWrite": true
        },
        "eventLog": {
            "level": {
                "foreground": "none",
                "background": "info"
            }
        },
        "appendLocation": false,
        "lineSize": 384,
        "enableAsync": true,
        "maxQueueBlocks": 16
    },
    "server": {
        "ipv4Only": false,
        "port": 5757,
        "requestTimeout": 180,
        "concurrencyLimit": 10,
        "enableLegacyTls": "no",
        "securityLevel": 5,
        "certificateChainFile": "kkmha.crt",
        "privateKeyFile": "kkmha.key",
        "privateKeyPassword": "",
        "secret": "lorem.ipsum.dolor.sit.amet",
        "loopbackWithoutSecret": false,
        "enableStatic": false,
        "staticDirectory": "..\\static",
        "indexFile": "index.html",
        "mimeMap": "mime.json",
        "enableUnknownType": false
    },
    "kkm": {
        "dbDirectory": "kkm",
        "defaultBaudRate": 115200,
        "defaultLineLength": 42,
        "timeZone": "device",
        "fallbackFfdVersion": "1.0.5",
        "ffdVersionDetect": "never",
        "documentClosingTimeout": 5000,
        "cliOperator": {
            "name": "Оператор"
        },
        "customerAccountField": "Лицевой счёт: ",
        "maxCashInOut": 500000,
        "maxPrice": 300000,
        "maxQuantity": 1000
    }
}
```

| ОПЦИЯ                           | ТИП                                                                                  | ОПИСАНИЕ                                                                                                              |
|---------------------------------|--------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| `log.console.level`             | Enum: "debug", "info", "warning", "error", "none"                                    | Уровень логирования в консоль.                                                                                        |
| `log.console.terse`             | Bool: true, false, "yes", "no"                                                       | Включить/выключить вывод даты и времени в консоль.                                                                    |
| `log.console.output`            | Enum: "separated", "stdout", "stderr"                                                | Вывод. При значении "separated" уровень выше или равно "warning" выводится в stderr, иначе в stdout.                  |
| `log.console.flushEveryWrite`   | Bool: true, false, "yes", "no"                                                       | Включить/выключить flush каждой записи.                                                                               |
| `log.file.level.foreground`     | Enum: "debug", "info", "warning", "error", "none"                                    | Уровень логирования в файл для foreground-процесса.                                                                   |
| `log.file.level.background`     | Enum: "debug", "info", "warning", "error", "none"                                    | Уровень логирования в файл для background-процесса.                                                                   |
| `log.file.directory`            | Path (string)                                                                        | Директория, в которую будет происходить логирование.                                                                  |
| `log.file.flushEveryWrite`      | Bool: true, false, "yes", "no"                                                       | Включить/выключить flush каждой записи.                                                                               |
| `log.eventLog.level.foreground` | Enum: "debug", "info", "warning", "error", "none"                                    | Уровень логирования в журнал событий Windows для foreground-процесса.                                                 |
| `log.eventLog.level.background` | Enum: "debug", "info", "warning", "error", "none"                                    | Уровень логирования в журнал событий Windows для background-процесса.                                                 |
| `log.appendLocation`            | Bool: true, false, "yes", "no"                                                       | Включить/выключить вывод точки происхождения сообщения в исходных файлах.                                             |
| `log.lineSize`                  | Integer: 64 - 1'024                                                                  | Первичное резезервирование памяти для строки.                                                                         |
| `log.enableAsync`               | Bool: true, false, "yes", "no"                                                       | Включить/выключить асинхронное логирование.                                                                           |
| `log.maxQueueBlocks`            | Integer: 1 - 64                                                                      | Максимально количество блоков допустимое для очереди в режиме асинхронного логирования (размер блока 1024 записи).    |
| `server.ipv4Only`               | Bool: true, false, "yes", "no"                                                       | Включить/выключить поддержку IPv6.                                                                                    |
| `server.port`                   | Interger: 1 - 65'535                                                                 | Порт, который будет слушать сервер.                                                                                   |
| `server.requestTimeout`         | Seconds (integer): 6 - 1'800                                                         | Таймаут (в секундах).                                                                                                 |
| `server.concurrencyLimit`       | Interger: 2 - 100                                                                    | Ограничение максимального количества одновременных соединений.                                                        |
| `server.enableLegacyTls`        | Bool: true, false, "yes", "no"                                                       | Разрешить/запретить поддержку TLS 1.0 и TLS 1.1.                                                                      |
| `server.securityLevel`          | Interger: 0 - 5                                                                      | Уровень безопасности устанавливаемый в библиотеке OpenSSL (0 - 5). Только для `"enableLegacyTls": false`.             |
| `server.certificateChainFile`   | Path (string)                                                                        | Путь к файлу сертификата.                                                                                             |
| `server.privateKeyFile`         | Path (string)                                                                        | Путь к файлу ключа.                                                                                                   |
| `server.privateKeyPassword`     | String                                                                               | Пароль от ключа.                                                                                                      |
| `server.secret`                 | String                                                                               | Access-токен.                                                                                                         |
| `server.loopbackWithoutSecret`  | Bool: true, false, "yes", "no"                                                       | Разрешить/запретить локальные запросы без access-токена.                                                              | 
| `server.enableStatic`           | Bool: true, false, "yes", "no"                                                       | Разрешить/запретить обработку запросов `https://127.0.0.1:5757/static/{file-path}`.                                   |
| `server.staticDirectory`        | Path (string)                                                                        | Путь к директории, содержимое которой будет отдаваться для запросов `/static/{file-path}`.                            |
| `server.indexFile`              | Path (string)                                                                        | Имя индексного файла. На этот файл происходит перенаправление, если запрашиваемый путь является директорией.          |
| `server.mimeMap`                | Path (string)                                                                        | Путь к файлу с описанием типа содержимого.                                                                            |
| `server.enableUnknownType`      | Bool: true, false, "yes", "no"                                                       | Разрешить/запретить отдавать файлы с расширениями не представленными в файле `mime.json` заданном опцией `"mimeMap"`. |
| `kkm.dbDirectory`               | Path (string)                                                                        | Путь к директории, в которой будет формироваться БД известных ККМ.                                                    |
| `kkm.defaultBaudRate`           | Integer: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 | Скорость COM-порта по-умолчанию.                                                                                      |
| `kkm.defaultLineLength`         | Integer: 24 - 192                                                                    | Ширина чековой ленты по-умолчанию. Используется, если данное свойство не удается получить опросом ККМ.                |
| `kkm.timeZone`                  | Enum                                                                                 | Временная зона передаваемая ОФД при регистрации чека.                                                                 |
| `kkm.fallbackFfdVersion`        | Enum: "1.0.5", "1.1", "1.2"                                                          | Предполагаемая версия ФФД, если не удалось получить информацию о версии от ККМ.                                       |
| `kkm.ffdVersionDetect`          | Enum: "never", "once", "always"                                                      | Момент определения версии ФФД.                                                                                        |
| `kkm.documentClosingTimeout`    | Milliseconds (integer): 200 - 10'000                                                 | Таймаут проверки корректности закрытия документа (в миллисекундах).                                                   |
| `kkm.cliOperator.name`          | String                                                                               | Имя оператора для консольных команд требующих логина оператора ККМ.                                                   |
| `kkm.customerAccountField`      | String                                                                               | Заголовок поля с номером лицевого счета клиента.                                                                      |
| `kkm.maxCashInOut`              | Float: 1 - 1'000'000'000                                                             | Максимальная сумма для внесения или выплаты.                                                                          |
| `kkm.maxPrice`                  | Float: 1 - 1'000'000'000                                                             | Максимальная цена товара/услуги в чеке.                                                                               |
| `kkm.maxQuantity`               | Float: 1 - 1'000'000'000                                                             | Максимальное количество товара/услуги в чеке.                                                                         |

***Сертификат и ключ никак не проверяются***, поэтому можно использовать самоподписанный сертификат.
<!-- Создать его можно например так:
```cmd
openssl req -x509 -sha256 -nodes -days 3650 -newkey rsa:2048 -keyout kkmha.test.ss.key -out kkmha.test.ss.crt
```
-->
Так же можно изменить параметры по-умолчанию и недоступные для конфигурирования в следующих файлах:
- `.\src\library\log2\defaults.h`;
- `.\src\library\log2\variables.h`;
- `.\src\library\kkm\defaults.h`;
- `.\src\library\kkm\variables.h`;
- `.\src\library\config\defaults.h`;
- `.\src\library\config\variables.h`;
- `.\src\library\variables.cpp`;
- `.\src\kkmha\http_defaults.h`;
- `.\src\kkmha\server_defaults.h`;
- `.\src\kkmha\server_variables.h`;
- `.\src\kkmha\server_static_defaults.h`;
- `.\src\kkmha\server_static_variables.h`;
- `.\src\kkmha\service_defaults.h`;
- `.\src\kkmha\kkmha_variables.cpp`;
- `.\src\kkmop\kkmop_variables.cpp`;
- `.\src\kkmjl\kkmjl_variables.cpp`.

---
[Назад к содержанию](../README.md)
