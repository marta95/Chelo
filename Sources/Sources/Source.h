#pragma once

#include "../Utilities/optional.h"
#include <fstream>
#include <string>

namespace Sources
{
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
            FileSource(std::string filename);
            optional<std::string> readChar();
    };
}