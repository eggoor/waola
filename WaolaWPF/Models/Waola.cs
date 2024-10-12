using WaolaCli;
using WaolaWPF.Extensions;

namespace WaolaWPF.Models
{
	public class Waola : IWaola
	{
		private readonly IScanner scanner;
		private readonly WaolaCli.IWaola waola;
		private readonly IPermanentStorage permanentStorage;

		public Waola(IScanner scanner, WaolaCli.IWaola waola,
			IStatusMonitor statusMonitor, IPermanentStorage permanentStorage)
		{
			this.scanner = scanner ?? throw new ArgumentNullException(nameof(scanner));
			this.waola = waola ?? throw new ArgumentNullException(nameof(waola));
			StatusMonitor = statusMonitor ?? throw new ArgumentNullException(nameof(statusMonitor));
			this.permanentStorage = permanentStorage ?? throw new ArgumentNullException(nameof(permanentStorage));

			scanner.StateEvent += OnStateEvent;
			scanner.VaultEvent += OnVaultEvent;

			if (PlatformHelper.Is32BitProcess)
			{
				scanner.SetMaxThreads(700);
			}
		}

		public event DelegateStateEvent? StateEvent;
		public event DelegateVaultEventAsync? VaultEvent;

		public IStatusMonitor StatusMonitor { get; }

		#region IMainWindowModel implementation

		public bool IsVaultEmpty => scanner.IsVaultEmpty;

		public async Task InitAsync()
		{
			permanentStorage.Load(scanner);

			if (IsVaultEmpty)
			{
				await DiscoverAsync();
			}
		}

		public async Task DiscoverAsync()
		{
			await scanner.DiscoverAsync();
		}

		public async Task RefreshAsync()
		{
			await scanner.RefreshAsync();
		}

		public async Task WakeUpAsync(IList<object> list)
		{
			var hostList = list.ToHostViewModelList().Select(vm => vm.HostView).ToList();
			await Task.Factory.StartNew(() => waola.WaolaH(hostList));
		}

		public void CancelTask()
		{
			scanner.CancelTask();
		}

		public void Save()
		{
			permanentStorage.Save(scanner);
		}

		public async Task AddNewHostAsync(HostData hostData)
		{
			await scanner.AddAsync(hostData);
		}

		public void DeleteHost(IHostView host)
		{
			scanner.DeleteHost(host);
		}

		public void RefreshHost(IHostView host)
		{
			scanner.RefreshHost(host);
		}

		#endregion IMainWindowModel implementation

		private void OnStateEvent(object sender, StateEventArgs stateEventArgs)
		{
			StateEvent?.Invoke(this, stateEventArgs);
		}

		private void OnVaultEvent(object sender, VaultEventArgs vaultEventArgs)
		{
			VaultEvent?.Invoke(this, vaultEventArgs);
		}

		#region IDisposable implementation

		private bool disposed;

		protected virtual void Dispose(bool disposing)
		{
			if (!disposed)
			{
				disposed = true;

				scanner.VaultEvent -= OnVaultEvent;

				if (scanner.GetCurrentTasks() != WaolaTask.Idle)
				{
					StatusMonitor.Stop();
					scanner.CancelTask();
				}

				if (disposing)
				{
					scanner.Dispose();
					waola.Dispose();
				}

				scanner.StateEvent -= OnStateEvent;
			}
		}

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		#endregion IDisposable implementation
	}
}
