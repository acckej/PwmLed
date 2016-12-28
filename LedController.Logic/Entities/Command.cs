using System.Linq;
using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	public class Command : ISerializableEntity
	{
		public Command(Constants.CommandType type, ISerializableEntity data = null)
		{
			_type = new ArduinoByte
			{
				Value = (byte)type
			};
			_data = data;
		}

		public byte[] Serialize()
		{

			var header = new byte[_type.Size];
			header[0] = _type.Value;
			byte[] data;
			if (_data == null)
			{
				data = new byte[sizeof(Constants.PacketType)];
				data[0] = (byte) Constants.PacketType.EmptyDataPacketId;
			}
			else
			{
				data = _data.Serialize();
			}

			return header.Union(data).ToArray();
		}

		public int Size => _type.Size + (_data?.Size ?? sizeof(Constants.PacketType));

		private readonly ArduinoByte _type;
		private readonly ISerializableEntity _data;
	}
}
