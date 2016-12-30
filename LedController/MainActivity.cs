using Android.App;
using Android.OS;
using LedController.Fragments;

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

		private void AddTabToActionBar(int labelResourceId, int iconResourceId)
		{
			ActionBar.Tab tab = ActionBar.NewTab()
										 .SetText(labelResourceId)
										 .SetIcon(iconResourceId);
			tab.TabSelected += TabOnTabSelected;
			ActionBar.AddTab(tab);
		}

		private void TabOnTabSelected(object sender, ActionBar.TabEventArgs tabEventArgs)
		{
			ActionBar.Tab tab = (ActionBar.Tab)sender;
			
			Fragment frag = _fragments[tab.Position];
			tabEventArgs.FragmentTransaction.Replace(Resource.Id.flMain, frag);
		}
	}
}

