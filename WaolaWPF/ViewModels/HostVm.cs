using System.Windows.Threading;
using WaolaCli;

namespace WaolaWPF.ViewModels;

public class HostVm
	: ViewModelBase
	, IEquatable<HostVm>
	, IDisposable
{
	private readonly IHostView hostView;
	private readonly DispatcherTimer timer = new();
	private bool disposed;

	public HostVm(IHostView hostView)
	{
		this.hostView = hostView ?? throw new ArgumentNullException(nameof(hostView));

		timer.Tick += OnTimerTick;
		timer.Interval = TimeSpan.FromSeconds(30);
		timer.Start();
	}

	public IHostView HostView => hostView;

	public string DisplayName
	{
		get => hostView.DisplayName ?? string.Empty;

		set
		{
			if (value != hostView.DisplayName)
			{
				hostView.DisplayName = value;
				RaisePropertyChanged();
			}
		}
	}

	public string Hostname
	{
		get => hostView.Hostname ?? string.Empty;

		set
		{
			if (value != hostView.Hostname)
			{
				hostView.Hostname = value;
				RaisePropertyChanged();
			}
		}
	}

	public string IpAddressString
	{
		get => hostView.IpAddressString ?? string.Empty;

		set
		{
			if (value != hostView.IpAddressString)
			{
				hostView.IpAddressString = value;
				RaisePropertyChanged();
			}
		}
	}

	public string MacAddressString
	{
		get => hostView.MacAddressString ?? string.Empty;

		set
		{
			if (value != hostView.MacAddressString)
			{
				hostView.MacAddressString = value;
				RaisePropertyChanged();
			}
		}
	}

	public string HumanReadableId => hostView.HumanReadableId;


	public DateTime LastSeenOnline => hostView.LastSeenOnline;

	public OpResult OpResult => hostView.Result;

	public void RaiseIfPropertyChanged()
	{
		if (hostView == null)
		{
			return;
		}

		if ((hostView.State & HostChangedField.DisplayName) != 0)
		{
			RaisePropertyChanged(nameof(DisplayName));
			hostView.SetFieldChangeProcessed(HostChangedField.DisplayName);
		}

		if ((hostView.State & HostChangedField.Hostname) != 0)
		{
			RaisePropertyChanged(nameof(Hostname));
			hostView.SetFieldChangeProcessed(HostChangedField.Hostname);
		}

		if ((hostView.State & HostChangedField.IpAddress) != 0)
		{
			RaisePropertyChanged(nameof(IpAddressString));
			hostView.SetFieldChangeProcessed(HostChangedField.IpAddress);
		}

		if ((hostView.State & HostChangedField.MacAddress) != 0)
		{
			RaisePropertyChanged(nameof(MacAddressString));
			hostView.SetFieldChangeProcessed(HostChangedField.MacAddress);
		}

		if ((hostView.State & HostChangedField.LastSeenOnline) != 0)
		{
			RaisePropertyChanged(nameof(LastSeenOnline));
			hostView.SetFieldChangeProcessed(HostChangedField.LastSeenOnline);
		}

		if ((hostView.State & HostChangedField.OpResult) != 0)
		{
			RaisePropertyChanged(nameof(OpResult));
			hostView.SetFieldChangeProcessed(HostChangedField.OpResult);
		}
	}

	public override bool Equals(object? obj)
	{
		return obj != null && Equals(obj as HostVm);
	}

	public bool Equals(HostVm? other)
	{
		return other != null
			&& HostView.Equals(other.HostView);
	}

	public override int GetHashCode()
	{
		return HostView.GetHashCode();
	}

	private void OnTimerTick(object? sender, EventArgs e)
	{
		RaisePropertyChanged(nameof(LastSeenOnline));
	}
	protected virtual void Dispose(bool disposing)
	{
		if (!disposed)
		{
			disposed = true;

			if (disposing)
			{
				timer.Stop();
				timer.Tick -= OnTimerTick;
				hostView.Dispose();
			}
		}
	}

	public void Dispose()
	{
		Dispose(disposing: true);
		GC.SuppressFinalize(this);
	}
}
