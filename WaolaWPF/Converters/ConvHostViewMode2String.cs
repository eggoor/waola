using System.Globalization;
using System.Windows.Data;

namespace WaolaWPF.Converters
{
	[ValueConversion(typeof(HostViewMode), typeof(string))]
	public class ConvHostViewMode2String : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value is not HostViewMode mode)
			{
				throw new InvalidOperationException($"{value} is expected to be a {typeof(HostViewMode)}");
			}

			if (mode == HostViewMode.Undefined)
			{
				throw new ArgumentOutOfRangeException(nameof(mode));
			}

			return mode == HostViewMode.Add
				? App.GetString("KeywordAddHost")
				: App.GetString("KeywordEditHost");
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
