//
// Created by mecha on 06.10.2021.
//

#pragma once

#include <core/Time.h>
#include <core/FileDialog.h>
#include <core/File.h>

#include <graphics/buffers/TextureBuffer.h>

#define EDITOR_TEXTURES_PATH "editor/textures"
#define EDITOR_SHADERS_PATH "editor/shaders"

namespace fairy {

    class AssetBrowserCallback {

    public:
        virtual ~AssetBrowserCallback() = default;

    public:
        virtual void onImageOpen(const std::string &fileName) = 0;
        virtual void onObjOpen(const std::string &fileName) = 0;
        virtual void onGlslOpen(const std::string &filePath, const std::string &fileName) = 0;
        virtual void onAssetImported(const std::string &assetPath) = 0;
        virtual void onAssetExported(const std::string &assetPath) = 0;
        virtual void onAssetRemoved(const std::string &assetPath) = 0;
    };

    struct AssetBrowserProps {
        const char* name;
        std::filesystem::path assetPath;
        float padding = 16.0f;
        float thumbnailSize = 64.0f;
    };

    struct AssetBrowserItem {
        const char* extension;
        const uint32_t iconId;
    };

    template<size_t itemsCount>
    struct AssetBrowserItems {
        AssetBrowserItem dirItem;
        AssetBrowserItem items[itemsCount];
    };

    class AssetBrowser {

    public:
        static constexpr size_t itemsCount = 5;

    public:
        AssetBrowser(const AssetBrowserProps &props,
                     const AssetBrowserItems<itemsCount> &items,
                     const engine::Ref<engine::FileDialog>& fileDialog) :
                _props(props),
                _items(items),
                _currentDir(props.assetPath),
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

        inline const AssetBrowserProps& getProps() {
            return _props;
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

    private:
        AssetBrowserProps _props;
        AssetBrowserItems<itemsCount> _items;

        std::filesystem::path _currentDir;

        engine::Ref<engine::FileDialog> _fileDialog;

        AssetBrowserCallback* _callback = nullptr;

        std::string _rightClickedAssetPath;
        std::filesystem::path _rightClickedDir;
    };

}