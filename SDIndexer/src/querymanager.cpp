#include "querymanager.h"

#include <algorithm>

using namespace sdindexer;


QueryManager::QueryManager( IndexHashtable* index ) {
	this->index = index;
}


std::string QueryManager::query_index( std::string query, int limit ) {
	rankedDocuments.clear();
	direct_match( &query );
	return convert_result_to_string( limit );;
}


std::string QueryManager::convert_result_to_string( int limit ) {

	std::string rets = "";
	int i;

	if(limit < rankedDocuments.size()) {
		for(i = 0; i < limit; i++) {
			rets += ( std::to_string( i ) + ". " + rankedDocuments[i].filename + "\n" );
		}

	} else {
		for(i = 0; i < rankedDocuments.size(); i++) {
			rets += ( std::to_string( i ) + ". " + rankedDocuments[i].filename + "\n" );
		}
	}

	return rets;
}



std::vector<RankedDocument> QueryManager::get_ranked_documents() {
	return rankedDocuments;
}



std::vector<std::string> QueryManager::split_string( std::string* query, char c ) {

	std::vector<std::string> words;
	std::string word;
	int i = 0;

	word.clear();
	for(i = 0; i < query->size(); i++) {
		if(( *query )[i] == c) {
			if(!word.empty())words.push_back( word );
			word.clear();
		}
	}

	return words;
}


void QueryManager::merge_occurences_to_ranked_documents( std::vector<IndexHashtableEntry>* entries ) {

	int i = 0;
	int j = 0;
	OccurenceNode* ocnp = nullptr;

	for(i = 0; i < entries->size(); i++) {
		ocnp = ( *entries )[i].get_occurances();
		for(j = 0; j < rankedDocuments.size(); j++) {
			if(ocnp->filename == rankedDocuments[j].filename) {
				rankedDocuments[j].count += 1;
				rankedDocuments[j].score += ocnp->occurences;
				break;
			}
		}
		if(j >= rankedDocuments.size()) {
			RankedDocument rd;
			rd.count = 1;
			rd.filename = ocnp->filename;
			rd.score = ocnp->occurences;
			rankedDocuments.push_back( rd );
		}
	}
}



void QueryManager::direct_match( std::string* query ) {
	std::vector<std::string> words;
	std::vector<IndexHashtableEntry> entries;
	IndexHashtableEntry* entry;
	int i;

	words = split_string( query, ' ' );

	for(i = 0; i < words.size(); i++) {
		entry = index->get_entry( &words[i] );

		if(entry != nullptr) {
			entries.push_back( *entry );
		}
	}

	merge_occurences_to_ranked_documents( &entries );

	for(i = 0; i < rankedDocuments.size(); i++) {
		rankedDocuments[i].score *= rankedDocuments[i].count;
	}


	//	Use std sort for speed and lamda to sort the struct based on score
	std::sort( rankedDocuments.begin(), rankedDocuments.end(), []( const RankedDocument& left, const RankedDocument& right ) {
		return left.score < right.score;
	} );

}
