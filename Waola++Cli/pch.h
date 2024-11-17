#pragma once

#include <string>
#include <stdexcept>
#include <functional>
#include <forward_list>
#include <future>

#include <msclr/gcroot.h>

#include "framework.h"

#include <WinSock2.h>

#include <EventCallbackData.h>
#include <IHostView.h>
#include <IScanner.h>
#include <IWaola.h>

#define nameof(x) (#x)
