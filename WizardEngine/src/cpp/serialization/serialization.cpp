#include <serialization/serialization.h>

template<>
void read<std::string>(std::fstream& file, std::string& string) {
    size_t strLength;
    file.read(reinterpret_cast<char*>(&strLength), static_cast<std::streamsize>(sizeof(size_t)));
    string.resize(strLength);
    file.read(reinterpret_cast<char*>(string.data()), static_cast<std::streamsize>(sizeof(char) * strLength));
}

template<>
void write<std::string>(std::fstream& file, std::string& string) {
    size_t nameSize = string.size();
    file.write(reinterpret_cast<const char*>(&nameSize), static_cast<std::streamsize>(sizeof(size_t)));
    file.write(reinterpret_cast<const char*>(string.c_str()), static_cast<std::streamsize>(string.size()));
}