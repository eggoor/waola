using System.Collections.ObjectModel;
using WaolaWPF.ViewModels;

namespace WaolaWPF.DialogSystem;

public class DialogSystemViewModelBase : ViewModelBase
{
	public ObservableCollection<IDialogViewModel> Dialogs { get; } = new();
}