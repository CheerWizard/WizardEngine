#include <visual/NodeEditor.h>

#include <visual/FontAwesome4.h>

#include <io/Logger.h>

#include <serialization/serialization.h>

#include <event/KeyCodes.h>
#include <event/MouseCodes.h>

#include <imnodes.h>
#include <fstream>
#include <imnodes_internal.h>

namespace engine::visual {

    ImNodesContext* context = NULL;
    ImDrawList* canvas = NULL;

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
        read(file, m_Id);
        read(file, m_Name);
        // input sockets
        size_t num_inputs;
        read(file, num_inputs);
        for (int i = 0 ; i < num_inputs ; i++) {
            Socket input;
            input.loadBin(file);
            m_Inputs.insert(input);
        }
        // output sockets
        size_t num_outputs;
        read(file, num_outputs);
        for (int i = 0 ; i < num_outputs ; i++) {
            Socket output;
            output.loadBin(file);
            m_Outputs.insert(output);
        }
        return true;
    }

    bool Node::saveBin(std::fstream &file) {
        write(file, m_Id);
        write(file, m_Name);
        // input sockets
        size_t inputs = m_Inputs.size();
        write(file, inputs);
        for (auto input : m_Inputs) {
            input.saveBin(file);
        }
        // output sockets
        size_t outputs = m_Outputs.size();
        write(file, outputs);
        for (auto output : m_Outputs) {
            output.saveBin(file);
        }
        return true;
    }

    void Node::updatePos() {
        m_Pos = ImNodes::GetNodeScreenSpacePos(m_Id);
    }

    void Node::draw() {
        ImNodes::BeginNode(m_Id);

        updatePos();

        // title
        ImNodes::BeginNodeTitleBar();
        std::string title = " " + m_Name;
        title = ICON_FA_PRINT + title;
        ImGui::TextUnformatted(m_Name.c_str());
        ImNodes::EndNodeTitleBar();

        // content

        // position
        // x pos
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::PushItemWidth(32);
        ImGui::DragFloat("##pos_x", &m_Pos.x);
        ImGui::PopItemWidth();

        ImGui::SameLine();

        // y pos
        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::PushItemWidth(32);
        ImGui::DragFloat("##pos_y", &m_Pos.y);
        ImGui::PopItemWidth();
        // position

        // inputs
        inputs([](Socket& socket) {
            ImNodes::BeginInputAttribute(socket.getId());
            ImNodes::EndInputAttribute();
        });
        // outputs
        outputs([](Socket& socket) {
            ImNodes::BeginOutputAttribute(socket.getId());
            ImNodes::EndOutputAttribute();
        });

        ImNodes::EndNode();
    }

    void nodeHoveredCallback(int id, void* userData) {
        ((Graph*) userData)->onNodeHovered(id);
    }

    void Graph::draw() {
        std::string title = " " + m_Name;
        title = ICON_FA_OBJECT_GROUP + title;
        ImGui::Begin(title.data());
        // settings
        ImGui::PushItemWidth(100);
        ImGui::SliderFloat("Paste Padding", &m_PastePadding, 0, 1, "%.1f");
        ImGui::PopItemWidth();
        // node editor content
        ImNodes::BeginNodeEditor();
        // context menu
        if (m_ShowContextMenu) {
            if (ImGui::BeginPopupContextWindow("scrolling_region")) {
                if (ImGui::BeginMenu("Node")) {
                    if (ImGui::MenuItem("Test")) {
                        newNode();
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }
        }
        // track mouse pos
        m_Cursor = context->MousePos;
        // dispatch editor content events
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered())
        {
            // create new node
            if (ImGui::IsMouseReleased(event::MouseCode::ButtonRight)) {
                m_ShowContextMenu = true;
            }
            else if (ImGui::IsMouseReleased(event::MouseCode::ButtonLeft)) {
                m_ShowContextMenu = false;
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
            // draw cutline
            else if (ImGui::IsKeyDown(event::KeyCode::X)) {
                if (m_CutlineBegin) {
                    drawCutline();
                } else {
                    beginCutline();
                }
            }
            // end cutline
            else if (ImGui::IsKeyReleased(event::KeyCode::X)) {
                endCutline();
            }
        }
        // draw nodes
        for (auto node : m_Nodes) {
            node.draw();
        }
        // draw links
        for (auto link : m_Links) {
            link.draw();
        }
        // draw minimap
        ImNodes::MiniMap(0.1f, ImNodesMiniMapLocation_BottomLeft, nodeHoveredCallback, this);
        // node editor content
        ImNodes::EndNodeEditor();
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
        std::string binFile = m_Name + ".graph";

        std::fstream file(binFile, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to save graph {0}", binFile.c_str());
            return false;
        }

        write(file, m_Id);
        // nodes
        size_t num_nodes = m_Nodes.size();
        write(file, num_nodes);
        for (auto node : m_Nodes) {
            node.saveBin(file);
        }
        // links
        size_t num_links = m_Links.size();
        write(file, num_links);
        for (auto link : m_Links) {
            link.saveBin(file);
        }
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
        std::string binFile = m_Name + ".graph";
        std::fstream file(binFile, std::ios_base::in | std::ios_base::binary);

        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to load graph {0}", binFile.c_str());
            return false;
        }

        read(file, m_Id);
        // nodes
        size_t num_nodes;
        read(file, num_nodes);
        for (int i = 0 ; i < num_nodes ; i++) {
            Node node;
            node.loadBin(file);
            m_Nodes.insert(node);
        }
        // links
        size_t num_links;
        read(file, num_links);
        for (int i = 0 ; i < num_links ; i++) {
            Link link;
            link.loadBin(file);
            m_Links.insert(link);
        }
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
        int newOutputId = 1;

        if (m_Nodes.rbegin() != m_Nodes.rend()) {
            newId = m_Nodes.rbegin()->getId() + 1;
            newInputId = m_Nodes.rbegin()->getLastInputId() + 2;
            newOutputId = m_Nodes.rbegin()->getLastOutputId() + 2;
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
            int newOutputId = 1;
            ImVec2 newPos = {
                    node.getPos().x + m_Cursor.x * m_PastePadding,
                    node.getPos().y + m_Cursor.y * m_PastePadding
            };

            if (m_Nodes.rbegin() != m_Nodes.rend()) {
                newId = m_Nodes.rbegin()->getId() + 1;
                newInputId = m_Nodes.rbegin()->getLastInputId() + 2;
                newOutputId = m_Nodes.rbegin()->getLastOutputId() + 2;
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
        m_CutlinePos = ImGui::GetCursorScreenPos();
        m_CutlineBegin = true;
    }

    void Graph::drawCutline() {
        context->CanvasDrawList->AddLine(m_CutlinePos, m_Cursor, IM_COL32(1, 1, 1, 1), 2);
        ImGui::SetTooltip("Cutline [%f;%f] -> [%f;%f]", m_CutlinePos.x, m_CutlinePos.y, m_Cursor.x, m_Cursor.y);
    }

    void Graph::endCutline() {
        m_CutlineBegin = false;
    }

    void Graph::zoomCanvas() {
    }

    Ref<FileDialog> NodeEditor::s_FileDialog = nullptr;

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

    void NodeEditor::create(void* nativeWindow) {
        context = ImNodes::CreateContext();
        canvas = context->CanvasDrawList;
        s_FileDialog = createRef<FileDialog>(nativeWindow);
        ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    }

    void NodeEditor::destroy() {
        get().save();
        ImNodes::PopAttributeFlag();
        ImNodes::DestroyContext();
        context = NULL;
        canvas = NULL;
    }

    void NodeEditor::setTheme() {
        ImNodesStyle* style = &ImNodes::GetStyle();
        style->PinCircleRadius = 6;
        style->LinkThickness = 3;

        style->Colors[ImNodesCol_TitleBar] = COLOR(0.1, 0, 0.5, 1);
        style->Colors[ImNodesCol_TitleBarHovered] = COLOR(0.1, 0, 0.7, 1);
        style->Colors[ImNodesCol_TitleBarSelected] = COLOR(0.1, 0, 0.5, 1);

        style->Colors[ImNodesCol_NodeBackground] = COLOR(0.1, 0.1, 0.1, 1);
        style->Colors[ImNodesCol_NodeBackgroundHovered] = COLOR(0.2, 0.2, 0.2, 1);
        style->Colors[ImNodesCol_NodeBackgroundSelected] = COLOR(0.4, 0.4, 0.4, 1);
        style->Colors[ImNodesCol_NodeOutline] = COLOR(0, 0, 0, 0);

        style->Colors[ImNodesCol_Link] = COLOR(0.1, 0, 0.5, 1);
        style->Colors[ImNodesCol_LinkHovered] = COLOR(0.1, 0, 0.7, 1);
        style->Colors[ImNodesCol_LinkSelected] = COLOR(0.1, 0, 0.5, 1);

        style->Colors[ImNodesCol_BoxSelectorOutline] = COLOR(0, 0, 1, 1);
        style->Colors[ImNodesCol_BoxSelector] = COLOR(0.1, 0, 0.5, 1);

        style->Colors[ImNodesCol_Pin] = COLOR(0, 1, 0, 1);
        style->Colors[ImNodesCol_PinHovered] = COLOR(0, 0.5, 0, 1);
    }

    void NodeEditor::newGraph() {
        int newId = 0;
        if (m_Graphs.rbegin() != m_Graphs.rend()) {
            newId = m_Graphs.rbegin()->getId() + 1;
        }
        m_Graphs.emplace_back(newId);
        m_ActiveGraph = &m_Graphs.back();
    }

    void NodeEditor::openGraph() {
        auto importPath = s_FileDialog->getImportPath("Graph (*.graph)\0*.graph\0");
    }

    void NodeEditor::saveGraph() {
        save();
    }

    void NodeEditor::saveAsGraph() {
        auto exportPath = s_FileDialog->getExportPath("Graph (*.graph)\0*.graph\0");
    }

    void NodeEditor::cutGraphItems() {

    }

    void NodeEditor::copyGraphItems() {

    }

    void NodeEditor::pasteGraphItems() {

    }

    void NodeEditor::deleteGraphItems() {

    }

    void NodeEditor::undoGraphItems() {

    }

    void NodeEditor::redoGraphItems() {

    }

    bool Socket::loadBin(std::fstream& file) {
        read(file, m_Id);
        return true;
    }

    bool Socket::saveBin(std::fstream &file) {
        write(file, m_Id);
        return true;
    }

    bool Link::loadBin(std::fstream &file) {
        read(file, m_Id);
        read(file, m_Begin);
        read(file, m_End);
        return true;
    }

    bool Link::saveBin(std::fstream &file) {
        write(file, m_Id);
        write(file, m_Begin);
        write(file, m_End);
        return true;
    }

    void Link::draw() {
        ImNodes::Link(m_Id, m_Begin, m_End);
    }
}