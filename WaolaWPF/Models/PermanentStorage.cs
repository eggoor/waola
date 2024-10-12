using Microsoft.Win32;
using WaolaCli;

namespace WaolaWPF.Models
{
	class PermanentStorage : IPermanentStorage
	{
		private const string eSoft = "eSoft";
		private const string WaolaWPF = "WaolaWPF";
		private const string Hosts = "Hosts";
		private const string DisplayName = "DisplayName";
		private const string Hostname = "Hostname";
		private const string IpAddr = "IpAddr";
		private const string MacAddr = "MacAddr";
		private const string LastSeenOnline = "LastSeenOnline";

		private delegate void RegistryActionDelegate(RegistryKey key, IScanner scanner);

		public void Load(IScanner scanner)
		{
			OpenRegistryKey(DoLoad, scanner);
		}

		public void Save(IScanner scanner)
		{
			OpenRegistryKey(DoSave, scanner);
		}

		private static void OpenRegistryKey(RegistryActionDelegate action, IScanner scanner)
		{
			using var kSoftware = Registry.CurrentUser.OpenSubKey("Software", true);

			if (kSoftware == null)
			{
				throw new ApplicationException($"Registry.CurrentUser.OpenSubKey(\"Software\", true) failed");
			}

			using var kESoft = kSoftware.CreateSubKey(eSoft);
			if (kESoft == null)
			{
				throw new ApplicationException($"egKey.CreateSubKey(eSoft) failed");
			}

			using var kWaolaWPF = kESoft.CreateSubKey(WaolaWPF);
			if (kWaolaWPF == null)
			{
				throw new ApplicationException($"egKey.OpenSubKey(kWaolaWPF, true) failed");
			}

			using var kHosts = kWaolaWPF.CreateSubKey(Hosts);
			if (kHosts == null)
			{
				throw new ApplicationException($"egKey.OpenSubKey(kHosts, true) failed");
			}

			action(kHosts, scanner);
		}

		private void DoLoad(RegistryKey kHosts, IScanner scanner)
		{
			var hostDataList = new List<HostData>();

			foreach (var hostKeyName in kHosts.GetSubKeyNames())
			{
				using var kHost = kHosts.OpenSubKey(hostKeyName, true);

				if (kHost == null)
				{
					continue;
				}

				var displayName = Convert.ToString(kHost.GetValue(DisplayName));
				var hostName = Convert.ToString(kHost.GetValue(Hostname));
				var ipAddr = Convert.ToString(kHost.GetValue(IpAddr));
				var macAddr = Convert.ToString(kHost.GetValue(MacAddr));
				var lastSeenOnline = Convert.ToDateTime(kHost.GetValue(LastSeenOnline));

				var hostData = new HostData(displayName, hostName,
					ipAddr, macAddr, lastSeenOnline);

				hostDataList.Add(hostData);
			}

			scanner.BatchAdd(hostDataList);
		}

		private void DoSave(RegistryKey kHosts, IScanner scanner)
		{
			int i = 0;

			CleanUp(kHosts);

			foreach (var host in scanner.Hosts)
			{
				var kHost = kHosts.CreateSubKey(i++.ToString());

				if (kHost == null)
				{
					continue;
				}

				if (!string.IsNullOrEmpty(host.DirectDisplayName))
				{
					kHost.SetValue(DisplayName, host.DirectDisplayName);
				}

				if (!string.IsNullOrEmpty(host.Hostname))
				{
					kHost.SetValue(Hostname, host.Hostname);
				}

				if (!string.IsNullOrEmpty(host.IpAddressString))
				{
					kHost.SetValue(IpAddr, host.IpAddressString);
				}

				if (!string.IsNullOrEmpty(host.MacAddressString))
				{
					kHost.SetValue(MacAddr, host.MacAddressString);
				}

				kHost.SetValue(LastSeenOnline, host.LastSeenOnline);
			}
		}

		private static void CleanUp(RegistryKey kHosts)
		{
			foreach (var hostKeyName in kHosts.GetSubKeyNames())
			{
				kHosts.DeleteSubKey(hostKeyName);
			}
		}
	}
}