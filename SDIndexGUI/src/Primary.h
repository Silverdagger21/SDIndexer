#pragma once
#include <wx/wx.h>
#include <wx/filectrl.h>
#include <wx/filepicker.h>

#include "fileparser.h"
#include "indexhashtable.h"
#include "querymanager.h"

namespace sdindexer {

	class Primary : public wxFrame {
		
		public:


		Primary();

		~Primary();

		IndexHashtable hashtable = IndexHashtable(100000);
		std::vector<std::string> filenames;
		//std::string queryStr;
		QueryManager queryManager = QueryManager(&hashtable);
		bool hasLoadedIndex = false;

		
		//Browse Index Load

		wxDirPickerCtrl* dirSelect = nullptr;
		wxButton* loadButton = nullptr;
		wxButton* saveFileButton = nullptr;
		


		// Query Index

		wxTextCtrl* queryArea = nullptr;
		wxButton* queryButton = nullptr;


		// Display Results
		wxListBox* resultsListArea = nullptr;
		
		int ysize;

		void OnSize( wxSizeEvent& event );

		void on_dir_changed( wxFileDirPickerEvent& evt );

		void on_load_clicked( wxCommandEvent& evt );

		void on_save_clicked(wxCommandEvent& evt);

		void on_query_clicked( wxCommandEvent& evt );

		//void on_dir_select_clicked( wxCommandEvent& evt );

		


		wxDECLARE_EVENT_TABLE();

	};

}

