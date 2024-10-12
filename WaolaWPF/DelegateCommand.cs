using System.Windows.Input;

namespace WaolaWPF;

public class DelegateCommand : ICommand
{
	private readonly Predicate<object?>? canExecute;
	private readonly Action<object?> execute;

	public DelegateCommand(Action<object?> execute,
		Predicate<object?>? canExecute = null)
	{
		ArgumentNullException.ThrowIfNull(execute);
		this.execute = execute;
		this.canExecute = canExecute;
	}

	public event EventHandler? CanExecuteChanged
	{
		add
		{
			CommandManager.RequerySuggested += value;
		}

		remove
		{
			CommandManager.RequerySuggested -= value;
		}
	}

	public bool CanExecute(object? parameter)
	{
		return canExecute?.Invoke(parameter) != false;
	}

	public void Execute(object? parameter)
	{
		execute(parameter);
	}
}