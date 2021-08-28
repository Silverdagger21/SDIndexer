#pragma once
#include <wx/wx.h>
#include <wx/filectrl.h>
#include <wx/filepicker.h>
#include <wx/tglbtn.h>


#include "fileparser.h"
#include "indexhashtable.h"
#include "querymanager.h"

namespace sdindex {


	// Extends wxFrame - Contains all the information required to run GUI elements
	class Primary : public wxFrame {

		public:


		Primary();
		~Primary();


		// Hashtable used to store the index
		IndexHashtable hashtable = IndexHashtable(100000);


		// Vector of strings used to store the filenames of a directory and subdirectories
		std::vector<std::string> filenames;


		// Used to parse queries and "ask" the index for results
		QueryManager queryManager = QueryManager(&hashtable);


		// Contains extensions to include or exclude
		std::vector<std::string> extensions;

		// Flags helping us determine options

		bool hasLoadedIndex = false;
		bool toLowercase = true;
		bool ommitNumbers = true;
		bool ommitSpecialCharacters = true;
		bool extensionsInclude = true;


		wxStaticText* optionsText = nullptr;

		wxStaticText* extensionsText = nullptr;

		wxStaticText* searchText = nullptr;

		wxStaticText* resultsText = nullptr;


		// Provides the "default" font for the window
		wxFont get_font1();


		// Widget enabling the user to select a directory
		wxDirPickerCtrl* dirSelect = nullptr;


		// Button enabling the user to load an index from a file in the drive
		wxButton* loadButton = nullptr;


		// Button enabling the user to save an index to a file
		wxButton* saveFileButton = nullptr;


		// Button enabling the user to clear the index and all fields in the UI
		wxButton* clearButton = nullptr;


		// Togglebutton enabling the user to determine whether to convert all characters to lowercase for ease or not
		wxToggleButton* toLowercaseToggleButton = nullptr;


		// Togglebutton enabling the user to determine whether to ommit numbers or not
		wxToggleButton* ommitNumbersToggleButton = nullptr;


		// Togglebutton enabling the user to determine whether to ommit special characters or not
		wxToggleButton* ommitSymbolsToggleButton = nullptr;


		// Textarea widget enabling the user to type what they want to search for
		wxTextCtrl* queryArea = nullptr;


		// BUtton enabling the user to query the index based on what they wrote in the textarea
		wxButton* queryButton = nullptr;


		// Button enabling the user to copy a selected result to the clipboard
		wxButton* copyToClipboardButton = nullptr;


		// Listbox widget used to display the results of the query operation as well as enabling the user to select a file
		wxListBox* resultsListArea = nullptr;


		// Textarea widget enabling the user to type the extensions they want
		wxTextCtrl* extensionsArea = nullptr;


		// Togglebutton enabling the user to determine whether to ommit numbers or not
		wxToggleButton* extensionsToggleButton = nullptr;



		// Handles resizing the frame components based on the window size
		void OnSize(wxSizeEvent& event);


		// Handles selecting a directory
		void on_dir_changed(wxFileDirPickerEvent& evt);


		// Handles loading a index file from the drive when the "load" button is clicked
		void on_load_clicked(wxCommandEvent& evt);


		// Handles saving an index file to the drive when the "save" button is clicked
		void on_save_clicked(wxCommandEvent& evt);


		// Handles clearing the index and all UI elements when the "clear" button is clicked
		void on_clear_clicked(wxCommandEvent& evt);


		// Handles updating the options and the UI when the "to lowercase" togglebutton is clicked
		void on_to_lowercase_clicked(wxCommandEvent& evt);


		// Handles updating the options and the UI when the "numbers" togglebutton is clicked
		void on_ommit_numbers_clicked(wxCommandEvent& evt);


		// Handles updating the options and the UI when the "symbols" togglebutton is clicked
		void on_ommit_special_clicked(wxCommandEvent& evt);


		// Handles updating the options and the UI when the "to lowercase" togglebutton is clicked
		void on_extensions_clicked(wxCommandEvent& evt);


		// Handles querying the index when the "query" button is clicked
		void on_query_clicked(wxCommandEvent& evt);


		// Handles copying a result selected from the "resultsListArea" to the clipboard when the "copy" button is clicked
		void on_copy_clicked(wxCommandEvent& evt);



		wxDECLARE_EVENT_TABLE();

	};

}

