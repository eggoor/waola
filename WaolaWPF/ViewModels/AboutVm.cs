using System.Diagnostics;
using System.Windows.Input;

namespace WaolaWPF.ViewModels
{
	public class AboutVm : ViewModelBase
	{
		public AboutVm()
		{
			CommandHyperlinkClick = new DelegateCommand(OnCommandHyperlinkClick);
		}

		public ICommand CommandHyperlinkClick { get; }

		private void OnCommandHyperlinkClick(object? obj)
		{
			if (obj is Uri uri)
			{
				var sInfo = new ProcessStartInfo(uri.ToString())
				{
					UseShellExecute = true,
				};

				Process.Start(sInfo);
			}
		}
	}
}
