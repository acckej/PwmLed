using System;
using System.Collections.Generic;
using System.Linq;
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
using Newtonsoft.Json;

namespace LedController.Fragments
{
	public class ColorProgramFragment : Fragment
	{
		private const string ColorProgramKey = "ColorProgram";
		private ColorProgramStepAdapter _listAdapter;
		private bool _testing;
		private View _view;
		private IList<ColorProgramStep> _steps;
		private BluetoothManager _btManager;
		private bool _firstLoad = true;


		public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			var view = inflater.Inflate(Resource.Layout.ColorProgram, null);
			_view = view;
			var list = view.FindViewById<ListView>(Resource.Id.lvColorProgram);
			_view.SetBackgroundColor(new Color(0, 0, 0));

			var saved = savedInstanceState?.GetString(ColorProgramKey);
			if (saved != null)
			{
				_steps = JsonConvert.DeserializeObject<ColorProgramStep[]>(saved);
			}

			_firstLoad = true;
			_steps = _steps ?? PresetsManager.DefaultColorProgramSteps["Default"].ToList();

			_listAdapter = new ColorProgramStepAdapter(_steps);
			
			list.Adapter = _listAdapter;
			_listAdapter.OnColorBoxClicked += ListAdapterOnOnColorBoxClicked;
			_listAdapter.OnStepButtonClicked += ListAdapterOnOnStepButtonClicked;

			var addButton = view.FindViewById<Button>(Resource.Id.btnAdd);
			addButton.Click += AddButton_Click;

			var uploadButton = view.FindViewById<Button>(Resource.Id.btnUpload);
			uploadButton.Click += UploadButton_Click;

			var testButton = view.FindViewById<Button>(Resource.Id.btnTest);
			testButton.Click += TestButton_Click;

			var getButton = view.FindViewById<Button>(Resource.Id.btnGetProgram);
			getButton.Click += GetButton_Click;

			var presets = view.FindViewById<Spinner>(Resource.Id.ddlPresets);
			var presetsAdapter = new ArrayAdapter(view.Context, Android.Resource.Layout.SimpleSpinnerDropDownItem, PresetsManager.DefaultColorProgramSteps.Keys.ToArray());

			presets.ItemSelected += Presets_ItemSelected;
			presets.Adapter = presetsAdapter;

			return view;
		}

		private void Presets_ItemSelected(object sender, AdapterView.ItemSelectedEventArgs e)
		{
			if (_firstLoad)
			{
				_firstLoad = false;
				return;
			}

			var presets = (Spinner)sender;
			var program = PresetsManager.DefaultColorProgramSteps[(string)presets.GetItemAtPosition(e.Position)];

			var colorProgram = new ColorProgram(program);
			_listAdapter.SetProgram(colorProgram);
			SetNoStepsDefined(colorProgram.Steps.Length == 0);
		}

		private void GetButton_Click(object sender, EventArgs e)
		{
			try
			{
				_btManager = BluetoothManager.Current;

				var cmd = new Command(Logic.Constants.CommandType.GetColorProgramCommandId);
				var resultData = _btManager.SendCommandAndGetResponse(cmd.Serialize(), CommandResult.GetExpectedDataSize);
				var result = CommandDispatcher.GetCommandResultFromByteArray(resultData);

				if (result.HasError)
				{
					ErrorHandler.HandleErrorWithMessageBox($"Command error: {result.Message}", _view.Context);
					return;
				}

				var colorProgram = result.Data as ColorProgram;
				if (colorProgram == null)
				{
					ErrorHandler.HandleErrorWithMessageBox("Result data is empty or has invalid type", _view.Context);
					return;
				}

				_listAdapter.SetProgram(colorProgram);
				SetNoStepsDefined(colorProgram.Steps.Length == 0);
			}
			catch (Exception ex)
			{
				ErrorHandler.HandleErrorWithMessageBox(ex.Message, _view.Context);
			}
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
				_btManager = BluetoothManager.Current;

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
				var resultData = _btManager.SendCommandAndGetResponse(cmd.Serialize(), CommandResult.GetExpectedDataSize);
				var result = CommandDispatcher.GetCommandResultFromByteArray(resultData);

				if (result.HasError)
				{
					ErrorHandler.HandleErrorWithMessageBox($"Command error: {result.Message}", _view.Context);
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
			if (_btManager != null)
			{
				_btManager.Dispose();
				_btManager = null;
			}
			_steps = _listAdapter.Steps;
			_testing = false;
			base.OnPause();
		}
		public override void OnSaveInstanceState(Bundle outState)
		{
			var serialized = JsonConvert.SerializeObject(_listAdapter.Steps);
			outState.PutString(ColorProgramKey, serialized);

			base.OnSaveInstanceState(outState);
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