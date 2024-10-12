using System.Windows;

namespace WaolaWPF.DialogSystem
{
	internal class MessageBoxPresenter : IDialogBoxPresenter<MessageBoxViewModel>
	{
		public void Show(MessageBoxViewModel viewModel)
		{
			viewModel.Result = MessageBox.Show(viewModel.Message,
				viewModel.Caption, viewModel.Buttons, viewModel.Image);
		}
	}
}
