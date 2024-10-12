using WaolaCli;
using WaolaWPF.ViewModels;

namespace WaolaWPF.Extensions
{
	internal static class HostDataVmExtensions
	{
		internal static HostData GetHostData(this HostDataVm hostDataVm)
		{
			return new HostData(hostDataVm.DisplayName, hostDataVm.Hostname,
				hostDataVm.IpAddress, hostDataVm.MacAddress, DateTime.MinValue);
		}
	}
}
