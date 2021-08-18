#include "indexerHashtable.h"


using namespace SDIndexer;

indexerHashtableEntry::indexerHashtableEntry( std::string word ) {
	this->value = word;
}


indexerHashtableEntry* indexerHashtableEntry::getNext() {
	return this->next;
}


std::string indexerHashtableEntry::getValue() {
	return this->value;
}


occurenceNode* indexerHashtableEntry::getNextOccurence() {
	return this->occurences;
}

bool indexerHashtableEntry::setNext( indexerHashtableEntry* entry ) {
	this->next = entry;
	return true;
}


bool indexerHashtableEntry::setValue( std::string value ) {
	this->value = value;
	return true;
}


bool indexerHashtableEntry::addOccurence( occurenceNode* occurence ) {
	occurence->next = this->occurences;
	this->occurences = occurence;
	return true;
}


std::string indexerHashtableEntry::toString() {

	std::string data;
	occurenceNode* ocnp;
	data.append( this->value + "\n" );
	ocnp = this->getNextOccurence();
	data.append( ocnp->filename + ":" + std::to_string( ocnp->occurences ));
	ocnp = ocnp->next;
	while ( ocnp != NULL ) {
		data.append( ":" + ocnp->filename + ":" + std::to_string(ocnp->occurences));
		ocnp = ocnp->next;
	}
	return data;
}



indexerHashtable::indexerHashtable( int size ) {

	this->hashtable = new indexerHashtableEntry * [size];
	this->size = size;

	int i;
	for ( i = 0; i < size; i++ ) {
		this->hashtable[i] = NULL;
	}
}


bool indexerHashtable::addToIndex( std::string word, std::string filename ) {
	indexerHashtableEntry* entry;
	bool status = false;
	unsigned int hashvalue;

	hashvalue = hashFunction( word );

	if ( hashtable[hashvalue] == NULL ) {
		entry = new indexerHashtableEntry( word );
		entry->setNext( NULL );
		occurenceNode* ocnp = new occurenceNode;
		ocnp->filename = filename;
		ocnp->next = NULL;
		ocnp->occurences = 1;
		entry->addOccurence( ocnp );
		hashtable[hashvalue] = entry;
		status = true;
	} else {
		entry = hashtable[hashvalue];
		while ( entry != NULL && entry->getValue() != word ) {
			entry = entry->getNext();
		}
		if ( entry != NULL ) {
			occurenceNode* ocnp = entry->getNextOccurence();
			while ( ocnp != NULL && ocnp->filename != filename ) {
				ocnp = ocnp->next;
			}
			if ( ocnp != NULL ) {
				ocnp->occurences = ocnp->occurences + 1;
				status = true;
			} else {
				ocnp = new occurenceNode;
				ocnp->filename = filename;
				ocnp->occurences = 1;
				entry->addOccurence( ocnp );
				status = true;
			}
		} else {
			entry = new indexerHashtableEntry( word );
			entry->setNext( hashtable[hashvalue] );
			occurenceNode* ocnp = new occurenceNode;
			ocnp->filename = filename;
			ocnp->next = NULL;
			ocnp->occurences = 1;
			entry->addOccurence( ocnp );
			hashtable[hashvalue] = entry;
			status = true;
		}
	}

	return status;
}


bool indexerHashtable::loadEntry( std::string id, std::string word, std::string occurences ) {

	if ( id.empty() || word.empty() || occurences.empty() ) return false;

	
	unsigned int hashv = std::stoi( id );
	indexerHashtableEntry* entry;
	if ( hashtable[hashv] == NULL ) {
		entry = new indexerHashtableEntry(word);
		entry->setNext( NULL );
		hashtable[hashv] = entry;
		entry->loadOccurences(occurences);
	} else {
		entry = hashtable[hashv];
		while ( entry != NULL && entry->getValue() != word) {
			entry = entry->getNext();
		}
		if ( entry == NULL ) {
			entry = new indexerHashtableEntry( word );
			entry->setNext( hashtable[hashv] );
			hashtable[hashv] = entry;
			entry->loadOccurences( occurences );
		}else{
			entry->loadOccurences( occurences );
		}
	}
	return true;
}


void indexerHashtableEntry::loadOccurences( std::string occurences ) {
	
	occurenceNode* onp;
	std::string filename;
	std::string appearances;
	int flag = false;
	
	for ( int i = 0; i < occurences.size(); i++ ) {
		if ( occurences[i] == ':' ) {
			if ( flag ) {
				onp = new occurenceNode;
				onp->filename = filename;
				onp->occurences = std::stoi( appearances );
				this->addOccurence( onp );
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

	onp = new occurenceNode;
	onp->filename = filename;
	onp->occurences = std::stoi( appearances );
	this->addOccurence( onp );
}



unsigned int indexerHashtable::hashFunction( std::string word ) {
	std::hash <std::string> str_hash;
	unsigned int hashvalue;
	hashvalue = (unsigned int)str_hash( word );
	hashvalue = hashvalue % this->size;
	return hashvalue;
}


std::string indexerHashtable::toString() {
	std::string data;
	indexerHashtableEntry* ihe = NULL;

	for ( int i = 0; i < this->size; i++ ) {
		if ( hashtable[i] != NULL ) {
			ihe = hashtable[i];
			data.append( std::to_string( i ) + "\n" );
			data.append( ihe->toString() + "\n\n" );
			ihe = ihe->getNext();
			while ( ihe != NULL ) {
				data.append( "-1\n" );
				data.append( ihe->toString() + "\n\n" );
				ihe = ihe->getNext();
			}
			
		}
	}
	return data;

}

