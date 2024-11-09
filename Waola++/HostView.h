#pragma once

#include "include/IHostView.h"

namespace Waola {

	class HostView : public IHostView
	{
	public:
		HostView(whost_view_t* const HostView);

		// IHostView implementation
		std::string GetHumanReadableId() const override;

		std::string GetDisplayName() const override;
		void SetDisplayName(const char* const displayName) override;

		std::string GetExactDisplayName() const override;

		std::string GetHostname() const override;
		void SetHostname(const char* const hostname) override;

		std::string GetIpAddressString() const	override;
		void SetIpAddressString(const char* const ipAddrString) override;

		std::string GetMacAddressString() const override;
		void SetMacAddressString(const char* const macAddrString) override;

		std::string GetLastSeenOnlineString() const override;

		const unsigned char* GetMacAddress() const override;

		time_t GetLastSeenOnline() const override;

		OpResult GetOpResult() const override;

		HostChangedField GetHostState() const override;
		void SetFieldChangeProcessed(HostChangedField changedFields) override;

		const HostId GetHostId() const override;
		IHostView* Clone() const override;

		std::string ToString() const override;

		bool Equals(const IHostView* const other) const override;
		
		void SetExtraData(const void* data, size_t size) override;
		void* GetExtraData() const override;
		
	public:
		~HostView() = default;

	private:
		HostView(const HostView&) = delete;
		HostView(HostView&&) = delete;
		HostView& operator=(const HostView&) = delete;
		HostView& operator=(HostView&&) = delete;

		static void AddToStream(std::stringstream& ss,
			const std::string& str, bool& prevValIsEmpty);

	private:
		whost_view_t* hostView;
	};
}
