using WaolaWPF.ViewModels;

namespace WaolaWPF.Extensions
{
	public static class IListExtensions
	{
		public static IReadOnlyList<IntPtr> ToHostViewIdList(this IList<object>? list)
		{
			return list != null ? list.Cast<KeyValuePair<IntPtr, HostVm>>().Select(kvp => kvp.Key).ToList() : [];
		}

		public static IReadOnlyList<HostVm> ToHostViewModelList(this IList<object>? list)
		{
			return list != null ? list.Cast<KeyValuePair<IntPtr, HostVm>>().Select(kvp => kvp.Value).ToList() : [];
		}
	}
}
