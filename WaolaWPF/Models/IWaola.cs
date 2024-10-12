using WaolaCli;

using Task = System.Threading.Tasks.Task;

namespace WaolaWPF.Models
{
	public delegate void DelegateStateEvent(object sender,
		StateEventArgs stateEventArgs);
	public delegate Task DelegateVaultEventAsync(object sender,
		VaultEventArgs vaultEventArgs);

	public interface IWaola : IDisposable
	{
		event DelegateStateEvent? StateEvent;
		event DelegateVaultEventAsync? VaultEvent;

		IStatusMonitor StatusMonitor { get; }

		bool IsVaultEmpty { get; }

		Task DiscoverAsync();

		Task RefreshAsync();

		Task WakeUpAsync(IList<object> hostList);

		Task InitAsync();

		void CancelTask();

		Task AddNewHostAsync(HostData hostData);

		void DeleteHost(IHostView host);

		void RefreshHost(IHostView host);

		void Save();
	}
}
