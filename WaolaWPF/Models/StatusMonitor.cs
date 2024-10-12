using System.ComponentModel;
using System.Net;
using System.Windows.Threading;
using WaolaCli;

namespace WaolaWPF.Models
{
	class StatusMonitor : IStatusMonitor
	{
		private const int UpdatesPerSecond = 25;
		private readonly IScanner scanner;
		private readonly DispatcherTimer timer = new();
		private Module lastModule = Module.Undefined;
		private int lastOpCode = 0;
		private object? lastStatusData = null;
		private bool updated;
		private string status = string.Empty;
		private bool running;

		public StatusMonitor(IScanner scanner)
		{
			this.scanner = scanner ?? throw new ArgumentNullException(nameof(scanner));
			timer.Tick += OnTimerTick;
			timer.Interval = TimeSpan.FromMicroseconds(1000 / UpdatesPerSecond);
		}

		public event DelegateStatusEvent? StatusUpdated;

		public void Start()
		{
			if (!running)
			{
				running = true;
				UpdateStatus();
				timer.Start();
			}
		}

		public void Stop()
		{
			if (running)
			{
				running = false;
				timer.Stop();
				UpdateStatus();
			}
		}

		private void OnTimerTick(object? sender, EventArgs e)
		{
			UpdateStatus();
		}

		private void UpdateStatus()
		{
			Module module = Module.Undefined;
			int opCode = 0;

			var statusData = scanner.GetStatus(ref module, ref opCode);
			ProcessStatus(module, opCode, statusData);
		}

		private void ProcessStatus(Module module, int opCode, object statusData)
		{
			if (lastModule != module)
			{
				updated = true;
				lastModule = module;
			}

			if (lastOpCode != opCode)
			{
				if (!updated)
				{
					updated = true;
				}

				lastOpCode = opCode;
			}

			if (module == Module.Undefined)
			{
				status = App.GetString("StatusError");
			}
			else if (updated || module != Module.Scanner)
			{
				ProcessUpdate(statusData);
			}

			if (updated)
			{
				StatusUpdated?.Invoke(this, new StatusEventArgs(status));
				updated = false;
			}
		}

		private void ProcessUpdate(object statusData)
		{
			switch (lastModule)
			{
				case Module.Scanner:
					ProcessScannerUpdate();
					break;
				case Module.Discoverer:
					ProcessDiscovererUpdate(statusData);
					break;
				case Module.Resolver:
					ProcessResolverUpdate(statusData);
					break;
				case Module.Undefined:
				default:
					throw new InvalidEnumArgumentException(nameof(lastModule),
						(int)lastModule, lastModule.GetType());
			}
		}

		private void ProcessScannerUpdate()
		{
			status = (OpCodeScanner)lastOpCode switch
			{
				OpCodeScanner.Idle => App.GetString("StatusScannerIdle"),
				OpCodeScanner.Discovering => App.GetString("StatusScannerDiscovering"),
				OpCodeScanner.Resolving => App.GetString("StatusScannerResolving"),
				OpCodeScanner.Cancelling => App.GetString("StatusScannerCancelling"),
				_ => throw new InvalidEnumArgumentException(nameof(lastOpCode),
										lastOpCode, lastOpCode.GetType()),
			};
		}

		private void ProcessDiscovererUpdate(object statusData)
		{
			switch ((OpCodeDiscoverer)lastOpCode)
			{
				case OpCodeDiscoverer.SchedulingArp:
				case OpCodeDiscoverer.SendingArp:
				case OpCodeDiscoverer.GotArp:
				case OpCodeDiscoverer.ArpFailed:
					OnDiscovererIpAddress(statusData);
					break;
				case OpCodeDiscoverer.Undefined:
				default:
					throw new InvalidEnumArgumentException(nameof(lastOpCode),
						lastOpCode, lastOpCode.GetType());
			}
		}

		private void ProcessResolverUpdate(object statusData)
		{
			switch ((OpCodeResolver)lastOpCode)
			{
				case OpCodeResolver.GetAddrInfo:
				case OpCodeResolver.GetAddrInfoFailed:
				case OpCodeResolver.GotNameInfo:
					OnResolverHostName(statusData);
					break;
				case OpCodeResolver.GotAddrInfo:
				case OpCodeResolver.GetNameInfo:
				case OpCodeResolver.GetNameInfoFailed:
				case OpCodeResolver.SendingArp:
				case OpCodeResolver.GotArp:
				case OpCodeResolver.ArpFailed:
					OnResolverIpAddress(statusData);
					break;
				case OpCodeResolver.Undefined:
				default:
					throw new InvalidEnumArgumentException(nameof(lastOpCode),
						lastOpCode, lastOpCode.GetType());
			}
		}

		private void OnResolverHostName(object statusData)
		{
			ArgumentNullException.ThrowIfNull(statusData);

			if (lastStatusData != statusData)
			{
				lastStatusData = statusData;
				if (!updated)
				{
					updated = true;
				}
			}

			if (updated)
			{
				var formatKey = ResolverOpCode2StringFormatKey();
				status = string.Format(formatKey, lastStatusData as string);
			}
		}

		private void OnResolverIpAddress(object statusData)
		{
			ArgumentNullException.ThrowIfNull(statusData);

			if (lastStatusData != statusData)
			{
				lastStatusData = statusData;
				if (!updated)
				{
					updated = true;
				}
			}

			if (updated)
			{
				var formatKey = ResolverOpCode2StringFormatKey();
				var ipAddress = new IPAddress((uint)lastStatusData);
				status = string.Format(formatKey, ipAddress.ToString());
			}
		}

		private void OnDiscovererIpAddress(object statusData)
		{
			ArgumentNullException.ThrowIfNull(statusData);

			if (lastStatusData != statusData)
			{
				lastStatusData = statusData;
				if (!updated)
				{
					updated = true;
				}
			}

			if (updated)
			{
				var formatKey = DiscovererOpCode2StringFormatKey();
				var ipAddress = new IPAddress((uint)lastStatusData);
				status = string.Format(formatKey, ipAddress.ToString());
			}
		}

		private string DiscovererOpCode2StringFormatKey()
		{
			return (OpCodeDiscoverer)lastOpCode switch
			{
				OpCodeDiscoverer.SchedulingArp => App.GetString("StatusSchedulingArp"),
				OpCodeDiscoverer.SendingArp => App.GetString("StatusSendingArp"),
				OpCodeDiscoverer.GotArp => App.GetString("StatusGotArp"),
				OpCodeDiscoverer.ArpFailed => App.GetString("StatusArpFailed"),
				OpCodeDiscoverer.Undefined => throw new InvalidEnumArgumentException(nameof(lastOpCode),
														lastOpCode, lastOpCode.GetType()),
				_ => throw new InvalidEnumArgumentException(nameof(lastOpCode),
														lastOpCode, lastOpCode.GetType()),
			};
		}

		private string ResolverOpCode2StringFormatKey()
		{
			return (OpCodeResolver)lastOpCode switch
			{
				OpCodeResolver.SendingArp => App.GetString("StatusSendingArp"),
				OpCodeResolver.GotArp => App.GetString("StatusGotArp"),
				OpCodeResolver.ArpFailed => App.GetString("StatusArpFailed"),
				OpCodeResolver.GetAddrInfo => App.GetString("StatusResolverGetAddrInfoFormat"),
				OpCodeResolver.GotAddrInfo => App.GetString("StatusResolverGotAddrInfoFormat"),
				OpCodeResolver.GetAddrInfoFailed => App.GetString("StatusResolverGetAddrInfoFailedFormat"),
				OpCodeResolver.GetNameInfo => App.GetString("StatusResolverGetNameInfoFormat"),
				OpCodeResolver.GotNameInfo => App.GetString("StatusResolverGotNameInfoFormat"),
				OpCodeResolver.GetNameInfoFailed => App.GetString("StatusResolverGetNameInfoFailedFormat"),
				OpCodeResolver.Undefined => throw new InvalidEnumArgumentException(nameof(lastOpCode),
														lastOpCode, lastOpCode.GetType()),
				_ => throw new InvalidEnumArgumentException(nameof(lastOpCode),
														lastOpCode, lastOpCode.GetType()),
			};
		}
	}
}