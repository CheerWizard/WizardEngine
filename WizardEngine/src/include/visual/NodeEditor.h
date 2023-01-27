#pragma once

#include <core/core.h>

#include <vector>
#include <functional>

namespace engine::visual {

    class ENGINE_API Socket final {

    public:
        Socket(int id) : m_Id(id) {}
        ~Socket() = default;

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }

    private:
        int m_Id = 0;
    };

    class ENGINE_API Link final {

    public:
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

    private:
        int m_Id = 0;
        int m_Begin = 0;
        int m_End = 0;
    };

    class ENGINE_API Node final {

    public:
        Node(int id) : m_Id(id) {}
        Node(int id, const char* name) : m_Id(id), m_Name(name) {}
        ~Node();

    public:
        [[nodiscard]] inline int getId() const { return m_Id; }
        [[nodiscard]] inline const char* getName() const { return m_Name.c_str(); }

        void addInput(Socket&& socket);
        void addInput(const Socket& socket);
        void addOutput(Socket&& socket);
        void addOutput(const Socket& socket);

        void inputs(const std::function<void(Socket&)>& handle);
        void outputs(const std::function<void(Socket&)>& handle);

    private:
        int m_Id = 0;
        std::string m_Name = "Undefined";
        std::vector<Socket> m_Inputs;
        std::vector<Socket> m_Outputs;
    };

    class ENGINE_API NodeEditor final {

    private:
        NodeEditor() = default;
        ~NodeEditor() = default;

    public:
        static NodeEditor& get();

        void draw();

        void addNode(Node&& node);
        void addNode(const Node& node);
        void addLink(Link&& link);
        void addLink(const Link& link);

        void onNodeHovered(int id);

    private:
        std::vector<Node> m_Nodes;
        std::vector<Link> m_Links;
    };

}