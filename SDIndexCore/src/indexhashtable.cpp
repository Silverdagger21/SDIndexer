#include "indexhashtable.h"


using namespace sdindex;


IndexHashtable::IndexHashtable(int size) {

	this->hashtable = new IndexHashtableEntry * [size];
	this->size = size;

	for(int i = 0; i < size; i++) {
		this->hashtable[i] = nullptr;
	}
}



sdindex::IndexHashtable::~IndexHashtable() {

	clear();
	delete hashtable;
}



bool IndexHashtable::add_to_index(const std::string& word, const std::string& filename) {

	IndexHashtableEntry* entry;
	bool status = false;
	unsigned int hashvalue;

	hashvalue = hashfunction(word);

	if(hashtable[hashvalue] == nullptr) {
		entry = new IndexHashtableEntry(word);
		entry->set_next(nullptr);
		OccurenceNode* ocnp = new OccurenceNode;
		ocnp->filename = filename;
		ocnp->next = nullptr;
		ocnp->occurences = 1;
		entry->add_occurence(ocnp);
		hashtable[hashvalue] = entry;
		status = true;
	} else {
		entry = hashtable[hashvalue];
		while(entry != nullptr && entry->get_value() != word) {
			entry = entry->get_next();
		}
		if(entry != nullptr) {
			OccurenceNode* ocnp = entry->get_occurances();
			while(ocnp != nullptr && ocnp->filename != filename) {
				ocnp = ocnp->next;
			}
			if(ocnp != nullptr) {
				ocnp->occurences = ocnp->occurences + 1;
				status = true;
			} else {
				ocnp = new OccurenceNode;
				ocnp->filename = filename;
				ocnp->occurences = 1;
				entry->add_occurence(ocnp);
				status = true;
			}
		} else {
			entry = new IndexHashtableEntry(word);
			entry->set_next(hashtable[hashvalue]);
			OccurenceNode* ocnp = new OccurenceNode;
			ocnp->filename = filename;
			ocnp->next = nullptr;
			ocnp->occurences = 1;
			entry->add_occurence(ocnp);
			hashtable[hashvalue] = entry;
			status = true;
		}
	}
	return status;
}



bool sdindex::IndexHashtable::entry_exists(const std::string& word) {

	IndexHashtableEntry* entry = nullptr;

	entry = get_entry(word);

	if(entry == nullptr)return false;
	else return true;
}



IndexHashtableEntry* sdindex::IndexHashtable::get_entry_copy(const std::string& word) {

	IndexHashtableEntry* entryptr;

	entryptr = get_entry(word);

	if(entryptr == nullptr) return nullptr;

	entryptr = entryptr->clone_entry();

	return entryptr;
}



IndexHashtableEntry* sdindex::IndexHashtable::get_entry(const std::string& word) {

	IndexHashtableEntry* entry = nullptr;
	unsigned int hashvalue;

	hashvalue = hashfunction(word);
	entry = hashtable[hashvalue];

	if(entry == nullptr) return nullptr;

	while(entry != nullptr && entry->get_value() != word) entry = entry->get_next();

	if(entry == nullptr) {
		return nullptr;
	} else {
		return entry;
	}

	return nullptr;
}



bool IndexHashtable::load_entry(const std::string& id, const std::string& word, const std::string& occurences) {

	if(id.empty() || word.empty() || occurences.empty()) return false;

	unsigned int hashv = std::stoi(id);
	IndexHashtableEntry* entry;
	if(hashtable[hashv] == nullptr) {
		entry = new IndexHashtableEntry(word);
		entry->set_next(nullptr);
		hashtable[hashv] = entry;
		entry->load_occurences(occurences);
	} else {
		entry = hashtable[hashv];
		while(entry != nullptr && entry->get_value() != word) {
			entry = entry->get_next();
		}
		if(entry == nullptr) {
			entry = new IndexHashtableEntry(word);
			entry->set_next(hashtable[hashv]);
			hashtable[hashv] = entry;
			entry->load_occurences(occurences);
		} else {
			entry->load_occurences(occurences);
		}
	}
	return true;
}



void sdindex::IndexHashtable::clear() {

	IndexHashtableEntry* entry;
	IndexHashtableEntry* pentry;
	int i;

	for(i = 0; i < size; i++) {
		entry = hashtable[i];
		while(entry != nullptr) {
			pentry = entry;
			entry = entry->get_next();
			delete pentry;
		}
		hashtable[i] = nullptr;
	}
}



unsigned int IndexHashtable::hashfunction(const std::string& word) {

	std::hash <std::string> str_hash;
	unsigned int hashvalue;
	hashvalue = (unsigned int)str_hash(word);
	hashvalue = hashvalue % this->size;
	return hashvalue;
}



std::string IndexHashtable::to_string() {

	std::string data;
	IndexHashtableEntry* ihe = nullptr;

	for(int i = 0; i < this->size; i++) {
		if(hashtable[i] != nullptr) {
			ihe = hashtable[i];
			data.append(std::to_string(i) + "\n");
			data.append(ihe->to_string() + "\n\n");
			ihe = ihe->get_next();
			while(ihe != nullptr) {
				data.append("-1\n");
				data.append(ihe->to_string() + "\n\n");
				ihe = ihe->get_next();
			}

		}
	}
	return data;
}
