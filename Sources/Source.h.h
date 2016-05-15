#pragma once

#include "optional.h"
#include <fstream>
#include <string>

using namespace std::experimental;

class Source
{
    public:
        virtual optional<std::string> readChar() = 0;
};

class FileSource : public Source
{
    std::ifstream file;

    public:
        FileSource(std::string filename) :
                file(filename)
        {

        }

        optional<std::string> readChar()
        {
            if (file.good() && !file.eof()) {
                std::string string = "";
                int character = file.get();
                string.push_back((char) character);

                if (character != EOF) {
                    return make_optional(string);
                } else {
                    return optional<std::string>();
                }
            } else {
                return optional<std::string>();
            }
        }
};
