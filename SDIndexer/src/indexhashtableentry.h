#pragma once
#include <string>


namespace sdindexer {

	//Node in the data structure representing how many times a word occured and at which file
	typedef struct OccurenceNode {
		std::string filename;
		int occurences = 0;
		struct OccurenceNode* next = nullptr;
	}OccurenceNode;


	//Entries are stored in IndexHashTable
	class IndexHashtableEntry {

		friend class IndexHashtable;

		public:

		IndexHashtableEntry( std::string word );
		IndexHashtableEntry( std::string* word );
		~IndexHashtableEntry();

		IndexHashtableEntry* get_next();
		std::string get_value();
		OccurenceNode* get_occurances();


		bool add_occurence( OccurenceNode* occurence );
		void load_occurences( std::string* occurences );

		std::string to_string();


		protected:

		IndexHashtableEntry* next;
		std::string value;
		OccurenceNode* occurences;


		bool set_next( IndexHashtableEntry* entry );
		bool set_value( std::string* value );
		IndexHashtableEntry* clone_entry();
	};
}
