namespace WaolaWPF.DialogSystem;

public class CustomDialogViewModelBase(bool isModal) : DialogSystemViewModelBase, ICustomDialogViewModel
{

	#region ICustomDialogViewModel Implementation

	public bool IsModal { get; } = isModal;

	public virtual bool RequestClose()
	{
		var closed = false;

		if (CloseConfirmation == null
			|| CloseConfirmation?.Invoke() == true)
		{
			Close();
			closed = true;
		}

		return closed;
	}

	public event EventHandler? DialogClosing;

	#endregion ICustomDialogViewModel Implementation

	#region CustomDialogViewModel Implementation

	public Func<bool>? CloseConfirmation { get; set; }

	public void Close()
	{
		DialogClosing?.Invoke(this, System.EventArgs.Empty);
	}

	#endregion CustomDialogViewModel Implementation
}