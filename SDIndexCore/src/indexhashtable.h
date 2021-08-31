#pragma once
#include "indexhashtableentry.h"

#include <string>
#include <vector>


namespace sdindex {

	// The Index represented as a hashtable
	class IndexHashtable {

		public:


		IndexHashtableEntry** hashtable = nullptr;
		

		IndexHashtable(int size = 100000);
		~IndexHashtable();

		int get_size();

		// Adds the specified string to the index
		bool add_to_index(const std::string& word, const std::string& filename);


		// Adds the specified string to the index. <faster than the previous one>
		bool add_to_index(const std::string& word, int filenameIndex);


		// Checks whether the specified word exists in the index
		bool entry_exists(const std::string& word);


		// Returns the index of filename always >=0
		int get_filename_index(const std::string& filename);


		// Returns the filename of the index
		std::string get_filename(int index);


		// Provides a copy of the index entry matching the word input
		IndexHashtableEntry* get_entry_copy(const std::string& word);


		// Returns a pointer reference to the entry matching the word input
		IndexHashtableEntry* get_entry(const std::string& word);


		// Loads an entry into the index
		bool load_entry(const std::string& id, const std::string& word, const std::string& occurences);


		// Loads a filename into the filenames vector
		void load_filename_entry(const std::string& filename);


		// Clears the index removing all entries
		void clear();


		// Merges index with otherIndex (delete otherIndex after using merge,
		//it is assumed that both indexes contain different files)
		bool merge(IndexHashtable* otherIndex);


		// Provides a string containing all the data contained in the index
		std::string to_string();



		protected:

		int size;


		// Contains the names of all files
		std::vector<std::string> filenames;


		// The hashfunction used to match words to hashvalues
		unsigned int hashfunction(const std::string& word);
	};

}
