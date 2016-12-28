using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.Graphics;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using LedController.Logic.Entities;

namespace LedController
{
	[Activity(Label = "SpeedColorConfiguringActivity")]
	public class SpeedColorConfiguringActivity : Activity
	{
		protected override void OnCreate(Bundle savedInstanceState)
		{
			base.OnCreate(savedInstanceState);

			SetContentView(Resource.Layout.SpeedColorConfiguring);

			var sigmaRed = FindViewById<SeekBar>(Resource.Id.slSigmaRed);
			sigmaRed.SetBackgroundColor(Color.Red);

			var sigmaGreen = FindViewById<SeekBar>(Resource.Id.slSigmaGreen);
			sigmaGreen.SetBackgroundColor(Color.Green);

			var sigmaBlue = FindViewById<SeekBar>(Resource.Id.slSigmaBlue);
			sigmaBlue.SetBackgroundColor(Color.Blue);

			var muRed = FindViewById<SeekBar>(Resource.Id.slMuRed);
			muRed.SetBackgroundColor(Color.Red);

			var muGreen = FindViewById<SeekBar>(Resource.Id.slMuGreen);
			muGreen.SetBackgroundColor(Color.Green);

			var muBlue = FindViewById<SeekBar>(Resource.Id.slMuBlue);
			muBlue.SetBackgroundColor(Color.Blue);
		}

		private void FillUiFromDataEntity(SpeedColorProgramSettings settings)
		{
			var lblColorTest = FindViewById<TextView>(Resource.Id.lblColorTest);
			lblColorTest.SetBackgroundColor(Color.White);

			var slSpeed = FindViewById<SeekBar>(Resource.Id.slMaxSpeed);
			slSpeed.Progress = (int)(settings.TopSpeed * 1000);

			var sigmaRed = FindViewById<SeekBar>(Resource.Id.slSigmaRed);
			sigmaRed.Progress = (int) (settings.SigmaRed*1000);

			var sigmaGreen = FindViewById<SeekBar>(Resource.Id.slSigmaGreen);
			sigmaGreen.Progress = (int)(settings.SigmaGreen * 1000);

			var sigmaBlue = FindViewById<SeekBar>(Resource.Id.slSigmaBlue);
			sigmaBlue.Progress = (int)(settings.SigmaBlue * 1000);

			var muRed = FindViewById<SeekBar>(Resource.Id.slMuRed);
			muRed.Progress = (int)(settings.MuRed * 1000);

			var muGreen = FindViewById<SeekBar>(Resource.Id.slMuGreen);
			muGreen.Progress = (int)(settings.MuGreen * 1000);

			var muBlue = FindViewById<SeekBar>(Resource.Id.slMuBlue);
			muBlue.Progress = (int)(settings.MuBlue * 1000);
		}
	}
}