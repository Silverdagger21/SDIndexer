#pragma once
#include "indexhashtableentry.h"
#include <string>


namespace sdindexer {

	//Hashtable containing IndexHashtableEntries
	class IndexHashtable {
		public:

		IndexHashtable( int size = 100000 );
		~IndexHashtable();


		IndexHashtableEntry** hashtable = nullptr;

		int size;


		bool add_to_index( std::string* word, std::string* filename );

		bool entry_exists( std::string* word );

		IndexHashtableEntry* get_entry_copy( std::string* word );

		IndexHashtableEntry* get_entry( std::string* word );

		bool load_entry( std::string* id, std::string* word, std::string* occurences );

		std::string to_string();


		protected:



		unsigned int hashfunction( std::string* word );
	};

}
