/** @file eWindowPos.h
* .NET binding to eWindowPos.
*/

#pragma once

namespace eWindowPosCli {

	using System::Drawing::Rectangle;

	public ref class WindowPos
	{
	public:
		static bool AdjustWndRect(Rectangle% rcWindow, bool% adjusted);

	private:
		static bool AdjustWndRect(Rectangle% rcWindow,
			std::vector<rect_t*>& screens, bool% adjusted);
		static int GetTaskBarHeight();
	};
}