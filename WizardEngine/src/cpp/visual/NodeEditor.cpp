#include <visual/NodeEditor.h>

#include <visual/FontAwesome4.h>

#include <io/Logger.h>

#include <imnodes.h>

namespace engine::visual {

    Node::~Node() {
        m_Inputs.clear();
        m_Outputs.clear();
    }

    void Node::addInput(Socket&& socket) {
        m_Inputs.emplace_back(socket);
    }

    void Node::addInput(const Socket& socket) {
        m_Inputs.emplace_back(socket);
    }

    void Node::addOutput(Socket &&socket) {
        m_Outputs.emplace_back(socket);
    }

    void Node::addOutput(const Socket& socket) {
        m_Outputs.emplace_back(socket);
    }

    void Node::inputs(const std::function<void(Socket &)> &handle) {
        for (auto& socket : m_Inputs) {
            handle(socket);
        }
    }

    void Node::outputs(const std::function<void(Socket &)> &handle) {
        for (auto& socket : m_Outputs) {
            handle(socket);
        }
    }

    NodeEditor &NodeEditor::get() {
        static NodeEditor instance;
        return instance;
    }

    void nodeHoveredCallback(int id, void* userData) {
        ((NodeEditor*) userData)->onNodeHovered(id);
    }

    void NodeEditor::draw() {
        ImGui::Begin(ICON_FA_LANGUAGE" RectLang");
        ImNodes::BeginNodeEditor();
        // set the titlebar color for all nodes
        ImNodesStyle& style = ImNodes::GetStyle();
        style.Colors[ImNodesCol_TitleBar] = IM_COL32(232, 27, 86, 255);
        style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(241, 108, 146, 255);
        // nodes
        for (auto& node : m_Nodes) {
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
        // link creation
        {
            int begin, end;
            if (ImNodes::IsLinkCreated(&begin, &end)) {
                m_Links.emplace_back((int) m_Links.size(), begin, end);
            }
        }
        // link deletion
        {
            int linkId;
            if (ImNodes::IsLinkDestroyed(&linkId)) {
                auto iter = std::find_if(
                        m_Links.begin(), m_Links.end(), [linkId](const Link& link) -> bool {
                            return link.getId() == linkId;
                        });
                assert(iter != m_Links.end());
                m_Links.erase(iter);
            }
        }

        ImGui::End();
    }

    void NodeEditor::addNode(Node&& node) {
        m_Nodes.emplace_back(node);
    }

    void NodeEditor::addNode(const Node& node) {
        m_Nodes.emplace_back(node);
    }

    void NodeEditor::addLink(Link&& link) {
        m_Links.emplace_back(link);
    }

    void NodeEditor::addLink(const Link& link) {
        m_Links.emplace_back(link);
    }

    void NodeEditor::onNodeHovered(int id) {
        ENGINE_INFO("NodeEditor: onNodeHovered={0}", id);
        for (auto& node : m_Nodes) {
            if (node.getId() == id) {
                ImGui::SetTooltip("%s", node.getName());
                break;
            }
        }
    }

}