namespace WaolaWPF.DialogSystem;

public interface IDialogBoxPresenter<in T> where T : IDialogViewModel
{
	void Show(T viewModel);
}