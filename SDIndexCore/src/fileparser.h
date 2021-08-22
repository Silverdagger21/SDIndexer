#pragma once
#include "indexhashtable.h"

#include <vector>
#include <string>

namespace sdindexer {

	class FileParser {

		public:

		static bool toLowercase;
		static bool ommitSpecialCharacters;
		static bool ommitNumbers;


		static IndexHashtable* parse_file( std::string* filename, IndexHashtable* hashtable );


		static bool parse_line( std::string* line, IndexHashtable* hashtable, std::string* filename );


		static void  parse_word( std::string* word );


		static void convert_to_lowercase( std::string* word );


		static void remove_special_characters( std::string* word );


		static void remove_numbers( std::string* word );


		static void write_index_file_to_drive( std::string* filename, IndexHashtable* index );


		//Returns a vector containing the paths/filenames of all files in the current and sub directories
		static std::vector<std::string> get_filenames_from_directories( std::string* path);


		static void filter_files_by_extension( std::vector<std::string>* filenames, std::vector<std::string>* extensions );


		//	Checks whether a file exists in the current directory
		static bool file_exists( std::string* filename );



		static bool load_index_file( std::string* filename, IndexHashtable* hashtable );
	};

}
