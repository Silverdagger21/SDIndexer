#pragma once
#include "indexerHashtable.h"
#include <vector>
#include <string>

namespace SDIndexer {

	class fileParser {

		public:

		static bool toLowercase;
		static bool ommitSpecialCharacters;
		static bool ommitNumbers;


		static indexerHashtable* parseFile( std::string filename, indexerHashtable* hashtable );


		static bool parseLine( std::string line, indexerHashtable* hashtable, std::string filename );


		static std::string  parseWord( std::string word );


		static std::string removeSpecialCharacters( std::string word );


		static std::string removeNumbers( std::string word );


		static void writeIndexFileToDrive( std::string filename, indexerHashtable* index );


		//Returns a vector containing the paths/filenames of all files in the current and sub directories
		static std::vector<std::string> getFilenamesFromDirectories();


		static std::vector<std::string> filterFiles( std::vector<std::string> filenames, std::vector<std::string> extensions );


		//	Checks whether a file exists in the current directory
		static bool fileExists( std::string );


		static bool loadIndexFile( std::string filename, indexerHashtable* hashtable );
	};

}
