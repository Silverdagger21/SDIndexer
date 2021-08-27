#pragma once
#include <wx/wx.h>
#include "Primary.h"

namespace sdindex {

	// This class is the entry point of the UI
	class WinEntry : public wxApp {

		public:

		WinEntry();
		~WinEntry();


		// Initializes the frame
		virtual bool OnInit() override;


		private:


		Primary* pFrame = nullptr;
	};
}
