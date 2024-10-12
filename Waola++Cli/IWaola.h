#pragma once

#include "IHostView.h"

using namespace System;
using namespace System::Collections::Generic;

namespace WaolaCli {

	public interface class IWaola : public IDisposable
	{
		void WaolaH(IReadOnlyList<IHostView^>^ hostList);
	};
}
