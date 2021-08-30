#include "indexhashtable.h"


using namespace sdindex;


IndexHashtable::IndexHashtable(int size) {

	this->size = size;
	this->hashtable = new IndexHashtableEntry * [size];
	for(int i = 0; i < size; i++) {
		this->hashtable[i] = nullptr;
	}
}



sdindex::IndexHashtable::~IndexHashtable() {

	clear();
	delete hashtable;
}

int sdindex::IndexHashtable::get_size() {
	return size;
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
		while(entry != nullptr && entry->value != word) {
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

	while(entry != nullptr && entry->value != word) entry = entry->get_next();

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
		while(entry != nullptr && entry->value != word) {
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



void sdindex::IndexHashtable::steal_occurences(IndexHashtable* otherIndex) {
	
	unsigned int hashv;
	int i;
	IndexHashtableEntry* entry = nullptr;
	IndexHashtableEntry* thisEntry = nullptr;
	OccurenceNode* occurence = nullptr;
	OccurenceNode* nextOccurence = nullptr;

	// Loops over otherIndex hashtable
	for(i = 0; i < otherIndex->size; i++) {

		// If a chain exists in otherIndex
		if(otherIndex->hashtable[i] != nullptr) {
			entry = otherIndex->hashtable[i];

			//Loops over hashtable entry chains
			while(entry != nullptr) {
				hashv = hashfunction(entry->value);

				// If a chain exists in this index
				if(hashtable[hashv] != nullptr) {
					thisEntry = hashtable[hashv];

					// Loops over this hashtable chain
					while(thisEntry != nullptr) {
						if(thisEntry->value == entry->value) {
							occurence = entry->occurences;
							entry->occurences = nullptr;
							nextOccurence = occurence->next;

							// Loop over occurences and add them
							while(nextOccurence != nullptr) {
								thisEntry->add_occurence(occurence);
								occurence = nextOccurence;
								nextOccurence = nextOccurence->next;
							}
							thisEntry->add_occurence(occurence);
							break;
						}
						thisEntry = thisEntry->get_next();
					}

					// If the entry did not match on the chain
					if(thisEntry == nullptr) {
						thisEntry = new IndexHashtableEntry(entry->value);
						thisEntry->set_next(hashtable[hashv]);
						hashtable[hashv] = thisEntry;
						thisEntry->occurences = entry->occurences;
						entry->occurences = nullptr;	
					}

					// If a chain does not exist in this index
				} else {
					hashtable[hashv] = new IndexHashtableEntry(entry->value);
					hashtable[hashv]->next = nullptr;
					hashtable[hashv]->occurences = entry->occurences;
					entry->occurences = nullptr;
				}
				entry = entry->get_next();
			}
		}
	}
}

bool sdindex::IndexHashtable::merge(IndexHashtable* otherIndex) {

	IndexHashtableEntry* otherEntry = nullptr;
	IndexHashtableEntry* thisEntry = nullptr;
	OccurenceNode* thisOccurence = nullptr;
	int i;

	if(size != otherIndex->size)return false;

	// Iterates over all the hashtable slots
	for(i = 0; i < size; i++) {
		
		// If the slot is empty grab the entry chain
		if(hashtable[i] == nullptr) {
			hashtable[i] = otherIndex->hashtable[i];
			otherIndex->hashtable[i] = nullptr;
		} else {
			otherEntry = otherIndex->hashtable[i];

			//Iterate over otherIndexes chain and try to merge elements
			while(otherEntry != nullptr) {
				thisEntry = hashtable[i];

				// Iterate over our chain
				while(thisEntry != nullptr) {

					// If you find a match
					if(thisEntry->value == otherEntry->value) {
						thisOccurence = thisEntry->occurences;

						// Find the last occurance
						while(thisOccurence->next != nullptr) {
							thisOccurence = thisOccurence->next;
						}

						// Connect the last occurance to otherIndexes first
						thisOccurence->next = otherEntry->occurences;
						otherEntry->occurences = nullptr;
						break;
					}
					thisEntry = thisEntry->next;
				}
				otherEntry = otherEntry->next;

				// If you haven't found a match, steal the entry and restructure
				if(thisEntry == nullptr) {
					otherIndex->hashtable[i]->next = hashtable[i];
					hashtable[i] = otherIndex->hashtable[i];
					otherIndex->hashtable[i] = otherEntry;
				} 
			}	
		}
	}
	return true;
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
