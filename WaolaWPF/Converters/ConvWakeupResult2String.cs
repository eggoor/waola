using System.Globalization;
using System.Windows.Data;
using WaolaCli;

namespace WaolaWPF.Converters
{
	[ValueConversion(typeof(OpResult), typeof(string))]
	public class ConvWakeupResult2String : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value is not OpResult result)
			{
				throw new InvalidOperationException($"{value} is expected to be a {typeof(HostViewMode)}");
			}

			if (result == OpResult.Undefined)
			{
				return string.Empty;
			}

			return result == OpResult.Success
				? App.GetString("KeywordSuccess")
				: App.GetString("KeywordFail");
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
