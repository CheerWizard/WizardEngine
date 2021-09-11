//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "string"
#include "fstream"

#include "Logger.h"

namespace engine {

    class File {

    public:
        File(const std::string& filePath = "Unknown") : filePath(filePath) {}
        virtual ~File() = default;

    public:
        std::string read();

    public:
        inline void setFilePath(const std::string& filePath) {
            this->filePath = filePath;
        }
        inline std::string getFilePath() const {
            return filePath;
        }

    protected:
        std::string filePath;

    };

}