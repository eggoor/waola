using System.Drawing;
using System.Windows;
using WaolaWPF.Properties;
using WaolaWPF.ViewModels;

namespace WaolaWPF
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow(MasterVm viewModel)
		{
			DataContext = viewModel ?? throw new ArgumentNullException(nameof(viewModel));

			InitializeComponent();

			SetWindowPos();
		}

		private void SetWindowPos()
		{
			if (!EnsureWindowInscribed())
			{
				WindowStartupLocation = WindowStartupLocation.CenterScreen;
			}
		}

		private bool EnsureWindowInscribed()
		{
			bool ensured = false;

			if (Settings.Default.MainWindowSizeWidth > 0
				&& Settings.Default.MainWindowSizeHeight > 0)
			{

				var rcWindow = MakeWindowRect();

				bool adjusted = false;
				ensured = eWindowPosCli.WindowPos.AdjustWndRect(ref rcWindow, ref adjusted);

				if (ensured && adjusted)
				{
					UpdateSettings(rcWindow);
				}
			}

			return ensured;
		}

		private Rectangle MakeWindowRect()
		{
			int left, top;
			CompositionHelper.ToPixels(this, Settings.Default.MainWindowPosX,
				 Settings.Default.MainWindowPosY, out left, out top);

			int width, height;
			CompositionHelper.ToPixels(this, Settings.Default.MainWindowSizeWidth,
				Settings.Default.MainWindowSizeHeight, out width, out height);
			var location = new System.Drawing.Point(left, top);
			var size = new System.Drawing.Size(width, height);

			return new Rectangle(location, size);
		}

		private void UpdateSettings(Rectangle rcWindow)
		{
			double left, top;
			CompositionHelper.FromPixels(this, rcWindow.Left,
				 rcWindow.Top, out left, out top);

			double width, height;
			CompositionHelper.FromPixels(this, rcWindow.Width,
				rcWindow.Height, out width, out height);

			Settings.Default.MainWindowPosX = left;
			Settings.Default.MainWindowPosY = top;
			Settings.Default.MainWindowSizeWidth = width;
			Settings.Default.MainWindowSizeHeight = height;

			Settings.Default.Save();
		}

		private void OnClosed(object? sender, EventArgs e)
		{
			Settings.Default.Save();
		}
	}
}