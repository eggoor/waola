using System.Windows;
using System.Windows.Documents;
using System.Windows.Input;

namespace WaolaWPF.Extensions
{
	public static class HyperlinkExtensions
	{
		private static void OnCommandPropertyChanged(DependencyObject sender, DependencyPropertyChangedEventArgs args)
		{
			if (sender is Hyperlink hyperlink)
			{
				hyperlink.Click -= OnHyperlinkClick;


				if (args.NewValue is ICommand command)
				{
					hyperlink.Click += OnHyperlinkClick;
				}
			}
		}

		private static void OnHyperlinkClick(object sender, RoutedEventArgs e)
		{
			if (sender is Hyperlink hyperlink)
			{
				var command = GetCommand(hyperlink);

				if (command != null)
				{
					command.Execute(hyperlink.NavigateUri);
				}
			}
		}

		// Using a DependencyProperty as the backing store for Command. This enables animation, styling, binding, etc...
		public static readonly DependencyProperty CommandProperty = DependencyProperty.RegisterAttached("Command",
			typeof(ICommand), typeof(HyperlinkExtensions), new PropertyMetadata(null, OnCommandPropertyChanged));

		public static ICommand GetCommand(DependencyObject obj)
		{
			return (ICommand)obj.GetValue(CommandProperty);
		}

		public static void SetCommand(DependencyObject obj, ICommand value)
		{
			obj.SetValue(CommandProperty, value);
		}
	}
}
