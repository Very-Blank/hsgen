/*
Copyright 2024 HSGEN AUTHORS

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <cctype>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

std::string To_upper(std::string name);

//HSGEN = header 'and' source 'file' generator
int main(int argc, char* argv[]){
	if (argc == 2) {
		if(strcmp("-help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0){
			std::cout << "Usage: hsfm <path> <filename1> <filename2> ..." << std::endl;
			std::cout << "It will create the files in the include and src folders as pointed by the supplied path." << std::endl;
		}
	}
	else if(argc > 2){
		std::string path = argv[1];
		for (int i = 2; i < argc; i++) {
			std::string name = argv[i];
			std::string header_file_name;
			std::string source_file_name;

			if (path[path.length() - 1] != '/') {
				header_file_name = path + "/include/" + name + ".hpp";
				source_file_name = path + "/src/" + name + ".cpp";
			}

			else {
				header_file_name = path + "include/" + name + ".hpp";
				source_file_name = path + "src/" + name + ".cpp";
			}

			int result_header = system(("touch " + header_file_name).c_str());

			if (result_header != 0) {
				std::cerr << "Failed to create file " + name + ".hpp" << std::endl;
				std::cerr << "Does include folder exist?" << std::endl;
				break;
			}

			else{
				std::cout << "Created file: " + name + ".hpp" << std::endl;
				std::ofstream header_file(header_file_name);
				if (header_file.is_open()) {
					std::string to_upper_name = To_upper(name);
					header_file << "#ifndef " << to_upper_name << "_HPP" << '\n';
					header_file << "#define " << to_upper_name << "_HPP" << '\n';
					header_file << '\n';
					header_file << '\n';
					header_file << "#endif" << '\n';
					header_file.close();
					std::cout << "Added header guards." << std::endl;
				}
				else{
					std::cerr << "Failed to open file it will be missing header guards." << std::endl;
				}
			}

			int result_source = system(("touch " + source_file_name).c_str());

			if (result_header != 0) {
				std::cerr << "Failed to create file " + name + ".cpp" << std::endl;
				std::cerr << "Does source folder exist?" << std::endl;
				break;
			}
			else{
				std::cout << "Created file: " + name + ".cpp" << std::endl;
				std::ofstream source_file(source_file_name);
				if (source_file.is_open()) {
					source_file << "#include " << '"' << name << ".hpp" << '"'  << '\n';
					source_file << '\n';
					source_file << '\n';
					source_file << '\n';
					source_file.close();
					std::cout << "Added #include preprocessor to the file." << std::endl;
				}
				else{
					std::cerr << "Failed to open file it will be missing the #include preprocessor." << std::endl;
				}
			}
		}
	}
}

//This isn't probably the best? it seems fine to me, we will see.
std::string To_upper(std::string name){
	for (char &character : name) {
		character = toupper(character);
	}
	return name;
}
