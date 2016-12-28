using System;

namespace LedController.Logic.Types
{
	public class ArduinoDouble : IArduinoType<Single>
	{
		public int Size => 4;
		public Single Value { get; set; }

		public byte[] GetBytes()
		{
			return BitConverter.GetBytes(Value);
		}

		public void FromBytes(byte[] buf, int index)
		{
			Value = BitConverter.ToSingle(buf, index);
		}
	}
}
