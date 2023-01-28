#include <visual/NodeEditor.h>

#include <visual/FontAwesome4.h>

#include <io/Logger.h>

#include <event/KeyCodes.h>
#include <event/MouseCodes.h>

#include <imnodes.h>
#include <fstream>

namespace engine::visual {

    void Node::addInput(Socket&& socket) {
        m_Inputs.insert(socket);
    }

    void Node::addInput(const Socket& socket) {
        m_Inputs.insert(socket);
    }

    void Node::addOutput(Socket &&socket) {
        m_Outputs.insert(socket);
    }

    void Node::addOutput(const Socket& socket) {
        m_Outputs.insert(socket);
    }

    void Node::inputs(const std::function<void(Socket &)> &handle) {
        for (auto socket : m_Inputs) {
            handle(socket);
        }
    }

    void Node::outputs(const std::function<void(Socket &)> &handle) {
        for (auto socket : m_Outputs) {
            handle(socket);
        }
    }

    int Node::getLastInputId() const {
        auto last = m_Inputs.rbegin();
        if  (last != m_Inputs.rend()) {
            return last->getId();
        }
        return 0;
    }

    int Node::getLastOutputId() const {
        auto last = m_Outputs.rbegin();
        if  (last != m_Outputs.rend()) {
            return last->getId();
        }
        return 0;
    }

    bool Node::loadBin(std::fstream& file) {
        // input sockets
        size_t num_inputs;
        file.read(reinterpret_cast<char*>(&num_inputs), static_cast<std::streamsize>(sizeof(size_t)));
        for (int i = 0 ; i < num_inputs ; i++) {
            Socket input;
            input.loadBin(file);
            m_Inputs.insert(input);
        }
        // output sockets
        size_t num_outputs;
        file.read(reinterpret_cast<char*>(&num_outputs), static_cast<std::streamsize>(sizeof(size_t)));
        for (int i = 0 ; i < num_outputs ; i++) {
            Socket output;
            output.loadBin(file);
            m_Outputs.insert(output);
        }
        // title
        file.read(reinterpret_cast<char*>(m_Name.data()),static_cast<std::streamsize>(sizeof(std::string)));
        // id
        file.read(reinterpret_cast<char*>(&m_Id),static_cast<std::streamsize>(sizeof(int)));
        return true;
    }

    bool Node::saveBin(std::fstream &file) {
        // input sockets
        size_t num_inputs;
        file.write(reinterpret_cast<char*>(&num_inputs), static_cast<std::streamsize>(sizeof(size_t)));
        for (auto input : m_Inputs) {
            input.saveBin(file);
        }
        // output sockets
        size_t num_outputs;
        file.write(reinterpret_cast<char*>(&num_outputs), static_cast<std::streamsize>(sizeof(size_t)));
        for (auto output : m_Outputs) {
            output.saveBin(file);
        }
        // title
        file.write(reinterpret_cast<char*>(m_Name.data()),static_cast<std::streamsize>(sizeof(std::string)));
        // id
        file.write(reinterpret_cast<char*>(&m_Id),static_cast<std::streamsize>(sizeof(int)));
        return true;
    }

    void nodeHoveredCallback(int id, void* userData) {
        ((Graph*) userData)->onNodeHovered(id);
    }

    void Graph::draw() {
        std::string title = " " + m_Name;
        title = ICON_FA_LANGUAGE + title;
        ImGui::Begin(title.data());

        ImGui::TextUnformatted("A -- add node");
        ImGui::TextUnformatted(
                "Close the executable and rerun it -- your nodes should be exactly "
                "where you left them!");

        ImGui::VSliderFloat("Paste Padding", { 22, 100 }, &m_PastePadding, 0, 1);

        ImNodes::BeginNodeEditor();
        // set the titlebar color for all nodes
        ImNodesStyle& style = ImNodes::GetStyle();
        style.Colors[ImNodesCol_TitleBar] = IM_COL32(232, 27, 86, 255);
        style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(241, 108, 146, 255);
        // track mouse pos
        m_Cursor = ImGui::GetMousePos();
        // new node
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered())
        {
            // create new node
            if (ImGui::IsMouseReleased(event::MouseCode::ButtonRight)) {
                newNode();
            }
            // delete selected nodes/links
            else if (ImGui::IsKeyReleased(event::KeyCode::Delete)) {
                deleteSelected();
            }
            // copy
            else if (ImGui::IsKeyDown(event::KeyCode::LeftControl) && ImGui::IsKeyPressed(event::KeyCode::C)) {
                copySelected();
            }
            // cut
            else if (ImGui::IsKeyDown(event::KeyCode::LeftControl) && ImGui::IsKeyPressed(event::KeyCode::X)) {
                cutSelected();
            }
            // paste
            else if (ImGui::IsKeyDown(event::KeyCode::LeftControl) && ImGui::IsKeyPressed(event::KeyCode::V)) {
                pasteSelected();
            }
            // begin cutline
            else if (ImGui::IsKeyPressed(event::KeyCode::X) && !m_CutlineBegin) {
                beginCutline();
            }
            // draw cutline
            else if (ImGui::IsKeyDown(event::KeyCode::X)) {
                drawCutline();
            }
            // end cutline
            else if (ImGui::IsKeyReleased(event::KeyCode::X)) {
                endCutline();
            }
        }
        // nodes
        for (auto node : m_Nodes) {
            // set the titlebar color of an individual node
            ImNodes::PushColorStyle(
                    ImNodesCol_TitleBar, IM_COL32(11, 109, 191, 255));
            ImNodes::PushColorStyle(
                    ImNodesCol_TitleBarSelected, IM_COL32(81, 148, 204, 255));

            ImNodes::BeginNode(node.getId());
            // title
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node.getName());
            ImNodes::EndNodeTitleBar();
            // content
            ImGui::Dummy(ImVec2(80.0f, 45.0f));
            // inputs
            node.inputs([](Socket& socket) {
                ImNodes::BeginInputAttribute(socket.getId());
                ImGui::Text("Input");
                ImNodes::EndInputAttribute();
            });
            // outputs
            node.outputs([](Socket& socket) {
                ImNodes::BeginOutputAttribute(socket.getId());
                ImGui::Text("Output");
                ImNodes::EndOutputAttribute();
            });

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }
        // links
        for (auto& link : m_Links) {
            ImNodes::Link(link.getId(), link.getBegin(), link.getEnd());
        }
        // show mini map
        ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomLeft, nodeHoveredCallback, this);
        ImNodes::EndNodeEditor();
        // check links
//        // link creation
//        {
//            int begin, end;
//            if (ImNodes::IsLinkCreated(&begin, &end)) {
//                m_Links.emplace_back((int) m_Links.size(), begin, end);
//            }
//        }
//        // link deletion
//        {
//            int linkId;
//            if (ImNodes::IsLinkDestroyed(&linkId)) {
//                removeLink(linkId);
//            }
//        }
        // node selection
        const int num_selected_nodes = ImNodes::NumSelectedNodes();
        m_SelectedNodes.resize(num_selected_nodes);
        if (num_selected_nodes > 0) {
            ImNodes::GetSelectedNodes(m_SelectedNodes.data());
        }
        // link selection
        const int num_selected_links = ImNodes::NumSelectedLinks();
        m_SelectedLinks.resize(num_selected_links);
        if (num_selected_links > 0) {
            ImNodes::GetSelectedLinks(m_SelectedLinks.data());
        }

        ImGui::End();
    }

    void Graph::addNode(Node&& node) {
        m_Nodes.insert(node);
    }

    void Graph::addNode(const Node& node) {
        m_Nodes.insert(node);
    }

    void Graph::addLink(Link&& link) {
        m_Links.insert(link);
    }

    void Graph::addLink(const Link& link) {
        m_Links.insert(link);
    }

    void Graph::onNodeHovered(int id) {
        ENGINE_INFO("NodeEditor: onNodeHovered={0}", id);
        auto it = m_Nodes.find(id);
        ImGui::SetTooltip("%s", it->getName());
    }

    bool Graph::saveIni() {
        std::string iniFile = m_Name + ".ini";
        std::fstream file(iniFile);
        ImNodes::SaveCurrentEditorStateToIniFile(iniFile.c_str());
        return true;
    }

    bool Graph::saveBin() {
        std::string binFile = m_Name + ".bytes";

        std::fstream file(binFile, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to save graph {0}", binFile.c_str());
            return false;
        }

        // nodes
        const size_t num_nodes = m_Nodes.size();
        file.write(reinterpret_cast<const char*>(&num_nodes), static_cast<std::streamsize>(sizeof(size_t)));
        for (auto node : m_Nodes) {
            node.saveBin(file);
        }
        // links
        const size_t num_links = m_Links.size();
        file.write(reinterpret_cast<const char*>(&num_links), static_cast<std::streamsize>(sizeof(size_t)));
        for (auto link : m_Links) {
            link.saveBin(file);
        }
        // id
        file.write(reinterpret_cast<const char*>(&m_Id), static_cast<std::streamsize>(sizeof(int)));

        return true;
    }

    bool Graph::loadIni() {
        std::string iniFile = m_Name + ".ini";
        std::fstream file(iniFile);
        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to load graph {0}", iniFile.c_str());
            return false;
        }
        ImNodes::LoadCurrentEditorStateFromIniFile(iniFile.c_str());
        return true;
    }

    bool Graph::loadBin() {
        std::string binFile = m_Name + ".bytes";
        std::fstream file(binFile, std::ios_base::in | std::ios_base::binary);

        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to load graph {0}", binFile.c_str());
            return false;
        }
        // nodes
        size_t num_nodes;
        file.read(reinterpret_cast<char*>(&num_nodes), static_cast<std::streamsize>(sizeof(size_t)));
        for (int i = 0 ; i < num_nodes ; i++) {
            Node node;
            node.loadBin(file);
            m_Nodes.insert(node);
        }
        // links
        size_t num_links;
        file.read(reinterpret_cast<char*>(&num_links), static_cast<std::streamsize>(sizeof(size_t)));
        for (int i = 0 ; i < num_links ; i++) {
            Link link;
            link.loadBin(file);
            m_Links.insert(link);
        }
        // id
        file.read(reinterpret_cast<char*>(&m_Id), static_cast<std::streamsize>(sizeof(int)));

        return true;
    }

    void Graph::removeNode(int id) {
        m_Nodes.erase(id);
    }

    void Graph::removeLink(int id) {
        m_Links.erase(id);
    }

    void Graph::newNode() {
        int newId = 0;
        int newInputId = 0;
        int newOutputId = 0;

        if (m_Nodes.rbegin() != m_Nodes.rend()) {
            newId = m_Nodes.rbegin()->getId() + 1;
            newInputId = m_Nodes.rbegin()->getLastInputId() + 1;
            newOutputId = m_Nodes.rbegin()->getLastOutputId() + 1;
        }

        Node node = { newId, m_Cursor };
        node.addInput(newInputId);
        node.addOutput(newOutputId);
        addNode(node);

        ImNodes::SetNodeScreenSpacePos(newId, node.getPos());
    }

    void Graph::deleteSelected() {
        for (int nodeId : m_SelectedNodes) {
            removeNode(nodeId);
        }
        for (int linkId : m_SelectedLinks) {
            removeLink(linkId);
        }
    }

    void Graph::copySelected() {
        m_TempNodes.clear();
        for (int nodeId : m_SelectedNodes) {
            m_TempNodes.insert(*m_Nodes.find(nodeId));
        }

        m_TempLinks.clear();
        for (int linkId : m_SelectedLinks) {
            m_TempLinks.insert(*m_Links.find(linkId));
        }
    }

    void Graph::cutSelected() {
        copySelected();
        deleteSelected();
    }

    void Graph::pasteSelected() {
        for (auto node : m_TempNodes) {
            int newId = 0;
            int newInputId = 0;
            int newOutputId = 0;
            ImVec2 newPos = {
                    node.getPos().x + m_Cursor.x * m_PastePadding,
                    node.getPos().y + m_Cursor.y * m_PastePadding
            };

            if (m_Nodes.rbegin() != m_Nodes.rend()) {
                newId = m_Nodes.rbegin()->getId() + 1;
                newInputId = m_Nodes.rbegin()->getLastInputId() + 1;
                newOutputId = m_Nodes.rbegin()->getLastOutputId() + 1;
            }

            node.setId(newId);

            node.inputs([&newInputId](Socket& socket) {
                socket.setId(newInputId++);
            });

            node.outputs([&newOutputId](Socket& socket) {
                socket.setId(newOutputId++);
            });

            m_Nodes.insert(node);

            ImNodes::SetNodeScreenSpacePos(newId, newPos);
        }

        for (auto link : m_TempLinks) {
            int newId = 0;
            int newBegin = 0;
            int newEnd = 0;

            if (m_Links.rbegin() != m_Links.rend()) {
                newId = m_Links.rbegin()->getId() + 1;
                newBegin = m_Links.rbegin()->getBegin() + 1;
                newEnd = m_Links.rbegin()->getEnd() + 1;
            }

            link.setId(newId);
            link.setBegin(newBegin);
            link.setEnd(newEnd);

            m_Links.insert(link);
        }
    }

    void Graph::beginCutline() {
        m_CutlinePos = ImGui::GetMousePos();
        m_CutlineBegin = true;
    }

    void Graph::drawCutline() {
        ImGui::GetWindowDrawList()->AddLine(m_CutlinePos, m_Cursor, IM_COL32(1, 1, 1, 1));
    }

    void Graph::endCutline() {
        m_CutlineBegin = false;
    }

    void Graph::zoomCanvas() {
    }

    void Graph::moveCanvas() {
    }

    NodeEditor &NodeEditor::get() {
        static NodeEditor instance;
        return instance;
    }

    void NodeEditor::draw() {
        if (m_ActiveGraph) {
            m_ActiveGraph->draw();
        }
    }

    bool NodeEditor::save() {
        for (auto& graph : m_Graphs) {
            if (!graph.saveBin()) {
                return false;
            }
        }
        if (m_ActiveGraph) {
            return m_ActiveGraph->saveIni();
        }
        return true;
    }

    bool NodeEditor::load(const char* graphName) {
        m_Graphs.emplace_back(0, graphName);
        auto& graph = m_Graphs.back();
        if (graph.loadIni() && graph.loadBin()) {
            m_ActiveGraph = &graph;
            return true;
        } else {
            m_Graphs.pop_back();
            return false;
        }
    }

    int NodeEditor::addGraph(Graph && graph) {
        m_Graphs.emplace_back(graph);
        return m_Graphs.size() - 1;
    }

    int NodeEditor::addGraph(const Graph& graph) {
        m_Graphs.emplace_back(graph);
        return m_Graphs.size() - 1;
    }

    void NodeEditor::create() {
        ImNodes::CreateContext();
        ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    }

    void NodeEditor::destroy() {
        get().save();
        ImNodes::PopAttributeFlag();
        ImNodes::DestroyContext();
    }

    void NodeEditor::setTheme() {
        ImNodesStyle* style = &ImNodes::GetStyle();
        // todo update node editor styles
        ImNodes::StyleColorsDark(style);
    }

    bool Socket::loadBin(std::fstream& file) {
        file.read(reinterpret_cast<char*>(&m_Id), static_cast<std::streamsize>(sizeof(int)));
        return true;
    }

    bool Socket::saveBin(std::fstream &file) {
        file.write(reinterpret_cast<const char*>(&m_Id), static_cast<std::streamsize>(sizeof(int)));
        return true;
    }

    bool Link::loadBin(std::fstream &file) {
        file.read(reinterpret_cast<char*>(&m_Id), static_cast<std::streamsize>(sizeof(int)));
        file.read(reinterpret_cast<char*>(&m_Begin), static_cast<std::streamsize>(sizeof(int)));
        file.read(reinterpret_cast<char*>(&m_End), static_cast<std::streamsize>(sizeof(int)));
        return true;
    }

    bool Link::saveBin(std::fstream &file) {
        file.write(reinterpret_cast<const char*>(&m_Id), static_cast<std::streamsize>(sizeof(int)));
        file.write(reinterpret_cast<const char*>(&m_Begin), static_cast<std::streamsize>(sizeof(int)));
        file.write(reinterpret_cast<const char*>(&m_End), static_cast<std::streamsize>(sizeof(int)));
        return true;
    }
}