//
// Created by mecha on 06.10.2021.
//

#pragma once

#include <core/Time.h>
#include <core/FileDialog.h>
#include <core/File.h>

#include <graphics/buffers/TextureBuffer.h>

#define EDITOR_TEXTURES_PATH "editorRes/textures"
#define EDITOR_SHADERS_PATH "editorRes/shaders"

namespace fairy {

    class AssetBrowserCallback {

    public:
        virtual ~AssetBrowserCallback() = default;

    public:
        virtual void onPngOpen(const std::string &fileName) = 0;
        virtual void onJpgOpen(const std::string &fileName) = 0;
        virtual void onObjOpen(const std::string &fileName) = 0;
        virtual void onGlslOpen(const std::string &filePath, const std::string &fileName) = 0;
        virtual void onAssetImported(const std::string &assetPath) = 0;
        virtual void onAssetExported(const std::string &assetPath) = 0;
        virtual void onAssetRemoved(const std::string &assetPath) = 0;
    };

    struct AssetBrowserProps {
        const char* name;
        const std::filesystem::path& assetPath;
    };

    class AssetBrowser {

    public:
        AssetBrowser(const AssetBrowserProps &props,
                     const engine::Ref<engine::TextureBuffer>& dirIcon,
                     const engine::Ref<engine::TextureBuffer>& fileIcon,
                     const engine::Ref<engine::FileDialog>& fileDialog) :
                     _props(props),
                     _currentDir(props.assetPath),
                     _dirIcon(dirIcon),
                     _fileIcon(fileIcon),
                     _fileDialog(fileDialog) {
            create();
        }

        ~AssetBrowser() {
            destroy();
        }

    public:
        inline void setCallback(AssetBrowserCallback* assetBrowserCallback) {
            _callback = assetBrowserCallback;
        }

        inline void removeCallback() {
            _callback = nullptr;
        }

    public:
        void onUpdate(engine::Time dt);

    private:
        void create();
        void destroy();
        void importAsset(const std::filesystem::path &assetDir);
        void exportAsset(const std::string &assetPath);
        void removeAsset(const std::string &assetPath);
        void popupAssetMenu(const char* id);

    public:
        float padding = 16.0f;
        float thumbnailSize = 64.0f;

    private:
        AssetBrowserProps _props;
        std::filesystem::path _currentDir;
        engine::Ref<engine::TextureBuffer> _dirIcon;
        engine::Ref<engine::TextureBuffer> _fileIcon;
        engine::Ref<engine::FileDialog> _fileDialog;

        std::string _rightClickedAssetPath;
        std::filesystem::path _rightClickedDir;

        AssetBrowserCallback* _callback = nullptr;
    };

}