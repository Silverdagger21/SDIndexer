#include "Primary.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/listbox.h>
#include <wx/clipbrd.h>


using namespace sdindex;


// Binding buttons to their functionality 

wxBEGIN_EVENT_TABLE(Primary, wxFrame)
EVT_DIRPICKER_CHANGED(1, on_dir_changed)
EVT_BUTTON(2, on_load_clicked)
EVT_BUTTON(3, on_save_clicked)
EVT_BUTTON(5, on_query_clicked)
EVT_BUTTON(7, on_clear_clicked)
EVT_TOGGLEBUTTON(8, on_to_lowercase_clicked)
EVT_TOGGLEBUTTON(9, on_ommit_numbers_clicked)
EVT_TOGGLEBUTTON(10, on_ommit_special_clicked)
EVT_BUTTON(11, on_copy_clicked)
wxEND_EVENT_TABLE()



Primary::Primary() : wxFrame(nullptr, 0, "SDIndex", wxDefaultPosition, wxSize(980, 800)) {

	wxPoint start(15, 15);
	wxSize buttonsize(80, 30);
	int  xgap = 15;
	int ygap = 30;
	wxSize dirSelectorSize(600, ygap);
	wxFont myf = get_font1();
	wxFont cbf = get_font1();

	Bind(wxEVT_SIZE, &Primary::OnSize, this);
	cbf.SetStrikethrough(true);

	dirSelect = new wxDirPickerCtrl(this, 1, wxEmptyString, wxDirSelectorPromptStr, start, dirSelectorSize);

	loadButton = new wxButton(this, 2, wxString("Load"),
		wxPoint(dirSelect->GetPosition().x+dirSelect->GetSize().x+xgap,
			start.y),
		buttonsize);

	saveFileButton = new wxButton(this, 3, wxString("Save"),
		wxPoint(loadButton->GetPosition().x+buttonsize.x+xgap,
			start.y),
		buttonsize);

	clearButton = new wxButton(this, 7, wxString("Clear"),
		wxPoint(saveFileButton->GetPosition().x+buttonsize.x+xgap,
			start.y),
		buttonsize);

	toLowercaseButton = new wxToggleButton(this, 8, wxString("all lowercase"),
		wxPoint(start.x,
			dirSelect->GetPosition().y+dirSelect->GetSize().y+ygap),
		wxSize(buttonsize.x+45, buttonsize.y));

	ommitNumbersButton = new wxToggleButton(this, 9, wxString("Numbers"),
		wxPoint(toLowercaseButton->GetPosition().x+
			toLowercaseButton->GetSize().x+xgap,
			toLowercaseButton->GetPosition().y),
		wxSize(buttonsize.x+45, buttonsize.y));

	ommitSymbolsButton = new wxToggleButton(this, 10, wxString("Symbols"),
		wxPoint(ommitNumbersButton->GetPosition().x+
			ommitNumbersButton->GetSize().x+xgap,
			ommitNumbersButton->GetPosition().y),
		wxSize(buttonsize.x+45, buttonsize.y));

	toLowercaseButton->SetValue(toLowercase);
	ommitNumbersButton->SetValue(!ommitNumbers);
	ommitSymbolsButton->SetValue(!ommitSpecialCharacters);

	queryArea = new wxTextCtrl(this, 4, wxEmptyString,
		wxPoint(start.x,
			toLowercaseButton->GetPosition().y+toLowercaseButton->GetSize().y+ygap),
		dirSelectorSize);

	queryButton = new wxButton(this, 5, wxString("Query"),
		wxPoint(queryArea->GetPosition().x+queryArea->GetSize().x+xgap,
			queryArea->GetPosition().y),
		buttonsize);

	copyToClipboardButton = new wxButton(this, 11, wxString("Copy"),
		wxPoint(queryButton->GetPosition().x+queryButton->GetSize().x+xgap,
			queryArea->GetPosition().y),
		buttonsize);



	resultsListArea = new wxListBox(this, 6,
		wxPoint(start.x,
			queryArea->GetPosition().y+queryArea->GetSize().y+ygap),
		wxSize(dirSelectorSize.x, 600));


	dirSelect->SetFont(myf);
	loadButton->SetFont(myf);
	saveFileButton->SetFont(myf);
	clearButton->SetFont(myf);
	toLowercaseButton->SetFont(myf);
	ommitNumbersButton->SetFont(myf);
	ommitSymbolsButton->SetFont(myf);
	queryArea->SetFont(myf);
	queryButton->SetFont(myf);
	copyToClipboardButton->SetFont(myf);
	resultsListArea->SetFont(myf);

	if(!toLowercase) toLowercaseButton->SetFont(cbf);
	if(ommitNumbers) ommitNumbersButton->SetFont(cbf);
	if(ommitSpecialCharacters) ommitSymbolsButton->SetFont(cbf);
}



Primary::~Primary() {}



wxFont sdindex::Primary::get_font1() {

	return wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
}



void Primary::on_dir_changed(wxFileDirPickerEvent& evt) {

	std::string directory;
	std::vector < std::string > filenames;

	directory = dirSelect->GetPath().ToStdString();

	if(directory.empty()) {
		wxMessageDialog dlg(this, "Please select a directory");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	filenames = FileParser::get_filenames_from_directories(&directory);
	FileParser::filter_files_by_extension(&filenames, &extensions);
	

	FileParser::toLowercase = toLowercase;
	FileParser::ommitNumbers = ommitNumbers;
	FileParser::ommitSpecialCharacters = ommitSpecialCharacters;

	for(int i = 0; i<filenames.size(); i++) {
		if(!FileParser::parse_file(&filenames[i], &hashtable)) {
			wxMessageDialog dlg(this, "Failed top open"+wxString(filenames[i]));
			dlg.Show();
		}
	}

	hasLoadedIndex = true;

	evt.Skip();
}



void Primary::on_load_clicked(wxCommandEvent& evt) {

	std::string indexFileName;

	wxFileDialog openFileDialog(this, _("Load an Index file"),
		"", "", "*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if(openFileDialog.ShowModal()==wxID_CANCEL) {
		evt.Skip();
		return;
	}

	indexFileName = openFileDialog.GetPath().ToStdString();

	if(!FileParser::load_index_file(&indexFileName, &hashtable)) {
		wxMessageDialog dlg(this, "Could not load file");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	hasLoadedIndex = true;

	evt.Skip();
}



void Primary::on_save_clicked(wxCommandEvent& evt) {

	if(!hasLoadedIndex) {
		wxMessageDialog dlg(this, "There is no index loaded");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	std::string filename;
	wxFileDialog saveFileDialog(this, _("Save index file"),
		"", "", "*", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if(saveFileDialog.ShowModal()==wxID_CANCEL) {
		evt.Skip();
		return;
	}

	filename = saveFileDialog.GetPath().ToStdString();

	FileParser::write_index_file_to_drive(&filename, &hashtable);

	evt.Skip();
}



void sdindex::Primary::on_clear_clicked(wxCommandEvent& evt) {

	hashtable.clear();
	queryArea->Clear();
	resultsListArea->Clear();
	dirSelect->SetPath("");
	hasLoadedIndex = false;
	evt.Skip();
}



void sdindex::Primary::on_to_lowercase_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	if(toLowercase) {
		cbf.SetStrikethrough(true);
	}
	toLowercaseButton->SetFont(cbf);
	toLowercase = !toLowercase;
	evt.Skip();
}



void sdindex::Primary::on_ommit_numbers_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	if(!ommitNumbers) {
		cbf.SetStrikethrough(true);
	}
	ommitNumbersButton->SetFont(cbf);
	ommitNumbers = !ommitNumbers;
	evt.Skip();
}



void sdindex::Primary::on_ommit_special_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	if(!ommitSpecialCharacters) {
		cbf.SetStrikethrough(true);
	}
	ommitSymbolsButton->SetFont(cbf);
	ommitSpecialCharacters = !ommitSpecialCharacters;
	evt.Skip();
}



void Primary::on_query_clicked(wxCommandEvent& evt) {

	std::string query;
	std::vector < wxString > documents;

	if(!hasLoadedIndex) {
		wxMessageDialog dlg(this, "There is no index loaded");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	if(!queryArea->IsModified()) {
		wxMessageDialog dlg(this, "Type something to search for first");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	query = queryArea->GetLineText(0).ToStdString();

	queryManager.convertToLowercase = toLowercase;
	queryManager.ommitNumbers = ommitNumbers;
	queryManager.ommitSpecialCharacters = ommitSpecialCharacters;

	std::vector<RankedDocument> rdv = queryManager.query_index(&query);

	resultsListArea->Clear();

	for(int i = 0; i<rdv.size(); i++) {
		wxString entry(std::to_string(i)+". "+rdv[i].filename);
		resultsListArea->AppendString(entry);
	}

	evt.Skip();
}



void sdindex::Primary::on_copy_clicked(wxCommandEvent& evt) {

	bool hasSelection = false;

	if(resultsListArea->GetCount()==0) {
		wxMessageDialog dlg(this, "No selected result to copy");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	if(wxTheClipboard->Open()) {
		for(unsigned int i = 0; i<resultsListArea->GetCount(); i++) {
			if(resultsListArea->IsSelected(i)) {
				wxTheClipboard->SetData(
					new wxTextDataObject(resultsListArea->GetString(i).AfterFirst(' ')));

				hasSelection = true;
				break;
			}
		}
		wxTheClipboard->Close();
	}

	if(!hasSelection) {
		wxMessageDialog dlg(this, "Select something from the list");
		dlg.ShowModal();
	}
	evt.Skip();
}



void Primary::OnSize(wxSizeEvent& event) {

	int xgap = 15;

	dirSelect->SetSize(wxSize(this->GetSize().x-330, dirSelect->GetSize().y));

	loadButton->SetPosition(
		wxPoint(dirSelect->GetPosition().x+dirSelect->GetSize().x+xgap,
			loadButton->GetPosition().y));

	saveFileButton->SetPosition(
		wxPoint(loadButton->GetPosition().x+loadButton->GetSize().x+xgap,
			saveFileButton->GetPosition().y));

	clearButton->SetPosition(
		wxPoint(saveFileButton->GetPosition().x+saveFileButton->GetSize().x+xgap,
			clearButton->GetPosition().y));

	queryArea->SetSize(wxSize(dirSelect->GetSize()));

	queryButton->SetPosition(
		wxPoint(queryArea->GetPosition().x+queryArea->GetSize().x+xgap,
			queryButton->GetPosition().y));

	copyToClipboardButton->SetPosition(
		wxPoint(queryButton->GetPosition().x+queryButton->GetSize().x+xgap,
			copyToClipboardButton->GetPosition().y));

	resultsListArea->SetSize(
		wxSize(this->GetSize().x-45,
			this->GetSize().y-resultsListArea->GetPosition().y-55));

	event.Skip();
}
