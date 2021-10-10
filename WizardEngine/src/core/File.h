//
// Created by mecha on 11.09.2021.
//

#pragma once

#include "String.h"
#include "Logger.h"

#include <filesystem>

#define CURRENT_WORKING_DIR engine::File::getCurrentWorkingDirectory()
#define READ_FILE(path) engine::File::read(path)

#define CLIENT_ASSET_PATH "assets"
#define ENGINE_ASSET_PATH "../WizardEngine/assets"

namespace engine {

    class File {

    public:
        File() = default;

        File(const std::string& name, const std::string& assetPath) : name(name), assetPath(assetPath) {
            if (name.empty()) {
                createName();
            }
        }

        virtual ~File() = default;

    public:
        inline const std::string& getName() const {
            return name;
        }

        inline void setName(const std::string& name) {
            this->name = name;
        }

        inline void setAssetPath(const std::string &assetPath) {
            this->assetPath = assetPath;
        }

    public:
        std::string createPath(const std::string &assetName) const;
        std::string readAsset(const std::string &assetName) const;

    public:
        static std::string read(const std::string &filePath);
        static bool write(const std::string &filePath, const std::string &source);
        static std::string getCurrentWorkingDirectory();
        static bool copy(const std::string &src, const std::string &dest);
        static bool remove(const std::string &target);

    protected:
        void createName();
        virtual const char* getExtensionName() const = 0;

    protected:
        std::string name;
        std::string assetPath;

    };

}