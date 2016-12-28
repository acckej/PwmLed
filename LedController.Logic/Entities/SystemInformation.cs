using System;
using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	public class SystemInformation : IDeserializableEntity
	{
		public void Deserialize(byte[] buffer)
		{
			var offset = 0;
			var packetType = new ArduinoByte();
			var size = new ArduinoSize();
			var speed = new ArduinoDouble();
			var voltage = new ArduinoDouble();

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, packetType);

			if (packetType.Value != (byte)Constants.PacketType.SystemInformationPacketId)
			{
				throw new ApplicationException($"Invalid packet id: { packetType }");
			}

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, size);

			if (size.Value != packetType.Size + size.Size + voltage.Size +  speed.Size)
			{
				throw new ApplicationException($"Invalid packet size: { size }");
			}

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, speed);
			SerializationHelper.ReadFromBuffer(buffer, offset, voltage);

			_speed = speed.Value;
			_voltage = voltage.Value;
		}

		public float Voltage => _voltage;

		public float Speed => _speed;

		private float _voltage;
		private float _speed;
	}
}
