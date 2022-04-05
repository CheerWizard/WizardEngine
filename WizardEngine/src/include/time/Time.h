//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <chrono>

#define MILLIS(millis) time::ms(millis)
#define NANO_UNIT 1.0E-9
#define MILLI_UNIT 1000.0f

namespace engine::time {

    typedef std::chrono::milliseconds ms;
    typedef std::chrono::nanoseconds nano;

    class Time {

    public:
        Time(const float &seconds = 0.0f) : _seconds(seconds) {}

        Time(const ms &ms) {
            _seconds = (float) ms.count() / (float) MILLI_UNIT ;
        }

        ~Time() = default;

    public:
        explicit operator float() const { return _seconds; }
        explicit operator long long() const { return (long long) _seconds; }

        inline bool operator< (const Time& other) const { return _seconds < other.getSeconds(); }
        inline bool operator> (const Time& other) const { return _seconds > other.getSeconds(); }
        inline bool operator== (const Time& other) const { return _seconds == other.getSeconds(); }
        inline bool operator>= (const Time& other) const { return _seconds >= other.getSeconds(); }
        inline bool operator<= (const Time& other) const { return _seconds <= other.getSeconds(); }
        inline float operator- (const Time& other) const { return _seconds - other.getSeconds(); }
        inline float operator/ (const Time& other) const { return _seconds / other.getSeconds(); }

    public:
        inline const float& getSeconds() const { return _seconds; }
        inline float getMilliseconds() const { return _seconds * MILLI_UNIT; }
        inline float getNanoSeconds() const { return _seconds * NANO_UNIT; }
        inline long long getLongMs() const {
            auto ms = (long long) getMilliseconds();
            return ms;
        }

        inline void setMs(const float &ms) {
            _seconds = ms / MILLI_UNIT;
        }

    private:
        float _seconds;
    };

    inline float operator* (const float& t1, const Time &t2) { return t1 * t2.getSeconds(); }
    inline float operator/ (const float& t1, const Time &t2) { return t1 / t2.getSeconds(); }

}