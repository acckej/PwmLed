using System;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Util;
using Android.Views;
using Android.Widget;
using LedController.Bluetooth;

namespace LedController.Fragments
{
	class TerminalFragment : Fragment
	{
		private View _view;
		private CancellationTokenSource _autoUpdate;
		private Task _updater;
		private BluetoothManager _manager;

		public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			var view = inflater.Inflate(Resource.Layout.BluetoothTerminal, null);

			_view = view;
			
			var btnGet = _view.FindViewById<Button>(Resource.Id.btnSendTerminalMessage);
			btnGet.Click += BtnGet_Click;
			var log = _view.FindViewById<EditText>(Resource.Id.txtTerminal);
			log.SetBackgroundColor(Color.Gray);

			InitTerminal();

			return view;
		}

		private void BtnGet_Click(object sender, EventArgs e)
		{
			var msg = _view.FindViewById<EditText>(Resource.Id.txtMessage);
			var log = _view.FindViewById<EditText>(Resource.Id.txtTerminal);
			if (string.IsNullOrEmpty(msg.Text) || _manager == null)
			{
				return;
			}
			
			log.Append($"\n<- {msg.Text}");
			_manager.SendData(Encoding.ASCII.GetBytes(msg.Text));
			msg.Text = string.Empty;
		}

		private void InitTerminal()
		{
			try
			{
				if (_updater != null && _updater.Status == TaskStatus.Running)
				{
					return;
				}

				_manager = BluetoothManager.Current;

				_autoUpdate = new CancellationTokenSource();

				_updater = new Task(() =>
				{
					try
					{
						do
						{
							var data = _manager.GetResponse();
							if (data != null && data.Length > 0)
							{
								var text = Encoding.ASCII.GetString(data);
								Activity.RunOnUiThread(() =>
								{
									var log = _view.FindViewById<EditText>(Resource.Id.txtTerminal);
									log.Append($"\n-> {text}");
								});
							}
						} while (!_autoUpdate.Token.WaitHandle.WaitOne(TimeSpan.FromMilliseconds(1000)));
					}
					catch (Exception ex)
					{
						Log.Error("LedControllerTerminalError", ex.ToString());
					}
				});

				_updater.Start();
			}
			catch (Exception ex)
			{
				ErrorHandler.HandleErrorWithMessageBox(ex.Message, _view.Context);
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
			_manager?.Dispose();
		}
	}
}