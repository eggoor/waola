using System.Collections.Specialized;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace WaolaWPF;

public class ObservableHashSet<T>
	: HashSet<T>
	, INotifyCollectionChanged
	, INotifyPropertyChanged
{
	public ObservableHashSet()
	{
	}

	public ObservableHashSet(IEqualityComparer<T>? comparer)
		: base(comparer)
	{
	}

	public ObservableHashSet(int capacity)
		: base(capacity) { }

	public ObservableHashSet(IEnumerable<T> collection)
		: base(collection) { }

	public ObservableHashSet(IEnumerable<T> collection, IEqualityComparer<T>? comparer)
		: base(collection, comparer)
	{
	}

	public ObservableHashSet(int capacity, IEqualityComparer<T>? comparer)
		: base(capacity, comparer)
	{
	}

	public new int Count => base.Count;

	public new bool Add(T item)
	{
		var added = base.Add(item);

		if (added)
		{
			FirePropertyChanged(nameof(Count));
			FireCollectionChanged(NotifyCollectionChangedAction.Add, item);
		}

		return added;
	}

	public new void Clear()
	{
		base.Clear();
		FireCollectionChanged(NotifyCollectionChangedAction.Reset, null);
	}

	public new bool Remove(T item)
	{
		var removed = base.Remove(item);

		if (removed)
		{
			FirePropertyChanged(nameof(Count));
			FireCollectionChanged(NotifyCollectionChangedAction.Remove, item);
		}

		return removed;
	}

	public event NotifyCollectionChangedEventHandler? CollectionChanged;
	public event PropertyChangedEventHandler? PropertyChanged;

	private void FireCollectionChanged(NotifyCollectionChangedAction action, object? changedItem)
	{
		CollectionChanged?.Invoke(this, new NotifyCollectionChangedEventArgs(action, changedItem));
	}

	private void FirePropertyChanged([CallerMemberName] string? propertyName = null)
	{
		PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
	}
}
