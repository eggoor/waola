#pragma once

using namespace msclr;

using namespace System::Threading::Tasks;

namespace WaolaCli {

    template<typename T>
    struct ClassOf {};

    template<typename Return, typename Class>
    struct ClassOf<Return(Class::*)> {
        using type = Class;
    };

    template<typename TNativeCallable, typename THelper, typename... Args>
    Task^ Feature2Task(TNativeCallable nativeMethod,
        typename ClassOf<decltype(nativeMethod)>::type* const self, Args&&... args)
    {
        gcroot<TaskCompletionSource^> taskCompletionSrc = gcnew TaskCompletionSource();

        auto future = (* self.*nativeMethod)(std::forward<Args>(args)...);
        std::thread([taskCompletionSrc](std::future<typename THelper::NativeResultT> future)
            {
                THelper::SetCompletionSource(future, taskCompletionSrc);
            }, std::move(future)).detach();

        return taskCompletionSrc->Task;
    }
}
