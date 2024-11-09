/** @file IHostView.h
* Waola network host view interface.

Represents simple host model view which converts
host's data from and to their string representation.
*/

#pragma once

/*
Include in precompiled header:
#include <string>
*/

#include "HostField.h"
#include "HostChangedField.h"
#include "OpResult.h"

namespace Waola {

	typedef void* HostId;

	class IHostView
	{
	public:
		virtual std::string GetHumanReadableId() const = 0;

		virtual std::string GetDisplayName() const = 0;
		virtual void SetDisplayName(const char* const displayName) = 0;

		virtual std::string GetExactDisplayName() const = 0;

		virtual std::string GetHostname() const = 0;
		virtual void SetHostname(const char* const hostname) = 0;

		virtual std::string GetIpAddressString() const = 0;
		virtual void SetIpAddressString(const char* const ipAddressString) = 0;

		virtual std::string GetMacAddressString() const = 0;
		virtual void SetMacAddressString(const char* const macAddressString) = 0;

		virtual std::string GetLastSeenOnlineString() const = 0;

		virtual const unsigned char* GetMacAddress() const = 0;

		virtual time_t GetLastSeenOnline() const = 0;

		virtual OpResult GetOpResult() const = 0;

		virtual HostChangedField GetHostState() const = 0;
		virtual void SetFieldChangeProcessed(HostChangedField changedFields) = 0;

		virtual const HostId GetHostId() const = 0;
		virtual IHostView* Clone() const = 0;

		virtual std::string ToString() const = 0;

		virtual bool Equals(const IHostView* const other) const = 0;
		
		virtual void SetExtraData(const void* data, size_t size) = 0;
		virtual void* GetExtraData() const = 0;

		static HostChangedField HostFieldToChangedField(HostField hf);

	public:
		virtual ~IHostView() = default;

	protected:
		IHostView() = default;

	private:
		IHostView(const IHostView&) = delete;
		IHostView(IHostView&&) = delete;
		IHostView& operator=(const IHostView&) = delete;
		IHostView& operator=(IHostView&&) = delete;
	};
}
