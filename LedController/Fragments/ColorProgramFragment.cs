using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Views;
using Android.Widget;
using LedController.Adapters;
using LedController.Bluetooth;
using LedController.Logic;
using LedController.Logic.Entities;

namespace LedController.Fragments
{
	public class ColorProgramFragment : Fragment
	{
		private ColorProgramStepAdapter _listAdapter;
		private bool _testing;
		private View _view;

		public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			View view = inflater.Inflate(Resource.Layout.ColorProgram, null);
			_view = view;
			var list = view.FindViewById<ListView>(Resource.Id.lvColorProgram);
			_view.SetBackgroundColor(new Color(0, 0, 0));
			var listAdapter = new ColorProgramStepAdapter(new List<ColorProgramStep>
			{
				new ColorProgramStep
				{
					Blue = 23,
					Red = 123,
					Green = 234,
					Delay = 1000
				},
				new ColorProgramStep
				{
					Blue = 123,
					Red = 223,
					Green = 134,
					Delay = 500
				}
			});

			_listAdapter = listAdapter;
			list.Adapter = listAdapter;
			listAdapter.OnColorBoxClicked += ListAdapterOnOnColorBoxClicked;
			listAdapter.OnStepButtonClicked += ListAdapterOnOnStepButtonClicked;

			var addButton = view.FindViewById<Button>(Resource.Id.btnAdd);
			addButton.Click += AddButton_Click;

			var uploadButton = view.FindViewById<Button>(Resource.Id.btnUpload);
			uploadButton.Click += UploadButton_Click;

			var testButton = view.FindViewById<Button>(Resource.Id.btnTest);
			testButton.Click += TestButton_Click;

			return view;
		}

		private void TestButton_Click(object sender, EventArgs e)
		{
			var items = ListAdapter;
			var testButton = _view.FindViewById<Button>(Resource.Id.btnTest);

			if (_testing)
			{
				_testing = false;
				testButton.Text = GetString(Resource.String.btnPlayText);
			}
			else
			{
				_testing = true;
				testButton.Text = GetString(Resource.String.btnPlayStopText);
			}

			if (items != null)
			{
				Task.Run(() =>
				{
					while (_testing)
					{
						for (var i = 0; i < items.Count; i++)
						{
							var item = items[i];

							if (!_testing)
							{
								break;
							}

							Activity.RunOnUiThread(() =>
							{
								_view.SetBackgroundColor(new Color(item.Red, item.Green, item.Blue));
							});

							Thread.Sleep(item.Delay);
						}
					}

					if (_testing)
					{
						Activity.RunOnUiThread(() =>
						{
							_view.SetBackgroundColor(new Color(0, 0, 0));
						});
					}
				});
			}
		}

		private void UploadButton_Click(object sender, EventArgs e)
		{
			try
			{
				using (var bm = BluetoothManager.Current)
				{
					if (_listAdapter.Count == 0)
					{
						return;
					}
					var program = new ColorProgram();
					foreach (var step in _listAdapter.Steps)
					{
						program.Add(step);
					}

					var cmd = new Command(Logic.Constants.CommandType.UploadColorProgramCommandId, program);
					var resultData = bm.SendCommandAndGetResponse(cmd.Serialize());
					var result = CommandDispatcher.GetCommandResultFromByteArray(resultData);

					if (result.HasError)
					{
						ErrorHandler.HandleErrorWithMessageBox($"Command error: {result.Message}", _view.Context);
					}
				}
			}
			catch (Exception ex)
			{
				ErrorHandler.HandleErrorWithMessageBox(ex.Message, _view.Context);
			}
		}

		private void AddButton_Click(object sender, EventArgs e)
		{
			ListAdapter.Add(new ColorProgramStep
			{
				Blue = 128,
				Red = 128,
				Green = 128,
				Delay = 1000
			});

			SetNoStepsDefined(false);
		}

		private ColorProgramStepAdapter ListAdapter
		{
			get
			{
				if (_listAdapter == null)
				{
					var list = _view.FindViewById<ListView>(Resource.Id.lvColorProgram);
					_listAdapter = list.Adapter as ColorProgramStepAdapter;
				}

				return _listAdapter;
			}
		}

		private void SetNoStepsDefined(bool noStepsDefined)
		{
			var noStepsVisible = noStepsDefined ? ViewStates.Visible : ViewStates.Invisible;
			var noStepsInvisible = noStepsDefined ? ViewStates.Invisible : ViewStates.Visible;

			var list = _view.FindViewById<ListView>(Resource.Id.lvColorProgram);
			list.Visibility = noStepsInvisible;
			var noSteps = _view.FindViewById<TextView>(Resource.Id.txtNoSteps);
			noSteps.Visibility = noStepsVisible;

			var uploadButton = _view.FindViewById<Button>(Resource.Id.btnUpload);
			uploadButton.Enabled = !noStepsDefined;

			var testButton = _view.FindViewById<Button>(Resource.Id.btnTest);
			testButton.Enabled = !noStepsDefined;

			_testing = _testing ? !noStepsDefined : _testing;
		}

		private void ListAdapterOnOnStepButtonClicked(int position)
		{
			ListAdapter?.Remove(position);

			SetNoStepsDefined(ListAdapter != null && ListAdapter.Count == 0);
		}

		public override void OnPause()
		{
			_testing = false;
			base.OnPause();
		}

		private void ListAdapterOnOnColorBoxClicked(int position)
		{
			var color = Color.Red;
			if (ListAdapter != null)
			{
				var item = ListAdapter[position];
				color = new Color(item.Red, item.Green, item.Blue);
			}

			var roundColorPickerDialog = new ColorPickerDialog(Activity, color);
			roundColorPickerDialog.ColorChanged += (o, args) =>
			{
				ListAdapter?.SetColor(args.Color, position);
			};
			roundColorPickerDialog.Show();
		}
	}
}