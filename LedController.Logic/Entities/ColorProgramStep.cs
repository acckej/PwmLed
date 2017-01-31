using System.Runtime.Serialization;
using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	[DataContract]
	public class ColorProgramStep : ISerializableEntity, IDeserializableEntity
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

		public void Deserialize(byte[] buffer)
		{
			var offset = 0;
			var delay = new ArduinoInt();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, delay);
			Delay = delay.Value;

			var red = new ArduinoByte();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, red);
			Red = red.Value;

			var green = new ArduinoByte();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, green);
			Green = green.Value;

			var blue = new ArduinoByte();
			SerializationHelper.ReadFromBuffer(buffer, offset, blue);
			Blue = blue.Value;
		}

		public int Size { get; }

		[DataMember]
		public short Delay { get; set; }

		[DataMember]
		public byte Red { get; set; }

		[DataMember]
		public byte Green { get; set; }

		[DataMember]
		public byte Blue { get; set; }
	}
}
