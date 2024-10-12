#pragma once

using namespace System::Threading::Tasks;

namespace WaolaCli {

	template<typename TNativeResult, typename TManagedResult>
	class Feature2TaskHelper
	{
	public:
		using NativeResultT = TNativeResult;
		using ManagedResultT = TManagedResult;

		template<typename TNativeResult, typename TManagedResult>
		static void SetCompletionSource(std::future<TNativeResult>& future,
			TaskCompletionSource<TManagedResult>^ completionResult)
		{
			try
			{
				completionResult->SetResult(future.get());
			}
			catch (const std::exception& err)
			{
				completionResult->SetException(gcnew System::IO::IOException(gcnew System::String(err.what())));
			}
		}
	};

	template<>
	class Feature2TaskHelper<void, void>
	{
	public:
		using NativeResultT = void;

		static void SetCompletionSource(std::future<void>& future,
			TaskCompletionSource^ completionResult)
		{
			try
			{
				future.get();
				completionResult->SetResult();
			}
			catch (const std::exception& err)
			{
				completionResult->SetException(gcnew System::IO::IOException(gcnew System::String(err.what())));
			}
		}
	};
}
