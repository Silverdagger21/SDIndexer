#include "fileparser.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <thread>


using namespace sdindex;


bool FileParser::toLowercase = false;
bool FileParser::ommitSpecialCharacters = false;
bool FileParser::ommitNumbers = false;
bool FileParser::allowExtensions = true;




bool FileParser::index_directory_and_subdirectories(const std::string& dirpath, IndexHashtable& index,
	const std::vector<std::string>& extensions) {


	const int thread_count = (int)std::thread::hardware_concurrency();
	int i, j;
	std::vector<std::string> filenames = FileParser::get_filenames_from_directories(dirpath);

	if(filenames.empty()) {
		std::cout << "Directory " + dirpath + " does not contain any files \n";
		return false;
	}

	if(!extensions.empty())FileParser::filter_files_by_extension(filenames, extensions);

	if(thread_count == 0) {
		std::cout << "Unable to calculate core count \n";
		for(j = 0; j < filenames.size(); j++) {
			if(!FileParser::parse_file(filenames[j], index)) {
				std::cerr << "Failed to open file \"" << filenames[j] << "\"" << std::endl;
			}
		}
		return true;

	} else {

		std::vector<IndexHashtable*> indexVector;
		std::vector<std::string*>* filenameVectorArray = new std::vector<std::string*>[thread_count];
		std::thread** indexWorkers = new std::thread * [thread_count];


		for(i = 0; i < thread_count - 1; i++) {
			indexVector.push_back(new IndexHashtable(index.get_size()));
		}

		i = 0;
		j = 0;
		while(i < filenames.size()) {
			filenameVectorArray[j].push_back(&filenames[i]);
			i++;
			j++;
			if(j == thread_count) j = 0;
		}

		for(i = 0; i < thread_count; i++) {

			// Using main index as well to reduce merging time
			if(i == 0) {
				indexWorkers[i] = new std::thread(index_files, std::cref(filenameVectorArray[i]), std::ref(index));
			} else {
				j = i - 1;
				indexWorkers[i] = new std::thread(index_files, std::cref(filenameVectorArray[i]), std::ref(*indexVector[j]));
			}
		}

		indexWorkers[0]->join();
		for(i = 1; i < thread_count; i++) {
			indexWorkers[i]->join();
			j = i - 1;
			if(!index.merge(indexVector[j])) {
				std::cerr << "Could not merge indexes" << std::endl;
			}
		}

		// Cleanup
		for(i = 0; i < thread_count - 1; i++) {
			delete indexVector[i];
		}

		for(i = 0; i < thread_count; i++) {
			delete indexWorkers[i];
		}

		delete[] filenameVectorArray;
		delete[] indexWorkers;

		return true;
	}
}



bool FileParser::index_directory(const std::string& dirpath, IndexHashtable& index,
	const std::vector<std::string>& extensions) {

	const int thread_count = (int)std::thread::hardware_concurrency();
	int i, j;
	std::vector<std::string> filenames = FileParser::get_filenames_from_directory(dirpath);

	if(filenames.empty()) {
		std::cout << "Directory " + dirpath + " does not contain any files \n";
		return false;
	}

	if(!extensions.empty())FileParser::filter_files_by_extension(filenames, extensions);

	if(thread_count == 0) {
		std::cout << "Unable to calculate core count \n";
		for(j = 0; j < filenames.size(); j++) {
			if(!FileParser::parse_file(filenames[j], index)) {
				std::cerr << "Failed to open file \"" << filenames[j] << "\"" << std::endl;
			}
		}
		return true;

	} else {

		std::vector<IndexHashtable*> indexVector;
		std::vector<std::string*>* filenameVectorArray = new std::vector<std::string*>[thread_count];
		std::thread** indexWorkers = new std::thread * [thread_count];


		for(i = 0; i < thread_count - 1; i++) {
			indexVector.push_back(new IndexHashtable(index.get_size()));
		}

		i = 0;
		j = 0;
		while(i < filenames.size()) {
			filenameVectorArray[j].push_back(&filenames[i]);
			i++;
			j++;
			if(j == thread_count) j = 0;
		}

		for(i = 0; i < thread_count; i++) {

			// Using main index as well to reduce merging time
			if(i == 0) {
				indexWorkers[i] = new std::thread(index_files, std::cref(filenameVectorArray[i]), std::ref(index));
			} else {
				j = i - 1;
				indexWorkers[i] = new std::thread(index_files, std::cref(filenameVectorArray[i]), std::ref(*indexVector[j]));
			}
		}

		indexWorkers[0]->join();
		for(i = 1; i < thread_count; i++) {
			indexWorkers[i]->join();
			j = i - 1;
			if(!index.merge(indexVector[j])) {
				std::cerr << "Could not merge indexes" << std::endl;
			}
		}

		// Cleanup
		for(i = 0; i < thread_count - 1; i++) {
			delete indexVector[i];
		}

		for(i = 0; i < thread_count; i++) {
			delete indexWorkers[i];
		}

		delete[] filenameVectorArray;
		delete[] indexWorkers;

		return true;
	}
}



bool FileParser::parse_file(const std::string& filename, IndexHashtable& index) {

	std::ifstream infile;
	std::string line;

	infile.open(filename, std::ios::in);

	if(infile.is_open()) {

		//Split filename from path and parse it first
		line = std::filesystem::path(filename).filename().stem().string();
		if(!parse_line(line, index, filename)) {
			infile.close();
			return false;
		}

		while(std::getline(infile, line)) {
			if(!parse_line(line, index, filename)) {
				infile.close();
				return false;
			}
		}
	} else {
		std::cerr << "Specified file " << filename << " not found" << std::endl;
		return false;
	}

	infile.close();

	return true;
}



bool FileParser::parse_line(const std::string& line, IndexHashtable& index, const std::string& filename) {

	std::string word;
	bool status = false;
	char c;
	int i;

	i = 0;
	word.clear();
	while(i < line.size()) {
		c = line[i];
		if(c != ' ') {
			word.push_back(c);

		} else {
			parse_word(word, toLowercase, ommitNumbers, ommitSpecialCharacters);
			if(!word.empty()) {
				status = index.add_to_index(word, filename);
				if(!status) std::cerr << "Problem inputing word into the hashtable" << std::endl;
			}
			word.clear();
		}
		i++;
	}
	if(word.length() > 0) {
		parse_word(word, toLowercase, ommitNumbers, ommitSpecialCharacters);
		if(!word.empty()) {
			status = index.add_to_index(word, filename);
			if(!status) std::cerr << "Problem inputing word into the hashtable" << std::endl;
		}
		word.clear();
	}
	return true;
}



void FileParser::parse_word(std::string& word, bool toLowercase, bool  ommitNumbers, bool ommitSpecialCharacters) {

	if(toLowercase) {
		convert_to_lowercase(word);
	}
	if(ommitSpecialCharacters) {
		remove_symbols(word);
	}
	if(ommitNumbers) {
		remove_numbers(word);
	}
}




std::vector<std::string> FileParser::split_string(const std::string& input, const char& c) {

	std::vector<std::string> fragments;
	std::string word;
	int i = 0;

	word.clear();
	for(i = 0; i < input.size(); i++) {
		if(input[i] == c) {
			if(!word.empty()) {
				fragments.push_back(word);
			}
			word.clear();

		} else {
			word.push_back(input[i]);
		}
	}

	if(!word.empty()) {
		fragments.push_back(word);
	}

	return fragments;
}



void FileParser::index_files(const std::vector<std::string*>& filenames, IndexHashtable& index) {

	for(int j = 0; j < filenames.size(); j++) {
		if(!FileParser::parse_file(*filenames[j], index)) {
			std::cerr << "Failed to open file \"" << filenames[j] << "\"" << std::endl;
		}
	}
}



void FileParser::convert_to_lowercase(std::string& word) {

	for(int i = 0; i < word.size(); i++) {
		word[i] = tolower(word[i]);
	}
}



void FileParser::remove_symbols(std::string& word) {

	for(int i = 0; i < word.size(); i++) {
		if((word[i] < 'A' || word[i] > 'Z') &&
			(word[i] < 'a' || word[i] > 'z') && (word[i] < '0' || word[i] > '9')) {
			word.erase(i, 1);
			i--;
		}
	}
}



void FileParser::remove_numbers(std::string& word) {

	for(int i = 0; i < word.size(); i++) {
		if(word[i] >= '0' && word[i] <= '9') {
			word.erase(i, 1);
			i--;
		}
	}
}



void FileParser::write_index_file_to_drive(const std::string& filename, IndexHashtable& index) {

	std::ofstream outfile;
	std::string line;

	outfile.open(filename, std::ios::out);
	outfile << index.to_string();
	outfile.close();
}



std::vector<std::string> FileParser::get_filenames_from_directories(const std::string& path) {

	std::vector < std::string> v;

	if(!std::filesystem::is_directory(path)) {
		std::cerr << "Directory path not found\n";
		return v;
	}

	for(const auto& file : std::filesystem::recursive_directory_iterator(path)) {
		if(std::filesystem::is_regular_file(file.path())) {
			v.push_back(file.path().string());
		}
	}
	return v;
}



std::vector<std::string> FileParser::get_filenames_from_directory(const std::string& path) {

	std::vector < std::string> v;

	if(!std::filesystem::is_directory(path)) {
		std::cerr << "Directory path not found\n";
		return v;
	}

	for(const auto& file : std::filesystem::directory_iterator(path)) {
		if(std::filesystem::is_regular_file(file.path())) {
			v.push_back(file.path().string());
		}
	}
	return v;
}



void FileParser::filter_files_by_extension(std::vector<std::string>& filenames,
	const std::vector<std::string>& extensions) {

	bool approval = false;

	if(!extensions.empty()) {
		if(allowExtensions) {
			for(int i = 0; i < filenames.size(); i++) {
				approval = false;
				for(int j = 0; j < extensions.size(); j++) {
					if(std::filesystem::path(filenames[i]).extension().string() == extensions[j]) {
						approval = true;
						break;
					}
				}
				if(!approval) {
					filenames.erase(filenames.begin() + i);
					i--;
				}
			}

		} else {
			for(int i = 0; i < filenames.size(); i++) {
				approval = false;
				for(int j = 0; j < extensions.size(); j++) {
					if(std::filesystem::path(filenames[i]).extension().string() != extensions[j]) {
						approval = true;
						break;
					}
				}
				if(!approval) {
					filenames.erase(filenames.begin() + i);
					i--;
				}
			}
		}

	}
}



sdindex::FileParser::FileParser() {}



bool FileParser::file_exists(const std::string& filename) {

	std::ifstream ifs;

	ifs.open(filename);
	if(ifs.is_open()) {
		ifs.close();
		return true;
	}
	return false;
}



bool FileParser::load_index_file(const std::string& filename, IndexHashtable& index) {

	std::ifstream ifs;
	std::string line;
	int state = 0;
	std::string id, word, occurences;


	ifs.open(filename, std::ios::in);

	if(ifs.is_open()) {
		while(std::getline(ifs, line)) {
			switch(state) {
			case 0:
				if(line != "-1") {
					id.clear();
					id = line;
				}
				state++;
				break;
			case 1:
				word.clear();
				word = line;
				state++;
				break;
			case 2:
				occurences.clear();
				occurences = line;
				if(!index.load_entry(id, word, occurences))return false;
				state++;
				break;
			case 3:
				state = 0;
				break;
			}
		}

	} else {
		std::cerr << "Specified index file \"" << filename << "\" not found" << std::endl;
		return false;
	}

	ifs.close();
	return true;
}
