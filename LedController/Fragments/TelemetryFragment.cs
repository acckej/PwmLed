using System;
using System.Threading;
using System.Threading.Tasks;
using Android.App;
using Android.OS;
using Android.Util;
using Android.Views;
using Android.Widget;
using LedController.Bluetooth;
using LedController.Logic;
using LedController.Logic.Entities;

namespace LedController.Fragments
{
	public class TelemetryFragment : Fragment
	{
		private View _view;
		private CancellationTokenSource _autoUpdate;
		private Task _updater;
		private BluetoothManager _btManager;

		public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			var view = inflater.Inflate(Resource.Layout.Telemetry, null);

			_view = view;
			var autoUpdate = _view.FindViewById<CheckBox>(Resource.Id.cbAutoUpdate);
			autoUpdate.CheckedChange += AutoUpdate_CheckedChange;

			var btnGet = _view.FindViewById<Button>(Resource.Id.btnGetTelemetry);
			btnGet.Click += BtnGet_Click;

			return view;
		}

		private void BtnGet_Click(object sender, EventArgs e)
		{
			DownloadAndUpdate(true);
		}

		private void DownloadAndUpdate(bool showMessageBox)
		{
			try
			{
				_btManager = BluetoothManager.Current;
				var cmd = new Command(Logic.Constants.CommandType.GetSystemInformationCommandId);
				var resultData = _btManager.SendCommandAndGetResponse(cmd.Serialize(), CommandResult.GetExpectedDataSize);
				var result = CommandDispatcher.GetCommandResultFromByteArray(resultData);

				if (result.HasError)
				{
					ErrorHandler.HandleErrorWithMessageBox($"Command error: {result.Message}", _view.Context);
				}
				else
				{
					var data = result.Data as SystemInformation;
					if (data == null)
					{
						throw new ApplicationException("Wrong response type");
					}

					Update(data);
				}
			}
			catch (Exception ex)
			{
				if (showMessageBox)
				{
					ErrorHandler.HandleErrorWithMessageBox(ex.Message, _view.Context);
				}
				else
				{
					Log.Error("TelemetryAuto", ex.ToString());
				}
			}
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
					DownloadAndUpdate(false);
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
			if (_btManager != null)
			{
				_btManager.Dispose();
				_btManager = null;
			}
			_autoUpdate?.Cancel();
		}

		private void Update(SystemInformation info)
		{
			var voltage = _view.FindViewById<ProgressBar>(Resource.Id.pbVoltage);
			voltage.Progress = (int) (info.Voltage*Constants.Telemetry.VoltageMultiplier);
			var speed = _view.FindViewById<TextView>(Resource.Id.lblSpeed);
			var voltLabel = _view.FindViewById<TextView>(Resource.Id.lblVoltage);

			voltLabel.Text = $"Voltage: {info.Voltage.ToString("F1")}v";
			speed.Text = $"Speed: {(info.Speed*Constants.Telemetry.SpeedMultiplier).ToString("F1")} km/h";
		}
	}
}