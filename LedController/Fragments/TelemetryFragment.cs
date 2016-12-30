using System.Threading;
using System.Threading.Tasks;
using Android.App;
using Android.OS;
using Android.Views;
using Android.Widget;
using LedController.Logic.Entities;

namespace LedController.Fragments
{
	public class TelemetryFragment : Fragment
	{
		private View _view;
		private CancellationTokenSource _autoUpdate;
		private Task _updater;

		public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			var view = inflater.Inflate(Resource.Layout.Telemetry, null);

			_view = view;
			var autoUpdate = _view.FindViewById<CheckBox>(Resource.Id.cbAutoUpdate);
			autoUpdate.CheckedChange += AutoUpdate_CheckedChange;

			return view;
		}

		private void AutoUpdate_CheckedChange(object sender, CompoundButton.CheckedChangeEventArgs e)
		{
			var get = _view.FindViewById<Button>(Resource.Id.btnGetTelemetry);
			var checkBox = sender as CheckBox;
			if (checkBox != null && checkBox.Checked)
			{
				_autoUpdate = new CancellationTokenSource();
				get.Enabled = false;
				_updater = new Task(() =>
				{
					Thread.Sleep(Constants.Telemetry.UpdatePeriod);
				}, _autoUpdate.Token);
				_updater.Start();
			}
			else
			{
				_autoUpdate.Cancel();
				get.Enabled = true;
			}
		}

		public override void OnPause()
		{
			Cleanup();
			base.OnPause();
		}

		private void Cleanup()
		{
			_autoUpdate?.Cancel();
		}

		private void Update(SystemInformation info)
		{
			var voltage = _view.FindViewById<ProgressBar>(Resource.Id.pbVoltage);
			voltage.Progress = (int) (info.Voltage*Constants.Telemetry.VoltageMultiplier);
			var speed = _view.FindViewById<TextView>(Resource.Id.lblSpeed);

			speed.Text = $"Speed: {(info.Speed*Constants.Telemetry.SpeedMultiplier).ToString("F1")} km/h";
		}
	}
}