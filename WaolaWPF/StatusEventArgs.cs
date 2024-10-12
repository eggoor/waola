namespace WaolaWPF;

public class StatusEventArgs(string status) : EventArgs
{
	public string Status { get; set; } = status ?? throw new ArgumentNullException(nameof(status));
}