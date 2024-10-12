namespace WaolaWPF.DialogSystem;

public interface ICustomDialogViewModel : IDialogViewModel
{
	bool IsModal { get; }

	bool RequestClose();

	event EventHandler DialogClosing;
}