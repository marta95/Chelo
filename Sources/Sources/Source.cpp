#include "Source.h"

namespace Sources
{
    using namespace std::experimental;

    FileSource::FileSource(std::string filename) :
        file(filename)
    {

    }

    optional<std::string> FileSource::readChar()
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
}