using System.Collections;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace WaolaWPF;

public class ObservableDictionary<TKey, TValue>
	: IDictionary<TKey, TValue>
		, INotifyCollectionChanged
		, INotifyPropertyChanged where TKey : notnull
{
	private readonly Dictionary<TKey, TValue> cache = new();
	private readonly ObservableKeyedCollection<TKey, TValue> keyedCollection;

	private int cacheSize;
	private int cacheVersion;
	private int version;

	public ObservableDictionary()
	{
		keyedCollection = new ObservableKeyedCollection<TKey, TValue>();
	}

	public ObservableDictionary(IDictionary<TKey, TValue> dictionary)
	{
		keyedCollection = new ObservableKeyedCollection<TKey, TValue>();

		foreach (var entry in dictionary)
		{
			DoAddEntry(entry.Key, entry.Value);
		}
	}

	public ObservableDictionary(IEqualityComparer<TKey> comparer)
	{
		keyedCollection = new ObservableKeyedCollection<TKey, TValue>(comparer);
	}

	public ObservableDictionary(IDictionary<TKey, TValue> dictionary, IEqualityComparer<TKey> comparer)
	{
		keyedCollection = new ObservableKeyedCollection<TKey, TValue>(comparer);

		foreach (var entry in dictionary)
		{
			DoAddEntry(entry.Key, entry.Value);
		}
	}

	public Dictionary<TKey, TValue>.KeyCollection Keys => Dictionary.Keys;

	public Dictionary<TKey, TValue>.ValueCollection Values => Dictionary.Values;

	private Dictionary<TKey, TValue> Dictionary
	{
		get
		{
			if (cacheVersion != version)
			{
				cache.Clear();

				foreach (var entry in keyedCollection)
				{
					cache.Add(entry.Key, entry.Value!);
				}

				cacheVersion = version;
			}

			return cache;
		}
	}

	private void DoAddEntry(TKey key, TValue value)
	{
		if (AddEntry(key, value))
		{
			++version;

			var index = GetIndexAndEntryForKey(key, out var entry);
			FireEntryAddedNotifications(entry, index);
		}
	}

	private void DoClearEntries()
	{
		if (ClearEntries())
		{
			++version;
			FireResetNotifications();
		}
	}

	private bool DoRemoveEntry(TKey key)
	{
		var index = GetIndexAndEntryForKey(key, out var entry);

		var result = RemoveEntry(key);
		if (result)
		{
			++version;
			if (index > -1)
			{
				FireEntryRemovedNotifications(entry, index);
			}
		}

		return result;
	}

	private void DoSetEntry(TKey key, TValue value)
	{
		var index = GetIndexAndEntryForKey(key, out var entry);

		if (SetEntry(key, value))
		{
			++version;

			// if prior entry existed for this key, fire the removed notifications
			if (index > -1)
			{
				FireEntryRemovedNotifications(entry, index);

				// force the property change notifications to fire for the modified entry
				cacheSize--;
			}

			// then fire the added notifications
			index = GetIndexAndEntryForKey(key, out entry);
			FireEntryAddedNotifications(entry, index);
		}
	}

	private void FireEntryAddedNotifications(KeyValuePair<TKey, TValue> entry, int index)
	{
		FirePropertyChangedNotifications();

		if (index > -1)
		{
			OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add,
				new KeyValuePair<TKey, TValue>(entry.Key, entry.Value), index));
		}
		else
		{
			OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
		}
	}

	private void FireEntryRemovedNotifications(KeyValuePair<TKey, TValue> entry, int index)
	{
		FirePropertyChangedNotifications();

		if (index > -1)
		{
			OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove,
				new KeyValuePair<TKey, TValue>(entry.Key, entry.Value), index));
		}
		else
		{
			OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
		}
	}

	private void FirePropertyChangedNotifications()
	{
		if (Count != cacheSize)
		{
			cacheSize = Count;
			OnPropertyChanged(nameof(Count));
			// ReSharper disable once ExplicitCallerInfoArgument
			OnPropertyChanged("Item[]");
			OnPropertyChanged(nameof(Keys));
			OnPropertyChanged(nameof(Values));
		}
	}

	private void FireResetNotifications()
	{
		// fire the relevant PropertyChanged notifications
		FirePropertyChangedNotifications();

		// fire CollectionChanged notification
		OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
	}

	private bool AddEntry(TKey key, TValue value)
	{
		keyedCollection.Add(new KeyValuePair<TKey, TValue>(key, value));
		return true;
	}

	private bool ClearEntries()
	{
		var result = Count > 0;

		if (result)
		{
			keyedCollection.Clear();
		}

		return result;
	}

	private int GetIndexAndEntryForKey(TKey key, out KeyValuePair<TKey, TValue> entry)
	{
		entry = new KeyValuePair<TKey, TValue>();
		var index = -1;

		if (keyedCollection.Contains(key))
		{
			entry = keyedCollection[key];
			index = keyedCollection.IndexOf(entry);
		}

		return index;
	}

	private void OnCollectionChanged(NotifyCollectionChangedEventArgs args)
	{
		CollectionChanged?.Invoke(this, args);
	}

	private bool RemoveEntry(TKey key)
	{
		return keyedCollection.Remove(key);
	}

	private bool SetEntry(TKey key, TValue value)
	{
		var keyExists = keyedCollection.Contains(key);

		// if identical key/value pair already exists, nothing to do
		if (keyExists && IsEqual(value, keyedCollection[key].Value))
		{
			return false;
		}

		// otherwise, remove the existing entry
		if (keyExists)
		{
			keyedCollection.Remove(key);
		}

		// add the new entry
		keyedCollection.Add(new KeyValuePair<TKey, TValue>(key, value));

		return true;
	}

	private static bool IsEqual(TValue left, TValue right)
	{
		return left == null ? right == null : left.Equals(right);
	}

	#region KeyedCollection<TEntryKey>

	private class ObservableKeyedCollection<TEntryKey, TEntryValue>
		: KeyedCollection<TEntryKey, KeyValuePair<TEntryKey, TEntryValue>> where TEntryKey : notnull
	{
		public ObservableKeyedCollection()
		{
		}

		public ObservableKeyedCollection(IEqualityComparer<TEntryKey> comparer)
			: base(comparer)
		{
		}

		protected override TEntryKey GetKeyForItem(KeyValuePair<TEntryKey, TEntryValue> entry)
		{
			return entry.Key;
		}
	}

	#endregion KeyedCollection<TEntryKey>

	#region Enumerator

	public struct Enumerator<TEnmKey, TEnmValue> :
		IEnumerator<KeyValuePair<TEnmKey, TEnmValue>>,
		IDictionaryEnumerator where TEnmKey : notnull
	{
		private readonly ObservableDictionary<TEnmKey, TEnmValue> dictionary;
		private readonly int version;
		private readonly bool isDictionaryEntryEnumerator;

		private int index;
		private KeyValuePair<TEnmKey, TEnmValue?> current;

		internal Enumerator(ObservableDictionary<TEnmKey, TEnmValue> dictionary,
			bool isDictionaryEntryEnumerator)
		{
			this.dictionary = dictionary;
			version = dictionary.version;
			index = -1;
			this.isDictionaryEntryEnumerator = isDictionaryEntryEnumerator;
			current = new KeyValuePair<TEnmKey, TEnmValue?>();
		}

		public KeyValuePair<TEnmKey, TEnmValue> Current
		{
			get
			{
				ValidateCurrent();
				return current!;
			}
		}

		public bool MoveNext()
		{
			ValidateVersion();

			if (++index < dictionary.keyedCollection.Count)
			{
				current = new KeyValuePair<TEnmKey, TEnmValue?>(dictionary.keyedCollection[index].Key,
					dictionary.keyedCollection[index].Value);

				return true;
			}

			index = -2;
			current = new KeyValuePair<TEnmKey, TEnmValue?>();

			return false;
		}

		private readonly void ValidateCurrent()
		{
			if (index == -1)
			{
				throw new InvalidOperationException("The enumerator has not been started.");
			}

			if (index == -2)
			{
				throw new InvalidOperationException("The enumerator has reached the end of the collection.");
			}
		}

		private void ValidateVersion()
		{
			if (version != dictionary.version)
			{
				throw new InvalidOperationException("The enumerator is not valid because the dictionary changed.");
			}
		}

		#region IEnumerator implementation

		object IEnumerator.Current
		{
			get
			{
				ValidateCurrent();
				if (isDictionaryEntryEnumerator)
				{
					return new DictionaryEntry(current.Key, current.Value);
				}

				return new KeyValuePair<TEnmKey, TEnmValue?>(current.Key, current.Value);
			}
		}

		void IEnumerator.Reset()
		{
			ValidateVersion();
			index = -1;
			current = new KeyValuePair<TEnmKey, TEnmValue?>();
		}

		#endregion IEnumerator implemenation

		#region IDictionaryEnumerator implemenation

		DictionaryEntry IDictionaryEnumerator.Entry
		{
			get
			{
				ValidateCurrent();
				return new DictionaryEntry(current.Key, current.Value);
			}
		}

		object IDictionaryEnumerator.Key
		{
			get
			{
				ValidateCurrent();
				return current.Key;
			}
		}

		object? IDictionaryEnumerator.Value
		{
			get
			{
				ValidateCurrent();
				return current.Value;
			}
		}

		#endregion

		public void Dispose()
		{
		}
	}

	#endregion Enumerator

	#region INotifyPropertyChanged implementation

	public event PropertyChangedEventHandler? PropertyChanged;

	private void OnPropertyChanged([CallerMemberName] string? propertyName = null)
	{
		PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
	}

	#endregion INotifyPropertyChanged implementation

	#region INotifyCollectionChanged implementation

	event NotifyCollectionChangedEventHandler? INotifyCollectionChanged.CollectionChanged
	{
		add => CollectionChanged += value;
		remove => CollectionChanged -= value;
	}

	protected virtual event NotifyCollectionChangedEventHandler? CollectionChanged;

	#endregion INotifyCollectionChanged implementation

	#region IDictionary implementation

	public IEnumerator GetEnumerator()
	{
		return new Enumerator<TKey, TValue>(this, false);
	}

	public void CopyTo(Array array, int index)
	{
		((ICollection)keyedCollection).CopyTo(array, index);
	}

	public int Count => keyedCollection.Count;

	#endregion IDictionary implementation

	#region IDictionary<TKey, TValue> implementation

	IEnumerator<KeyValuePair<TKey, TValue>> IEnumerable<KeyValuePair<TKey, TValue>>.GetEnumerator()
	{
		return new Enumerator<TKey, TValue>(this, false);
	}

	public void Add(KeyValuePair<TKey, TValue> item)
	{
		DoAddEntry(item.Key, item.Value);
	}

	public void Clear()
	{
		DoClearEntries();
	}

	public bool Contains(KeyValuePair<TKey, TValue> item)
	{
		return keyedCollection.Contains(item.Key);
	}

	public void CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex)
	{
		if (array == null)
		{
			throw new ArgumentNullException(nameof(array));
		}

		if (arrayIndex < 0 || arrayIndex > array.Length)
		{
			throw new ArgumentOutOfRangeException($"{nameof(arrayIndex)} must be in between {0} and {array.Length}");
		}

		if (array.Length - arrayIndex < keyedCollection.Count)
		{
			throw new ArgumentException($"{nameof(array)} too small");
		}

		foreach (var entry in keyedCollection)
		{
			array[arrayIndex++] = new KeyValuePair<TKey, TValue>(entry.Key,
				entry.Value ?? throw new InvalidOperationException($"{nameof(entry.Value)} is null"));
		}
	}

	public bool Remove(KeyValuePair<TKey, TValue> item)
	{
		return DoRemoveEntry(item.Key);
	}

	public bool IsReadOnly => false;

	public void Add(TKey key, TValue value)
	{
		DoAddEntry(key, value);
	}

	public bool ContainsKey(TKey key)
	{
		return keyedCollection.Contains(key);
	}

	public bool Remove(TKey key)
	{
		return DoRemoveEntry(key);
	}

	public bool TryGetValue(TKey key, out TValue value)
	{
		var result = keyedCollection.TryGetValue(key,
			out var kvp);

		value = result ? kvp.Value! : default!;

		return result;
	}

	public TValue this[TKey key]
	{
		get => keyedCollection[key].Value;
		set => DoSetEntry(key, value);
	}

	ICollection<TKey> IDictionary<TKey, TValue>.Keys => Keys;

	ICollection<TValue> IDictionary<TKey, TValue>.Values => Values;

	#endregion IDictionary<TKey, TValue> implementation
}