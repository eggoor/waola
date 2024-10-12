using System.ComponentModel;
using System.Windows.Input;

namespace WaolaWPF.ViewModels;

public class HostDataVm : ViewModelBase
{
	private readonly MasterVm mainWindowVm;
	private string displayName = string.Empty;
	private string hostName = string.Empty;
	private string ipAddress = string.Empty;
	private string macAddress = string.Empty;

	public HostDataVm(MasterVm mainWindowVm)
	{
		this.mainWindowVm = mainWindowVm ?? throw new ArgumentNullException(nameof(mainWindowVm));
		CommandSave = new DelegateCommand(OnSave);
		CommandCancel = new DelegateCommand(OnCancel);
	}

	private void OnCancel(object? obj)
	{
		mainWindowVm.ResetCurrentPage();
	}

	private async void OnSave(object? obj)
	{
		switch (Mode)
		{
			case HostViewMode.Add:
				await mainWindowVm.AddNewHostAsync(this);
				break;
			case HostViewMode.Edit:
				mainWindowVm.RefreshHost(this);
				break;
			case HostViewMode.Undefined:
			default:
				throw new InvalidEnumArgumentException(nameof(Mode),
					(int)Mode, Mode.GetType());
		}
	}

	public HostViewMode Mode { get; set; }

	public ICommand CommandSave { get; }

	public ICommand CommandCancel { get; }

	public string DisplayName
	{
		get => displayName;
		set
		{
			if (value != displayName)
			{
				displayName = value;
				RaisePropertyChanged();
			}
		}
	}

	public string Hostname
	{
		get => hostName;
		set
		{
			if (value != hostName)
			{
				hostName = value;
				RaisePropertyChanged();
			}
		}
	}

	public string IpAddress
	{
		get => ipAddress;
		set
		{
			if (value != ipAddress)
			{
				ipAddress = value;
				RaisePropertyChanged();
			}
		}
	}

	public string MacAddress
	{
		get => macAddress;
		set
		{
			if (value != macAddress)
			{
				macAddress = value;
				RaisePropertyChanged();
			}
		}
	}
}
