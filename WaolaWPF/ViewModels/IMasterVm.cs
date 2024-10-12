namespace WaolaWPF.ViewModels;

public interface IMasterVm
{
	Task AddNewHostAsync(HostDataVm hostDataVm);

	void RefreshHost(HostDataVm hostDataVm);

	void EditSelectedHost(object? obj);
}
