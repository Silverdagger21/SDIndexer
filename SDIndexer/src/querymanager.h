#pragma once
#include "indexhashtable.h"

#include <string>
#include <vector>

namespace sdindexer {

	typedef struct RankedDocument {
		std::string filename;
		int score;
	}RankedDocument;


	class QueryManager {

		public:

		QueryManager( IndexHashtable* index );

		std::string query_index( std::string query );

		std::vector<RankedDocument> get_ranked_documents();


		private:

		std::vector<RankedDocument> rankedDocuments;

		void durect_match( std::string query );

	};

}
