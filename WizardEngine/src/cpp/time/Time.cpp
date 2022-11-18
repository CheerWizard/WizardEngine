//
// Created by mecha on 03.09.2022.
//

#include <time/Time.h>

#include <ctime>
#include <sstream>
#include <iomanip>

namespace engine::time {

    float Time::milliseconds() const {
        return ms;
    }

    float Time::seconds() const {
        return ms / 1000;
    }

    std::string currentLocalTime() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "[%d-%m-%Y %H-%M-%S]");
        return oss.str();
    }

}