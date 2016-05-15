#include <iostream>
#include <memory>
#include <regex>

#include <fstream>
#include "Source.h.h"
#include "Parser.h"

int main()\
{
	std::shared_ptr<Source> source = std::make_shared<FileSource>("C:\\Users\\Marta\\Documents\\projekt_z_cpp\\Chelo\\test.che");
	Parser parser(source);

	try {
		parser.getProgram();
	} catch (std::runtime_error exception) {
		std::cout << "U DUN GOOF'D: " << exception.what() << std::endl;
	}
	
	system("pause");

    return 0;
}