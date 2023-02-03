#pragma once

#include <core/core.h>
#include <core/Memory.h>
#include <platform/tools/FileDialog.h>

#include <ecs/Scene.h>

#include <imgui.h>
#include <TextEditor.h>

#include <vector>
#include <functional>
#include <set>

#define COLOR(r, g, b, a) IM_COL32(255 * r, 255 * g, 255 * b, 255 * a)

// Node styling

#define COL_NODE_OUTLINE COLOR(0, 0, 0, 0)
#define COL_NODE_SELECTED COLOR(1, 0.2, 0, 1)

// Link styling

#define COL_LINK_OUTLINE COLOR(0, 0, 0, 0)
#define COL_LINK_SELECTED COLOR(1, 0.2, 0, 1)

namespace engine::visual {

    using namespace core;
    using namespace tools;
    using namespace ecs;

    template<typename T>
    struct IdComparator final {
        bool operator() (const T& a, const T& b) const {
            return a.getId() < b.getId();
        }
    };

    class ENGINE_API Socket final {

    public:
        Socket() = default;
        Socket(int id) : m_Id(id) {}
        ~Socket() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        inline void setId(int id) { m_Id = id; }

        bool loadBin(std::fstream& file);
        bool saveBin(std::fstream& file);

    private:
        int m_Id = 0;
    };

    class Graph;

    class ENGINE_API Link final {

    public:
        Link() = default;

        Link(int id) : m_Id(id) {}

        Link(int id, int beginPinId, int endPinId)
        : m_Id(id), m_Begin(beginPinId), m_End(endPinId) {}

        Link(int id, int beginPinId, int endPinId, Graph* graph)
        : m_Id(id), m_Begin(beginPinId), m_End(endPinId), m_Graph(graph) {}

        ~Link() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline int getBegin() const { return m_Begin; }
        [[nodiscard]] inline int getEnd() const { return m_End; }

        inline void setId(int id) { m_Id = id; }
        inline void setBegin(int id) { m_Begin = id; }
        inline void setEnd(int id) { m_End = id; }
        inline void setGraph(Graph* graph) { m_Graph = graph; }

        bool loadBin(std::fstream& file);
        bool saveBin(std::fstream& file);

        void draw();

    private:
        int m_Id = 0;
        int m_Begin = 0;
        int m_End = 0;
        Graph* m_Graph = nullptr;
    };

    class ENGINE_API Node final {

    public:
        Node() = default;
        Node(int id) : m_Id(id) {}
        Node(int id, const char* name) : m_Id(id), m_Name(name) {}
        Node(int id, float x, float y) : m_Id(id), m_Pos(x, y) {}
        Node(int id, const ImVec2& pos) : m_Id(id), m_Pos(pos) {}
        ~Node() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline const char* getName() const { return m_Name.c_str(); }
        [[nodiscard]] inline const ImVec2& getPos() const { return m_Pos; }

        inline void setId(int id) { m_Id = id; }

        void addInput(Socket&& socket);
        void addInput(const Socket& socket);
        void addOutput(Socket&& socket);
        void addOutput(const Socket& socket);

        void inputs(const std::function<void(Socket&)>& handle);
        void outputs(const std::function<void(Socket&)>& handle);

        [[nodiscard]] int getLastInputId() const;
        [[nodiscard]] int getLastOutputId() const;

        bool loadBin(std::fstream& file);
        bool saveBin(std::fstream& file);

        void updatePos();
        void draw();

    private:
        int m_Id = 0;
        ImVec2 m_Pos = { 0, 0 };
        std::string m_Name = "Untitled";
        std::set<Socket, IdComparator<Socket>> m_Inputs;
        std::set<Socket, IdComparator<Socket>> m_Outputs;
    };

    class ENGINE_API Graph final {

    public:
        explicit Graph(int id) : m_Id(id) {}
        Graph(int id, const char* name) : m_Id(id), m_Name(name) {}

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline const char* getName() const { return m_Name.c_str(); }
        [[nodiscard]] inline u32 getRenderTargetId() const { return m_RenderTargetId; }
        [[nodiscard]] inline Ref<Scene>& getScene() { return m_Scene; };

        inline void showContextMenu(bool show) { m_ShowContextMenu = show; }

        void addNode(Node&& node);
        void addNode(const Node& node);
        void addLink(Link&& link);
        void addLink(const Link& link);

        void removeNode(int id);
        void removeLink(int id);

        void draw();

        bool saveBin(const char* filepath);
        bool loadBin(const char* filepath);

        bool saveIni();
        bool loadIni();

        void onNodeHovered(int id);

        void newNode();
        void newLink(int beginId, int endId);

        void deleteSelected();
        void copySelected();
        void cutSelected();
        void pasteSelected();

        void undo();
        void redo();

    private:
        void beginCutline();
        void drawCutline();
        void endCutline();

        void zoomCanvas();

    private:
        int m_Id = 0;
        std::string m_Name = "Untitled";

        ImVec2 m_CutlinePos = { 0, 0 };
        bool m_CutlineBegin = false;

        ImVec2 m_Cursor = { 0, 0 };

        float m_PastePadding = 1.0f;

        bool m_ShowContextMenu = false;
        bool m_LinkDropped = false;

        std::set<Node, IdComparator<Node>> m_Nodes;
        std::set<Link, IdComparator<Link>> m_Links;

        std::set<Node, IdComparator<Node>> m_TempNodes;
        std::set<Link, IdComparator<Link>> m_TempLinks;

        std::vector<int> m_SelectedNodes;
        std::vector<int> m_SelectedLinks;

        Ref<Scene> m_Scene;
        u32 m_RenderTargetId = 0;
    };

    class ENGINE_API NodeEditor final {

    private:
        NodeEditor() = default;
        ~NodeEditor() = default;

    public:
        static NodeEditor& get();

        static void create(void* nativeWindow);
        static void destroy();

        static void setTheme();

        inline void setActiveGraph(Graph* graph) { m_ActiveGraph = graph; }
        inline void setActiveGraph(int index) {
            m_ActiveGraph = &m_Graphs.at(index);
        }
        [[nodiscard]] inline Graph* getActiveGraph() { return m_ActiveGraph; }

        void draw();

        bool saveAll();

        int addGraph(Graph&& graph);
        int addGraph(const Graph& graph);

        // graph files
        void newGraph();
        void openGraph();
        void saveGraph();
        void saveAsGraph();
        // clipboard/edit
        void cutGraphItems();
        void copyGraphItems();
        void pasteGraphItems();
        void deleteGraphItems();
        void undoGraphItems();
        void redoGraphItems();

    private:
        void drawViewPort();
        void drawTextEditor();

    public:
        bool blockEvents = false;

    private:
        static Ref<FileDialog> s_FileDialog;
        Graph* m_ActiveGraph = nullptr;
        std::vector<Graph> m_Graphs;
        TextEditor m_TextEditor;
    };
}