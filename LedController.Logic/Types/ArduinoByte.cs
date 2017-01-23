namespace LedController.Logic.Types
{
	public class ArduinoByte : IArduinoType<byte>
	{
		public int Size => 1;
		public byte Value { get; set; }

		public byte[] GetBytes()
		{
			return new[] { Value };
		}

		public void FromBytes(byte[] buf, int index)
		{
			Value = buf[index];
		}
	}
}
