#pragma once
#include <string>

namespace SDIndexer {

	//Node in the data structure representing how many times a word occured and at which file
	typedef struct occurenceNode {
		std::string filename;
		int occurences = 0;
		struct occurenceNode* next = NULL;
	}occurenceNode;


	//Entry class for the hashtable data structure
	class indexerHashtableEntry {
		public:


		indexerHashtableEntry( std::string word );

		indexerHashtableEntry* getNext();
		std::string getValue();
		occurenceNode* getNextOccurence();

		bool setNext( indexerHashtableEntry* entry );
		bool setValue( std::string value );
		bool addOccurence( occurenceNode* occurence );
		void loadOccurences( std::string occurences );

		std::string toString();
		

		private:

		indexerHashtableEntry* next;
		std::string value;
		occurenceNode* occurences;
	};


	//The hashtable data structure
	class indexerHashtable {
		public:

		indexerHashtable( int size = 100000 );

		indexerHashtableEntry** hashtable = NULL;

		int size = 0;

		bool addToIndex( std::string word, std::string filename );

		bool loadEntry( std::string id, std::string word, std::string occurences );

		std::string toString();

		private:

		unsigned int hashFunction( std::string word );
	};

}
