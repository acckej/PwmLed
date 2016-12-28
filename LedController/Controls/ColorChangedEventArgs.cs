using System;
using Android.Graphics;

namespace LedController.Controls
{
    public delegate void ColorChangedEventHandler(object sender, ColorChangedEventArgs e);

    public class ColorChangedEventArgs : EventArgs
    {
        public Color Color { get; set; }
    }
}