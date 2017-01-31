using System;
using LedController.Logic.Entities;
using LedController.Logic.Interfaces;

namespace LedController.Logic
{
	public class DataPacketFactory : IDataPacketFactory
	{
		public IDeserializableEntity GetEntityFromBuffer(byte[] buffer)
		{
			byte packetType = buffer[0];

			switch (packetType)
			{
				case (byte) Constants.PacketType.SpeedColorProgramPacketId:
				{
					var result = new SpeedColorProgramSettings();
					result.Deserialize(buffer);
					return result;
				}
				case (byte) Constants.PacketType.ColorProgramPacketId:
				{
					var result = new ColorProgram();
					result.Deserialize(buffer);
					return result;
				}
				case (byte) Constants.PacketType.SystemInformationPacketId:
				{
					var result = new SystemInformation();
					result.Deserialize(buffer);
					return result;
				}
				case (byte) Constants.PacketType.EmptyDataPacketId:
				{
					return null;
				}
				default:
				{
					throw new InvalidOperationException($"Invalid packet id: {packetType}");
				}
			}
		}
	}
}
