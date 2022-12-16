//
// Created by mecha on 05.10.2022.
//

#pragma once

#include <time/Time.h>
#include <core/filesystem.h>
#include <core/ProjectManager.h>

#include <platform/tools/FileDialog.h>
#include <platform/graphics/TextureBuffer.h>

#define DRAG_DROP_ITEM_TYPE "ASSET_BROWSER_ITEM"

namespace engine::visual {

    using namespace engine::core;
    using namespace engine::tools;
    using namespace engine::time;

    class ENGINE_API AssetBrowserCallback {

    public:
        virtual ~AssetBrowserCallback() = default;

    public:
        virtual void onImageOpen(const std::string &fileName) = 0;
        virtual void onObjOpen(const std::string &fileName) = 0;
        virtual void onAssetImported(const std::string &assetPath) = 0;
        virtual void onAssetExported(const std::string &assetPath) = 0;
        virtual void onAssetRemoved(const std::string &assetPath) = 0;
    };

    struct ENGINE_API AssetBrowserItem {
        const char* extension;
        const uint32_t iconId;
    };

    class ENGINE_API AssetBrowser final {

    public:
        static void create(void* nativeWindow);
        static void destroy();
        static void setProject(const Project& project);

        static void draw(Time dt);

    private:
        static void importAsset(const std::filesystem::path &assetDir);
        static void exportAsset(const std::string &assetPath);
        static void removeAsset(const std::string &assetPath);
        static void popupAssetMenu(const char* id);

        static ProjectVersion getSelectedProjectVersion();

    public:
        static AssetBrowserCallback* callback;

    private:
        static float padding;
        static float thumbnailSize;
        static vector<AssetBrowserItem> _items;

        static Ref<FileDialog> _fileDialog;

        static std::filesystem::path _assetsPath;
        static std::filesystem::path _currentDir;
        static std::filesystem::path _rightClickedDir;
        static std::filesystem::path _rightClickedAssetPath;
        static std::string _rightClickedAssetExtension;
        static std::string _rightClickedDirExtension;

        static Project _project;
        // renaming
        static bool renameMode;
        static std::string newAssetName;
        static std::string oldAssetName;
        // project versions
        static const char* projectVersions[];
        static int selectedProjectVersion;
        static bool autoSync;
    };
}