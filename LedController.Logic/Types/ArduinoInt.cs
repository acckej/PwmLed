using System;

namespace LedController.Logic.Types
{
	public class ArduinoInt : IArduinoType<Int16>
	{
		public int Size => 2;
		public Int16 Value { get; set; }

		public byte[] GetBytes()
		{
			return BitConverter.GetBytes(Value);
		}

		public void FromBytes(byte[] buf, int index)
		{
			Value = BitConverter.ToInt16(buf, index);
		}
	}
}
