#include "querymanager.h"

#include "fileparser.h"
#include <algorithm>


using namespace sdindex;


const std::set <std::string> QueryManager::commonWords = {
			"i", "you", "he", "she", "it", "we", "they", "is", "are", "at", "in", "on", "the",
			"be", "to", "of", "and", "a", "that", "have", "for", "not", "with", "as", "do",
			"this", "but", "his", "her", "by", "from", "or", "there", "so", "up", "out", "if",
			"about", "who", "get", "which", "go", "me", "when", "can", "its", "us"
};


QueryManager::QueryManager(IndexHashtable* index) {

	this->index = index;
	this->results_limit = 50;
}

sdindex::QueryManager::QueryManager(IndexHashtable* index, int limit) {

	this->index = index;
	this->results_limit = limit;
}





std::vector<RankedDocument> QueryManager::query_index(const std::string& query) {

	rankedDocuments.clear();
	direct_match(query);
	return rankedDocuments;
}




std::string QueryManager::query_index_to_string(const std::string& query) {

	rankedDocuments.clear();
	direct_match(query);
	return convert_result_to_string(results_limit);;
}



std::string QueryManager::convert_result_to_string(int limit) {

	std::string rets = "";
	int i;

	if(limit < rankedDocuments.size()) {
		for(i = 0; i < limit; i++) {
			rets += (std::to_string(i) + ". " + rankedDocuments[i].filename + "\n");
		}

	} else {
		for(i = 0; i < rankedDocuments.size(); i++) {
			rets += (std::to_string(i) + ". " + rankedDocuments[i].filename + "\n");
		}
	}

	return rets;
}



std::vector<RankedDocument> QueryManager::get_ranked_documents() {

	return rankedDocuments;
}


bool QueryManager::is_common_word(const std::string& word) {

	if(QueryManager::commonWords.find(word) != QueryManager::commonWords.end()) return true;
	return false;
}




std::vector<std::string> QueryManager::format_query(const std::string& query, char c) {

	std::vector<std::string> words;
	std::string word;
	int i = 0;

	word.clear();
	for(i = 0; i < query.size(); i++) {
		if(query[i] == c) {
			if(!word.empty()) {
				FileParser::parse_word(word, convertToLowercase, ommitNumbers, ommitSpecialCharacters);
				words.push_back(word);
			}

			word.clear();
		} else {
			word.push_back(query[i]);
		}
	}
	if(!word.empty()) {
		FileParser::parse_word(word, convertToLowercase, ommitNumbers, ommitSpecialCharacters);
		words.push_back(word);
	}

	return words;
}


void QueryManager::merge_occurences_to_ranked_documents(std::vector<IndexHashtableEntry*>* entries) {

	int i = 0;
	int j = 0;
	bool common = false;
	OccurenceNode* ocnp = nullptr;

	for(i = 0; i < entries->size(); i++) {
		common = is_common_word(entries->at(i)->get_value());
		ocnp = entries->at(i)->get_occurances();
		while(ocnp != nullptr) {
			for(j = 0; j < rankedDocuments.size(); j++) {
				if(ocnp->filenameIndex == rankedDocuments[j].filenameIndex) {
					rankedDocuments[j].count += 1;
					if(common) {
						rankedDocuments[j].score += (ocnp->occurences / 0.5f);
					} else {
						rankedDocuments[j].score += ocnp->occurences;
					}

					break;
				}
			}
			if(j >= rankedDocuments.size()) {
				RankedDocument rd;
				rd.count = 1;
				rd.filenameIndex = ocnp->filenameIndex;
				rd.score = ocnp->occurences;
				rankedDocuments.push_back(rd);
			}
			ocnp = ocnp->next;
		}
	}
}





void QueryManager::direct_match(const std::string& query) {

	std::vector<std::string> words;
	std::vector<IndexHashtableEntry*> entries;
	IndexHashtableEntry* entry;
	int i;

	words = format_query(query, ' ');

	for(i = 0; i < words.size(); i++) {
		entry = index->get_entry(words[i]);

		if(entry != nullptr) {
			entries.push_back(entry);
		}
	}

	merge_occurences_to_ranked_documents(&entries);

	for(i = 0; i < rankedDocuments.size(); i++) {
		rankedDocuments[i].score *= rankedDocuments[i].count;
	}


	//	Used std sort for speed and lamda to sort the struct based on score
	std::sort(rankedDocuments.begin(), rankedDocuments.end(), [](const RankedDocument& left, const RankedDocument& right) {
		return left.score > right.score;
	});


	// Assign names to documents
	for(i = 0; i < rankedDocuments.size(); i++) {
		rankedDocuments[i].filename = index->get_filename(rankedDocuments[i].filenameIndex);
	}

}
