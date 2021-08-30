#pragma once
#include "indexhashtableentry.h"

#include <string>


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


		// Checks whether the specified word exists in the index
		bool entry_exists(const std::string& word);


		// Provides a copy of the index entry matching the word input
		IndexHashtableEntry* get_entry_copy(const std::string& word);


		// Returns a pointer reference to the entry matching the word input
		IndexHashtableEntry* get_entry(const std::string& word);


		// Loads an entry into the index
		bool load_entry(const std::string& id, const std::string& word, const std::string& occurences);


		// Clears the index removing all entries
		void clear();


		// Takes all the occurences from the otherIndex (slow impl)
		void steal_occurences(IndexHashtable* otherIndex);


		// Merges index with otherIndex (delete otherIndex after using merge)
		bool merge(IndexHashtable* otherIndex);


		// Provides a string containing all the data contained in the index
		std::string to_string();



		protected:

		int size;

		// The hashfunction used to match words to hashvalues
		unsigned int hashfunction(const std::string& word);
	};

}
