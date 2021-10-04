//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "fstream"

#include "String.h"
#include "Logger.h"

#define CURRENT_WORKING_DIR engine::File::getCurrentWorkingDirectory()
#define READ_FILE(path) engine::File::read(path)

#define CLIENT_ASSET_PATH "assets"
#define ENGINE_ASSET_PATH "../WizardEngine/assets"

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

        inline void setAssetPath(const std::string &assetPath) {
            this->assetPath = assetPath;
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
        std::string assetPath = CLIENT_ASSET_PATH;

    };

}