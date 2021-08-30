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
EVT_BUTTON(4, on_clear_clicked)
EVT_TOGGLEBUTTON(5, on_case_clicked)
EVT_TOGGLEBUTTON(6, on_ommit_numbers_clicked)
EVT_TOGGLEBUTTON(7, on_ommit_special_clicked)
EVT_TOGGLEBUTTON(8, on_subdirectories_clicked)
EVT_TOGGLEBUTTON(10, on_extensions_clicked)
EVT_TEXT_ENTER(12, on_enter_pressed)
EVT_BUTTON(13, on_query_clicked)
EVT_BUTTON(14, on_copy_clicked)
wxEND_EVENT_TABLE()



Primary::Primary() : wxFrame(nullptr, 0, "SDIndex", wxDefaultPosition, wxSize(980, 800)) {

	wxPoint start(15, 15);
	wxSize buttonsize(85, 30);
	int  xgap = 15;
	int ygap = 30;
	wxSize dirSelectorSize(600, ygap);
	wxFont myf = get_font1();
	wxFont cbf = get_font1();
	wxFont itf = get_font1();

	Bind(wxEVT_SIZE, &Primary::OnSize, this);
	cbf.SetStrikethrough(true);

	itf.SetPointSize(12);

	dirSelect = new wxDirPickerCtrl(this, 1, wxEmptyString, wxDirSelectorPromptStr,
		start, dirSelectorSize, wxDIRP_DEFAULT_STYLE);

	dirSelect->GetTextCtrl()->SetEditable(false);

	loadButton = new wxButton(this, 2, wxString("Load"),
		wxPoint(dirSelect->GetPosition().x + dirSelect->GetSize().x + xgap, start.y),
		buttonsize);

	saveFileButton = new wxButton(this, 3, wxString("Save"),
		wxPoint(loadButton->GetPosition().x + buttonsize.x + xgap, start.y),
		buttonsize);

	clearButton = new wxButton(this, 4, wxString("Clear"),
		wxPoint(saveFileButton->GetPosition().x + buttonsize.x + xgap, start.y),
		buttonsize);

	caseToggleButton = new wxToggleButton(this, 5, wxString("Case"),
		wxPoint(start.x, dirSelect->GetPosition().y + dirSelect->GetSize().y + ygap),
		buttonsize);

	ommitNumbersToggleButton = new wxToggleButton(this, 6, wxString("Numbers"),
		wxPoint(caseToggleButton->GetPosition().x +
			caseToggleButton->GetSize().x + xgap,
			caseToggleButton->GetPosition().y),
		buttonsize);

	ommitSymbolsToggleButton = new wxToggleButton(this, 7, wxString("Symbols"),
		wxPoint(ommitNumbersToggleButton->GetPosition().x +
			ommitNumbersToggleButton->GetSize().x + xgap,
			ommitNumbersToggleButton->GetPosition().y),
		buttonsize);

	subdirectoriesToggleButton = new wxToggleButton(this, 8, wxString("Subdirs"),
		wxPoint(ommitSymbolsToggleButton->GetPosition().x +
			ommitSymbolsToggleButton->GetSize().x + xgap,
			ommitSymbolsToggleButton->GetPosition().y),
		buttonsize);

	extensionsArea = new wxTextCtrl(this, 9, wxEmptyString,
		wxPoint(subdirectoriesToggleButton->GetPosition().x + subdirectoriesToggleButton->GetSize().x + xgap,
			subdirectoriesToggleButton->GetPosition().y), wxSize(
				dirSelectorSize.x - subdirectoriesToggleButton->GetPosition().x
				- subdirectoriesToggleButton->GetSize().x - xgap, dirSelectorSize.y));

	extensionsToggleButton = new wxToggleButton(this, 10, wxString("Include"),
		wxPoint(wxPoint(loadButton->GetPosition().x, caseToggleButton->GetPosition().y)),
		wxSize(buttonsize));;

	instructions_text = new wxStaticText(this, 11, wxString("Select a directory to index"),
		wxPoint(saveFileButton->GetPosition().x,
			extensionsToggleButton->GetPosition().y), wxSize(buttonsize.x * 2 + xgap, buttonsize.y + ygap));

	caseToggleButton->SetValue(!toLowercase);
	ommitNumbersToggleButton->SetValue(!ommitNumbers);
	ommitSymbolsToggleButton->SetValue(!ommitSpecialCharacters);
	subdirectoriesToggleButton->SetValue(indexSubdirectories);
	extensionsToggleButton->SetValue(extensionsInclude);

	queryArea = new wxTextCtrl(this, 12, wxEmptyString,
		wxPoint(start.x, caseToggleButton->GetPosition().y + caseToggleButton->GetSize().y + ygap),
		dirSelectorSize, wxTE_PROCESS_ENTER);

	queryButton = new wxButton(this, 13, wxString("Query"),
		wxPoint(queryArea->GetPosition().x + queryArea->GetSize().x + xgap, queryArea->GetPosition().y),
		buttonsize);

	copyToClipboardButton = new wxButton(this, 14, wxString("Copy"),
		wxPoint(queryButton->GetPosition().x + queryButton->GetSize().x + xgap,
			queryArea->GetPosition().y),
		buttonsize);

	resultsListArea = new wxListBox(this, 15,
		wxPoint(start.x,
			queryArea->GetPosition().y + queryArea->GetSize().y + ygap),
		wxSize(dirSelectorSize.x, 600));

	optionsText = new wxStaticText(this, 16, wxString("Options"), wxPoint(dirSelect->GetPosition().x,
		dirSelect->GetPosition().y + dirSelect->GetSize().y + 5), wxSize(buttonsize.x, buttonsize.y - 5));

	extensionsText = new wxStaticText(this, 17, wxString("Extensions"), wxPoint(extensionsArea->GetPosition().x,
		dirSelect->GetPosition().y + dirSelect->GetSize().y + 5), wxSize(buttonsize.x, buttonsize.y - 5));

	searchText = new wxStaticText(this, 18, wxString("Search"), wxPoint(caseToggleButton->GetPosition().x,
		caseToggleButton->GetPosition().y + caseToggleButton->GetSize().y + 5), wxSize(buttonsize.x, buttonsize.y - 5));

	resultsText = new wxStaticText(this, 19, wxString("Results"), wxPoint(queryArea->GetPosition().x,
		queryArea->GetPosition().y + queryArea->GetSize().y + 5), wxSize(buttonsize.x, buttonsize.y - 5));


	dirSelect->SetFont(myf);
	loadButton->SetFont(myf);
	saveFileButton->SetFont(myf);
	clearButton->SetFont(myf);
	caseToggleButton->SetFont(myf);
	ommitNumbersToggleButton->SetFont(myf);
	ommitSymbolsToggleButton->SetFont(myf);
	subdirectoriesToggleButton->SetFont(myf);
	extensionsArea->SetFont(myf);
	extensionsToggleButton->SetFont(myf);
	instructions_text->SetFont(itf);
	queryArea->SetFont(myf);
	queryButton->SetFont(myf);
	copyToClipboardButton->SetFont(myf);
	resultsListArea->SetFont(myf);
	optionsText->SetFont(myf);
	extensionsText->SetFont(myf);
	searchText->SetFont(myf);
	resultsText->SetFont(myf);

	dirSelect->SetToolTip("Select a directory to index");
	loadButton->SetToolTip("Load a previously saved index file");
	saveFileButton->SetToolTip("Save index into a file");
	clearButton->SetToolTip("Clears index and all fields");
	caseToggleButton->SetToolTip("Choose whether to convert everything to lowercase");
	ommitNumbersToggleButton->SetToolTip("Choose whether to ommit indexing numbers");
	ommitSymbolsToggleButton->SetToolTip("Choose whether to ommit indexing symbols");
	subdirectoriesToggleButton->SetToolTip("Select whether sub directories are indexed");
	extensionsArea->SetToolTip("Type the extensions you want seperate with space \".txt .cpp\"");
	extensionsToggleButton->SetToolTip("Choose whether to include files with set extensions or exclude them");
	instructions_text->SetToolTip("Shows whether index has been loaded or not");
	queryArea->SetToolTip("Type what you want to search for");
	queryButton->SetToolTip("Click to search");
	copyToClipboardButton->SetToolTip("Copy a selected result to clipboard");
	resultsListArea->SetToolTip("Search results");


	if(toLowercase) caseToggleButton->SetFont(cbf);
	if(ommitNumbers) ommitNumbersToggleButton->SetFont(cbf);
	if(ommitSpecialCharacters) ommitSymbolsToggleButton->SetFont(cbf);
	if(!indexSubdirectories) subdirectoriesToggleButton->SetFont(cbf);
	if(!extensionsInclude)extensionsToggleButton->SetLabel("Exclude");

	instructions_text->SetForegroundColour(wxColour(wxT("BLACK")));
}



Primary::~Primary() {}



wxFont sdindex::Primary::get_font1() {

	return wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
}



void Primary::on_dir_changed(wxFileDirPickerEvent& evt) {

	std::string directory;
	std::vector < std::string > filenames;

	if(!wxDirExists(dirSelect->GetPath()))return;

	directory = dirSelect->GetPath().ToStdString();

	if(indexing) return;
	indexing = true;

	if(directory.empty()) {
		wxMessageDialog dlg(this, "Please select a directory");
		dlg.ShowModal();
		evt.Skip();
		indexing = false;
		return;
	}

	instructions_text->SetLabel("Indexing...");
	instructions_text->SetForegroundColour(wxColor(204, 102, 0));

	FileParser::allowExtensions = extensionsInclude;
	std::string extString = std::string(extensionsArea->GetLineText(0));
	extensions = FileParser::split_string(extString, ' ');

	if(indexSubdirectories) {
		hasLoadedIndex = FileParser::index_directory_and_subdirectories(directory, hashtable, extensions);
	} else {
		hasLoadedIndex = FileParser::index_directory(directory, hashtable, extensions);
	}

	indexing = false;

	instructions_text->SetLabel("Index Loaded!");
	instructions_text->SetForegroundColour(wxColor(0, 153, 0));

	evt.Skip();
}



void Primary::on_load_clicked(wxCommandEvent& evt) {

	std::string indexFileName;

	wxFileDialog openFileDialog(this, _("Load an Index file"),
		"", "", "*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if(openFileDialog.ShowModal() == wxID_CANCEL) {
		evt.Skip();
		return;
	}

	if(indexing) return;
	indexing = true;

	instructions_text->SetLabel("Loading index...");
	instructions_text->SetForegroundColour(wxColor(204, 102, 0));

	indexFileName = openFileDialog.GetPath().ToStdString();

	if(!FileParser::load_index_file(indexFileName, hashtable)) {
		wxMessageDialog dlg(this, "Could not load file");
		dlg.ShowModal();
		evt.Skip();
		indexing = false;
		return;
	}

	hasLoadedIndex = true;
	indexing = false;

	instructions_text->SetLabel("Index Loaded!");
	instructions_text->SetForegroundColour(wxColor(0, 51, 0));

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
		"", "", "*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if(saveFileDialog.ShowModal() == wxID_CANCEL) {
		evt.Skip();
		return;
	}

	filename = saveFileDialog.GetPath().ToStdString();

	FileParser::write_index_file_to_drive(filename, hashtable);

	evt.Skip();
}



void sdindex::Primary::on_clear_clicked(wxCommandEvent& evt) {

	hashtable.clear();
	queryArea->Clear();
	resultsListArea->Clear();
	dirSelect->SetPath("");
	extensionsArea->Clear();
	hasLoadedIndex = false;

	instructions_text->SetLabel("Index Cleared");
	instructions_text->SetForegroundColour(wxColor(255, 255, 255));
	evt.Skip();
}



void sdindex::Primary::on_case_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	toLowercase = !toLowercase;
	if(toLowercase) {
		cbf.SetStrikethrough(true);
	}
	caseToggleButton->SetFont(cbf);

	evt.Skip();
}



void sdindex::Primary::on_ommit_numbers_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	ommitNumbers = !ommitNumbers;
	if(ommitNumbers) {
		cbf.SetStrikethrough(true);
	}
	ommitNumbersToggleButton->SetFont(cbf);

	evt.Skip();
}



void sdindex::Primary::on_ommit_special_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	ommitSpecialCharacters = !ommitSpecialCharacters;
	if(ommitSpecialCharacters) {
		cbf.SetStrikethrough(true);
	}
	ommitSymbolsToggleButton->SetFont(cbf);

	evt.Skip();
}



void sdindex::Primary::on_subdirectories_clicked(wxCommandEvent& evt) {

	wxFont cbf = get_font1();

	indexSubdirectories = !indexSubdirectories;
	if(!indexSubdirectories) {
		cbf.SetStrikethrough(true);
	}
	subdirectoriesToggleButton->SetFont(cbf);

	evt.Skip();
}



void sdindex::Primary::on_extensions_clicked(wxCommandEvent& evt) {

	if(extensionsToggleButton->GetValue()) {
		extensionsToggleButton->SetLabel("Include");
		extensionsInclude = true;
	} else {
		extensionsToggleButton->SetLabel("Exclude");
		extensionsInclude = false;
	}
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

	std::vector<RankedDocument> rdv = queryManager.query_index(query);

	resultsListArea->Clear();

	for(int i = 0; i < rdv.size(); i++) {
		wxString entry(std::to_string(i) + ". " + rdv[i].filename);
		resultsListArea->AppendString(entry);
	}
	//evt.Skip();
}



void sdindex::Primary::on_copy_clicked(wxCommandEvent& evt) {

	bool hasSelection = false;

	if(resultsListArea->GetCount() == 0) {
		wxMessageDialog dlg(this, "No selected result to copy");
		dlg.ShowModal();
		evt.Skip();
		return;
	}

	if(wxTheClipboard->Open()) {
		for(unsigned int i = 0; i < resultsListArea->GetCount(); i++) {
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



void sdindex::Primary::on_enter_pressed(wxCommandEvent& evt) {

	on_query_clicked(evt);
}



void Primary::OnSize(wxSizeEvent& event) {

	int xgap = 15;

	dirSelect->SetSize(wxSize(this->GetSize().x - 345, dirSelect->GetSize().y));

	loadButton->SetPosition(
		wxPoint(dirSelect->GetPosition().x + dirSelect->GetSize().x + xgap,
			loadButton->GetPosition().y));

	saveFileButton->SetPosition(
		wxPoint(loadButton->GetPosition().x + loadButton->GetSize().x + xgap,
			saveFileButton->GetPosition().y));

	clearButton->SetPosition(
		wxPoint(saveFileButton->GetPosition().x + saveFileButton->GetSize().x + xgap,
			clearButton->GetPosition().y));

	extensionsArea->SetSize(wxSize(dirSelect->GetSize().x - subdirectoriesToggleButton->GetPosition().x
		- subdirectoriesToggleButton->GetSize().x, dirSelect->GetSize().y));

	extensionsToggleButton->SetPosition(wxPoint(loadButton->GetPosition().x,
		caseToggleButton->GetPosition().y));

	instructions_text->SetPosition(wxPoint(saveFileButton->GetPosition().x,
		extensionsToggleButton->GetPosition().y));

	queryArea->SetSize(wxSize(dirSelect->GetSize()));

	queryButton->SetPosition(
		wxPoint(queryArea->GetPosition().x + queryArea->GetSize().x + xgap,
			queryButton->GetPosition().y));

	copyToClipboardButton->SetPosition(
		wxPoint(queryButton->GetPosition().x + queryButton->GetSize().x + xgap,
			copyToClipboardButton->GetPosition().y));

	resultsListArea->SetSize(
		wxSize(this->GetSize().x - 45,
			this->GetSize().y - resultsListArea->GetPosition().y - 55));

	// Refreshes the window to remove artifacts from resizing
	Refresh();
}
