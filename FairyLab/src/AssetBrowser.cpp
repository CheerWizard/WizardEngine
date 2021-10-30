//
// Created by mecha on 06.10.2021.
//

#include "AssetBrowser.h"
#include "core/FileExtensions.h"

#include <core/MouseCodes.h>

#include "imgui/imgui/imgui.h"

namespace fairy {

    void AssetBrowser::create() {
    }

    void AssetBrowser::destroy() {
        removeCallback();
    }

    void AssetBrowser::onUpdate(engine::Time dt) {
        ImGui::Begin(_props.name);

        if (_currentDir != _props.assetPath) {
            if (ImGui::Button("<-")) {
                _currentDir = _currentDir.parent_path();
            }
        }

        float cellSize = _props.thumbnailSize + _props.padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) {
            columnCount = 1;
        }

        ImGui::Columns(columnCount, nullptr, false);

        for (auto& directoryEntry : std::filesystem::directory_iterator(_currentDir)) {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, _props.assetPath);
            const auto& fileName = relativePath.filename().string();
            const auto& fileExtension = relativePath.extension().string();
            ENGINE_INFO("Asset file name : {0}, relative path : {1}", fileName, path);

            bool isDirectory = directoryEntry.is_directory();
            uint32_t iconId = _items.dirItem.iconId;

            if (!isDirectory) {
                for (const auto& item : _items.items) {
                    const auto* fileExtensionStr = fileExtension.c_str();
                    ENGINE_INFO("Current file extension : {0}", fileExtensionStr);
                    ENGINE_INFO("Current item extension : {0}", item.extension);
                    if (strcmp(fileExtensionStr, item.extension) == 0) {
                        ENGINE_INFO("Found icon id {0} for current {1} file!", item.iconId, item.extension);
                        iconId = item.iconId;
                        break;
                    }
                }
            }

            auto itemId = fileName.c_str();
            ImGui::PushID(itemId);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            auto iconClicked = ImGui::ImageButton(
                    (ImTextureID) iconId,
                    { _props.thumbnailSize, _props.thumbnailSize },
                    { 0, 0 },
                    { 1, 1 }
            );

            if (!isDirectory && iconClicked) {
                SWITCH(fileExtension.c_str()) {
                    CASE(engine::file_extensions::JPG):
                    _callback->onImageOpen(fileName);
                    break;

                    CASE(engine::file_extensions::PNG):
                    _callback->onImageOpen(fileName);
                    break;

                    CASE(engine::file_extensions::OBJ):
                    _callback->onObjOpen(fileName);
                    break;

                    CASE(engine::file_extensions::GLSL):
                    _callback->onGlslOpen(path.string(), fileName);
                    break;
                }
            }

            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if (isDirectory && ImGui::IsItemClicked(engine::ButtonLeft)) {
                _currentDir /= path.filename();
            }

            if (ImGui::IsItemClicked(engine::ButtonRight)) {
                if (isDirectory) {
                    _rightClickedAssetPath = "";
                    _rightClickedDir = path;
                } else {
                    _rightClickedAssetPath = path.string();
                }
            }

            ImGui::TextWrapped("%s", fileName.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }

        if (_rightClickedAssetPath.empty()) {
            if (ImGui::BeginPopupContextWindow(nullptr)) {
                if (ImGui::MenuItem("Add Asset")) {
                    importAsset(_rightClickedDir);
                }
                ImGui::EndPopup();
            }
        } else {
            popupAssetMenu(nullptr);
        }

        ImGui::Columns(1);

        ImGui::SliderFloat("Thumbnail Size", &_props.thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &_props.padding, 0, 32);

        ImGui::End();
    }

    void AssetBrowser::importAsset(const std::filesystem::path &assetDir) {
        auto assetDirName = assetDir.filename().string();
        auto assetDirPath = assetDir.string();

        const char* filter;
        SWITCH(assetDirName.c_str()) {
            CASE("textures"):
                filter = "PNG image (*.png)\0*.png\0"
                         "JPG image (*.jpg)\0*.jpg\0";
                break;
            CASE("shaders"):
                filter = "GLSL shader (*.glsl)\0*.glsl\0";
                break;
            CASE("obj"):
                filter = "OBJ model (*.obj)\0*.obj\0";
                break;
        }

        auto importPath = _fileDialog->getImportPath(filter);
        if (importPath.empty()) {
            ENGINE_INFO("Importing asset has been dismissed!");
            return;
        }

        std::filesystem::path import_path(importPath);
        std::filesystem::path asset_dir_path(assetDirPath);
        auto importFileName = import_path.filename().string();
        asset_dir_path /= importFileName;
        ENGINE_INFO("Import path : {0}, import file name : {1}", importPath, importFileName);

        auto isImported = engine::File::copy(importPath, asset_dir_path.string());
        if (isImported) {
            _callback->onAssetImported(assetDirPath);
        }
    }

    void AssetBrowser::exportAsset(const std::string &assetPath) {
        std::filesystem::path asset_path(assetPath);
        auto assetExtension = asset_path.extension().string();

        const char* filter;
        SWITCH(assetExtension.c_str()) {
            CASE(".png"):
                filter = "PNG image (*.png)\0*.png\0";
                break;
            CASE(".jpg"):
                filter = "JPG image (*.jpg)\0*.jpg\0";
                break;
            CASE(".glsl"):
                filter = "GLSL shader (*.glsl)\0*.glsl\0";
                break;
            CASE(".obj"):
                filter = "OBJ model (*.obj)\0*.obj\0";
                break;
        }

        auto exportPath = _fileDialog->getExportPath(filter);
        ENGINE_INFO("Export path : {0}", exportPath);

        if (exportPath.empty()) {
            ENGINE_INFO("Exporting asset has been dismissed!");
            return;
        }

        auto isExported = engine::File::copy(assetPath, exportPath);
        if (isExported) {
            _callback->onAssetExported(exportPath);
        }
    }

    void AssetBrowser::removeAsset(const std::string &assetPath) {
        auto isRemoved = engine::File::remove(assetPath);
        if (isRemoved) {
            _callback->onAssetRemoved(assetPath);
        }
    }

    void AssetBrowser::popupAssetMenu(const char* id) {
        if (ImGui::BeginPopupContextWindow(id)) {
            if (ImGui::MenuItem("Export")) {
                exportAsset(_rightClickedAssetPath);
                _rightClickedAssetPath = "";
            }

            if (ImGui::MenuItem("Remove")) {
                removeAsset(_rightClickedAssetPath);
                _rightClickedAssetPath = "";
            }

            ImGui::EndPopup();
        }
    }

}