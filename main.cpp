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

namespace Mode {
	enum values{
		help,
		create,
		rename,
		remove,
		error,
	};
}

Mode::values Get_mode(int argc, char* argv[]);
std::string To_upper(std::string name);
void Set_string_path(std::string &header, std::string &source, const std::string &path, const std::string &name);

//HSGEN = header 'and' source 'file' generator
int main(int argc, char* argv[]){
	switch (Get_mode(argc, argv)) {
		case Mode::help : {
			std::cout << "hsgen: Create: hsgen <path> <filename1> <filename2> ..." << std::endl;
			std::cout << "hsgen: It will create the files in the include and src folders as pointed by the supplied path." << std::endl;
			std::cout << "hsgen: " <<  '\n';
			std::cout << "hsgen: Rename: hsgen -rn <path> <old_name> <new_name>" << std::endl;
			std::cout << "hsgen: It will rename the files in the include and src folders as pointed by the supplied path." << std::endl;
			std::cout << "hsgen: " <<  '\n';
			std::cout << "hsgen: Remove: hsgen -del <path> <name>" << std::endl;
			std::cout << "hsgen: It will remove the files in the include and src folders as pointed by the supplied path." << std::endl;
			return 0;
		}

		case Mode::create : {
			std::string path = argv[1];
			for (int i = 2; i < argc; i++) {
				std::string name = argv[i];
				std::string header_file_name;
				std::string source_file_name;
				Set_string_path(header_file_name, source_file_name, path, name);

				if (fs::exists(source_file_name) || fs::exists(header_file_name)) {
					std::cerr << "hsgen: Source or header file already exist, aborting file creation." << std::endl;
					return -1;
				} 

				std::ofstream header_file(header_file_name);
				if (header_file.is_open()) {
					std::cout << "hsgen: Created header file: " << name << std::endl;

					std::string to_upper_name = To_upper(name);
					header_file << "#ifndef " << to_upper_name << "_HPP" << '\n';
					header_file << "#define " << to_upper_name << "_HPP" << '\n';
					header_file << '\n';
					header_file << "#endif" << '\n';
					
					header_file.flush();
					header_file.close();
					std::cout << "hsgen: Added header guards." << std::endl;

				} else{
					std::cerr << "hsgen: Couldn't create the header file." << std::endl;
					return -1;
				}

				std::ofstream source_file(source_file_name);
				if (source_file.is_open()) {
					std::cout << "hsgen: Created source file: " << name << std::endl;
					source_file << "#include " << '"' << name << ".hpp" << '"'  << '\n';
					source_file << '\n';
					source_file << '\n';
					source_file.flush();
					source_file.close();
					std::cout << "hsgen: Added #include preprocessor to the file." << std::endl;

				} else{
					std::cerr << "hsgen: Couldn't create the source file." << std::endl;
					return -1;
				}
			}
			
			return 0;
		}

		case Mode::rename : {
			std::string path = argv[2];
			std::string old_name = argv[3];
			std::string new_name = argv[4];

			std::string old_header_file_name;
			std::string old_source_file_name;
			Set_string_path(old_header_file_name, old_source_file_name, path, old_name);

			std::string new_header_file_name;
			std::string new_source_file_name;
			Set_string_path(new_header_file_name, new_source_file_name, path, new_name);

			//header file check
			if (fs::exists(new_header_file_name) || !fs::exists(old_header_file_name)) {
				std::cerr << "hsgen: A header file with the new name already exist or the old header file doesn't exist." << std::endl;
				return -1;

			} else if (fs::exists(new_source_file_name) || !fs::exists(old_source_file_name)) {
				std::cerr << "hsgen: A source file with the new name already exist or the old source file doesn't exist." << std::endl;
				return -1;
			}

			std::ofstream new_header_file(new_header_file_name);
			if (new_header_file.is_open()) {
				std::cout << "hsgen: Created new header file: " << new_name << std::endl;
				std::string to_upper_name = To_upper(new_name);
				new_header_file << "#ifndef " << to_upper_name << "_HPP" << '\n';
				new_header_file << "#define " << to_upper_name << "_HPP" << '\n';
				new_header_file << '\n';

				std::ifstream old_header_file(old_header_file_name);
				if (old_header_file.is_open()) {
					std::string line;
					char i = 1;

					while (std::getline(old_header_file, line)) {
						if (i < 4) {
							i++;
						} else {
							new_header_file << line << '\n';
						}
					}

					old_header_file.close();

					int header_delete = std::remove(old_header_file_name.c_str());
					std::cout << "hsgen: Copyied " << old_name << " header files's contents";
					if (header_delete != 0) {
						std::cout << ", but failed to delete it" << std::endl;
					} else {
						std::cout << " and deleted it" << std::endl;
					}

				} else {
					std::cerr << "hsgen: Couldn't open the old header file, new header file doesn't have the old's contents" << std::endl;
					std::cerr << "hsgen: Old header file will not be deleted" << std::endl;
				}
				
				new_header_file.flush();
				new_header_file.close();

			} else {
				std::cerr << "hsgen: Couldn't create the header file: " << new_name << std::endl;
				return -1;
			}

			std::ofstream new_source_file(new_source_file_name);
			if (new_source_file.is_open()) {
				std::cout << "hsgen: Created new source file: " << new_name << std::endl;
				new_source_file << "#include " << '"' << new_name << ".hpp" << '"'  << '\n';
				new_source_file << '\n';

				std::ifstream old_source_file(old_source_file_name);
				if (old_source_file.is_open()) {
					std::string line;
					char i = 0;

					while (std::getline(old_source_file, line)) {
						if (i < 1) {
							i++;
						} else {
							new_header_file << line << '\n';
						}
					}

					old_source_file.close();

					int source_delete = std::remove(old_source_file_name.c_str());
					std::cout << "hsgen: Copyied " << old_name << " source files's contents";
					if (source_delete != 0) {
						std::cout << ", but failed to delete it" << std::endl;
					} else {
						std::cout << " and deleted it" << std::endl;
					}

				} else {
					std::cerr << "hsgen: Couldn't open the old source file, new header file doesn't have the old's contents" << std::endl;
					std::cerr << "hsgen: Old source file will not be deleted" << std::endl;
				}

				new_source_file.flush();
				new_source_file.close();

			} else{
				std::cerr << "hsgen: Couldn't create the source file: " << new_name << std::endl;
				return -1;
			}

			return 0;
		}

		case Mode::remove : {
			std::string header_file_name;
			std::string source_file_name;
			Set_string_path(header_file_name, source_file_name, argv[2], argv[3]);
			int header_remove = std::remove(header_file_name.c_str());
			int source_remove = std::remove(source_file_name.c_str());
			if (header_remove != 0) {
				std::cerr << "hsgen: Couldn't remove the file: " << header_file_name << std::endl;

			} else {
				std::cerr << "hsgen: Removed the file: " << header_file_name << std::endl;
			}

			if (source_remove != 0) {
				std::cerr << "hsgen: Couldn't remove the file: " << source_file_name << std::endl;
			} else {
				std::cerr << "hsgen: Removed the file: " << source_file_name << std::endl;
			}

			return 0;
		}

		default : {
			std::cerr << "hsgen: Incorrect arguments supplied" << std::endl;
			std::cout << "hsgen: hsgen -help" << std::endl;
			return -1;
		}
	}
}

Mode::values Get_mode(int argc, char* argv[]){
	if (argc == 2 && (strcmp("-help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)) {
		return Mode::help;

	} else if(argc == 4 && strcmp("-del", argv[1]) == 0){
		return Mode::remove;

	} else if(argc == 5 && strcmp("-rn", argv[1]) == 0){
		return Mode::rename;

	} else if (argc > 2) {
		return Mode::create;

	} else{
		return Mode::error;
	}
}

//This isn't probably the best? it seems fine to me, we will see.
std::string To_upper(std::string name){
	for (char &character : name) {
		character = toupper(character);
	}
	return name;
}

void Set_string_path(std::string &header, std::string &source, const std::string &path, const std::string &name){
	if (path[path.length() - 1] != '/') {
		header = path + "/include/" + name + ".hpp";
		source = path + "/src/" + name + ".cpp";
	}

	else {
		header = path + "include/" + name + ".hpp";
		source = path + "src/" + name + ".cpp";
	}
}
