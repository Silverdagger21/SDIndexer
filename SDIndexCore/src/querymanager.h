#pragma once
#include "indexhashtable.h"

#include <string>
#include <vector>
#include <set>


namespace sdindex {


	// Struct representing documents paired with a score and appearance count
	typedef struct RankedDocument {
		std::string filename;
		double score = 0;
		int count = 0;
	}RankedDocument;


	// Contains all the required methods to extract relevant data from the index
	class QueryManager {

		public:

		QueryManager(IndexHashtable* index);
		QueryManager(IndexHashtable* index, int limit);
		bool convertToLowercase = false;
		bool ommitNumbers = false;
		bool ommitSpecialCharacters = false;


		// Queries the index matching relevant words with the "query"
		std::vector<RankedDocument> query_index(std::string* query);


		// Queries the index matching relevant words with the "query"
		std::vector<RankedDocument> query_index(std::string& query);


		// Queries the index matching relevant words with the "query"
		std::string query_index_to_string(std::string* query);


		// Queries the index matching relevant words with the "query"
		std::string query_index_to_string(std::string& query);


		// Returns the vector containing the results of the most recent query
		std::vector<RankedDocument> get_ranked_documents();


		// Checks whether a string is considered a common word
		bool is_common_word(std::string word);



		private:


		std::vector<RankedDocument> rankedDocuments;

		IndexHashtable* index;

		int results_limit = 50;

		std::set <std::string> commonWords = {
			"i", "you", "he", "she", "it", "we", "they", "is", "are", "at", "in", "on", "the",
			"be", "to", "of", "and", "a", "that", "have", "for", "not", "with", "as", "do",
			"this", "but", "his", "her", "by", "from", "or", "there", "so", "up", "out", "if",
			"about", "who", "get", "which", "go", "me", "when", "can", "its", "us"
		};


		// Merges the occurences of all entries into ranked documents
		void merge_occurences_to_ranked_documents(std::vector<IndexHashtableEntry*>* entries);


		// Converts ranked documents into a string representation of their data
		std::string convert_result_to_string(int limit);


		// Splits a string into a vector of strings based on the specified character
		std::vector<std::string> split_string(std::string* query, char c);


		// Algorithm used to match the query to the index data
		void direct_match(std::string* query);

	};

}
