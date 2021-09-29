//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "string"
#include "fstream"

#include "Logger.h"

#define CURRENT_WORKING_DIR engine::File::getCurrentWorkingDirectory()
#define READ_FILE(path) engine::File::read(path)

#define ASSET_PATH "../../WizardTest/assets" // todo temp fix. instead cmake script should move assets
                                            // into out dir without damaging files.

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

        inline const std::string& getFilePath() const {
            return path;
        }

        inline const std::string& getName() const {
            return name;
        }

        inline void setName(const std::string& name) {
            this->name = name;
        }

        void setAssetName(const std::string &assetName);

    public:
        std::string read() const;

    public:
        static std::string read(const std::string &filePath);
        static std::string getCurrentWorkingDirectory();

    protected:
        void createName();
        virtual const char* getExtensionName() const = 0;
        virtual const char* getAssetPath() const = 0;

    protected:
        std::string path;
        std::string name;

    };

}