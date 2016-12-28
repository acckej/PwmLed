using LedController.Logic.Types;

namespace LedController.Logic
{
	public static class SerializationHelper
	{
		public static int WriteToBuffer<T>(byte[] buffer, int offset, IArduinoType<T> value)
		{
			value.GetBytes().CopyTo(buffer, offset);
			return offset + value.Size;
		}

		public static int ReadFromBuffer<T>(byte[] buffer, int offset, IArduinoType<T> value)
		{
			value.FromBytes(buffer, offset); 
			return offset + value.Size;
		}
	}
}
