using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;

namespace WaolaWPF;

public class CompositionHelper
{
	public static void ToPixels(Visual visual, double unitX,
		double unitY, out int pixelX, out int pixelY)
	{
		Matrix matrix;

		var source = PresentationSource.FromVisual(visual);

		if (source != null)
		{
			matrix = source.CompositionTarget.TransformToDevice;
		}
		else
		{
			using var src = new HwndSource(new HwndSourceParameters());
			matrix = src.CompositionTarget.TransformToDevice;
		}

		pixelX = (int)(matrix.M11 * unitX);
		pixelY = (int)(matrix.M22 * unitY);
	}

	public static void FromPixels(Visual visual, int pixelX,
		int pixelY, out double unitX, out double unitY)
	{
		Matrix matrix;

		var source = PresentationSource.FromVisual(visual);

		if (source != null)
		{
			matrix = source.CompositionTarget.TransformFromDevice;
		}
		else
		{
			using var src = new HwndSource(new HwndSourceParameters());
			matrix = src.CompositionTarget.TransformFromDevice;
		}

		unitX = pixelX * matrix.M11;
		unitY = pixelY * matrix.M22;
	}
}
