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
		static const std::set <std::string> commonWords;



		// Queries the index matching relevant words with the "query"
		std::vector<RankedDocument> query_index(const std::string& query);


		// Queries the index matching relevant words with the "query"
		std::string query_index_to_string(const std::string& query);


		// Returns the vector containing the results of the most recent query
		std::vector<RankedDocument> get_ranked_documents();


		// Checks whether a string is considered a common word
		static bool is_common_word(const std::string& word);


		// Splits a string into a vector of strings based on the specified character and options
		std::vector<std::string> format_query(const std::string & query, char c);




		private:


		std::vector<RankedDocument> rankedDocuments;

		IndexHashtable* index;

		int results_limit = 50;


		// Merges the occurences of all entries into ranked documents
		void merge_occurences_to_ranked_documents(std::vector<IndexHashtableEntry*>* entries);


		// Converts ranked documents into a string representation of their data
		std::string convert_result_to_string(int limit);


		// Algorithm used to match the query to the index data
		void direct_match(const std::string& query);

	};

}
