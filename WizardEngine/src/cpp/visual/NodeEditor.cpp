#include <visual/NodeEditor.h>
#include <visual/FontAwesome4.h>

#include <event/MouseCodes.h>

#include <imnodes.h>
#include <imnodes_internal.h>

#include <fstream>

namespace engine::visual {

    ImNodesContext* context = NULL;
    ImDrawList* canvas = NULL;

    void Node::addInput(Socket&& socket) {
        m_Inputs.emplace(socket);
    }

    void Node::addInput(const Socket& socket) {
        m_Inputs.insert(socket);
    }

    void Node::addOutput(Socket &&socket) {
        m_Outputs.emplace(socket);
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
        read(file, m_Pos.x);
        read(file, m_Pos.y);
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

        ImNodes::SetNodeScreenSpacePos(m_Id, m_Pos);

        return true;
    }

    bool Node::saveBin(std::fstream &file) {
        write(file, m_Id);
        write(file, m_Name);
        write(file, m_Pos.x);
        write(file, m_Pos.y);
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
        // styling
        u32 nodeOutlineColor = ImNodes::IsNodeSelected(m_Id) ? COL_NODE_SELECTED : COL_NODE_OUTLINE;
        ImNodes::PushColorStyle(ImNodesCol_NodeOutline, nodeOutlineColor);

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

        // end styling
        ImNodes::PopColorStyle();
    }

    void nodeHoveredCallback(int id, void* userData) {
        ((Graph*) userData)->onNodeHovered(id);
    }

    void Graph::draw() {
        std::string title = " " + m_Name;
        title = ICON_FA_OBJECT_GROUP + title;
        ImGui::Begin(title.data());
        // settings

        // node editor content
        ImNodes::BeginNodeEditor();
        // context menu
        if (m_ShowContextMenu || m_LinkDropped) {
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
            // show/hide context menu
            if (ImGui::IsMouseReleased(event::ButtonRight)) {
                m_ShowContextMenu = true;
            }
            else if (ImGui::IsMouseReleased(event::ButtonLeft)) {
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

        // end node editor content
        ImNodes::EndNodeEditor();

        // new link connection
        int beginPinId = 0;
        int endPinId = 0;
        bool createdBySnap = false;
        if (ImNodes::IsLinkCreated(&beginPinId, &endPinId, &createdBySnap)) {
            newLink(beginPinId, endPinId);
        }

        // link dropped -> show context menu
        m_LinkDropped = ImNodes::IsLinkDropped(&beginPinId);

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
        m_Nodes.emplace(node);
    }

    void Graph::addNode(const Node& node) {
        m_Nodes.insert(node);
    }

    void Graph::addLink(Link&& link) {
        m_Links.emplace(link);
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

    bool Graph::saveBin(const char* filepath) {
        std::fstream file(filepath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to save graph {0}", filepath);
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

        file.close();
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

    bool Graph::loadBin(const char* filepath) {
        std::fstream file(filepath, std::ios_base::in | std::ios_base::binary);

        if (!file.is_open()) {
            ENGINE_ERR("NodeEditor: Failed to load graph {0}", filepath);
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

        file.close();
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
        ImNodes::SnapNodeToGrid(newId);
    }

    void Graph::newLink(int beginId, int endId) {
        int newId = 0;

        if (m_Links.rbegin() != m_Links.rend()) {
            newId = m_Links.rbegin()->getId() + 1;
        }

        m_Links.emplace(newId, beginId, endId, this);
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
            ImNodes::SnapNodeToGrid(newId);
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

            m_Links.emplace(newId, newBegin, newEnd, this);
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

    void Graph::undo() {

    }

    void Graph::redo() {

    }

    Ref<FileDialog> NodeEditor::s_FileDialog = nullptr;

    NodeEditor &NodeEditor::get() {
        static NodeEditor instance;
        return instance;
    }

    void NodeEditor::draw() {
        if (m_ActiveGraph) {
            m_ActiveGraph->draw();
            drawViewPort();
            drawTextEditor();
        }
    }

    void NodeEditor::drawViewPort() {
        auto& scene = m_ActiveGraph->getScene();
        u32 renderTargetId = m_ActiveGraph->getRenderTargetId();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.f, 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::Begin("ViewPort");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();

        bool viewportFocused = ImGui::IsWindowFocused();
        bool viewportHovered = ImGui::IsWindowHovered();
        NodeEditor::get().blockEvents = !viewportHovered || !viewportFocused;

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        if (scene) {
            scene->viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            scene->viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
            scene->viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }

        ImGui::Image(
                (ImTextureID)(renderTargetId),
                viewportPanelSize,
                { 0, 1 },
                { 1, 0 }
        );

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    }

    void NodeEditor::drawTextEditor() {
        m_TextEditor.Render("Preview", { 100, 300 });
    }

    bool NodeEditor::saveAll() {
        for (auto& graph : m_Graphs) {
            std::string filepath = graph.getName();
            filepath.append(".graph");
            if (!graph.saveBin(filepath.c_str())) {
                return false;
            }
        }
        if (m_ActiveGraph) {
            return m_ActiveGraph->saveIni();
        }
        return true;
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
        get().saveAll();
        ImNodes::PopAttributeFlag();
        ImNodes::DestroyContext();
        context = NULL;
        canvas = NULL;
    }

    void NodeEditor::setTheme() {
        ImNodesStyle* style = &ImNodes::GetStyle();
        style->PinCircleRadius = 6;
        style->LinkThickness = 3;
        style->NodeBorderThickness = 2;
        style->NodeCornerRounding = 4;
        style->NodePadding = { 16, 16 };

        style->Colors[ImNodesCol_TitleBar] = COLOR(0.2, 0, 0, 1);
        style->Colors[ImNodesCol_TitleBarHovered] = COLOR(0.25, 0, 0, 1);
        style->Colors[ImNodesCol_TitleBarSelected] = COLOR(0.3, 0, 0, 1);

        style->Colors[ImNodesCol_NodeBackground] = COLOR(0.1, 0.1, 0.1, 1);
        style->Colors[ImNodesCol_NodeBackgroundHovered] = COLOR(0.15, 0.15, 0.15, 1);
        style->Colors[ImNodesCol_NodeBackgroundSelected] = COLOR(0.2, 0.2, 0.2, 1);
        style->Colors[ImNodesCol_NodeOutline] = COL_NODE_OUTLINE;

        style->Colors[ImNodesCol_Link] = COLOR(0.2, 0, 0, 1);
        style->Colors[ImNodesCol_LinkHovered] = COLOR(0.25, 0, 0, 1);
        style->Colors[ImNodesCol_LinkSelected] = COLOR(0.3, 0, 0, 1);

        style->Colors[ImNodesCol_BoxSelector] = COLOR(0, 0, 0, 0.25);
        style->Colors[ImNodesCol_BoxSelectorOutline] = COLOR(1, 0.2, 0, 1);

        style->Colors[ImNodesCol_Pin] = COLOR(0.2, 0, 0, 1);
        style->Colors[ImNodesCol_PinHovered] = COLOR(0.25, 0, 0, 1);
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
        if (importPath.empty()) {
            ENGINE_WARN("NodeEditor::openGraph: import closed");
            return;
        }
        newGraph();
        m_ActiveGraph->loadBin(importPath.c_str());
    }

    void NodeEditor::saveGraph() {
        std::string filepath = m_ActiveGraph->getName();
        filepath.append(".graph");
        m_ActiveGraph->saveBin(filepath.c_str());
    }

    void NodeEditor::saveAsGraph() {
        auto exportPath = s_FileDialog->getExportPath("Graph (*.graph)\0*.graph\0");
        if (exportPath.empty()) {
            ENGINE_WARN("NodeEditor::openGraph: import closed");
            return;
        }
        m_ActiveGraph->saveBin(exportPath.c_str());
    }

    void NodeEditor::cutGraphItems() {
        if (m_ActiveGraph) {
            m_ActiveGraph->cutSelected();
        }
    }

    void NodeEditor::copyGraphItems() {
        if (m_ActiveGraph) {
            m_ActiveGraph->copySelected();
        }
    }

    void NodeEditor::pasteGraphItems() {
        if (m_ActiveGraph) {
            m_ActiveGraph->pasteSelected();
        }
    }

    void NodeEditor::deleteGraphItems() {
        if (m_ActiveGraph) {
            m_ActiveGraph->deleteSelected();
        }
    }

    void NodeEditor::undoGraphItems() {
        if (m_ActiveGraph) {
            m_ActiveGraph->undo();
        }
    }

    void NodeEditor::redoGraphItems() {
        if (m_ActiveGraph) {
            m_ActiveGraph->redo();
        }
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
        // styling
        u32 linkOutlineColor = ImNodes::IsLinkSelected(m_Id) ? COL_LINK_SELECTED : COL_LINK_OUTLINE;
        ImNodes::PushColorStyle(ImNodesCol_LinkSelected, linkOutlineColor);

        ImNodes::Link(m_Id, m_Begin, m_End);

        // end styling
        ImNodes::PopColorStyle();
    }
}