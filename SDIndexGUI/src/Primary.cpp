#include "Primary.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/listbox.h>


using namespace sdindexer;

wxBEGIN_EVENT_TABLE( Primary, wxFrame )
EVT_DIRPICKER_CHANGED(1, on_dir_changed )
EVT_BUTTON( 2, on_load_clicked )
EVT_BUTTON( 3, on_save_clicked )
EVT_BUTTON( 5, on_query_clicked )
wxEND_EVENT_TABLE()


Primary::Primary(): wxFrame( nullptr, wxID_ANY, "SDIndex", wxDefaultPosition, wxSize( 980, 800 ) ) {

	Bind( wxEVT_SIZE, &Primary::OnSize, this );
	
	wxFont myf( 14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false );

	wxPoint start( 15, 15 );
	wxSize buttonsize(80, 30);
	int  xgap = 15;
	int ygap = 30;

	wxSize dirSelectorSize( 600, ygap );

	dirSelect = new wxDirPickerCtrl( this, 1, wxEmptyString, wxDirSelectorPromptStr, start, dirSelectorSize );

	loadButton = new wxButton( this, 2, "Load", wxPoint( dirSelect->GetPosition().x + dirSelect->GetSize().x + xgap, 15 ), buttonsize );
	saveFileButton =  new wxButton( this, 3, "Save", wxPoint( loadButton->GetPosition().x + buttonsize.x + xgap, 15 ), buttonsize );
	
	

	queryArea = new wxTextCtrl(this, 4, wxEmptyString, wxPoint( start.x, start.y + ygap + saveFileButton->GetSize().y ) , dirSelectorSize );

	queryButton = new wxButton( this, 5, "Query", wxPoint( queryArea->GetPosition().x + queryArea->GetSize().x + xgap, queryArea->GetPosition().y ), buttonsize );


	resultsListArea = new wxListBox(this, 6, wxPoint(start.x, queryArea->GetPosition().y + queryArea->GetSize().y + ygap), wxSize(dirSelectorSize.x, 600) );
	
	
	
	

	dirSelect->SetFont( myf );
	loadButton->SetFont( myf );
	saveFileButton->SetFont( myf );
	queryArea->SetFont( myf );
	queryButton->SetFont( myf );
	resultsListArea->SetFont( myf );
}


Primary::~Primary() {
	
}



void Primary::on_dir_changed( wxFileDirPickerEvent& evt ) {
	
	std::string directory;
	std::vector < std::string > filenames;

	directory = dirSelect->GetPath().ToStdString();

	if(directory.empty()) {
		wxMessageDialog dlg( this, "Please select a directory" );
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	filenames = FileParser::get_filenames_from_directories(&directory);

	for(int i = 0; i < filenames.size(); i++) {
		if(!FileParser::parse_file( &filenames[i], &hashtable )) {
			wxMessageDialog dlg( this, "Failed top open" +  wxString(filenames[i]));
			dlg.Show();
			//std::cerr << "Failed to open file \"" << filenames[i] << "\"" << std::endl;
		}
	}

	hasLoadedIndex = true;

	evt.Skip();
}



void Primary::on_load_clicked( wxCommandEvent& evt ) {
	std::string indexFileName;


	wxFileDialog openFileDialog( this, _( "Load an Index file" ), "", "", "*", wxFD_OPEN | wxFD_FILE_MUST_EXIST );
	if(openFileDialog.ShowModal() == wxID_CANCEL) {
		evt.Skip();
		return;
	}   

	indexFileName = openFileDialog.GetPath().ToStdString();
	
	if(!FileParser::load_index_file( &indexFileName, &hashtable )) {
		wxMessageDialog dlg( this, "Could not load file");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	hasLoadedIndex = true;

	evt.Skip();
}



void Primary::on_save_clicked( wxCommandEvent& evt ) {

	if(!hasLoadedIndex) {
		wxMessageDialog dlg(this, "There is no index loaded");
		dlg.ShowModal();
		evt.Skip();
		return;
	}
	
	std::string filename;
	wxFileDialog saveFileDialog( this, _( "Save index file" ), "", "", "*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
	if(saveFileDialog.ShowModal() == wxID_CANCEL) {
		evt.Skip();
		return;
	}

	filename = saveFileDialog.GetPath().ToStdString();

	FileParser::write_index_file_to_drive( &filename, &hashtable );

	evt.Skip();
}



void Primary::on_query_clicked( wxCommandEvent& evt ) {
	std::string query;
	std::vector < wxString > documents;
	
	if(!hasLoadedIndex) {
		wxMessageDialog dlg( this, "There is no index loaded" );
		dlg.ShowModal();
		evt.Skip();
		return;
	}
	
	if(!queryArea->IsModified()) {
		wxMessageDialog dlg( this, "Type something to search for first" );
		dlg.ShowModal();
		evt.Skip();
		return;
	}
	
	query = queryArea->GetLineText(0).ToStdString();

	std::vector<RankedDocument> rdv = queryManager.query_index( &query );

	resultsListArea->Clear();

	for(int i = 0; i < rdv.size(); i++) {
		wxString entry(std::to_string(i) + ". " + rdv[i].filename);
		resultsListArea->AppendString(entry);
	}

	evt.Skip();
}



void Primary::OnSize(wxSizeEvent& event) {
	int xgap = 15;
	
	
	dirSelect->SetSize( wxSize( this->GetSize().x - 325, dirSelect->GetSize().y ));
	loadButton->SetPosition( wxPoint( dirSelect->GetPosition().x + dirSelect->GetSize().x + xgap, loadButton->GetPosition().y) );
	saveFileButton->SetPosition(wxPoint( loadButton->GetPosition().x + loadButton->GetSize().x + xgap, saveFileButton->GetPosition().y));
	

	queryArea->SetSize(wxSize(dirSelect->GetSize() ));
	queryButton->SetPosition( wxPoint( queryArea->GetPosition().x + queryArea->GetSize().x + xgap, queryButton->GetPosition().y ) );


	//resultsListArea->SetSize( wxSize( dirSelect->GetSize().x, this->GetSize().y - resultsListArea->GetPosition().y - 55 ) );
	resultsListArea->SetSize( wxSize( this->GetSize().x - 135, this->GetSize().y - resultsListArea->GetPosition().y - 55 ) );

	event.Skip();
}

