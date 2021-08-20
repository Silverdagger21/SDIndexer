#pragma once
#include "indexhashtable.h"

#include <string>
#include <vector>
#include <set>

namespace sdindexer {

	typedef struct RankedDocument {
		std::string filename;
		double score = 0;
		int count = 0;
	}RankedDocument;


	class QueryManager {

		public:

		QueryManager( IndexHashtable* index );
		QueryManager( IndexHashtable* index, int limit );

		std::string query_index( std::string* query );

		std::vector<RankedDocument> get_ranked_documents();

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

		void merge_occurences_to_ranked_documents( std::vector<IndexHashtableEntry*>* entries );


		std::string convert_result_to_string( int limit );


		std::vector<std::string> split_string( std::string* query, char c );


		void direct_match( std::string* query );

	};

}
