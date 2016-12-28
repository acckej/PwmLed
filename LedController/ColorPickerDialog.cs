using System;
using Android.App;
using Android.Content;
using Android.Graphics;
using Android.OS;
using Android.Runtime;
using LedController.Controls;

namespace LedController
{
	public class ColorPickerDialog : Dialog
	{
		public event ColorChangedEventHandler ColorChanged;
		private static Color _initialColor;

		public ColorPickerDialog(Context context, Color initialColor)
			: base(context)
		{
			_initialColor = initialColor;
		}

		protected ColorPickerDialog(IntPtr javaReference, JniHandleOwnership transfer)
			: base(javaReference, transfer)
		{
			_initialColor = Color.Black;
		}

		protected override void OnCreate(Bundle savedInstanceState)
		{
			base.OnCreate(savedInstanceState);
			var cPickerView = new RoundColorPickerView(Context, _initialColor);
			cPickerView.ColorChanged += delegate(object sender, ColorChangedEventArgs args)
			{
				ColorChanged?.Invoke(this, args);

				Dismiss();
			};
			SetContentView(cPickerView);
			SetTitle("Pick a Color");
		}
	}
}