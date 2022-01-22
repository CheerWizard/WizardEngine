//
// Created by mecha on 06.10.2021.
//

#include "AssetBrowser.h"
#include <core/FileExtensions.h>
#include <core/MouseCodes.h>
#include <core/Build.h>

#include "imgui/imgui/imgui.h"

namespace fairy {

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
            EDITOR_INFO("Asset file name : {0}, relative path : {1}", fileName, path);

            bool isDirectory = directoryEntry.is_directory();
            uint32_t iconId = _items.dirItem.iconId;

            if (!isDirectory) {
                for (const auto& item : _items.items) {
                    const auto* fileExtensionStr = fileExtension.c_str();
                    if (strcmp(fileExtensionStr, item.extension) == 0) {
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
                const char* fileExtensionStr = fileExtension.c_str();
                SWITCH(fileExtensionStr) {
                    CASE(JPG_EXT):
                        _callback->onImageOpen(fileName);
                        break;
                    CASE(PNG_EXT):
                        _callback->onImageOpen(fileName);
                        break;
                    CASE(OBJ_EXT):
                        _callback->onObjOpen(fileName);
                        break;
                }
            }

            // send drag-drop item path
            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload(DRAG_DROP_ITEM_TYPE, itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                EDITOR_INFO("AssetBrowser: Packaging drag-drop data item path : {0}", STR_FROM_WCHAR(itemPath));
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if (isDirectory && ImGui::IsItemClicked(engine::ButtonLeft)) {
                _currentDir /= path.filename();
            }

            if (ImGui::IsItemClicked(engine::ButtonRight)) {
                if (isDirectory) {
                    _rightClickedDir = path;
                    _rightClickedDirExtension = fileExtension;
                    _rightClickedAssetPath = "";
                    _rightClickedAssetExtension = "";
                } else {
                    _rightClickedAssetPath = path;
                    _rightClickedAssetExtension = fileExtension;
                }
            }

            std::string rawName = fileName.substr(0, fileName.find_last_of('.'));
            ImGui::TextWrapped("%s", rawName.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }

        if (_rightClickedAssetPath.empty()) {
            if (ImGui::BeginPopupContextWindow(nullptr)) {
                if (ImGui::MenuItem("New File")) {
                    engine::FileSystem::newFile(_rightClickedDir, "templatefile" + _rightClickedDirExtension);
                }
                if (ImGui::MenuItem("New Directory")) {
                    engine::FileSystem::newFile(_rightClickedDir, "templatedir");
                }
                if (ImGui::MenuItem("Import")) {
                    importAsset(_rightClickedDir);
                }
                ImGui::EndPopup();
            }
        } else {
            popupAssetMenu(nullptr);
        }

        ImGui::Columns(1);
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
            CASE("scripts"):
                filter = "C++ script (*.cpp)\0*.cpp\0";
                break;
        }

        auto importPath = _fileDialog->getImportPath(filter);
        if (importPath.empty()) {
            EDITOR_INFO("Importing asset has been dismissed!");
            return;
        }

        std::filesystem::path import_path(importPath);
        std::filesystem::path asset_dir_path(assetDirPath);
        auto importFileName = import_path.filename().string();
        asset_dir_path /= importFileName;
        EDITOR_INFO("Import path : {0}, import file name : {1}", importPath, importFileName);

        auto isImported = engine::FileSystem::copy(importPath, asset_dir_path.string());
        if (isImported) {
            _callback->onAssetImported(assetDirPath);
        }
    }

    void AssetBrowser::exportAsset(const std::string &assetPath) {
        std::filesystem::path asset_path(assetPath);
        auto assetExtension = asset_path.extension().string();

        const char* filter;
        SWITCH(assetExtension.c_str()) {
            CASE(PNG_EXT):
                filter = "PNG image (*.png)\0*.png\0";
                break;
            CASE(JPG_EXT):
                filter = "JPG image (*.jpg)\0*.jpg\0";
                break;
            CASE(GLSL_EXT):
                filter = "GLSL shader (*.glsl)\0*.glsl\0";
                break;
            CASE(OBJ_EXT):
                filter = "OBJ model (*.obj)\0*.obj\0";
                break;
            CASE(CPP_EXT):
                filter = "C++ script (*.cpp)\0*.cpp\0";
                break;
        }

        auto exportPath = _fileDialog->getExportPath(filter);
        EDITOR_INFO("Export path : {0}", exportPath);

        if (exportPath.empty()) {
            EDITOR_INFO("Exporting asset has been dismissed!");
            return;
        }

        auto isExported = engine::FileSystem::copy(assetPath, exportPath);
        if (isExported) {
            _callback->onAssetExported(exportPath);
        }
    }

    void AssetBrowser::removeAsset(const std::string &assetPath) {
        auto isRemoved = engine::FileSystem::remove(assetPath);
        if (isRemoved) {
            _callback->onAssetRemoved(assetPath);
        }
    }

    void AssetBrowser::popupAssetMenu(const char* id) {
        if (ImGui::BeginPopupContextWindow(id)) {
            if (ImGui::MenuItem("Rename")) {
                engine::FileSystem::rename(_rightClickedAssetPath, "unknown" + _rightClickedAssetExtension);
                _rightClickedAssetPath = "";
            }

            auto* assetExtension = _rightClickedAssetExtension.c_str();
            if (SAME(assetExtension, CPP_EXT)) {
                auto assetName = engine::FileSystem::getFileName(_rightClickedAssetPath.string());
                auto itemName = "Build " + assetName;
                auto libName = "ScriptDLL";
                if (ImGui::MenuItem(itemName.c_str())) {
//                    engine::Libs::compile(_rightClickedAssetPath.string());
//                    engine::Libs::compile("../WizardEngine/src/scripting/Scriptable.cpp");
//                    engine::Libs::generate(libName);
                    engine::Libs::add(libName, "../ScriptDLL/ScriptDLL.dll");
                }
                if (ImGui::MenuItem("Load")) {
//                    auto* parent = engine::createObject<Scriptable>(libName, "create");
//                    if (parent) {
//                        parent->onCreate();
//                    }
                }
            }

            if (IS_SCRIPT(assetExtension)) {
                if (ImGui::MenuItem("Open in Visual Studio")) {
                    engine::Tools::openVisualStudio(_rightClickedAssetPath.string());
                }

                if (ImGui::MenuItem("Open in VS Code")) {
                    engine::Tools::openVSCode(_rightClickedAssetPath.string());
                }

                if (ImGui::MenuItem("Open in Notepad")) {
                    engine::Tools::openNotepad(_rightClickedAssetPath.string());
                }
            }

            if (IS_TEXTURE(assetExtension)) {
                if (ImGui::MenuItem("Open in Photoshop")) {
                    engine::Tools::openPhotoshop(_rightClickedAssetPath.string());
                }
            }

            if (IS_MODEL(assetExtension)) {
                if (ImGui::MenuItem("Open in Blender")) {
                    engine::Tools::openBlender(_rightClickedAssetPath.string());
                }

                if (ImGui::MenuItem("Open in ZBrush")) {
                    engine::Tools::openZBrush(_rightClickedAssetPath.string());
                }
            }

            if (ImGui::MenuItem("Export")) {
                exportAsset(_rightClickedAssetPath.string());
                _rightClickedAssetPath = "";
            }

            if (ImGui::MenuItem("Remove")) {
                removeAsset(_rightClickedAssetPath.string());
                _rightClickedAssetPath = "";
            }

            ImGui::EndPopup();
        }
    }

}