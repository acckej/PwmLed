using System;
using Android.App;
using Android.Content;
using Android.Graphics;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
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
			var layout = new LinearLayout(Context);
			layout.LayoutDirection = LayoutDirection.Ltr;
			var cPickerView = new RoundColorPickerView(Context, _initialColor);
			var white = new Button(Context) {Text = "White"};
			white.Click += White_Click;
			var black = new Button(Context) {Text = "Black"};
			black.Click += Black_Click;

			layout.AddView(cPickerView);
			layout.AddView(white);
			layout.AddView(black);
			
			cPickerView.ColorChanged += delegate(object sender, ColorChangedEventArgs args)
			{
				ColorChanged?.Invoke(this, args);

				Dismiss();
			};
			SetContentView(layout);
			SetTitle("Pick a Color");
		}

		private void Black_Click(object sender, EventArgs e)
		{
			ColorChanged?.Invoke(this, new ColorChangedEventArgs
			{
				Color = Color.Black
			});
			Dismiss();
		}

		private void White_Click(object sender, EventArgs e)
		{
			ColorChanged?.Invoke(this, new ColorChangedEventArgs
			{
				Color	= Color.White
			});
			Dismiss();
		}
	}
}