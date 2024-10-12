using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Windows.Input;
using WaolaCli;
using WaolaWPF.DialogSystem;
using WaolaWPF.Extensions;

using IWaola = WaolaWPF.Models.IWaola;

namespace WaolaWPF.ViewModels;

public class DefaultVm : DialogSystemViewModelBase, IDisposable
{
	private readonly IWaola model;
	private readonly object hostListLock = new();
	private IMasterVm? masterVm;
	private IList<object>? selectedHosts = null;
	private bool disposed;

	public DefaultVm(IWaola model)
	{
		this.model = model ?? throw new ArgumentNullException(nameof(model));
		model.VaultEvent += OnVaultEventAsync;
		CommandWakeUp = new DelegateCommand(OnCommandWakeUpAsync);
		CommandCopy = new DelegateCommand(OnCommandCopy);
		CommandEdit = new DelegateCommand(OnCommandEdit);
		CommandDelete = new DelegateCommand(OnCommandDelete);
	}

	public ObservableDictionary<IntPtr, HostVm> HostList { get; } = new();

	public IList<object>? SelectedHosts
	{
		get => selectedHosts;
		set
		{
			if (value != selectedHosts)
			{
				selectedHosts = value;
				RaisePropertyChanged();
			}
		}
	}

	internal void SetMasterVm(IMasterVm masterVm)
	{
		this.masterVm = masterVm;
	}

	private async Task OnVaultEventAsync(object sender, VaultEventArgs vaultEventArgs)
	{
		switch (vaultEventArgs.OpCode)
		{
			case VaultOperation.Added:
				await Application.Current.Dispatcher.InvokeAsync(() =>
				{
					lock (hostListLock)
					{
						HostList.Add(vaultEventArgs.HostView.Id, new HostVm(vaultEventArgs.HostView));
					}
				});
				break;
			case VaultOperation.Modified:
				await Application.Current.Dispatcher.InvokeAsync(() =>
				{
					lock (hostListLock)
					{
						if (HostList.TryGetValue(vaultEventArgs.HostView.Id, out var hostVm))
						{
							hostVm.RaiseIfPropertyChanged();
						}
					}
				});
				break;
			case VaultOperation.Deleted:
				await Application.Current.Dispatcher.InvokeAsync(() =>
				{
					lock (hostListLock)
					{
						HostList.Remove(vaultEventArgs.HostView.Id);
					}
				});
				break;
			default:
				throw new InvalidEnumArgumentException(nameof(vaultEventArgs.OpCode),
					(int)vaultEventArgs.OpCode, vaultEventArgs.OpCode.GetType());
		}
	}

	#region Commands

	public ICommand CommandWakeUp { get; }

	public ICommand CommandCopy { get; }

	public ICommand CommandEdit { get; }

	public ICommand CommandDelete { get; }

	#endregion Commands

	#region Command handlers

	private async void OnCommandWakeUpAsync(object? obj)
	{
		await WakeUpAsync(obj);
	}

	internal void OnCommandCopy(object? obj)
	{
		CopySelectedHosts();
	}

	private void OnCommandEdit(object? obj)
	{
		masterVm?.EditSelectedHost(obj);
	}

	internal void OnCommandDelete(object? obj)
	{
		var selectedVms = SelectedHosts.ToHostViewModelList();
		var caption = $"{App.GetString("KeywordShure")}?";
		var message = selectedVms.Count > 1
			? App.GetString("DeleteSeveralHostsConfirmation")
			: string.Format(App.GetString("DeleteHostConfirmationFormat"),
			selectedVms[0].HumanReadableId);

		MessageBoxViewModel messageBox = new MessageBoxViewModel(message, caption)
		{
			Buttons = MessageBoxButton.YesNo,
			Image = MessageBoxImage.Question
		};

		if (messageBox.Show(Dialogs) == MessageBoxResult.Yes)
		{
			foreach (var vm in selectedVms)
			{
				model.DeleteHost(vm.HostView);
			}
		}
	}

	#endregion Command handlers

	internal async Task WakeUpAsync(object? obj)
	{
		if (SelectedHosts != null)
		{
			await model.WakeUpAsync(SelectedHosts);

			foreach (var vm in SelectedHosts.ToHostViewModelList())
			{
				vm.RaiseIfPropertyChanged();
			}
		}
	}

	private void CopySelectedHosts()
	{
		if (SelectedHosts != null)
		{
			StringBuilder sb = new();

			foreach (var vm in SelectedHosts.ToHostViewModelList())
			{
				sb.AppendLine(vm.HostView.ToString());
			}

			Clipboard.SetText(sb.ToString());
		}
	}

	protected virtual void Dispose(bool disposing)
	{
		if (!disposed)
		{
			disposed = true;

			model.VaultEvent -= OnVaultEventAsync;

			if (disposing)
			{
				var viewModelList = HostList.Values;

				HostList.Clear();

				foreach (var hostView in viewModelList)
				{
					hostView.Dispose();
				}

				model.Dispose();
			}
		}
	}

	public void Dispose()
	{
		// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
		Dispose(disposing: true);
		GC.SuppressFinalize(this);
	}
}
