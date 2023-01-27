//
// Created by mecha on 06.10.2021.
//

#include <visual/AssetBrowser.h>

#include <core/FileExtensions.h>
#include <event/MouseCodes.h>
#include <graphics/core/sources/ShaderSource.h>
#include <tools/terminal.h>
#include <visual/Widgets.h>
#include <visual/FontAwesome4.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

namespace engine::visual {

    using namespace engine::event;
    using namespace engine::graphics;

    float AssetBrowser::padding = 16.0f;
    float AssetBrowser::thumbnailSize = 64.0f;

    AssetBrowserCallback* AssetBrowser::callback = nullptr;

    vector<AssetBrowserItem> AssetBrowser::_items;

    Ref<FileDialog> AssetBrowser::_fileDialog;

    std::filesystem::path AssetBrowser::_assetsPath = "";
    std::filesystem::path AssetBrowser::_currentDir = "";
    std::filesystem::path AssetBrowser::_rightClickedDir;
    std::filesystem::path AssetBrowser::_rightClickedAssetPath;
    std::string AssetBrowser::_rightClickedAssetExtension;
    std::string AssetBrowser::_rightClickedDirExtension;

    Project AssetBrowser::_project;

    bool AssetBrowser::renameMode = false;
    std::string AssetBrowser::oldAssetName = "";
    std::string AssetBrowser::newAssetName = "";

    const char* AssetBrowser::projectVersions[] {
        "Debug", "Release", "Visual", "Profile"
    };
    int AssetBrowser::selectedProjectVersion = 0;
    bool AssetBrowser::autoSync = true;

    void AssetBrowser::create(void* nativeWindow) {
        _fileDialog = createRef<FileDialog>(nativeWindow);

        io::TextureData dirTD = io::TextureFile::read("assets/textures/dir_icon.png");
        auto dirItem = AssetBrowserItem {
                "",
                TextureBuffer::upload(dirTD)
        };
        io::TextureFile::free(dirTD);

        io::TextureData pngTD = io::TextureFile::read("assets/textures/png_icon.png");
        auto pngItem = AssetBrowserItem {
                PNG_EXT,
                TextureBuffer::upload(pngTD)
        };
        io::TextureFile::free(pngTD);

        io::TextureData jpgTD = io::TextureFile::read("assets/textures/jpg_icon.png");
        auto jpgItem = AssetBrowserItem {
                JPG_EXT,
                TextureBuffer::upload(jpgTD)
        };
        io::TextureFile::free(jpgTD);

        io::TextureData glslTD = io::TextureFile::read("assets/textures/glsl_icon.png");
        auto glslItem = AssetBrowserItem {
                GLSL_EXT,
                TextureBuffer::upload(glslTD)
        };
        io::TextureFile::free(glslTD);

        io::TextureData objTD = io::TextureFile::read("assets/textures/obj_icon.png");
        auto objItem = AssetBrowserItem {
                OBJ_EXT,
                TextureBuffer::upload(objTD)
        };
        io::TextureFile::free(objTD);

        io::TextureData ttfTD = io::TextureFile::read("assets/textures/ttf_icon.png");
        auto ttfItem = AssetBrowserItem {
                TTF_EXT,
                TextureBuffer::upload(ttfTD)
        };
        io::TextureFile::free(ttfTD);

        io::TextureData cppTD = io::TextureFile::read("assets/textures/cpp_icon.png");
        auto cppItem = AssetBrowserItem {
                CPP_EXT,
                TextureBuffer::upload(cppTD)
        };
        io::TextureFile::free(cppTD);

        _items = vector<AssetBrowserItem> {
                dirItem,
                pngItem, jpgItem,
                glslItem,
                objItem,
                ttfItem,
                cppItem
        };
    }

    void AssetBrowser::setProject(const Project& project) {
        _project = project;
        _assetsPath = project.getAssetsPath();
        _currentDir = _assetsPath;
    }

    void AssetBrowser::destroy() {
        callback = nullptr;
        for (const AssetBrowserItem& item : _items) {
            TextureBuffer::destroy(item.iconId);
        }
    }

    void AssetBrowser::draw(Time dt) {
        static bool open = true;
        if (!ImGui::Begin(ICON_FA_FOLDER" Assets", &open)) {
            ImGui::End();
            return;
        }

        if (_currentDir != _assetsPath) {
            // Back button in toolbar
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
            if (ImGui::Button(ICON_FA_LONG_ARROW_LEFT)) {
                _currentDir = _currentDir.parent_path();
            }
            ImGui::PopStyleVar();
            if (_currentDir.filename() == "scripts") {
                ImGui::SameLine();

                // file manager toolbar

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                if (ImGui::ButtonEx(ICON_FA_CODE" New Script", { 104, 24 })) {
                    std::string newAsset = "Untitled";
                    std::string newAssetPath = _currentDir.string() + "/" + newAsset + CPP_EXT;
                    engine::filesystem::newFile(_currentDir, newAsset + CPP_EXT);
                    ProjectManager::newScript(newAssetPath, newAsset);
                    // activate renaming mode
                    renameMode = true;
                    oldAssetName = engine::filesystem::getFileName(newAssetPath);
                    newAssetName = oldAssetName;
                }
                ImGui::PopStyleVar();
                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                if (ImGui::ButtonEx(ICON_FA_FOLDER" New Folder", { 104, 24 })) {
                    engine::filesystem::newFile(_currentDir, "Untitled");
                }
                ImGui::PopStyleVar();
                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                if (ImGui::ButtonEx(ICON_FA_DOWNLOAD" Import", { 72, 24 })) {
                    importAsset(_currentDir);
                }
                ImGui::PopStyleVar();
                ImGui::Separator();

                // script manager toolbar

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                ImGui::PushItemWidth(128);
                ImGui::Combo("##project_versions", &selectedProjectVersion, projectVersions,
                             IM_ARRAYSIZE(projectVersions));
                ImGui::PopItemWidth();
                ImGui::PopStyleVar();
                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                Checkbox::draw("Auto-Sync", autoSync);
                ImGui::PopStyleVar();
                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                if (ImGui::Button(ICON_FA_MAGIC" Sync", { 72, 24 })) {
                    ProjectManager::cmakeScripts(_project, getSelectedProjectVersion());
                }
                ImGui::PopStyleVar();
                ImGui::SameLine();

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
                if (ImGui::Button(ICON_FA_OBJECT_GROUP" Build", { 72, 24 })) {
                    ProjectManager::buildScripts(_project, getSelectedProjectVersion());
                }
                ImGui::PopStyleVar();

                ImGui::Separator();
            }
        }

        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) {
            columnCount = 1;
        }

        ImGui::Columns(columnCount, nullptr, false);

        ENGINE_INFO("AssetBrowser::draw: currentDir={0}", _currentDir);
        if (engine::filesystem::exists(_currentDir)) {
            for (auto& directoryEntry : std::filesystem::directory_iterator(_currentDir)) {
                const auto& path = directoryEntry.path();
                auto relativePath = std::filesystem::relative(path, _assetsPath);
                const auto& fileName = relativePath.filename().string();
                const auto& fileExtension = relativePath.extension().string();
                EDITOR_INFO("Asset file name : {0}, relative path : {1}", fileName, path);

                bool isDirectory = directoryEntry.is_directory();
                uint32_t iconId = _items[0].iconId; // directory icon

                if (!isDirectory) {
                    for (const auto& item : _items) {
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
                        { thumbnailSize, thumbnailSize },
                        { 0, 0 },
                        { 1, 1 },
                        -static_cast<int>(padding)
                );

                if (!isDirectory && iconClicked) {
                    const char* fileExtensionStr = fileExtension.c_str();
                    SWITCH(fileExtensionStr) {
                        CASE(JPG_EXT):
                        if (callback) {
                            callback->onImageOpen(fileName);
                        }
                        break;
                        CASE(PNG_EXT):
                        if (callback) {
                            callback->onImageOpen(fileName);
                        }
                        break;
                        CASE(OBJ_EXT):
                        if (callback) {
                            callback->onObjOpen(fileName);
                        }
                        break;
                        CASE(CPP_EXT):
                            ProjectManager::openScriptsSln(_project);
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

                if (isDirectory && ImGui::IsItemClicked(ButtonLeft)) {
                    _currentDir /= path.filename();
                }

                if (ImGui::IsItemClicked(ButtonRight)) {
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

                if (renameMode) {
                    ImGui::InputText("##asset_rename", &newAssetName);
                    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                        if (_rightClickedAssetExtension == CPP_EXT) {
                            ProjectManager::renameScript(_rightClickedAssetPath.string(), newAssetName);
                            if (autoSync) {
                                ProjectManager::cmakeScripts(_project, getSelectedProjectVersion());
                            }
                        } else {
                            engine::filesystem::rename(_rightClickedAssetPath, newAssetName + _rightClickedAssetExtension);
                        }
                        renameMode = false;
                        _rightClickedAssetPath = "";
                    }
                } else {
                    Text::centered(rawName.c_str(), thumbnailSize * 1.5, 1, false);
                }

                ImGui::NextColumn();
                ImGui::PopID();
            }
        }

        if (!_rightClickedAssetPath.empty()) {
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
                         "JPG image (*.jpg)\0*.jpg\0"
                         "TGA image (*.tga)\0*.tga\0";
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

        auto isImported = engine::filesystem::copy(importPath, asset_dir_path.string());
        if (isImported) {
            if (callback) {
                callback->onAssetImported(assetDirPath);
            }
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

        auto isExported = engine::filesystem::copy(assetPath, exportPath);
        if (isExported) {
            if (callback) {
                callback->onAssetExported(exportPath);
            }
        }
    }

    void AssetBrowser::removeAsset(const std::string &assetPath) {
        if (SAME(_rightClickedAssetExtension.c_str(), CPP_EXT)) {
            if (autoSync) {
                ProjectManager::cmakeScripts(_project, getSelectedProjectVersion());
            }
        }
        auto isRemoved = engine::filesystem::remove(assetPath);
        if (isRemoved) {
            if (callback) {
                callback->onAssetRemoved(assetPath);
            }
        }
    }

    void AssetBrowser::popupAssetMenu(const char* id) {
        if (ImGui::BeginPopupContextWindow(id)) {
            if (ImGui::MenuItem("Rename")) {
                renameMode = true;
                oldAssetName = engine::filesystem::getFileName(_rightClickedAssetPath);
                newAssetName = oldAssetName;
            }

            auto* assetExtension = _rightClickedAssetExtension.c_str();

            if (IS_TEXTURE(assetExtension)) {
                if (ImGui::MenuItem("Open in Photoshop")) {
                    engine::terminal::openPhotoshopTask(_rightClickedAssetPath.string());
                }
            }

            if (IS_MODEL(assetExtension)) {
                if (ImGui::MenuItem("Open in Blender")) {
                    engine::terminal::openBlenderTask(_rightClickedAssetPath.string());
                }

                if (ImGui::MenuItem("Open in ZBrush")) {
                    engine::terminal::openZBrushTask(_rightClickedAssetPath.string());
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

    ProjectVersion AssetBrowser::getSelectedProjectVersion() {
        SWITCH(projectVersions[selectedProjectVersion]) {
            CASE("Debug"): return ProjectVersion::V_DEBUG;
            CASE("Release"): return ProjectVersion::V_RELEASE;
            CASE("Visual"): return ProjectVersion::V_VISUAL;
            CASE("Profile"): return ProjectVersion::V_PROFILING;
        }
    }

}