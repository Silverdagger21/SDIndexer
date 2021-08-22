#include "indexhashtableentry.h"


using namespace sdindexer;


IndexHashtableEntry::IndexHashtableEntry( std::string word ) {
	this->value = word;
}


IndexHashtableEntry::IndexHashtableEntry( std::string* word ) {
	this->value = *word;
}


sdindexer::IndexHashtableEntry::~IndexHashtableEntry() {

	OccurenceNode* ocnp;
	OccurenceNode* pcnp;

	ocnp = occurences;
	while(ocnp != nullptr) {
		pcnp = ocnp;
		ocnp = ocnp->next;
		delete pcnp;
	}
}



IndexHashtableEntry* IndexHashtableEntry::get_next() {
	return this->next;
}


std::string IndexHashtableEntry::get_value() {
	return this->value;
}


OccurenceNode* IndexHashtableEntry::get_occurances() {
	return this->occurences;
}

bool IndexHashtableEntry::set_next( IndexHashtableEntry* entry ) {
	this->next = entry;
	return true;
}


bool IndexHashtableEntry::set_value( std::string* value ) {
	this->value = *value;
	return true;
}

bool IndexHashtableEntry::add_occurence( OccurenceNode* occurence ) {
	occurence->next = this->occurences;
	this->occurences = occurence;
	return true;
}


IndexHashtableEntry* IndexHashtableEntry::clone_entry() {

	IndexHashtableEntry* newEntry;
	OccurenceNode* ocnp;
	OccurenceNode* newoc;

	newEntry = new IndexHashtableEntry( "" );

	newEntry->value = this->value;
	newEntry->next = this->next;
	newEntry->occurences = nullptr;

	//Copy Occurence LinkedList
	ocnp = this->occurences;

	while(ocnp != nullptr) {
		newoc = new OccurenceNode;
		*newoc = *ocnp;
		newoc->next = newEntry->occurences;
		newEntry->occurences = newoc;
		ocnp = ocnp->next;
	}

	return newEntry;
}

void IndexHashtableEntry::load_occurences( std::string* occurences ) {

	OccurenceNode* onp;
	std::string filename;
	std::string appearances;
	int flag = false;

	for(int i = 0; i < occurences->size(); i++) {
		if(( *occurences )[i] == ';') {
			if(flag) {
				onp = new OccurenceNode;
				onp->filename = filename;
				onp->occurences = std::stoi( appearances );
				this->add_occurence( onp );
				filename.clear();
				appearances.clear();
			}
			flag = !flag;
		} else if(flag) {
			appearances.push_back( ( *occurences )[i] );
		} else {
			filename.push_back( ( *occurences )[i] );
		}
	}

	onp = new OccurenceNode;
	onp->filename = filename;
	onp->occurences = std::stoi( appearances );
	this->add_occurence( onp );
}



std::string IndexHashtableEntry::to_string() {

	std::string data;
	OccurenceNode* ocnp;
	data.append( this->value + "\n" );
	ocnp = this->get_occurances();
	data.append( ocnp->filename + ";" + std::to_string( ocnp->occurences ) );
	ocnp = ocnp->next;
	while(ocnp != nullptr) {
		data.append( ";" + ocnp->filename + ";" + std::to_string( ocnp->occurences ) );
		ocnp = ocnp->next;
	}
	return data;
}
