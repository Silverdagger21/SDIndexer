#include "indexerHashtable.h"
#include "fileParser.h"
#include <iostream>
#include <vector>
#include <string>


using namespace SDIndexer;


// Apllication mode
enum class application_mode {
	default_index, create_index, create_write_index, query_index
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

	indexerHashtable index( hashtableSize );
	std::vector<std::string> filenames;

	fileParser::toLowercase = true;
	fileParser::ommitSpecialCharacters = true;
	fileParser::ommitNumbers = true;

	

	///*
	setMode( argc, argv );

	if ( mode == application_mode::default_index ) {
		if ( !fileParser::fileExists( indexFileName ) ) {
			filenames = fileParser::getFilenamesFromDirectories();
			filenames = fileParser::filterFiles( filenames, approvedExtensions );

			for ( int i = 0; i < filenames.size(); i++ ) {
				if ( !fileParser::parseFile( filenames[i], &index ) ) {
					std::cerr << "Failed to open file \"" << filenames[i] << "\"" << std::endl;
				}
			}
			fileParser::writeIndexFileToDrive( indexFileName, &index );
		}

		//	@TODO Add Index Query Code here

	}
	//*/

	indexerHashtable index2( hashtableSize );
	///*
	if ( fileParser::loadIndexFile( indexFileName, &index2 ) ) {
		std::cout << "Success\n";
		fileParser::writeIndexFileToDrive( "iteration2.index", &index2 );
	}
	//*/


	

	//	@TODO Implement other modes

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
		} else if ( sarg == "-cw" ) {
			mode = application_mode::create_write_index;
		} else if ( sarg == "-q" ) {
			mode = application_mode::query_index;
		}
	}
}
