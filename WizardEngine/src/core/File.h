//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "string"
#include "fstream"

#include "Logger.h"

#define CURRENT_WORKING_DIR engine::File::getCurrentWorkingDirectory()
#define READ_FILE(path) engine::File::read(path)

namespace engine {

    class File {

    public:
        File() = default;

        File(const std::string& name, const std::string& path) :
                name(name),
                path(path)
        {
            if (name.empty()) {
                createName();
            }
        }

        virtual ~File() = default;

    public:
        inline void setFilePath(const std::string& filePath) {
            this->path = filePath;
        }

        inline std::string getFilePath() const {
            return path;
        }

        inline std::string getName() const {
            return name;
        }

        inline void setName(const std::string& name) {
            this->name = name;
        }

    public:
        std::string read() const;

    public:
        static std::string read(const std::string &filePath);
        static std::string getCurrentWorkingDirectory();

    protected:
        void createName();

    protected:
        std::string path;
        std::string name;

    };

}