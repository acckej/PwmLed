using System;
using System.Globalization;
using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Views;
using Android.Widget;
using LedController.Logic.Entities;
using static LedController.Constants.SpeedColor;

namespace LedController.Fragments
{
	public class SpeedColorFragment : Fragment
	{
		private View _view;

		public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			View view = inflater.Inflate(Resource.Layout.SpeedColorConfiguring, null);

			var sigmaRed = view.FindViewById<SeekBar>(Resource.Id.slSigmaRed);
			sigmaRed.SetBackgroundColor(Color.Red);
			sigmaRed.Max = CoefficientsSliderMaxValue;

			var sigmaGreen = view.FindViewById<SeekBar>(Resource.Id.slSigmaGreen);
			sigmaGreen.SetBackgroundColor(Color.Green);
			sigmaGreen.Max = CoefficientsSliderMaxValue;
			
			var sigmaBlue = view.FindViewById<SeekBar>(Resource.Id.slSigmaBlue);
			sigmaBlue.SetBackgroundColor(Color.Blue);
			sigmaBlue.Max = CoefficientsSliderMaxValue;

			var muRed = view.FindViewById<SeekBar>(Resource.Id.slMuRed);
			muRed.SetBackgroundColor(Color.Red);
			muRed.Max = CoefficientsSliderMaxValue;

			var muGreen = view.FindViewById<SeekBar>(Resource.Id.slMuGreen);
			muGreen.SetBackgroundColor(Color.Green);
			muGreen.Max = CoefficientsSliderMaxValue;

			var muBlue = view.FindViewById<SeekBar>(Resource.Id.slMuBlue);
			muBlue.SetBackgroundColor(Color.Blue);
			muBlue.Max = CoefficientsSliderMaxValue;

			var maxSpeed = view.FindViewById<SeekBar>(Resource.Id.slMaxSpeed);
			maxSpeed.Max = MaxSpeed;

			var speed = view.FindViewById<SeekBar>(Resource.Id.slTestSpeed);
			speed.Max = MaxSpeed;
			speed.ProgressChanged += Speed_ProgressChanged;

			var blink = view.FindViewById<SeekBar>(Resource.Id.slBlinkDelay);
			blink.Max = MaxBlinkDelay;

			var colorChange = view.FindViewById<SeekBar>(Resource.Id.slColorChange);
			colorChange.Max = MaxColorChangePeriod;

			_view = view;

			SetDefaultValues();

			return view;
		}

		private void Speed_ProgressChanged(object sender, SeekBar.ProgressChangedEventArgs e)
		{
			var test = _view.FindViewById<TextView>(Resource.Id.lblTestSpeed);

			var sigmaRed = _view.FindViewById<SeekBar>(Resource.Id.slSigmaRed);
			var sigmaGreen = _view.FindViewById<SeekBar>(Resource.Id.slSigmaGreen);
			var sigmaBlue = _view.FindViewById<SeekBar>(Resource.Id.slSigmaBlue);
			var muRed = _view.FindViewById<SeekBar>(Resource.Id.slMuRed);
			var muGreen = _view.FindViewById<SeekBar>(Resource.Id.slMuGreen);
			var muBlue = _view.FindViewById<SeekBar>(Resource.Id.slMuBlue);
			var maxSpeed = _view.FindViewById<SeekBar>(Resource.Id.slMaxSpeed);
			var speed = _view.FindViewById<SeekBar>(Resource.Id.slTestSpeed);

			var color = GetColor(speed.Progress,
				maxSpeed.Progress,
				Threshold,
				muRed.Progress,
				muGreen.Progress,
				muBlue.Progress,
				sigmaRed.Progress,
				sigmaGreen.Progress,
				sigmaBlue.Progress);

			test.SetBackgroundColor(color);
		}

		private Color GetColor(int currentSpeed, int maxSpeed, double threshold, int redMu, int greenMu, int blueMu, int redSigma, int greenSigma, int blueSigma)
		{
			var positionMax = (double)maxSpeed/SpeedSliderMultiplier;
			var positionCurrent = (double)currentSpeed/SpeedSliderMultiplier;

			var muGreen = greenMu == 0 ? 0 : positionMax / ((double)greenMu / CoefficientsSliderMultiplier);
			var muRed = redMu == 0 ? 0 : positionMax / ((double)redMu / CoefficientsSliderMultiplier);
			var muBlue = blueMu == 0 ? 0 : positionMax / ((double)blueMu / CoefficientsSliderMultiplier);

			var sigmaGreen = greenSigma == 0 ? 0 : Math.Sqrt(positionMax / ((double)greenSigma / CoefficientsSliderMultiplier));
			var sigmaRed = redSigma == 0 ? 0 : Math.Sqrt(positionMax / ((double)redSigma / CoefficientsSliderMultiplier));
			var sigmaBlue = blueSigma == 0 ? 0 : Math.Sqrt(positionMax / ((double)blueSigma / CoefficientsSliderMultiplier));

			var range = 255 - threshold;

			var red = threshold + range * (1 - Gaussian(sigmaRed, muRed, positionCurrent));
			var green = threshold + range * Gaussian(sigmaGreen, muGreen, positionCurrent);
			var blue = threshold + range * Gaussian(sigmaBlue, muBlue, positionCurrent);

			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}

			return new Color((int)red, (int)green, (int)blue);
		}

		private double Gaussian(double sigma, double mu, double x)
		{
			var result = 1 / sigma * Math.Sqrt(2 * Math.PI) * Math.Exp(-1 * Math.Pow(x - mu, 2) / 2 * Math.Pow(sigma, 2));

			return result;
		}

		private void SetDefaultValues()
		{
			var sigmaRed = _view.FindViewById<SeekBar>(Resource.Id.slSigmaRed);
			sigmaRed.Progress = DefaultSigmaRed;

			var sigmaGreen = _view.FindViewById<SeekBar>(Resource.Id.slSigmaGreen);
			sigmaGreen.Progress = DefaultSigmaGreen;

			var sigmaBlue = _view.FindViewById<SeekBar>(Resource.Id.slSigmaBlue);
			sigmaBlue.Progress = DefaultSigmaBlue;

			var muRed = _view.FindViewById<SeekBar>(Resource.Id.slMuRed);
			muRed.Progress = DefaultMuRed;

			var muGreen = _view.FindViewById<SeekBar>(Resource.Id.slMuGreen);
			muGreen.Progress = DefaultMuGreen;
			
			var muBlue = _view.FindViewById<SeekBar>(Resource.Id.slMuBlue);
			muBlue.Progress = DefaultMuBlue;

			var maxSpeed = _view.FindViewById<SeekBar>(Resource.Id.slMaxSpeed);
			maxSpeed.Progress = DefaultMaxSpeed;

			var speed = _view.FindViewById<SeekBar>(Resource.Id.slTestSpeed);
			speed.Progress = DefaultMaxSpeed;

			var idleDelay = _view.FindViewById<EditText>(Resource.Id.txtIdleDelay);
			idleDelay.Text = DefaultIdleDelay.ToString();

			var notMovingDelay = _view.FindViewById<EditText>(Resource.Id.txtNotMovingDelay);
			notMovingDelay.Text = DefaultNotMovingDelay.ToString();

			var wheelCircle = _view.FindViewById<EditText>(Resource.Id.txtDistance);
			wheelCircle.Text = DefaultWheelCircle.ToString();

			var blink = _view.FindViewById<SeekBar>(Resource.Id.slBlinkDelay);
			blink.Progress = DefaultBlinkDelay;

			var colorChange = _view.FindViewById<SeekBar>(Resource.Id.slColorChange);
			colorChange.Progress = DefaultColorChangePeriod;
		}

		private void FillUiFromDataEntity(SpeedColorProgramSettings settings)
		{
			var lblColorTest = _view.FindViewById<TextView>(Resource.Id.lblColorTest);
			lblColorTest.SetBackgroundColor(Color.White);

			var slSpeed = _view.FindViewById<SeekBar>(Resource.Id.slMaxSpeed);
			slSpeed.Progress = (int)(settings.TopSpeed * CoefficientsSliderMultiplier);

			var sigmaRed = _view.FindViewById<SeekBar>(Resource.Id.slSigmaRed);
			sigmaRed.Progress = (int)(settings.SigmaRed * CoefficientsSliderMultiplier);

			var sigmaGreen = _view.FindViewById<SeekBar>(Resource.Id.slSigmaGreen);
			sigmaGreen.Progress = (int)(settings.SigmaGreen * CoefficientsSliderMultiplier);

			var sigmaBlue = _view.FindViewById<SeekBar>(Resource.Id.slSigmaBlue);
			sigmaBlue.Progress = (int)(settings.SigmaBlue * CoefficientsSliderMultiplier);

			var muRed = _view.FindViewById<SeekBar>(Resource.Id.slMuRed);
			muRed.Progress = (int)(settings.MuRed * CoefficientsSliderMultiplier);

			var muGreen = _view.FindViewById<SeekBar>(Resource.Id.slMuGreen);
			muGreen.Progress = (int)(settings.MuGreen * CoefficientsSliderMultiplier);

			var muBlue = _view.FindViewById<SeekBar>(Resource.Id.slMuBlue);
			muBlue.Progress = (int)(settings.MuBlue * CoefficientsSliderMultiplier);

			var maxSpeed = _view.FindViewById<SeekBar>(Resource.Id.slMaxSpeed);
			maxSpeed.Progress = (int)(settings.TopSpeed * SpeedSliderMultiplier);

			var idleDelay = _view.FindViewById<EditText>(Resource.Id.txtIdleDelay);
			idleDelay.Text = settings.IdleDelay.ToString(CultureInfo.InvariantCulture);

			var notMovingDelay = _view.FindViewById<EditText>(Resource.Id.txtNotMovingDelay);
			notMovingDelay.Text = settings.NotMovingDelay.ToString();

			var wheelCircle = _view.FindViewById<EditText>(Resource.Id.txtDistance);
			wheelCircle.Text = (settings.Distance * CoefficientsSliderMultiplier).ToString(CultureInfo.InvariantCulture);

			var blink = _view.FindViewById<SeekBar>(Resource.Id.slBlinkDelay);
			blink.Progress = settings.BlinkDelay;

			var colorChange = _view.FindViewById<SeekBar>(Resource.Id.slColorChange);
			colorChange.Progress = settings.ColorChangePeriod;
		}
	}
}