#include "indexhashtable.h"


using namespace sdindexer;

IndexHashtableEntry::IndexHashtableEntry( std::string word ) {
	this->value = word;
}


IndexHashtableEntry* IndexHashtableEntry::get_next() {
	return this->next;
}


std::string IndexHashtableEntry::get_value() {
	return this->value;
}


OccurenceNode* IndexHashtableEntry::get_next_occurence() {
	return this->occurences;
}

bool IndexHashtableEntry::set_next( IndexHashtableEntry* entry ) {
	this->next = entry;
	return true;
}


bool IndexHashtableEntry::set_value( std::string value ) {
	this->value = value;
	return true;
}


bool IndexHashtableEntry::add_occurence( OccurenceNode* occurence ) {
	occurence->next = this->occurences;
	this->occurences = occurence;
	return true;
}


std::string IndexHashtableEntry::to_string() {

	std::string data;
	OccurenceNode* ocnp;
	data.append( this->value + "\n" );
	ocnp = this->get_next_occurence();
	data.append( ocnp->filename + ":" + std::to_string( ocnp->occurences ) );
	ocnp = ocnp->next;
	while ( ocnp != NULL ) {
		data.append( ":" + ocnp->filename + ":" + std::to_string( ocnp->occurences ) );
		ocnp = ocnp->next;
	}
	return data;
}



IndexHashtable::IndexHashtable( int size ) {

	this->hashtable = new IndexHashtableEntry * [size];
	this->size = size;

	int i;
	for ( i = 0; i < size; i++ ) {
		this->hashtable[i] = NULL;
	}
}


bool IndexHashtable::add_to_index( std::string word, std::string filename ) {
	IndexHashtableEntry* entry;
	bool status = false;
	unsigned int hashvalue;

	hashvalue = hashfunction( word );

	if ( hashtable[hashvalue] == NULL ) {
		entry = new IndexHashtableEntry( word );
		entry->set_next( NULL );
		OccurenceNode* ocnp = new OccurenceNode;
		ocnp->filename = filename;
		ocnp->next = NULL;
		ocnp->occurences = 1;
		entry->add_occurence( ocnp );
		hashtable[hashvalue] = entry;
		status = true;
	} else {
		entry = hashtable[hashvalue];
		while ( entry != NULL && entry->get_value() != word ) {
			entry = entry->get_next();
		}
		if ( entry != NULL ) {
			OccurenceNode* ocnp = entry->get_next_occurence();
			while ( ocnp != NULL && ocnp->filename != filename ) {
				ocnp = ocnp->next;
			}
			if ( ocnp != NULL ) {
				ocnp->occurences = ocnp->occurences + 1;
				status = true;
			} else {
				ocnp = new OccurenceNode;
				ocnp->filename = filename;
				ocnp->occurences = 1;
				entry->add_occurence( ocnp );
				status = true;
			}
		} else {
			entry = new IndexHashtableEntry( word );
			entry->set_next( hashtable[hashvalue] );
			OccurenceNode* ocnp = new OccurenceNode;
			ocnp->filename = filename;
			ocnp->next = NULL;
			ocnp->occurences = 1;
			entry->add_occurence( ocnp );
			hashtable[hashvalue] = entry;
			status = true;
		}
	}

	return status;
}


bool IndexHashtable::load_entry( std::string id, std::string word, std::string occurences ) {

	if ( id.empty() || word.empty() || occurences.empty() ) return false;


	unsigned int hashv = std::stoi( id );
	IndexHashtableEntry* entry;
	if ( hashtable[hashv] == NULL ) {
		entry = new IndexHashtableEntry( word );
		entry->set_next( NULL );
		hashtable[hashv] = entry;
		entry->load_occurences( occurences );
	} else {
		entry = hashtable[hashv];
		while ( entry != NULL && entry->get_value() != word ) {
			entry = entry->get_next();
		}
		if ( entry == NULL ) {
			entry = new IndexHashtableEntry( word );
			entry->set_next( hashtable[hashv] );
			hashtable[hashv] = entry;
			entry->load_occurences( occurences );
		} else {
			entry->load_occurences( occurences );
		}
	}
	return true;
}


void IndexHashtableEntry::load_occurences( std::string occurences ) {

	OccurenceNode* onp;
	std::string filename;
	std::string appearances;
	int flag = false;

	for ( int i = 0; i < occurences.size(); i++ ) {
		if ( occurences[i] == ':' ) {
			if ( flag ) {
				onp = new OccurenceNode;
				onp->filename = filename;
				onp->occurences = std::stoi( appearances );
				this->add_occurence( onp );
				filename.clear();
				appearances.clear();
			}
			flag = !flag;
		} else if ( flag ) {
			appearances.push_back( occurences[i] );
		} else {
			filename.push_back( occurences[i] );
		}
	}

	onp = new OccurenceNode;
	onp->filename = filename;
	onp->occurences = std::stoi( appearances );
	this->add_occurence( onp );
}



unsigned int IndexHashtable::hashfunction( std::string word ) {
	std::hash <std::string> str_hash;
	unsigned int hashvalue;
	hashvalue = (unsigned int)str_hash( word );
	hashvalue = hashvalue % this->size;
	return hashvalue;
}


std::string IndexHashtable::to_string() {
	std::string data;
	IndexHashtableEntry* ihe = NULL;

	for ( int i = 0; i < this->size; i++ ) {
		if ( hashtable[i] != NULL ) {
			ihe = hashtable[i];
			data.append( std::to_string( i ) + "\n" );
			data.append( ihe->to_string() + "\n\n" );
			ihe = ihe->get_next();
			while ( ihe != NULL ) {
				data.append( "-1\n" );
				data.append( ihe->to_string() + "\n\n" );
				ihe = ihe->get_next();
			}

		}
	}
	return data;

}

