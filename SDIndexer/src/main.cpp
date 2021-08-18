#include "indexhashtable.h"
#include "fileparser.h"

#include <iostream>
#include <vector>
#include <string>


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


/*
	Pipeline:
		1. state machien asking for options
		2. creating an index || Quering an index for data
		3. return the data to the user

	Possibly implement a window interface
*/

//	Set the mode of operation based on command line arguments
void setMode( int argc, char* argv[] );




int main( int argc, char* argv[] ) {

	IndexHashtable index( hashtableSize );
	std::vector<std::string> filenames;

	FileParser::toLowercase = true;
	FileParser::ommitSpecialCharacters = true;
	FileParser::ommitNumbers = true;


	///*
	setMode( argc, argv );

	if ( mode == application_mode::default_index ) {
		if ( !FileParser::file_exists( indexFileName ) ) {
			filenames = FileParser::get_filenames_from_directories();
			filenames = FileParser::filter_files( filenames, approvedExtensions );

			for ( int i = 0; i < filenames.size(); i++ ) {
				if ( !FileParser::parse_file( filenames[i], &index ) ) {
					std::cerr << "Failed to open file \"" << filenames[i] << "\"" << std::endl;
				}
			}
			FileParser::write_index_file_to_drive( indexFileName, &index );

		} else {
			if ( !FileParser::load_index_file( indexFileName, &index ) ) {
				std::cerr << "Unable to load index file with the name \"" + indexFileName + "\"\n";
				return 1;
			}
			//	Call Queries
		}

	} else if ( mode == application_mode::create_index ) {
		filenames = FileParser::get_filenames_from_directories();
		filenames = FileParser::filter_files( filenames, approvedExtensions );

		for ( int i = 0; i < filenames.size(); i++ ) {
			if ( !FileParser::parse_file( filenames[i], &index ) ) {
				std::cerr << "Failed to open file \"" << filenames[i] << "\"" << std::endl;
			}
		}
		FileParser::write_index_file_to_drive( indexFileName, &index );

	} else if ( mode == application_mode::query_index ) {
		if ( FileParser::load_index_file( indexFileName, &index ) ) {
			//	Call Queries
		}

	} else {
		std::cerr << "No valid mods passed\n";
		return 1;
	}
	//*/
	return 0;
}


void setMode( int argc, char* argv[] ) {
	std::string sarg;

	if ( argc == 1 ) {
		mode = application_mode::default_index;
	} else {
		sarg = argv[1];
		if ( sarg == "-c" ) {
			mode = application_mode::create_index;
		} else if ( sarg == "-q" ) {
			mode = application_mode::query_index;
		}
	}
}
