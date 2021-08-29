#include "indexhashtable.h"
#include "fileparser.h"
#include "querymanager.h"

#include <iostream>
#include <vector>
#include <string>


//#define BUFFERED_IO

#ifndef BUFFERED_IO
#ifdef _WIN64 
#include <conio.h>
#endif
#endif

using namespace sdindex;


// Initializing core objects

const int hashtableSize = 100000;
IndexHashtable index(hashtableSize);
QueryManager queryManager(&index);
std::string query;
std::string dirpath;
std::string input;
std::vector<std::string> extensions;
bool application_loop = true;
bool hasIndex = false;


// Handles command line argument input
void parse_arguments(int argc, char* argv[]);

// Receives input from the user based on the platform
// retrieves a single letter from the keyboard without pressing enter (if BUFFERED_IO is not defined)
char get_input();

// First menu
void initial_menu();

// Main menu with all options
void main_menu();

// Menu where the user controls different options
void options_menu();

// The main application loop
void interactive_loop();



int main(int argc, char* argv[]) {

	// Setting default options

	FileParser::toLowercase = true;
	FileParser::ommitSpecialCharacters = true;
	FileParser::ommitNumbers = true;
	queryManager.convertToLowercase = true;
	queryManager.ommitNumbers = true;
	queryManager.ommitSpecialCharacters = true;


	// Parse command line arguments first
	if(argc > 1) {
		parse_arguments(argc, argv);
	}

	if(!application_loop)return 0;

	interactive_loop();

	return 0;
}



void parse_arguments(int argc, char* argv[]) {

	int i;
	std::string cla;

	for(i = 1; i < argc; i++) {
		cla = argv[i];

		// To lowercase option
		if(cla == "-tlc") {
			FileParser::toLowercase = true;
			queryManager.convertToLowercase = true;

			// Do not to lowercase option
		} else if(cla == "-ntlc") {
			FileParser::toLowercase = false;
			queryManager.convertToLowercase = false;

			// Ommit numbers option
		} else if(cla == "-on") {
			FileParser::ommitNumbers = true;
			queryManager.ommitNumbers = true;

			// Do not ommit numbers option
		} else if(cla == "-non") {
			FileParser::ommitNumbers = false;
			queryManager.ommitNumbers = false;

			// Ommit symbols option
		} else if(cla == "-os") {
			FileParser::ommitSpecialCharacters = true;
			queryManager.ommitSpecialCharacters = true;

			// allow only documents with certain extensions
		} else if(cla == "-ae") {
			FileParser::allowExtensions = true;

			// allow all documents except the ones with certain extensions
		} else if(cla == "-nae") {
			FileParser::allowExtensions = false;

			// Set extensions
		} else if(cla == "-ext") {
			i++;
			if(i < argc) {
				extensions = FileParser::split_string(argv[i], ',');
			} else {
				std::cout << "Extensions not specified properly \n";
				return;
			}

			// Do not ommit symbols option
		} else if(cla == "-nos") {
			FileParser::ommitSpecialCharacters = false;
			queryManager.ommitSpecialCharacters = false;

			// Automatically exit the application after execution
		} else if(cla == "-e") {
			application_loop = false;

			// Select directory for indexing
		} else if(cla == "-sd") {
			i++;
			if(i < argc) {
				dirpath = argv[i];
				hasIndex = FileParser::index_directory_and_subdirectories(dirpath, index, extensions);

			} else {
				std::cout << "Path not specified correctly after option -sd\n";
				return;
			}

			// Load index from file
		} else if(cla == "-li") {
			i++;
			if(i < argc) {
				std::string indexFileName = argv[i];
				if(!FileParser::load_index_file(indexFileName, index)) {
					std::cout << "Could not load index file " + indexFileName + "\n";
					return;
				}
				hasIndex = true;
			} else {
				std::cout << "No valid index filename after -li option \n";
				return;
			}

			// Write index to file
		} else if(cla == "-wi") {
			i++;
			if(i < argc) {
				std::string indexFileName = argv[i];
				if(!hasIndex) {
					std::cout << "No index file loaded. Make sure the options are in the correct order \n";
					return;
				}
				FileParser::write_index_file_to_drive(indexFileName, index);
			} else {
				std::cout << "No valid filename after -wi option \n";
				return;
			}

			// Query index
		} else if(cla == "-q") {
			i++;
			if(i < argc) {
				query = argv[i];
				if(!hasIndex) {
					std::cout << "No index file loaded. Make sure the options are in the correct order \n";
					return;
				}
				std::cout << "Results:\n" << queryManager.query_index_to_string(query) << "\n";

			} else {
				std::cout << "No valid query after -q option \n";
				return;
			}
		} else if(cla == "-c") {
			index.clear();
		}
	}
}



char get_input() {

	// Buffered requires "enter" press after each option, using conio.h s _getch
	// makes the input snappier but is platform specific (windows) ehnce pp macros

	char c = ' ';

#ifdef BUFFERED_IO
	std::string inp;
	std::getline(std::cin, inp);
	c = inp[0];
#endif

#ifndef BUFFERED_IO

#ifdef _WIN64 
	c = _getch();
#endif

#ifdef __linux__
	std::string inp;
	std::getline(std::cin, inp);
	c = inp[0];
#endif

#endif

	return c;
}




void load_directory() {

	std::string indexFileName;

	std::getline(std::cin, indexFileName);

	if(!FileParser::load_index_file(indexFileName, index)) {
		std::cout << "Could not load index file " + indexFileName + "\n";
		return;
	}
	hasIndex = true;
}



void initial_menu() {

	char c = ' ';

	std::cout << "Options: 'd' select a directory to index | "
		<< "'l' load the index from a file | "
		<< "'x' extensions | "
		<< "'o' change options | "
		<< "'e' exit \n";

	c = get_input();

	switch(c) {
	case 'd':
		std::cout << "Enter the directory you wish to index\n";
		std::getline(std::cin, dirpath);
		hasIndex = FileParser::index_directory_and_subdirectories(dirpath, index, extensions);
		break;

	case 'l':
		std::cout << "Enter the index filename\n";
		load_directory();
		break;

	case 'x':
		std::cout << "Enter the extensions seperated by comma (.pdf,.txt)\n";
		std::getline(std::cin, input);
		extensions = FileParser::split_string(input, ',');
		break;

	case 'o':
		options_menu();
		break;

	case 'e':
		application_loop = false;
		break;
	}
}



void main_menu() {

	char c = ' ';

	std::cout << "Options: 'd' select a directory to add to index | "
		<< "'q' query | "
		<< "'s' save index to file | "
		<< "'e' exit \n";

	c = get_input();

	switch(c) {
	case 'd':
		std::cout << "Enter the directory you wish to index\n";
		std::getline(std::cin, dirpath);
		if(!hasIndex) {
			hasIndex = FileParser::index_directory_and_subdirectories(dirpath, index, extensions);
		} else {
			FileParser::index_directory_and_subdirectories(dirpath, index, extensions);
		}
		break;

	case 'q':
		std::cout << "Enter your query\n";
		query.clear();
		std::getline(std::cin, query);
		if(!hasIndex) {
			std::cout << "No index file loaded\n";
			return;
		}
		std::cout << "Results:\n" << queryManager.query_index_to_string(query) << "\n";
		break;

	case 's':
		std::cout << "Enter the path and name of the file you want to save\n";
		std::getline(std::cin, dirpath);
		FileParser::write_index_file_to_drive(dirpath, index);
		break;

	case 'e':
		application_loop = false;
		break;
	}
}



void options_menu() {

	char c = ' ';

	do {
		std::cout << "Change: 't' convert to lowercase : " << std::boolalpha << FileParser::toLowercase << " | "
			<< "'n' ommit numbers : " << FileParser::ommitNumbers << " | "
			<< "'s' ommit symbols : " << FileParser::ommitSpecialCharacters << " | "
			<< "'x' index only certain files by extension : " << FileParser::allowExtensions << " | "
			<< "'p' previous menu \n";

		c = get_input();

		switch(c) {
		case 't':
			FileParser::toLowercase = !FileParser::toLowercase;
			queryManager.convertToLowercase = !queryManager.convertToLowercase;
			break;

		case 'n':
			FileParser::ommitNumbers = !FileParser::ommitNumbers;
			queryManager.ommitNumbers = !queryManager.ommitNumbers;
			break;

		case 's':
			FileParser::ommitSpecialCharacters = !FileParser::ommitSpecialCharacters;
			queryManager.ommitSpecialCharacters = !queryManager.ommitSpecialCharacters;
			break;

		case 'x':
			FileParser::allowExtensions = !FileParser::allowExtensions;
			break;
		}
	} while(c != 'p');
}



void interactive_loop() {

	do {
		if(hasIndex) {
			main_menu();
		} else {
			initial_menu();
		}
	} while(application_loop);
}
