#pragma once
#include "indexhashtable.h"

#include <vector>
#include <string>


namespace sdindex {

	// A static class representing a collection of functions required to parse files
	class FileParser {

		public:


		// Options

		static bool toLowercase;
		static bool ommitSpecialCharacters;
		static bool ommitNumbers;
		static bool allowExtensions;


		static bool index_directory(std::string& dirpath, IndexHashtable& index);


		static bool index_directory(std::string& dirpath, IndexHashtable& index, std::vector<std::string>& extensions);
		
		// Parses the file specified in "filename" and populates the index
		static IndexHashtable* parse_file(std::string* filename, IndexHashtable* index);


		/// Writes the contents of the hashtable to an index file withthe specifiec "filename"
		static void write_index_file_to_drive(std::string* filename, IndexHashtable* index);


		// Returns a vector containing the paths of all files in the current and sub directories
		static std::vector<std::string> get_filenames_from_directories(std::string* path);


		// Converts the specifiec string into only lowercase characters
		static void convert_to_lowercase(std::string* word);


		// Removes all symbols from the specified string
		static void remove_symbols(std::string* word);


		// Removes all numbers from the specified string
		static void remove_numbers(std::string* word);


		// Checks whether a file exists
		static bool file_exists(std::string* filename);


		// Loads a file containing index data to the index hashtable specified
		static bool load_index_file(std::string* filename, IndexHashtable* index);


		// Filters the vector containing strings of filenames allowing only the ones 
		//specified in the extensions vector
		static void filter_files_by_extension(std::vector<std::string>* filenames,
			std::vector<std::string>* extensions);



		// Parses a word doing operations based on the options provided
		static void parse_word(std::string* word, bool toLowercase, bool  ommitNumbers, bool ommitSpecialCharacters);



		protected:

		FileParser();

		// Parses a line checking the data and populating the index
		static bool parse_line(std::string* line, IndexHashtable* index, std::string* filename);
	};

}
