//
// Created by mecha on 06.10.2021.
//

#include "AssetBrowser.h"

#include "imgui/imgui/imgui.h"

namespace fairy {

    void AssetBrowser::create() {
        _fileIcon->setAssetPath(EDITOR_RES_PATH);
        _fileIcon->bind();
        _fileIcon->load("file_icon.png");

        _dirIcon->setAssetPath(EDITOR_RES_PATH);
        _dirIcon->bind();
        _dirIcon->load("dir_icon.png");
    }

    void AssetBrowser::destroy() {
        removeCallback();
    }

    void AssetBrowser::onUpdate(engine::Time dt) {
        ImGui::Begin(_props.name);

        if (_currentDir != std::filesystem::path(_props.assetPath)) {
            if (ImGui::Button("<-")) {
                _currentDir = _currentDir.parent_path();
            }
        }

        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) {
            columnCount = 1;
        }

        ImGui::Columns(columnCount, nullptr, false);

        for (auto& directoryEntry : std::filesystem::directory_iterator(_currentDir)) {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, _props.assetPath);
            const auto& fullFileName = relativePath.filename().string();
            ENGINE_INFO("Asset file name : {0}", fullFileName);
            bool isDirectory = directoryEntry.is_directory();

            ImGui::PushID(fullFileName.c_str());
            auto icon = isDirectory ? _dirIcon : _fileIcon;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            auto iconClicked = ImGui::ImageButton(
                    (ImTextureID)icon->getId(),
                    { thumbnailSize, thumbnailSize },
                    { 1, 0 },
                    { 0, 1 }
            );

            if (!isDirectory && iconClicked) {
                const auto& fullFileNameTokens = engine::split(fullFileName, ".");
                const auto& fileName = fullFileNameTokens[0];
                const auto& fileExtension = fullFileNameTokens[1];

                SWITCH(fileExtension.c_str()) {
                    CASE("jpg"):
                        _callback->onJpgOpen(fileName);
                        break;
                    CASE("png"):
                        _callback->onPngOpen(fileName);
                        break;
                    CASE("obj"):
                        _callback->onObjOpen(fileName);
                        break;
                    CASE("glsl"):
                        _callback->onGlslOpen(fileName);
                        break;
                }
            }

            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }

            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (directoryEntry.is_directory()) {
                    _currentDir /= path.filename();
                }
            }

            ImGui::TextWrapped("%s", fullFileName.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
            if (ImGui::MenuItem("Import Asset")) {
                _callback->onImport("Asset File (*.glsl, *.png, *.jpg, *.obj)");
            }
            if (ImGui::MenuItem("Export Asset")) {
                _callback->onExport("Asset File (*.glsl, *.png, *.jpg, *.obj)");
            }
            ImGui::EndPopup();
        }

        ImGui::Columns(1);

        ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);

        ImGui::End();
    }

}