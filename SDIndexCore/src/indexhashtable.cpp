#include "indexhashtable.h"


using namespace sdindex;


IndexHashtable::IndexHashtable(int size) {

	this->size = size;
	this->hashtable = new IndexHashtableEntry * [size];
	for(int i = 0; i < size; i++) {
		this->hashtable[i] = nullptr;
	}
}



IndexHashtable::~IndexHashtable() {

	clear();
	delete hashtable;
}

int sdindex::IndexHashtable::get_size() {
	return size;
}



bool IndexHashtable::add_to_index(const std::string& word, const std::string& filename) {

	int filenameIndex = -1;

	filenameIndex = get_filename_index(filename);
	if(filenameIndex == -1) {
		filenameIndex = static_cast<int>(filenames.size());
		filenames.push_back(filename);
	}
	return add_to_index(word, filenameIndex);
}



bool IndexHashtable::add_to_index(const std::string& word, int filenameIndex) {

	IndexHashtableEntry* entry;
	bool status = false;
	unsigned int hashvalue;

	hashvalue = hashfunction(word);

	if(hashtable[hashvalue] == nullptr) {
		entry = new IndexHashtableEntry(word);
		entry->set_next(nullptr);
		OccurenceNode* ocnp = new OccurenceNode;

		ocnp->filenameIndex = filenameIndex;
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
			while(ocnp != nullptr && ocnp->filenameIndex != filenameIndex) {
				ocnp = ocnp->next;
			}
			if(ocnp != nullptr) {
				ocnp->occurences = ocnp->occurences + 1;
				status = true;
			} else {
				ocnp = new OccurenceNode;
				ocnp->filenameIndex = filenameIndex;
				ocnp->occurences = 1;
				entry->add_occurence(ocnp);
				status = true;
			}
		} else {
			entry = new IndexHashtableEntry(word);
			entry->set_next(hashtable[hashvalue]);
			OccurenceNode* ocnp = new OccurenceNode;
			ocnp->filenameIndex = filenameIndex;
			ocnp->next = nullptr;
			ocnp->occurences = 1;
			entry->add_occurence(ocnp);
			hashtable[hashvalue] = entry;
			status = true;
		}
	}
	return status;
}



bool IndexHashtable::entry_exists(const std::string& word) {

	IndexHashtableEntry* entry = nullptr;

	entry = get_entry(word);

	if(entry == nullptr)return false;
	else return true;
}



std::string IndexHashtable::get_filename(int index) {

	if(!(index < filenames.size())) {
		return std::string();
	}

	return filenames[index];
}

IndexHashtableEntry* IndexHashtable::get_entry_copy(const std::string& word) {

	IndexHashtableEntry* entryptr;

	entryptr = get_entry(word);

	if(entryptr == nullptr) return nullptr;

	entryptr = entryptr->clone_entry();

	return entryptr;
}



IndexHashtableEntry* IndexHashtable::get_entry(const std::string& word) {

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

void IndexHashtable::load_filename_entry(const std::string& filename) {

	filenames.push_back(filename);
}



void IndexHashtable::clear() {

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

	filenames.clear();
}



bool IndexHashtable::merge(IndexHashtable* otherIndex) {

	IndexHashtableEntry* otherEntry = nullptr;
	IndexHashtableEntry* thisEntry = nullptr;
	OccurenceNode* thisOccurence = nullptr;
	int oldFilenamesSize;
	int i;

	if(size != otherIndex->size)return false;

	oldFilenamesSize = static_cast<int> (filenames.size());

	// Get all other filenames (always different unless we index the same directory twice)
	for(i = 0; i < otherIndex->filenames.size(); i++) {
		filenames.push_back(otherIndex->filenames[i]);
	}

	// Increase all filenameIndexes by size
	for(i = 0; i < size; i++) {
		otherEntry = otherIndex->hashtable[i];
		while(otherEntry != nullptr) {
			thisOccurence = otherEntry->get_occurances();
			while(thisOccurence != nullptr) {
				thisOccurence->filenameIndex = thisOccurence->filenameIndex + oldFilenamesSize;
				thisOccurence = thisOccurence->next;
			}
			otherEntry = otherEntry->next;
		}
	}

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
				

				// If you haven't found a match, steal the entry and restructure
				if(thisEntry == nullptr) {
					thisEntry = new IndexHashtableEntry(otherEntry->get_value());
					thisEntry->occurences = otherEntry->occurences;
					otherEntry->occurences = nullptr;
					thisEntry->next = hashtable[i];
					hashtable[i] = thisEntry;
				}
				
				otherEntry = otherEntry->next;
				

				

			}
		}
	}
	return true;
}



int IndexHashtable::get_filename_index(const std::string& filename) {

	int i;

	for(i = 0; i < filenames.size(); i++) {
		if(filename == filenames[i]) return i;
	}

	if(i == filenames.size())filenames.push_back(filename);

	return i;
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
	int i;

	for(i = 0; i < this->size; i++) {
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

	data.append("Filenames:\n");
	for(i = 0; i < filenames.size(); i++) {
		data.append(filenames[i] + "\n");
	}

	return data;
}
