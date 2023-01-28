#pragma once

#include <core/core.h>

#include <vector>
#include <functional>

namespace engine::visual {

    class ENGINE_API Socket final {

    public:
        Socket() = default;
        Socket(int id) : m_Id(id) {}
        ~Socket() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }

        bool loadBin(std::fstream& file);
        bool saveBin(std::fstream& file);

    private:
        int m_Id = 0;
    };

    class ENGINE_API Link final {

    public:
        Link() = default;
        Link(int id) : m_Id(id) {}
        Link(int id, int beginPinId, int endPinId)
        : m_Id(id), m_Begin(beginPinId), m_End(endPinId) {}
        ~Link() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline int getBegin() const { return m_Begin; }
        [[nodiscard]] inline int getEnd() const { return m_End; }

        inline void setBegin(int id) { m_Begin = id; }
        inline void setEnd(int id) { m_End = id; }

        bool loadBin(std::fstream& file);
        bool saveBin(std::fstream& file);

    private:
        int m_Id = 0;
        int m_Begin = 0;
        int m_End = 0;
    };

    class ENGINE_API Node final {

    public:
        Node() = default;
        Node(int id) : m_Id(id) {}
        Node(int id, const char* name) : m_Id(id), m_Name(name) {}
        ~Node() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline const char* getName() const { return m_Name.c_str(); }

        void addInput(Socket&& socket);
        void addInput(const Socket& socket);
        void addOutput(Socket&& socket);
        void addOutput(const Socket& socket);

        void inputs(const std::function<void(Socket&)>& handle);
        void outputs(const std::function<void(Socket&)>& handle);

        bool loadBin(std::fstream& file);
        bool saveBin(std::fstream& file);

    private:
        int m_Id = 0;
        std::string m_Name = "Undefined";
        std::vector<Socket> m_Inputs;
        std::vector<Socket> m_Outputs;
    };

    class ENGINE_API Graph final {

    public:
        explicit Graph(int id) : m_Id(id) {}
        Graph(int id, const char* name) : m_Id(id), m_Name(name) {}

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline const char* getName() const { return m_Name.c_str(); }

        void addNode(Node&& node);
        void addNode(const Node& node);
        void addLink(Link&& link);
        void addLink(const Link& link);

        void removeNode(int id);
        void removeLink(int id);

        void draw();

        bool saveBin();
        bool loadBin();

        bool saveIni();
        bool loadIni();

        void onNodeHovered(int id);

    private:
        int m_Id = 0;
        std::string m_Name;
        std::vector<Node> m_Nodes;
        std::vector<Link> m_Links;
        std::vector<int> m_SelectedNodes;
        std::vector<int> m_SelectedLinks;
        std::vector<Node> m_CopiedNodes;
        std::vector<Link> m_CopiedLinks;
        ImVec2 m_PreviousMousePos;
    };

    class ENGINE_API NodeEditor final {

    private:
        NodeEditor() = default;
        ~NodeEditor() = default;

    public:
        static NodeEditor& get();

        static void create();
        static void destroy();

        static void setTheme();

        inline void setActiveGraph(Graph* graph) { m_ActiveGraph = graph; }
        inline void setActiveGraph(int index) { m_ActiveGraph = &m_Graphs.at(index); }
        [[nodiscard]] inline Graph* getActiveGraph() { return m_ActiveGraph; }

        void draw();

        bool save();
        bool load(const char* graphName);

        int addGraph(Graph&& graph);
        int addGraph(const Graph& graph);

        void onNodeHovered(int id);

    private:
        Graph* m_ActiveGraph = nullptr;
        std::vector<Graph> m_Graphs;
    };

}