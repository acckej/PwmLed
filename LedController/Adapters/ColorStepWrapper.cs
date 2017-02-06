using System.Linq;
using Android.App;
using Android.Content;
using Android.Widget;
using LedController.Logic.Entities;

namespace LedController.Adapters
{
	public class ColorStepWrapper : ColorProgramStep
	{
		public void Delay_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
		{
			var str = new string(e.Text.ToArray());

			short val;
			var snd = (EditText)sender;
			if (str == string.Empty || !short.TryParse(str, out val) || val <= 0)
			{
				new AlertDialog.Builder(Context).SetMessage("Incorrect value. Delay should be more than zero.").Show();
				val = 100;
				snd.Text = val.ToString();
			}

			Delay = val;
		}

		public Context Context { get; set; }
	}
}