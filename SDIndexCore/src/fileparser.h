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




		// Goes through all the files in the specified directory and sub directories and indexes the contents
		static bool index_directory_and_subdirectories(const std::string& dirpath, IndexHashtable& index, const std::vector<std::string>& extensions);


		// Goes through all the files in the specified directory and indexes the contents
		static bool index_directory(const std::string& dirpath, IndexHashtable& index, const std::vector<std::string>& extensions);


		// Parses the file specified in "filename" and populates the index
		static bool parse_file(const std::string& filename, IndexHashtable& index);


		/// Writes the contents of the hashtable to an index file withthe specifiec "filename"
		static void write_index_file_to_drive(const std::string& filename, IndexHashtable& index);


		// Returns a vector containing the paths of all files in the current and sub directories
		static std::vector<std::string> get_filenames_from_directories(const std::string& path);


		// Returns a vector containing the paths of all files in the current directory
		static std::vector<std::string> get_filenames_from_directory(const std::string& path);


		// Converts the specifiec string into only lowercase characters
		static void convert_to_lowercase(std::string& word);


		// Removes all symbols from the specified string
		static void remove_symbols(std::string& word);


		// Removes all numbers from the specified string
		static void remove_numbers(std::string& word);


		// Loads a file containing index data to the index hashtable specified
		static bool load_index_file(const std::string& filename, IndexHashtable& index);


		// Filters the vector containing strings of filenames allowing only the ones 
		//specified in the extensions vector
		static void filter_files_by_extension(std::vector<std::string>& filenames,
			const std::vector<std::string>& extensions);


		// Parses a word doing operations based on the options provided
		static void parse_word(std::string& word, bool toLowercase, bool  ommitNumbers, bool ommitSpecialCharacters);


		// Splits a string into a vector of strings based on the specified character
		static std::vector<std::string> split_string(const std::string& input, const char& c);


		//
		static void index_files(const std::vector<std::string*>& filenames, IndexHashtable& index);


		protected:

		FileParser();


		// Parses a line checking the data and populating the index
		static bool parse_line(const std::string& line, IndexHashtable& index, const int filenameIndex);
	};

}
