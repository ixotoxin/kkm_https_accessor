// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake/options.h>
#include <lib/setcli.h>
#include <lib/except.h>
#include <lib/errexp.h>
#include <lib/wconv.h>
#include <debug/memprof.h>
#include <constants.h>
#include <cstdlib>
#include <iostream>
#include <format>
#include <shellapi.h>
#include "registry.h"

using Basic::Failure;

constexpr auto c_pipeName { L"\\\\.\\pipe\\KKMHA_REGMSG" };
wchar_t s_binaryFile[MAX_PATH] {};

void registerResources(const std::wstring_view application) {
    const std::wstring registryPath {
        std::format(L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\{}", application)
    };
    const Registry::RwKey reg(HKEY_LOCAL_MACHINE, registryPath);
    reg.setNumber(L"TypesSupported", EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE);
    reg.setString(L"EventMessageFile", s_binaryFile);
    reg.setString(L"CategoryMessageFile", s_binaryFile);
    reg.setNumber(L"CategoryCount", 6);
}

void writeToPipe(const std::wstring_view message) {
    std::wcerr << message << std::endl;
    ::HANDLE pipe { ::CreateFileW(c_pipeName, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr) };
    if (pipe != INVALID_HANDLE_VALUE) {
        ::DWORD bytesWrite;
        ::WriteFile(pipe, message.data(), message.size(), &bytesWrite, nullptr);
        ::CloseHandle(pipe);
    }
}

int wmain() {
    Config::initConsole(Config::c_u16Text);
    Config::initProfiler();

    try {
        std::wstring wsBinaryFile;
        if (!::GetModuleFileNameW(nullptr, s_binaryFile, ARRAYSIZE(s_binaryFile))) {
            throw Failure(System::explainError(L"GetModuleFileName(...)")); // NOLINT(*-exception-baseclass)
        }

        bool isElevated { false };
        ::HANDLE token { nullptr };
        if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &token)) {
            ::TOKEN_ELEVATION elevation;
            ::DWORD size = sizeof(::TOKEN_ELEVATION);
            if (::GetTokenInformation(token, TokenElevation, &elevation, size, &size)) {
                isElevated = static_cast<bool>(elevation.TokenIsElevated);
            }
            ::CloseHandle(token);
        }

        if (!isElevated) {
            std::wcout << L"WRN: Недостаточно прав.\nINF: Запрашиваем повышение..." << std::endl;

            ::HANDLE pipe {
                ::CreateNamedPipeW(
                    c_pipeName, PIPE_ACCESS_INBOUND,  PIPE_TYPE_BYTE | PIPE_WAIT, 1, 1024, 1024, 0, nullptr
                )
            };
            if (pipe == INVALID_HANDLE_VALUE) {
                throw Failure(System::explainError(L"CreateNamedPipeW(...)")); // NOLINT(*-exception-baseclass)
            }

            ::SHELLEXECUTEINFOW sei {};
            sei.cbSize = sizeof(sei);
            sei.fMask = SEE_MASK_DEFAULT;
            sei.hwnd = nullptr;
            sei.lpVerb = L"runas";
            sei.lpFile = s_binaryFile;
            sei.nShow = SW_NORMAL;

            if (::ShellExecuteExW(&sei)) {
                if (::ConnectNamedPipe(pipe, nullptr) || ::GetLastError() == ERROR_PIPE_CONNECTED) {
                    wchar_t buffer[1024] {};
                    constexpr auto bufferSize { sizeof(buffer) - sizeof(wchar_t) };
                    ::DWORD bytesRead;
                    while (::ReadFile(pipe, buffer, bufferSize, &bytesRead, nullptr) && bytesRead > 0) {
                        buffer[bytesRead] = '\0';
                        std::wcout << buffer;
                    }
                }
                ::WaitForSingleObject(sei.hProcess, INFINITE);
                ::CloseHandle(sei.hProcess);
            } else {
                if (::GetLastError() == ERROR_CANCELLED) {
                    std::wcerr << L"ERR: Пользователь отказался предоставить права." << std::endl;
                }
            }
            return EXIT_SUCCESS;
        }

        registerResources(Log::EventLog::c_kkmHttpAccessor);
#if BUILD_SEPARATED
        registerResources(Log::EventLog::c_kkmOperator);
        registerResources(Log::EventLog::c_kkmJsonLoader);
#endif

        writeToPipe(L"INF: Ресурсы для корректного отображения сообщений в EvenLog зарегистрированы.");

        return EXIT_SUCCESS;
    } catch (const Failure & e) {
        std::wstring message { L"ERR: " };
        message.append(e.what());
        writeToPipe(message);
    } catch (const std::exception & e) {
        std::wstring message { L"ERR: " };
        Text::appendConverted(message, e.what());
        writeToPipe(message);
    } catch (...) {
        std::wstring message { L"ERR: " };
        message.append(Basic::Wcs::c_somethingWrong);
        writeToPipe(message);
    }

    return EXIT_FAILURE;
}
