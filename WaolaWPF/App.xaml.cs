using Microsoft.Extensions.DependencyInjection;
using System.Windows;
using WaolaCli;
using WaolaWPF.Models;
using WaolaWPF.Properties;
using WaolaWPF.ViewModels;

using IWaola = WaolaWPF.Models.IWaola;

namespace WaolaWPF
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	public partial class App : Application
	{
		private readonly ServiceProvider serviceProvider = MakeServiceProvider();

		public void OnStartup(object sender, StartupEventArgs e)
		{
			UpdateSettings();

			var mainView = serviceProvider.GetRequiredService<MainWindow>();
			mainView.Show();
		}

		protected override void OnExit(ExitEventArgs e)
		{
			serviceProvider.Dispose();
			base.OnExit(e);
		}

		private static ServiceProvider MakeServiceProvider()
		{
			var serviceCollection = new ServiceCollection();
			ConfigureServices(serviceCollection);

			return serviceCollection.BuildServiceProvider();
		}

		private static void ConfigureServices(IServiceCollection serviceCollection)
		{
			serviceCollection.AddSingleton<IScanner, Scanner>();
			serviceCollection.AddSingleton<WaolaCli.IWaola, WaolaCli.Waola>();
			serviceCollection.AddSingleton<IStatusMonitor, StatusMonitor>();
			serviceCollection.AddSingleton<IPermanentStorage, PermanentStorage>();
			serviceCollection.AddSingleton<IWaola, Models.Waola>();
			serviceCollection.AddSingleton<MasterVm>();
			serviceCollection.AddSingleton<MainWindow>();
			serviceCollection.AddSingleton<DefaultVm>();
			serviceCollection.AddSingleton<App>();
		}

		private static void UpdateSettings()
		{
			//this defaults to true when a new version of this software has been released
			if (Settings.Default.IsSettingsUpgradeRequired)
			{
				Settings.Default.Upgrade();
				Settings.Default.Reload();
				Settings.Default.IsSettingsUpgradeRequired = false;
				Settings.Default.Save();
			}
		}

		public static string GetString(string stringKey)
		{
			var stringResource = Current.FindResource(stringKey);
			return stringResource != null ? (string)stringResource : stringKey;
		}
	}
}
