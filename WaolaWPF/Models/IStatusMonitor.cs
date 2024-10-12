namespace WaolaWPF.Models
{
	public delegate void DelegateStatusEvent(object sender, StatusEventArgs statusEventArgs);

	public interface IStatusMonitor
	{
		event DelegateStatusEvent? StatusUpdated;
		void Start();
		void Stop();
	}
}
