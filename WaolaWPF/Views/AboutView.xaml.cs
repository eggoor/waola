using System.IO;
using System.Windows;
using System.Windows.Controls;

namespace WaolaWPF.Views
{
	/// <summary>
	/// Interaction logic for AboutView.xaml
	/// </summary>
	public partial class AboutView : UserControl
	{
		public AboutView()
		{
			InitializeComponent();
		}

		private void LoadLicense()
		{
			var license = Properties.Resources.License;
			var stream = new MemoryStream(license);
			LicenseRichTextBox.Selection.Load(stream, DataFormats.Rtf);
			LicenseRichTextBox.Selection.Select(LicenseRichTextBox.Selection.Start,
				LicenseRichTextBox.Selection.Start);
		}

		private void OnLicenseRichTextBoxLoaded(object sender, RoutedEventArgs e)
		{
			LoadLicense();
		}
	}
}
