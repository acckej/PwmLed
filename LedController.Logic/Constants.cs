namespace LedController.Logic
{
	public static class Constants
	{
		public const int MaxColorProgramLength = 32;
		public enum CommandType : byte
		{
			UploadColorProgramCommandId = 8,
			UploadSpeedColorProgramCommandId = 5,
			GetSystemInformationCommandId = 2,
			GetSpeedColorProgramCommandId = 4,
			ErrorCommandId = 10,
			GetColorProgramCommandId = 11
		}

		public enum PacketType : byte
		{
			SystemInformationPacketId = 1,
			SpeedColorProgramPacketId = 3,
			CommandResultPacketId = 6,
			ColorProgramPacketId = 7,
			EmptyDataPacketId = 9
		}
	}
}
