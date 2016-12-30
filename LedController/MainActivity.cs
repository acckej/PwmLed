using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Android.App;
using Android.Bluetooth;
using Android.Graphics;
using Android.Widget;
using Android.OS;
using Android.Views;
using LedController.Adapters;
using LedController.Fragments;
using LedController.Logic.Entities;

namespace LedController
{
	[Activity(Label = "LedController", MainLauncher = true, Icon = "@drawable/icon")]
	public class MainActivity : Activity
	{
		Fragment[] _fragments;

		protected override void OnCreate(Bundle bundle)
		{
			base.OnCreate(bundle);

			SetContentView(Resource.Layout.Main);

			ActionBar.NavigationMode = ActionBarNavigationMode.Tabs;
			SetContentView(Resource.Layout.Main);

			_fragments = new Fragment[]
						 {
							 new SpeedColorFragment(),
							 new ColorProgramFragment(), 
							 new TelemetryFragment() 
						 };

			AddTabToActionBar(Resource.String.tbSpeedColor, Resource.Drawable.speed_color_tab);
			AddTabToActionBar(Resource.String.tbColorProgram, Resource.Drawable.color_program_tab);
			AddTabToActionBar(Resource.String.tbTelemetry, Resource.Drawable.telemetry_tab);
		}

		void AddTabToActionBar(int labelResourceId, int iconResourceId)
		{
			ActionBar.Tab tab = ActionBar.NewTab()
										 .SetText(labelResourceId)
										 .SetIcon(iconResourceId);
			tab.TabSelected += TabOnTabSelected;
			ActionBar.AddTab(tab);
		}

		void TabOnTabSelected(object sender, ActionBar.TabEventArgs tabEventArgs)
		{
			ActionBar.Tab tab = (ActionBar.Tab)sender;
			
			Fragment frag = _fragments[tab.Position];
			tabEventArgs.FragmentTransaction.Replace(Resource.Id.flMain, frag);
		}

		//private ColorProgramStepAdapter _listAdapter;
		//private bool _testing;

		//protected override void OnCreate(Bundle bundle)
		//{
		//	base.OnCreate(bundle);

		//	// Set our view from the "main" layout resource
		//	SetContentView(Resource.Layout.Main);

		//	var list = FindViewById<ListView>(Resource.Id.lvColorProgram);

		//	var listAdapter = new ColorProgramStepAdapter(new List<ColorProgramStep>
		//	{
		//		new ColorProgramStep
		//		{
		//			Blue = 23,
		//			Red = 123,
		//			Green = 234,
		//			Delay = 1000
		//		},
		//		new ColorProgramStep
		//		{
		//			Blue = 123,
		//			Red = 223,
		//			Green = 134,
		//			Delay = 500
		//		}
		//	});

		//	_listAdapter = listAdapter;
		//	list.Adapter = listAdapter;
		//	listAdapter.OnColorBoxClicked += ListAdapterOnOnColorBoxClicked;
		//	listAdapter.OnStepButtonClicked += ListAdapterOnOnStepButtonClicked;

		//	var addButton = FindViewById<Button>(Resource.Id.btnAdd);
		//	addButton.Click += AddButton_Click;

		//	var uploadButton = FindViewById<Button>(Resource.Id.btnUpload);
		//	uploadButton.Click += UploadButton_Click; 

		//	var downloadButton = FindViewById<Button>(Resource.Id.btnDownload);
		//	downloadButton.Click += DownloadButton_Click;

		//	var testButton = FindViewById<Button>(Resource.Id.btnTest);
		//	testButton.Click += TestButton_Click; 
		//}

		//private void TestButton_Click(object sender, EventArgs e)
		//{
		//	var items = ListAdapter;
		//	var testButton = FindViewById<Button>(Resource.Id.btnTest);

		//	if (_testing)
		//	{
		//		_testing = false;
		//		testButton.Text = GetString(Resource.String.btnPlayText);
		//	}
		//	else
		//	{
		//		_testing = true;
		//		testButton.Text = GetString(Resource.String.btnPlayStopText);
		//	}

		//	if (items != null)
		//	{
		//		Task.Run(() =>
		//		{
		//			while (_testing)
		//			{
		//				for (var i = 0; i < items.Count; i++)
		//				{
		//					var item = items[i];

		//					RunOnUiThread(() =>
		//					{
		//						Window.DecorView.SetBackgroundColor(new Color(item.Red, item.Green, item.Blue));
		//					});

		//					Thread.Sleep(item.Delay);
		//				}
		//			}

		//			RunOnUiThread(() =>
		//			{
		//				Window.DecorView.SetBackgroundColor(new Color(0, 0, 0));
		//			});

		//			Thread.Sleep(1000);
		//		});
		//	}
		//}

		//private void DownloadButton_Click(object sender, EventArgs e)
		//{
		//}

		//private void UploadButton_Click(object sender, EventArgs e)
		//{
		//	//BluetoothAdapter ba = BluetoothAdapter.DefaultAdapter;
		//}

		//private void AddButton_Click(object sender, EventArgs e)
		//{
		//	ListAdapter.Add(new ColorProgramStep
		//	{
		//		Blue = 128,
		//		Red = 128,
		//		Green = 128,
		//		Delay = 1000
		//	});

		//	SetNoStepsDefined(false);
		//}

		//private ColorProgramStepAdapter ListAdapter
		//{
		//	get
		//	{
		//		if (_listAdapter == null)
		//		{
		//			var list = FindViewById<ListView>(Resource.Id.lvColorProgram);
		//			_listAdapter = list.Adapter as ColorProgramStepAdapter;
		//		}

		//		return _listAdapter;
		//	}
		//}

		//private void SetNoStepsDefined(bool noStepsDefined)
		//{
		//	var noStepsVisible = noStepsDefined ? ViewStates.Visible : ViewStates.Invisible;
		//	var noStepsInvisible = noStepsDefined ? ViewStates.Invisible : ViewStates.Visible;

		//	var list = FindViewById<ListView>(Resource.Id.lvColorProgram);
		//	list.Visibility = noStepsInvisible;
		//	var noSteps = FindViewById<TextView>(Resource.Id.txtNoSteps);
		//	noSteps.Visibility = noStepsVisible;

		//	var uploadButton = FindViewById<Button>(Resource.Id.btnUpload);
		//	uploadButton.Enabled = !noStepsDefined;

		//	var testButton = FindViewById<Button>(Resource.Id.btnTest);
		//	testButton.Enabled = !noStepsDefined;

		//	_testing = _testing ? !noStepsDefined : _testing;
		//}

		//private void ListAdapterOnOnStepButtonClicked(int position)
		//{
		//	ListAdapter?.Remove(position);

		//	SetNoStepsDefined(ListAdapter != null && ListAdapter.Count == 0);
		//}

		//private void ListAdapterOnOnColorBoxClicked(int position)
		//{
		//	var color = Color.Red;
		//	if (ListAdapter != null)
		//	{
		//		var item = ListAdapter[position];
		//		color = new Color(item.Red, item.Green, item.Blue);
		//	}

		//	var roundColorPickerDialog = new ColorPickerDialog(this, color);
		//	roundColorPickerDialog.ColorChanged += (o, args) =>
		//	{
		//		ListAdapter?.SetColor(args.Color, position);
		//	};
		//	roundColorPickerDialog.Show();
		//}
	}
}

