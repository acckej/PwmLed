using System;
using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	public class SpeedColorProgramSettings : ISerializableEntity, IDeserializableEntity
	{
		public float Distance { get; set; }

		public float TopSpeed { get; set; }

		public short NotMovingDelay { get; set; }

		public short ColorChangePeriod { get; set; }

		public short BlinkDelay { get; set; }

		public short IdleDelay { get; set; }
		
		public float MuRed { get; set; }

		public float MuGreen { get; set; }

		public float MuBlue { get; set; }

		public float SigmaRed { get; set; }

		public float SigmaGreen { get; set; }

		public float SigmaBlue { get; set; }

		public byte[] Serialize()
		{
			var buf = new byte[Size];
			var offset = 0;

			offset = SerializationHelper.WriteToBuffer(buf, offset,
				new ArduinoByte {Value = (byte) Constants.PacketType.SpeedColorProgramPacketId});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoSize {Value = (short) Size});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = Distance});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoInt {Value = BlinkDelay});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoInt {Value = ColorChangePeriod});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoInt {Value = IdleDelay});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = MuBlue});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = MuGreen});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = MuRed});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoInt {Value = NotMovingDelay});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = SigmaBlue});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = SigmaGreen});
			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = SigmaRed});
			SerializationHelper.WriteToBuffer(buf, offset, new ArduinoDouble {Value = TopSpeed});

			return buf;
		}

		public int Size => new ArduinoDouble().Size * 8 + new ArduinoInt().Size * 4 + sizeof(Constants.PacketType) + new ArduinoSize().Size;

		public void Deserialize(byte[] buffer)
		{
			var offset = 0;
			var packetType = new ArduinoByte();
			var size = new ArduinoSize();

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, packetType);

			if (packetType.Value != (byte) Constants.PacketType.SpeedColorProgramPacketId)
			{
				throw new ApplicationException($"Invalid packet id: { packetType }");
			}

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, size);

			if (size.Value != Size)
			{
				throw new ApplicationException($"Invalid packet size: { size }");
			}

			
			var distance = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, distance);
			Distance = distance.Value;
			var blinkDelay = new ArduinoInt();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, blinkDelay);
			BlinkDelay = blinkDelay.Value;
			var colorChangePeriod = new ArduinoInt();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, colorChangePeriod);
			ColorChangePeriod = colorChangePeriod.Value;
			var idleDelay = new ArduinoInt();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, idleDelay);
			IdleDelay = idleDelay.Value;
			var muBlue = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, muBlue);
			MuBlue = muBlue.Value;
			var muGreen = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, muGreen);
			MuGreen = muGreen.Value;
			var muRed = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, muRed);
			MuRed = muRed.Value;
			var notMovingDelay = new ArduinoInt();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, notMovingDelay);
			NotMovingDelay = notMovingDelay.Value;
			var sigmaBlue = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, sigmaBlue);
			SigmaBlue = sigmaBlue.Value;
			var sigmaGreen = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, sigmaGreen);
			SigmaGreen = sigmaGreen.Value;
			var sigmaRed = new ArduinoDouble();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, sigmaRed);
			SigmaRed = sigmaRed.Value;
			var topSpeed = new ArduinoDouble();
			SerializationHelper.ReadFromBuffer(buffer, offset, topSpeed);
			TopSpeed = topSpeed.Value;
		}
	}
}
