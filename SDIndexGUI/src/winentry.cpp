#include "winentry.h"


using namespace sdindexer;

wxIMPLEMENT_APP( WinEntry );

WinEntry::WinEntry(){}

WinEntry::~WinEntry() {}

bool WinEntry::OnInit() {

	pFrame = new Primary();
	pFrame->Show();
	return true;
}
