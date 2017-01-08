using Android.App;
using Android.Content;
using Android.Util;

namespace LedController
{
	public static class ErrorHandler
	{
		public static void HandleErrorWithMessageBox(string message, Context context)
		{
			Log.Error("LedControllerError", message);
			new AlertDialog.Builder(context).SetMessage(message).Show();
		}

		public static void HandleError(string message)
		{
			Log.Error("LedControllerError", message);
		}
	}
}