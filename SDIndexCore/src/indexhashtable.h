#pragma once
#include "indexhashtableentry.h"

#include <string>


namespace sdindexer {

	// The Index represented as a hashtable
	class IndexHashtable {

		public:


		IndexHashtableEntry** hashtable = nullptr;
		int size;

		IndexHashtable( int size = 100000 );
		~IndexHashtable();


		// Adds the specified string to the index
		bool add_to_index( std::string* word, std::string* filename );


		// Checks whether the specified word exists in the index
		bool entry_exists( std::string* word );


		// Provides a copy of the index entry matching the word input
		IndexHashtableEntry* get_entry_copy( std::string* word );


		// Returns a pointer reference to the entry matching the word input
		IndexHashtableEntry* get_entry( std::string* word );


		// Loads an entry into the index
		bool load_entry( std::string* id, std::string* word, std::string* occurences );


		// Clears the index removing all entries
		void clear();


		// Provides a string containing all the data contained in the index
		std::string to_string();



		protected:


		// The hashfunction used to match words to hashvalues
		unsigned int hashfunction( std::string* word );
	};

}
