#pragma once
#include <string>

namespace sdindexer {

	//Node in the data structure representing how many times a word occured and at which file
	typedef struct OccurenceNode {
		std::string filename;
		int occurences = 0;
		struct OccurenceNode* next = NULL;
	}OccurenceNode;


	//Entry class for the hashtable data structure
	class IndexHashtableEntry {
		public:


		IndexHashtableEntry( std::string word );

		IndexHashtableEntry* get_next();
		std::string get_value();
		OccurenceNode* get_next_occurence();

		bool set_next( IndexHashtableEntry* entry );
		bool set_value( std::string value );
		bool add_occurence( OccurenceNode* occurence );
		void load_occurences( std::string occurences );

		std::string to_string();


		private:

		IndexHashtableEntry* next;
		std::string value;
		OccurenceNode* occurences;
	};


	//The hashtable data structure
	class IndexHashtable {
		public:

		IndexHashtable( int size = 100000 );

		IndexHashtableEntry** hashtable = NULL;

		int size = 0;

		bool add_to_index( std::string word, std::string filename );

		bool load_entry( std::string id, std::string word, std::string occurences );

		std::string to_string();

		private:

		unsigned int hashfunction( std::string word );
	};

}
