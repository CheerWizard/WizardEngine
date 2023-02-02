//
// Created by mecha on 29.05.2022.
//

#include <network/gdp.h>
#include <io/Logger.h>

namespace engine::network {

    void GDHeader::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        yaml::serialize(out, "address", address);
        yaml::serialize(out, "type", type);
        out << YAML::EndMap;
    }

    void GDHeader::deserialize(const YAML::Node &parent) {
        address = parent["address"].as<u32>();
        type = parent["type"].as<u32>();
    }

    void GDHeader::read(std::fstream &file) {
        ::read(file, address);
        ::read(file, type);
    }

    void GDHeader::write(std::fstream &file) {
        ::write(file, address);
        ::write(file, type);
    }

    bool GDSerializer::deserialize(char *gameData, std::pair<YAML::Node, GDHeader>& gdNodeHeader) {
        try {
            YAML::Node gdNode = YAML::Load(gameData)["GameData"];
            if (gdNode) {
                auto headerNode = gdNode["header"];
                GDHeader header;
                if (headerNode) {
                    header.deserialize(headerNode);
                }

                auto bodyNode = gdNode["body"];
                if (bodyNode) {
                    gdNodeHeader = { bodyNode, header };
                    return true;
                }

                return false;
            }

            return false;
        } catch (YAML::ParserException& e) {
            ENGINE_ERR("GDSerializer: Failed to parse YAML text!");
            ENGINE_ERR(e.msg);
            return false;
        }
    }

    void GDResponse::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        yaml::serialize(out, "statusCode", statusCode);
        yaml::serialize(out, "message", message);
        out << YAML::EndMap;
    }

    void GDResponse::deserialize(const YAML::Node &parent) {
        statusCode = parent["statusCode"].as<u16>();
        message = parent["message"].as<std::string>().c_str();
    }

    void GDResponse::read(std::fstream &file) {
        ::read(file, statusCode);
        ::read(file, message);
    }

    void GDResponse::write(std::fstream &file) {
        ::write(file, statusCode);
        ::write(file, message);
    }
}