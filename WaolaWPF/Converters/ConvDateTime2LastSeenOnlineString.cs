using System.Globalization;
using System.Windows.Data;

namespace WaolaWPF.Converters;

[ValueConversion(typeof(DateTime), typeof(string))]
public class ConvDateTime2LastSeenOnlineString : IValueConverter
{
	private const string Prefix = "FormatSeenOnline";
	private const string DualSuffix = "Dual";
	private const string PluralSuffix = "s";

	public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
	{
		if (value is not DateTime lastSeenOnlineUtc)
		{
			throw new InvalidOperationException($"{value} is expected to be a {typeof(DateTime)}");
		}

		string lastSeenOnlineString;

		if (lastSeenOnlineUtc == DateTime.MinValue.ToUniversalTime())
		{
			lastSeenOnlineString = App.GetString("SeenOnlineNever");
		}
		else
		{
			var diff = DateTime.UtcNow - lastSeenOnlineUtc;
			var days = (int)diff.TotalDays;

			var years = (int)(days / 365.26);
			lastSeenOnlineString = years switch
			{
				> 2 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Year, years)), years),
				> 1 => string.Format(App.GetString("SeenOnlineOverYear"), years),
				_ => CheckDays(diff, days),
			};
		}

		return lastSeenOnlineString;
	}

	private static string CheckDays(TimeSpan diff, int days)
	{
		return days switch
		{
			> 182 => App.GetString("SeenOnlineLessThanYear"),
			> 155 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Month, days / 31)), days / 31),
			> 62 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Month, days / 31)), days / 31),
			> 31 => App.GetString("SeenOnlineOverMonth"),
			> 20 => App.GetString("SeenOnlineLessThanMonth"),
			> 14 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Week, days / 7)), days / 7),
			> 7 => App.GetString("SeenOnlineOverWeek"),
			> 5 => App.GetString("SeenOnlineLessThanWeek"),
			> 4 => App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Day, days)),
			> 2 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Day, days)), days),
			> 1 => App.GetString("SeenOnlineYesterday"),
			_ => CheckHours(diff),
		};
	}

	private static string CheckHours(TimeSpan diff)
	{
		var hours = (int)diff.TotalHours;

		return hours switch
		{
			> 12 => App.GetString("SeenOnlineLessThanDay"),
			> 4 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Hour, hours)), hours),
			> 2 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Hour, hours)), hours),
			> 1 => App.GetString("SeenOnlineOverHour"),
			_ => CheckMinutes(diff),
		};
	}

	private static string CheckMinutes(TimeSpan diff)
	{
		var minutes = diff.Minutes;
		if (diff.Seconds >= 30)
		{
			++minutes;
		}

		return minutes switch
		{
			> 30 => App.GetString("SeenOnlineLessThanHour"),
			> 4 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Minute, minutes)), minutes),
			>= 2 => string.Format(App.GetString(ChronoUnitsResourceStringCtor(ChronologicalUnit.Minute, minutes)), minutes),
			>= 1 => App.GetString("SeenOnlineOverMinute"),
			_ => CheckSeconds(diff)
		};
	}

	private static string CheckSeconds(TimeSpan diff)
	{
		return diff.Seconds >= 45 ?
			App.GetString("SeenOnlineLessThanMinute")
			: App.GetString("SeenOnlineJustNow");
	}

	public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
	{
		throw new NotImplementedException();
	}

	private static string ChronoUnitsResourceStringCtor(ChronologicalUnit unit, int value)
	{
		string resourceString;

		if (value is >= 5 and <= 20)
		{
			resourceString = string.Format($"{Prefix}{unit}{PluralSuffix}");
		}
		else
		{
			var remainder = value % 10;

			resourceString = remainder switch
			{
				1 => string.Format($"{Prefix}{unit}"),
				>= 2 and <= 4 => string.Format($"{Prefix}{unit}{DualSuffix}"),
				_ => string.Format($"{Prefix}{unit}{PluralSuffix}")
			};
		}

		return resourceString;
	}
}