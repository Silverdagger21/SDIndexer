#include "fileParser.h"
#include <fstream>
#include <iostream>
#include <filesystem>


using namespace SDIndexer;


bool fileParser::toLowercase = false;
bool fileParser::ommitSpecialCharacters = false;
bool fileParser::ommitNumbers = false;


indexerHashtable* fileParser::parseFile( std::string filename, indexerHashtable* hashtable ) {

	std::ifstream infile;
	std::string line;

	if ( hashtable == NULL ) {
		return NULL;
	}

	infile.open( filename, std::ios::in );

	if ( infile.is_open() ) {
		while ( std::getline( infile, line ) ) {
			if ( !parseLine( line, hashtable, filename ) ) {
				infile.close();
				return NULL;
			}
		}
	} else {
		std::cerr << "Specified file " << filename << " not found" << std::endl;
		return NULL;
	}

	infile.close();

	return hashtable;
}


bool fileParser::parseLine( std::string line, indexerHashtable* hashtable, std::string filename ) {

	std::string word;
	bool status = false;
	char c;
	int i;

	i = 0;
	word.clear();
	while ( i < line.size() ) {
		c = line[i];
		if ( c != ' ' ) {
			word.push_back( c );

		} else {
			word = parseWord( word );
			if ( !word.empty() )
				status = hashtable->addToIndex( word, filename );
			if ( !status ) std::cerr << "Problem inputing word into the hashtable" << std::endl;
			word.clear();
		}
		i++;
	}
	if ( word.length() > 0 ) {
		word = parseWord( word );
		if ( !word.empty() )
			status = hashtable->addToIndex( word, filename );
		if ( !status ) std::cerr << "Problem inputing word into the hashtable" << std::endl;
		word.clear();
	}
	return true;
}


std::string fileParser::parseWord( std::string word ) {
	if ( toLowercase ) {
		for ( int i = 0; i < word.size(); i++ ) {
			word[i] = tolower( word[i] );
		}
	}
	if ( ommitSpecialCharacters ) {
		word = removeSpecialCharacters( word );
	}
	if ( ommitNumbers ) {
		word = removeNumbers( word );
	}
	return word;
}


std::string fileParser::removeSpecialCharacters( std::string word ) {
	for ( int i = 0; i < word.size(); i++ ) {
		if ( (word[i] < 'A' || word[i] > 'Z') && (word[i] < 'a' || word[i] > 'z') && (word[i] < '0' || word[i] > '9') ) {
			word.erase( i, 1 );
			i--;
		}
	}
	return word;
}


std::string fileParser::removeNumbers( std::string word ) {
	for ( int i = 0; i < word.size(); i++ ) {
		if ( word[i] >= '0' && word[i] <= '9' ) {
			word.erase( i, 1 );
			i--;
		}
	}
	return word;
}


void fileParser::writeIndexFileToDrive( std::string filename, indexerHashtable* index ) {

	std::ofstream outfile;
	std::string line;

	outfile.open( filename, std::ios::out );
	outfile << index->toString();
	outfile.close();
}


std::vector<std::string> fileParser::getFilenamesFromDirectories() {

	std::vector < std::string> v;
	std::string path = ".";

	for ( const auto& file : std::filesystem::recursive_directory_iterator( path ) ) {
		if ( std::filesystem::is_regular_file( file.path() ) ) {
			v.push_back( file.path().string() );
		}
	}
	return v;
}


std::vector<std::string> fileParser::filterFiles( std::vector<std::string> filenames, std::vector<std::string> extensions ) {

	bool approval = false;

	if ( !extensions.empty() ) {
		for ( int i = 0; i < filenames.size(); i++ ) {
			approval = false;
			for ( int j = 0; j < extensions.size(); j++ ) {
				if ( std::filesystem::path( filenames[i] ).extension().string() == extensions[j] ) {
					approval = true;
					break;
				}
			}
			if ( !approval ) {
				filenames.erase( filenames.begin() + i );
				i--;
			}
		}
	}
	return filenames;
}


bool fileParser::fileExists( std::string filename ) {

	std::ifstream ifs;

	ifs.open( filename );
	if ( ifs.is_open() ) {
		ifs.close();
		return true;
	}
	return false;
}


//	@TODO finish it
bool fileParser::loadIndexFile( std::string filename, indexerHashtable* hashtable ) {

	std::ifstream ifs;
	std::string line;
	int state = 0;
	std::string id, word, occurences;
	int i;

	if ( hashtable == NULL ) return false;

	ifs.open( filename, std::ios::in );

	if ( ifs.is_open() ) {
		while ( std::getline( ifs, line ) ) {
			switch ( state ) {
				case 0:
					if ( line != "-1" ) {
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
					if ( !hashtable->loadEntry( id, word, occurences ) )return false;
					state++;
					break;
				case 3:
					state=0;
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



