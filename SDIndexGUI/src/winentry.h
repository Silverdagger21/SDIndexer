#pragma once
#include <wx/wx.h>
#include "Primary.h"

namespace sdindexer {

	class WinEntry : public wxApp{
		
		public:

		WinEntry();
		~WinEntry();

		virtual bool OnInit();

		private:

		Primary* pFrame = nullptr;
	};
}
