using System.Collections;
using System.Windows;
using System.Windows.Controls;

namespace WaolaWPF
{
	class EnhancedDataGrid : DataGrid
	{
		public static readonly DependencyProperty SelectedItemsProperty = DependencyProperty.Register(nameof(SelectedItems),
			typeof(IList<object>), typeof(EnhancedDataGrid), new PropertyMetadata(default(IList<object>), OnSelectedItemsPropertyChanged));

		protected override void OnSelectionChanged(SelectionChangedEventArgs e)
		{
			base.OnSelectionChanged(e);
			SetValue(SelectedItemsProperty, base.SelectedItems);
		}

		public new IList<object> SelectedItems
		{
			get => (IList<object>)GetValue(SelectedItemsProperty);
			set => throw new Exception("This property is read-only. To bind to it use 'Mode=OneWayToSource'.");
		}


		private static void OnSelectedItemsPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
		{
			((EnhancedDataGrid)d).OnSelectedItemsChanged((IList<object>)e.OldValue, (IList)e.NewValue);
		}

		protected virtual void OnSelectedItemsChanged(IList<object> oldSelectedItems, IList newSelectedItems)
		{
		}
	}
}
