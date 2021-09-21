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
        File(const std::string& filePath = "Unknown") : filePath(filePath) {
            createName();
        }
        virtual ~File() = default;

    public:
        std::string read() const;

    public:
        inline void setFilePath(const std::string& filePath) {
            this->filePath = filePath;
        }

        inline std::string getFilePath() const {
            return filePath;
        }

        inline std::string getName() const {
            return name;
        }

        inline void setName(const std::string& name) {
            this->name = name;
        }

    protected:
        void createName();

    protected:
        std::string filePath;
        std::string name;

    };

}