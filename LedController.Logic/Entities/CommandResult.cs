using System;
using System.Linq;
using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	public class CommandResult : IDeserializableEntity
	{
		public CommandResult(IDataPacketFactory factory)
		{
			_factory = factory;
		}

		public void Deserialize(byte[] buffer)
		{
			var offset = 0;
			ArduinoByte packetType = new ArduinoByte();
			ArduinoSize size = new ArduinoSize();

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, packetType);

			if (packetType.Value != (byte)Constants.PacketType.CommandResultPacketId)
			{
				throw new ApplicationException($"Invalid packet id: { packetType }");
			}

			offset = SerializationHelper.ReadFromBuffer(buffer, offset, size);

			if (size.Value != buffer.Length)
			{
				throw new ApplicationException($"Invalid packet size: { size }, expected { buffer.Length }");
			}

			ArduinoByte commandId = new ArduinoByte();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, commandId);

			_commandType = (Constants.CommandType)commandId.Value;

			ArduinoBool hasError = new ArduinoBool();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, hasError);
			ArduinoSize messageLength = new ArduinoSize();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, messageLength);
			_hasError = hasError.Value;

			if (hasError.Value)
			{
				_data = null;
				_message = System.Text.Encoding.ASCII.GetString(buffer, offset, messageLength.Value);
			}
			else
			{
				if (messageLength.Value == 0)
				{
					_data = null;
				}
				else
				{
					var data = buffer.Skip(offset).ToArray();
					_data = _factory.GetEntityFromBuffer(data);
				}
				
				_message = null;
			}
		}

		public Constants.CommandType CommandType => _commandType;

		public string Message => _message;

		public IDeserializableEntity Data => _data;

		public bool HasError => _hasError;

		private Constants.CommandType _commandType;
		private string _message;
		private bool _hasError;
		private IDeserializableEntity _data;
		private readonly IDataPacketFactory _factory;
	}
}
