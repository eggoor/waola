using System.Windows;
using System.Windows.Input;
using WaolaCli;
using WaolaWPF.Extensions;

using IWaola = WaolaWPF.Models.IWaola;

namespace WaolaWPF.ViewModels;

public class MasterVm : ViewModelBase, IMasterVm, IDisposable
{
	private readonly IWaola model;
	private DefaultVm defaultVm;
	private string status = string.Empty;
	private WaolaTask state = WaolaTask.Idle;
	private ViewModelBase currentPage;
	private IHostView? selectedHost = null;
	private bool disposed;

	public MasterVm(IWaola model, DefaultVm defaultVm)
	{
		this.model = model ?? throw new ArgumentNullException(nameof(model));
		this.defaultVm = defaultVm ?? throw new ArgumentNullException(nameof(defaultVm));
		currentPage = this.defaultVm;

		model.StateEvent += OnStateEvent;
		model.StatusMonitor.StatusUpdated += OnStatusUpdated;

		CommandInit = new DelegateCommand(OnCommandInitAsync);
		CommandWakeUp = new DelegateCommand(OnCommandWakeUpAsync, CanWakeUp);
		CommandRescanNetwork = new DelegateCommand(OnCommandRescanNetworkAsync, CanRescan);
		CommandRefresh = new DelegateCommand(OnCommandRefreshAsync, CanRefresh);
		CommandAdd = new DelegateCommand(OnCommandAdd, CanAdd);
		CommandCopy = new DelegateCommand(OnCommandCopy, CanCopy);
		CommandEdit = new DelegateCommand(OnCommandEdit, CanEdit);
		CommandDelete = new DelegateCommand(OnCommandDelete, CanDelete);
		CommandAbout = new DelegateCommand(OnCommandAbout);
		CommandSave = new DelegateCommand(OnCommandSave);

		defaultVm.SetMasterVm(this);
	}

	public ViewModelBase CurrentPage
	{
		get => currentPage;
		set
		{
			if (value != currentPage)
			{
				currentPage = value;
				RaisePropertyChanged();
			}
		}
	}

	public string Status
	{
		get => status;

		set
		{
			if (value != status)
			{
				status = value;
				RaisePropertyChanged();
			}
		}
	}

	public WaolaTask State
	{
		get => state;

		set
		{
			if (value != state)
			{
				state = value;
				RaisePropertyChanged();
				RaisePropertyChanged(nameof(IsScanning));
				RaisePropertyChanged(nameof(IsRefreshing));
			}
		}
	}

	public bool IsScanning => (State & WaolaTask.Discover) != 0;

	public bool IsRefreshing => (State & WaolaTask.Refresh) != 0;

	internal void ResetCurrentPage()
	{
		if (CurrentPage != defaultVm)
		{
			CurrentPage = defaultVm;
		}
	}

	public async Task AddNewHostAsync(HostDataVm hostDataVm)
	{
		ResetCurrentPage();

		Mouse.OverrideCursor = Cursors.AppStarting;

		await model.AddNewHostAsync(hostDataVm.GetHostData());

		Mouse.OverrideCursor = Cursors.Arrow;
	}

	public void RefreshHost(HostDataVm hostDataVm)
	{
		if (selectedHost == null)
		{
			throw new InvalidOperationException($"{nameof(selectedHost)} must not be null at {nameof(RefreshHost)}");
		}

		ResetCurrentPage();

		Mouse.OverrideCursor = Cursors.Wait;

		selectedHost.DisplayName = hostDataVm.DisplayName;
		selectedHost.Hostname = hostDataVm.Hostname;
		selectedHost.IpAddressString = hostDataVm.IpAddress;
		selectedHost.MacAddressString = hostDataVm.MacAddress;

		model.RefreshHost(selectedHost);

		selectedHost = null;

		Mouse.OverrideCursor = Cursors.Arrow;
	}

	public void EditSelectedHost(object? obj)
	{
		selectedHost = defaultVm.SelectedHosts.ToHostViewModelList()[0].HostView;
		CurrentPage = new HostDataVm(this)
		{
			Mode = HostViewMode.Edit,
			DisplayName = selectedHost.DirectDisplayName,
			Hostname = selectedHost.Hostname,
			IpAddress = selectedHost.IpAddressString,
			MacAddress = selectedHost.MacAddressString
		};
	}

	#region Commands

	public ICommand CommandInit { get; }

	public ICommand CommandRescanNetwork { get; }

	public ICommand CommandRefresh { get; }

	public ICommand CommandWakeUp { get; }

	public ICommand CommandAdd { get; }

	public ICommand CommandCopy { get; }

	public ICommand CommandEdit { get; }

	public ICommand CommandDelete { get; }

	public ICommand CommandAbout { get; }

	public ICommand CommandSave { get; }

	#endregion Commands

	#region Command handlers

	private async void OnCommandInitAsync(object? obj)
	{
		await model.InitAsync();
	}

	private async void OnCommandWakeUpAsync(object? obj)
	{
		if (CurrentPage != defaultVm)
		{
			ResetCurrentPage();
		}
		else
		{
			await defaultVm.WakeUpAsync(obj);
		}
	}

	private bool CanWakeUp(object? obj)
	{
		return CurrentPage != defaultVm
			|| (CurrentPage == defaultVm && defaultVm.SelectedHosts?.Count > 0);
	}

	private async void OnCommandRescanNetworkAsync(object? obj)
	{
		if ((State & WaolaTask.Discover) == 0)
		{
			await model.DiscoverAsync();
		}
		else
		{
			model.CancelTask();
		}
	}

	private bool CanRescan(object? obj) => CurrentPage == defaultVm && !IsRefreshing;

	private async void OnCommandRefreshAsync(object? obj)
	{
		ResetCurrentPage();

		if ((State & WaolaTask.Refresh) == 0)
		{
			await model.RefreshAsync();
		}
		else
		{
			model.CancelTask();
		}
	}

	private bool CanRefresh(object? obj) => CurrentPage == defaultVm && !IsScanning;

	private void OnCommandCopy(object? obj)
	{
		defaultVm.OnCommandCopy(obj);
	}

	private bool CanCopy(object? obj)
	{
		return CurrentPage == defaultVm && defaultVm.SelectedHosts?.Count > 0;
	}

	private void OnCommandAdd(object? obj)
	{
		CurrentPage = new HostDataVm(this) { Mode = HostViewMode.Add };
	}

	private bool CanAdd(object? obj)
	{
		return CurrentPage == defaultVm;
	}

	internal void OnCommandEdit(object? obj)
	{
		EditSelectedHost(obj);
	}

	private bool CanEdit(object? obj)
	{
		return CurrentPage == defaultVm && defaultVm.SelectedHosts?.Count == 1;
	}

	private void OnCommandDelete(object? obj)
	{
		defaultVm.OnCommandDelete(obj);
	}

	private bool CanDelete(object? obj)
	{
		return CurrentPage == defaultVm && defaultVm.SelectedHosts?.Count > 0;
	}

	private void OnCommandAbout(object? obj)
	{
		CurrentPage = new AboutVm();
	}

	private void OnCommandSave(object? obj)
	{
		model.Save();
	}

	#endregion Command handlers

	#region Model event handlers

	private void OnStateEvent(object sender, StateEventArgs stateEventArgs)
	{
		if (stateEventArgs.Tasks != WaolaTask.Idle)
		{
			model.StatusMonitor.Start();
		}
		else
		{
			model.StatusMonitor.Stop();
		}

		Application.Current.Dispatcher.Invoke(() =>
		{
			State = stateEventArgs.Tasks;

			Mouse.OverrideCursor = State == WaolaTask.Idle
			? Cursors.Arrow
			: Cursors.AppStarting;
		});
	}

	private void OnStatusUpdated(object sender, StatusEventArgs statusEventArgs)
	{
		Status = statusEventArgs.Status;
	}

	#endregion Model event handlers

	#region IDisposable implementation

	protected virtual void Dispose(bool disposing)
	{
		if (!disposed)
		{
			disposed = true;

			model.StatusMonitor.StatusUpdated -= OnStatusUpdated;

			if (disposing)
			{
				defaultVm.Dispose();
			}

			model.StateEvent -= OnStateEvent;
		}
	}

	public void Dispose()
	{
		Dispose(disposing: true);
		GC.SuppressFinalize(this);
	}

	#endregion IDisposable implementation
}
