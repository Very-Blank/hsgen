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
#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

std::string To_upper(std::string name);

//HSGEN = header 'and' source 'file' generator
int main(int argc, char* argv[]){
	if (argc == 2) {
		if(strcmp("-help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0){
			std::cout << "Create: hsgen <path> <filename1> <filename2> ..." << std::endl;
			std::cout << "It will create the files in the include and src folders as pointed by the supplied path." << std::endl;
			std::cout << '\n';
			std::cout << "Rename: hsgen -rn <path> <old_name> <new_name>" << std::endl;
			std::cout << "It will rename the files in the include and src folders as pointed by the supplied path." << std::endl;
		}
		return 0;
	}

	else if(argc < 2){
		std::cout << "Too few arguments supplied" << std::endl;
		return -1;
	}
	// Renaming source and hearder files
	if(argc == 3 && strcmp("-rn", argv[1]) == 0){
		std::string path = argv[2];
		std::string old_name = argv[3];
		std::string new_name = argv[4];

		std::string old_header_file_name;
		std::string old_source_file_name;

		std::string new_header_file_name;
		std::string new_source_file_name;

		if (path[path.length() - 1] != '/') {
			old_header_file_name = path + "/include/" + old_name + ".hpp";
			old_source_file_name = path + "/src/" + old_name  + ".cpp";

			new_header_file_name = path + "/include/" + new_name + ".hpp";
			new_source_file_name = path + "/src/" + new_name + ".cpp";
		}

		else {
			old_header_file_name = path + "include/" + old_name + ".hpp";
			old_source_file_name = path + "src/" + old_name + ".cpp";

			new_header_file_name = path + "/include/" + new_name + ".hpp";
			new_source_file_name = path + "/src/" + new_name + ".cpp";
		}

		//header file
		if (fs::exists(new_header_file_name) || !fs::exists(old_header_file_name)) {
			std::cerr << "Source file already exist or the old header file doesn't exist, aborting file creation." << std::endl;
			return -1;
		}

		std::ofstream new_header_file(new_header_file_name);
		if (new_header_file.is_open()) {
			std::string to_upper_name = To_upper(new_name);
			new_header_file << "#ifndef " << to_upper_name << "_HPP" << '\n';
			new_header_file << "#define " << to_upper_name << "_HPP" << '\n';
			new_header_file << '\n';

			// write the the data of the file
			std::ifstream old_header_file(old_header_file_name);
			if (old_header_file.is_open()) {
				std::string line;
				char i = 0;

				while (std::getline(old_header_file, line)) {
					if (i < 4) {
						i++;
					}
					else {
						new_header_file << line << '\n';
					}
				}

				old_header_file.close();
				std::remove(old_header_file_name.c_str());
				std::cout << "Copyied header files's contents and deleted it" << std::endl;
			}
			else {
				std::cerr << "Couldn't open the old header file, new header file doesn't have the old's contents" << std::endl;
				std::cerr << "Old header file will not be deleted" << std::endl;
			}
			
			new_header_file.flush();
			new_header_file.close();

		}
		else{
			std::cerr << "Couldn't create a the header file" << std::endl;
			return -1;
		}

		//source file
		if (fs::exists(new_source_file_name) || !fs::exists(old_source_file_name)) {
			std::cerr << "Source file already exist or the old source file doesn't exist, aborting file creation." << std::endl;
			return -1;
		}

		std::ofstream source_file(new_source_file_name);
		if (source_file.is_open()) {
			source_file << "#include " << '"' << new_name << ".hpp" << '"'  << '\n';
			source_file << '\n';

			// write the the data of the file
			std::ifstream old_source_file(old_source_file_name);
			if (old_source_file.is_open()) {
				std::string line;
				char i = 0;

				while (std::getline(old_source_file, line)) {
					if (i < 1) {
						i++;
					}
					else {
						new_header_file << line << '\n';
					}
				}

				old_source_file.close();
				std::remove(old_source_file_name.c_str());
				std::cout << "Copyied source files's contents and deleted it" << std::endl;
			}
			else {
				std::cerr << "Couldn't open the old source file, new header file doesn't have the old's contents" << std::endl;
				std::cerr << "Old source file will not be deleted" << std::endl;
			}
			source_file.flush();
			source_file.close();
		}
		else{
			std::cerr << "Source file already exists aborting" << std::endl;
			return -1;
		}
	}
	// Creating header and source files
	else {
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
			
			//header file
			if (fs::exists(header_file_name)) {
				std::cerr << "Header file already exist, aborting file creation." << std::endl;
				return -1;
			}

			std::ofstream header_file(header_file_name);
			if (header_file.is_open()) {
				std::string to_upper_name = To_upper(name);
				header_file << "#ifndef " << to_upper_name << "_HPP" << '\n';
				header_file << "#define " << to_upper_name << "_HPP" << '\n';
				header_file << '\n';
				header_file << '\n';
				header_file << "#endif" << '\n';
				
				header_file.flush();
				header_file.close();
				std::cout << "Added header guards." << std::endl;
			}
			else{
				std::cerr << "Couldn't create the header file." << std::endl;
				return -1;
			}

			//source file
			if (fs::exists(source_file_name)) {
				std::cerr << "Source file already exist, aborting file creation." << std::endl;
				return -1;
			}

			std::ofstream source_file(source_file_name);
			if (source_file.is_open()) {
				source_file << "#include " << '"' << name << ".hpp" << '"'  << '\n';
				source_file << '\n';
				source_file << '\n';
				source_file.flush();
				source_file.close();
				std::cout << "Added #include preprocessor to the file." << std::endl;
			}
			else{
				std::cerr << "Couldn't create the source file." << std::endl;
				return -1;
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
