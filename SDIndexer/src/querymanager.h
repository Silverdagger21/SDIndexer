#pragma once
#include "indexhashtable.h"

#include <string>
#include <vector>

namespace sdindexer {

	typedef struct RankedDocument {
		std::string filename;
		int score;
		int count;
	}RankedDocument;


	class QueryManager {

		public:

		QueryManager( IndexHashtable* index );

		std::string query_index( std::string query, int limit = 500 );

		std::vector<RankedDocument> get_ranked_documents();



		private:

		std::vector<RankedDocument> rankedDocuments;

		void merge_occurences_to_ranked_documents( std::vector<IndexHashtableEntry>* entries );

		IndexHashtable* index;

		std::string convert_result_to_string( int limit );


		std::vector<std::string> split_string( std::string* query, char c );


		void direct_match( std::string* query );

	};

}
