#include "querymanager.h"

using namespace sdindexer;

QueryManager::QueryManager( IndexHashtable* index ) {}

std::string QueryManager::query_index( std::string query ) {
	return std::string();
}

std::vector<RankedDocument> QueryManager::get_ranked_documents() {
	return std::vector<RankedDocument>();
}

void QueryManager::durect_match( std::string query ) {}
