using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows;

namespace WaolaWPF.DialogSystem;

public static class DialogBehavior
{
	private const string DialogViewModelsPropertyName = "DialogViewModels";
	private const string ClosingPropertyName = "Closing";
	private const string ClosedPropertyName = "Closed";

	private static readonly Dictionary<Window, NotifyCollectionChangedEventHandler> ChangeNotificationHandlers = new();

	private static readonly Dictionary<ObservableCollection<IDialogViewModel>, List<IDialogViewModel>> DialogBoxViewModels = new();

	private static readonly Dictionary<IDialogViewModel, Window> DialogBoxes = new();

	public static readonly DependencyProperty ClosingProperty = DependencyProperty.RegisterAttached(ClosingPropertyName,
		typeof(bool), typeof(DialogBehavior), new PropertyMetadata(false));

	public static readonly DependencyProperty ClosedProperty = DependencyProperty.RegisterAttached(ClosedPropertyName,
		typeof(bool), typeof(DialogBehavior), new PropertyMetadata(false));

	public static readonly DependencyProperty DialogViewModelsProperty = DependencyProperty.RegisterAttached(DialogViewModelsPropertyName,
		typeof(object), typeof(DialogBehavior), new PropertyMetadata(null, OnDialogViewModelsChange));

	public static void SetDialogViewModels(DependencyObject source, object value)
	{
		source.SetValue(DialogViewModelsProperty, value);
	}

	public static object GetDialogViewModels(DependencyObject source)
	{
		return source.GetValue(DialogViewModelsProperty);
	}

	private static void OnDialogViewModelsChange(DependencyObject d, DependencyPropertyChangedEventArgs e)
	{
		if (d is not Window owner)
		{
			owner = Application.Current.MainWindow;
		}

		owner.Closed += (_, _) => ChangeNotificationHandlers.Remove(owner);

		if (!ChangeNotificationHandlers.ContainsKey(owner))
		{
			ChangeNotificationHandlers[owner] = (sender, args) =>
			{
				if (sender is ObservableCollection<IDialogViewModel> collection)
				{
					switch (args.Action)
					{
						case NotifyCollectionChangedAction.Add:
						case NotifyCollectionChangedAction.Remove:
						case NotifyCollectionChangedAction.Replace:
							{
								if (args.NewItems != null)
									foreach (IDialogViewModel viewModel in args.NewItems)
									{
										if (!DialogBoxViewModels.ContainsKey(collection))
										{
											DialogBoxViewModels[collection] = new List<IDialogViewModel>();
										}

										DialogBoxViewModels[collection].Add(viewModel);
										AddDialog(viewModel, collection, owner);
									}

								if (args.OldItems != null)
								{
									foreach (IDialogViewModel viewModel in args.OldItems)
									{
										RemoveDialog(viewModel);
										DialogBoxViewModels[collection].Remove(viewModel);

										if (DialogBoxViewModels[collection].Count == 0)
										{
											DialogBoxViewModels.Remove(collection);
										}
									}
								}
								break;
							}
						case NotifyCollectionChangedAction.Reset:
							{
								if (DialogBoxViewModels.ContainsKey(collection))
								{
									foreach (var viewModel in DialogBoxViewModels[collection])
									{
										RemoveDialog(viewModel);
									}

									DialogBoxViewModels.Remove(collection);
								}

								break;
							}
						case NotifyCollectionChangedAction.Move:
							// Do nothing
							break;
						default:
							throw new InvalidEnumArgumentException(nameof(args.Action), (int)args.Action,
								args.Action.GetType());
					}
				}
			};
		}

		if (e.NewValue is ObservableCollection<IDialogViewModel> newCollection)
		{
			newCollection.CollectionChanged += ChangeNotificationHandlers[owner];

			foreach (var viewModel in newCollection.ToList())
			{
				AddDialog(viewModel, newCollection, owner);
			}
		}

		if (e.OldValue is ObservableCollection<IDialogViewModel> oldCollection)
		{
			oldCollection.CollectionChanged -= ChangeNotificationHandlers[owner];
			foreach (var viewModel in oldCollection.ToList())
			{
				RemoveDialog(viewModel);
			}
		}
	}

	private static void AddDialog(IDialogViewModel viewModel, ICollection<IDialogViewModel> collection,
		Window? owner)
	{
		var resource = Application.Current.TryFindResource(viewModel.GetType());

		if (resource == null)
		{
			return;
		}

		if (IsAssignableToGenericType(resource.GetType(), typeof(IDialogBoxPresenter<>)))
		{
			resource.GetType().GetMethod("Show")?.Invoke(resource, new object[] { viewModel });
			collection.Remove(viewModel);
		}
		else if (resource is Window dialog)
		{
			if (viewModel is not ICustomDialogViewModel userViewModel)
			{
				return;
			}

			dialog.DataContext = userViewModel;
			DialogBoxes[userViewModel] = dialog;

			userViewModel.DialogClosing += (sender, _) =>
			{
				if (sender is ICustomDialogViewModel userDialogViewModel)
				{
					collection.Remove(userDialogViewModel);
				}
			};

			dialog.Closing += (_, args) =>
			{
				if (!(bool)dialog.GetValue(ClosingProperty))
				{
					dialog.SetValue(ClosingProperty, true);
					userViewModel.RequestClose();

					if (!(bool)dialog.GetValue(ClosedProperty))
					{
						args.Cancel = true;
						dialog.SetValue(ClosingProperty, false);
					}
				}
			};

			dialog.Closed += (_, _) =>
			{
				Debug.Assert(DialogBoxes.ContainsKey(userViewModel));
				DialogBoxes.Remove(userViewModel);
			};

			dialog.Owner = owner;

			if (userViewModel.IsModal)
			{
				dialog.ShowDialog();
			}
			else
			{
				dialog.Show();
			}
		}
	}

	private static void RemoveDialog(IDialogViewModel viewModel)
	{
		if (DialogBoxes.ContainsKey(viewModel))
		{
			var dialog = DialogBoxes[viewModel];

			if (!(bool)dialog.GetValue(ClosingProperty))
			{
				dialog.SetValue(ClosingProperty, true);
				DialogBoxes[viewModel].Close();
			}

			dialog.SetValue(ClosedProperty, true);
		}
	}

	private static bool IsAssignableToGenericType(Type givenType, Type genericType)
	{
		var interfaceTypes = givenType.GetInterfaces();

		if (interfaceTypes.Any(it => it.IsGenericType && it.GetGenericTypeDefinition() == genericType))
		{
			return true;
		}

		if (givenType.IsGenericType && givenType.GetGenericTypeDefinition() == genericType)
		{
			return true;
		}

		var baseType = givenType.BaseType;

		return baseType != null && IsAssignableToGenericType(baseType, genericType);
	}
}