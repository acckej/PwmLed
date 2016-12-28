using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	public class ColorProgramStep : ISerializableEntity
	{
		public ColorProgramStep()
		{
			Size = new ArduinoInt().Size + new ArduinoByte().Size*3;
		}
		public byte[] Serialize()
		{
			var buf = new byte[Size];
			var offset = 0;

			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoInt
			{
				Value = Delay
			});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoByte
			{
				Value = Red
			});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoByte
			{
				Value = Green
			});
			SerializationHelper.WriteToBuffer(buf, offset, new ArduinoByte
			{
				Value= Blue
			});

			return buf;
		}

		public int Size { get; }

		public short Delay { get; set; }

		public byte Red { get; set; }

		public byte Green { get; set; }

		public byte Blue { get; set; }
	}
}
