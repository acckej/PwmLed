using System;

namespace LedController.Logic.Types
{
	public class ArduinoBool : IArduinoType<bool>
	{
		public int Size => 1;
		public bool Value { get; set; }
		public byte[] GetBytes()
		{
			return BitConverter.GetBytes(Value);
		}

		public void FromBytes(byte[] buf, int index)
		{
			Value = buf[index] == 1;
		}
	}
}
