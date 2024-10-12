using System.Windows;

namespace WaolaWPF.DialogSystem;

public class MessageBoxViewModel : IDialogViewModel
{
	public string Caption { get; set; }

	public string Message { get; set; }

	public MessageBoxButton Buttons { get; set; } = MessageBoxButton.OK;

	public MessageBoxImage Image { get; set; } = MessageBoxImage.None;

	public MessageBoxResult Result { get; set; }

	public MessageBoxViewModel(string message, string caption)
	{
		Message = message;
		Caption = caption;
	}

	public MessageBoxResult Show(IList<IDialogViewModel> collection)
	{
		collection.Add(this);
		return Result;
	}
}
