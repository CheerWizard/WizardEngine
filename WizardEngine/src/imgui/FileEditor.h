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
    };

    class FileEditor {

    public:
        FileEditor(const FileEditorProps &props = FileEditorProps()) : props(props) {}

        ~FileEditor() {
            destroy();
        }

    public:
        void open(const std::string &filePath);
        void onUpdate(Time dt);
        void save();

    public:
        inline void show() {
            props.isVisible = true;
        }

        inline void hide() {
            props.isVisible = false;
        }

        inline void setPath(const std::string &path) {
            _path = path;
        }

        inline void setTextFieldFont(ImFont* textFieldFont) {
            this->textFieldFont = textFieldFont;
        }

    private:
        void destroy();
        void end();

    public:
        FileEditorProps props;

    private:
        std::string _path;
        std::string _source;

        ImFont* textFieldFont = nullptr;
    };

}