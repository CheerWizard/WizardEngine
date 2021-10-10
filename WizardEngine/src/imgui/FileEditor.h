//
// Created by mecha on 09.10.2021.
//

#pragma once

#include "../core/File.h"
#include "../core/Time.h"

struct ImFont;

namespace engine {

    struct FileEditorProps {
        std::string title = "File Editor";
        bool isVisible = false;
        bool isClosable = true;
    };

    class FileEditor {

    public:
        FileEditor(const FileEditorProps &props = FileEditorProps()) : _props(props) {}

        ~FileEditor() {
            destroy();
        }

    public:
        void open(const std::string &filePath);
        void onUpdate(Time dt);

    public:
        inline void setTitle(const char* title) {
            _props.title = title;
        }

        inline void setTitle(const std::string &title) {
            _props.title = title;
        }

        inline void show() {
            _props.isVisible = true;
        }

        inline void hide() {
            _props.isVisible = false;
        }

        inline void setClosable(const bool &isClosable) {
            _props.isClosable = isClosable;
        }

        inline void setPath(const std::string &path) {
            _path = path;
        }

        inline void setTextFieldFont(ImFont* textFieldFont) {
            this->textFieldFont = textFieldFont;
        }

    private:
        void save();
        void destroy();

    private:
        FileEditorProps _props;
        std::string _path;
        std::string _source;

        ImFont* textFieldFont = nullptr;
    };

}