#pragma once
#include <string>


namespace sdindex {

	//A representation of how many times a word occured and at which file
	typedef struct OccurenceNode {
		std::string filename;
		int occurences = 0;
		struct OccurenceNode* next = nullptr;
	}OccurenceNode;


	//Entries that are stored in IndexHashTable
	class IndexHashtableEntry {

		friend class IndexHashtable;

		public:


		IndexHashtableEntry(std::string word);
		IndexHashtableEntry(std::string* word);
		~IndexHashtableEntry();

		// Returns the next entry in the list
		IndexHashtableEntry* get_next();


		// Returns a copy of the current value
		std::string get_value();


		// Returns a pointer reference to the occurences list
		OccurenceNode* get_occurances();


		// Adds an occurence node to the list
		bool add_occurence(OccurenceNode* occurence);


		// Loads the occurences stored in the string
		void load_occurences(std::string* occurences);


		// Provides a string representation of all the data contained in the entry
		std::string to_string();


		protected:


		// Pointer to the next item in the list
		IndexHashtableEntry* next;

		// Value of the entry
		std::string value;

		// Pointer to the occurence list
		OccurenceNode* occurences;


		// Sets the next entry in the list
		bool set_next(IndexHashtableEntry* entry);


		// Sets the value of the entry
		bool set_value(std::string* value);

		// Provides a copy of the entry and all its data
		IndexHashtableEntry* clone_entry();
	};
}
