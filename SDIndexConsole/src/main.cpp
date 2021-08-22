#include "indexhashtable.h"
#include "fileparser.h"
#include "querymanager.h"

#include <iostream>
#include <vector>
#include <string>



// @TODO Write querymanager implementation

using namespace sdindexer;


// Apllication mode
enum class application_mode {
	default_index, create_index, query_index
};


// Options

int hashtableSize = 100000;
application_mode mode;
std::string indexFileName = "index.index";
std::vector<std::string> approvedExtensions = { ".txt", ".doc" };


//	Set the mode of operation based on command line arguments
void setMode( int argc, char* argv[] );

void user_input( std::string* query );

void interactive_loop( IndexHashtable* index, QueryManager* queryManager, std::string* query );



int main( int argc, char* argv[] ) {

	IndexHashtable index( hashtableSize );
	std::vector<std::string> filenames;
	QueryManager queryManager( &index );
	std::string query;
	std::string dirpath;


	FileParser::toLowercase = true;
	FileParser::ommitSpecialCharacters = true;
	FileParser::ommitNumbers = true;

	dirpath = ".\\..\\testdata\\";
	

	setMode( argc, argv );
	
	if(mode == application_mode::default_index) {
		if(!FileParser::file_exists( &indexFileName )) {
			filenames = FileParser::get_filenames_from_directories(&dirpath);
			FileParser::filter_files_by_extension( &filenames, &approvedExtensions );

			for(int i = 0; i < filenames.size(); i++) {
				if(!FileParser::parse_file( &filenames[i], &index )) {
					std::cerr << "Failed to open file \"" << filenames[i] << "\"" << std::endl;
				}
			}
			FileParser::write_index_file_to_drive( &indexFileName, &index );

		} else {
			if(!FileParser::load_index_file( &indexFileName, &index )) {
				std::cerr << "Unable to load index file with the name \"" + indexFileName + "\"\n";
				return 1;
			}
		}

		interactive_loop( &index, &queryManager, &query );

	} else if(mode == application_mode::create_index) {
		filenames = FileParser::get_filenames_from_directories(&dirpath);
		FileParser::filter_files_by_extension( &filenames, &approvedExtensions );

		for(int i = 0; i < filenames.size(); i++) {
			if(!FileParser::parse_file( &filenames[i], &index )) {
				std::cerr << "Failed to open file \"" << filenames[i] << "\"" << std::endl;
			}
		}
		FileParser::write_index_file_to_drive( &indexFileName, &index );

	} else if(mode == application_mode::query_index) {
		if(FileParser::load_index_file( &indexFileName, &index )) {
			interactive_loop( &index, &queryManager, &query );
		}

	} else {
		std::cerr << "No valid mods passed\n";
		return 1;
	}
	

	return 0;
}




void setMode( int argc, char* argv[] ) {
	std::string sarg;

	if(argc == 1) {
		mode = application_mode::default_index;
	} else {
		sarg = argv[1];
		if(sarg == "-c") {
			mode = application_mode::create_index;
		} else if(sarg == "-q") {
			mode = application_mode::query_index;
		}
	}
}


void user_input( std::string* query ) {
	query->clear();
	std::cout << "Type your query: ";
	std::getline( std::cin, *query );
}


void interactive_loop( IndexHashtable* index, QueryManager* queryManager, std::string* query ) {
	std::cout << "Type :: to exit" << std::endl;
	do {
		user_input( query );

		if(*query != "::") {
			std::string outputs = queryManager->query_index_to_string( query );
			std::cout << outputs << std::endl;
		}
	} while(*query != "::");
}
