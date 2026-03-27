// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "core.h"
#include "strings.h"
#include <atomic>
#include <barrier>

namespace Log {
    static Record s_record {};

    RecordVariant syncRecordAccessor() {
        return RecordVariant { std::in_place_type<RecordAccessor>, s_record };
    }

    using RecordAccessorFunc = RecordVariant (*)();
    static std::atomic s_recordAccessor { syncRecordAccessor };

#ifndef SINGLE_THREAD
    static std::shared_ptr<LoggerQueue> s_queue { nullptr };
    static std::thread s_bgWriter {};
    static std::barrier s_barrier { 2 };
    static bool s_atExitDisableAsync { false };

    RecordVariant asyncRecordAccessor() {
        return RecordVariant { std::in_place_type<LoggerQueue::ProducerAccessor>, s_queue->producerSlot() };
    }

    [[maybe_unused]]
    void enableAsync() noexcept {
        std::scoped_lock lock { s_mutex };
        if (!s_enableAsync || s_queue) {
            return;
        }
        write(Level::Debug, Wcs::c_enableAsync);
        s_queue = std::make_shared<LoggerQueue>();
        s_bgWriter = std::thread { [] {
            bool notEmpty {};
            while (s_queue->consuming() || notEmpty) {
                auto slot = s_queue->consumerSlot();
                notEmpty = static_cast<bool>(slot);
                if (notEmpty) {
                    write(*slot);
                    slot.complete();
                } else {
                    std::this_thread::yield();
                }
            }
            s_barrier.arrive_and_wait();
        } };
        s_recordAccessor = asyncRecordAccessor;
        if (!s_atExitDisableAsync) {
            s_atExitDisableAsync = true;
            std::atexit([] {
                disableAsync();
            });
        }
    }

    [[maybe_unused]]
    void disableAsync() noexcept {
        std::scoped_lock lock { s_mutex };
        if (!s_queue) {
            return;
        }
        s_recordAccessor = syncRecordAccessor;
        if (s_queue) {
            s_queue->stop();
            s_barrier.arrive_and_wait();
            s_queue.reset();
        }
        if (s_bgWriter.joinable()) {
            s_bgWriter.join();
        }
        write(Level::Debug, Wcs::c_disableAsync);
    }
#endif

    [[nodiscard, maybe_unused]]
    RecordVariant getFreeRecord() {
        return s_recordAccessor.load(std::memory_order_relaxed)();
    }

    [[nodiscard, maybe_unused]]
    std::wstring levelLabel(LevelUnderlying level) {
        assert(level >= c_levelDebug && level <= c_levelNone);
        if (level == c_levelNone) {
            return L"none";
        }
        if (Wcs::c_levelLabels.contains(static_cast<Level>(level))) {
            return Text::lowered(Wcs::c_levelLabels.at(static_cast<Level>(level)));
        }
        return Basic::Wcs::c_fallbackErrorMessage;
    }
}

namespace Config {
    [[maybe_unused]]
    void initLogger() {
        Log::initWriters();
    }

    [[maybe_unused]]
    void reinitLogger() {
        Log::s_record.m_message.reserve(Log::s_lineSize);
        Log::reinitWriters();
    }
}
