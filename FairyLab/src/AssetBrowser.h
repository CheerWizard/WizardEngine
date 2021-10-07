//
// Created by mecha on 06.10.2021.
//

#pragma once

#include <core/Layout.h>
#include <graphics/buffers/TextureBuffer.h>

#include <filesystem>

#define EDITOR_RES_PATH "editorRes"

namespace fairy {

    class AssetBrowserCallback {
    public:
        virtual void onFileOpen(const std::string &fileName) = 0;
    };

    struct AssetBrowserProps {
        const char* name;
        const std::filesystem::path& assetPath;
    };

    class AssetBrowser : public engine::Layout {

    public:
        AssetBrowser(const AssetBrowserProps &props,
                     const engine::Ref<engine::TextureBuffer>& dirIcon,
                     const engine::Ref<engine::TextureBuffer>& fileIcon
                     ) :
                     _props(props),
                     _currentDir(props.assetPath),
                     _dirIcon(dirIcon),
                     _fileIcon(fileIcon) {
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
        void onUpdate(engine::Time dt) override;

    private:
        void create();
        void destroy();

    public:
        float padding = 16.0f;
        float thumbnailSize = 64.0f;

    private:
        AssetBrowserProps _props;
        std::filesystem::path _currentDir;
        engine::Ref<engine::TextureBuffer> _dirIcon;
        engine::Ref<engine::TextureBuffer> _fileIcon;

        AssetBrowserCallback* _callback = nullptr;
    };

}