//
// Created by mecha on 06.10.2021.
//

#pragma once

#include <time/Time.h>
#include <core/filesystem.h>
#include <tools/Tools.h>

#include <platform/tools/FileDialog.h>
#include <platform/graphics/TextureBuffer.h>

#define EDITOR_TEXTURES_PATH "editor/textures"
#define EDITOR_SHADERS_PATH "editor/shaders"
#define DRAG_DROP_ITEM_TYPE "ASSET_BROWSER_ITEM"

namespace studio {

    class AssetBrowserCallback {

    public:
        virtual ~AssetBrowserCallback() = default;

    public:
        virtual void onImageOpen(const std::string &fileName) = 0;
        virtual void onObjOpen(const std::string &fileName) = 0;
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
        AssetBrowser(const AssetBrowserProps &props,
                     const AssetBrowserItems<6> &items,
                     const engine::Ref<engine::FileDialog>& fileDialog) :
                _props(props),
                _items(items),
                _currentDir(props.assetPath),
                _fileDialog(fileDialog) {
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
        void destroy();
        void importAsset(const std::filesystem::path &assetDir);
        void exportAsset(const std::string &assetPath);
        void removeAsset(const std::string &assetPath);
        void popupAssetMenu(const char* id);

    private:
        AssetBrowserProps _props;
        AssetBrowserItems<6> _items;

        engine::Ref<engine::FileDialog> _fileDialog;

        AssetBrowserCallback* _callback = nullptr;

        std::filesystem::path _currentDir;
        std::filesystem::path _rightClickedDir;
        std::filesystem::path _rightClickedAssetPath;
        std::string _rightClickedAssetExtension;
        std::string _rightClickedDirExtension;
    };

}