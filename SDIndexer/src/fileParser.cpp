#include "fileparser.h"

#include <fstream>
#include <iostream>
#include <filesystem>


using namespace sdindexer;


bool FileParser::toLowercase = false;
bool FileParser::ommitSpecialCharacters = false;
bool FileParser::ommitNumbers = false;


IndexHashtable* FileParser::parse_file( std::string* filename, IndexHashtable* hashtable ) {

	std::ifstream infile;
	std::string line;

	if(hashtable == nullptr) {
		return nullptr;
	}

	infile.open( *filename, std::ios::in );

	if(infile.is_open()) {
		while(std::getline( infile, line )) {
			if(!parse_line( &line, hashtable, filename )) {
				infile.close();
				return nullptr;
			}
		}
	} else {
		std::cerr << "Specified file " << filename << " not found" << std::endl;
		return nullptr;
	}

	infile.close();

	return hashtable;
}


bool FileParser::parse_line( std::string* line, IndexHashtable* hashtable, std::string* filename ) {

	std::string word;
	bool status = false;
	char c;
	int i;

	i = 0;
	word.clear();
	while(i < line->size()) {
		c = ( *line )[i];
		if(c != ' ') {
			word.push_back( c );

		} else {
			parse_word( &word );
			if(!word.empty())
				status = hashtable->add_to_index( &word, filename );
			if(!status) std::cerr << "Problem inputing word into the hashtable" << std::endl;
			word.clear();
		}
		i++;
	}
	if(word.length() > 0) {
		parse_word( &word );
		if(!word.empty())
			status = hashtable->add_to_index( &word, filename );
		if(!status) std::cerr << "Problem inputing word into the hashtable" << std::endl;
		word.clear();
	}
	return true;
}


void FileParser::parse_word( std::string* word ) {
	if(toLowercase) {
		for(int i = 0; i < word->size(); i++) {
			( *word )[i] = tolower( ( *word )[i] );
		}
	}
	if(ommitSpecialCharacters) {
		remove_special_characters( word );
	}
	if(ommitNumbers) {
		remove_numbers( word );
	}
}


void FileParser::remove_special_characters( std::string* word ) {
	for(int i = 0; i < word->size(); i++) {
		if(( ( *word )[i] < 'A' || ( *word )[i] > 'Z' ) && ( ( *word )[i] < 'a' || ( *word )[i] > 'z' ) && ( ( *word )[i] < '0' || ( *word )[i] > '9' )) {
			word->erase( i, 1 );
			i--;
		}
	}
}


void FileParser::remove_numbers( std::string* word ) {
	for(int i = 0; i < word->size(); i++) {
		if(( *word )[i] >= '0' && ( *word )[i] <= '9') {
			word->erase( i, 1 );
			i--;
		}
	}
}


void FileParser::write_index_file_to_drive( std::string* filename, IndexHashtable* index ) {

	std::ofstream outfile;
	std::string line;

	outfile.open( *filename, std::ios::out );
	outfile << index->to_string();
	outfile.close();
}


std::vector<std::string> FileParser::get_filenames_from_directories() {

	std::vector < std::string> v;
	std::string path = ".";

	for(const auto& file : std::filesystem::recursive_directory_iterator( path )) {
		if(std::filesystem::is_regular_file( file.path() )) {
			v.push_back( file.path().string() );
		}
	}
	return v;
}


void FileParser::filter_files( std::vector<std::string>* filenames, std::vector<std::string>* extensions ) {

	bool approval = false;

	if(!extensions->empty()) {
		for(int i = 0; i < filenames->size(); i++) {
			approval = false;
			for(int j = 0; j < extensions->size(); j++) {
				if(std::filesystem::path( ( *filenames )[i] ).extension().string() == ( *extensions )[j]) {
					approval = true;
					break;
				}
			}
			if(!approval) {
				filenames->erase( filenames->begin() + i );
				i--;
			}
		}
	}
}


bool FileParser::file_exists( std::string* filename ) {

	std::ifstream ifs;

	ifs.open( *filename );
	if(ifs.is_open()) {
		ifs.close();
		return true;
	}
	return false;
}



bool FileParser::load_index_file( std::string* filename, IndexHashtable* hashtable ) {

	std::ifstream ifs;
	std::string line;
	int state = 0;
	std::string id, word, occurences;

	if(hashtable == nullptr) return false;

	ifs.open( *filename, std::ios::in );

	if(ifs.is_open()) {
		while(std::getline( ifs, line )) {
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
				if(!hashtable->load_entry( &id, &word, &occurences ))return false;
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



