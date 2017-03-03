using System;
using System.Collections.Generic;
using System.Linq;
using LedController.Logic.Interfaces;
using LedController.Logic.Types;

namespace LedController.Logic.Entities
{
	public class ColorProgram : ISerializableEntity, IDeserializableEntity
	{
		public ColorProgram()
		{
			_size = new ArduinoByte().Size + new ArduinoSize().Size;
		}

		public ColorProgram(ColorProgramStep[] steps)
		{
			_size = new ArduinoByte().Size + new ArduinoSize().Size;
			_steps = steps.ToList();
		}

		public byte[] Serialize()
		{
			var buf = new byte[Size];
			int offset = 0;

			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoByte
			{
				Value	= (byte)Constants.PacketType.ColorProgramPacketId
			});

			offset = SerializationHelper.WriteToBuffer(buf, offset, new ArduinoInt
			{
				Value = (Int16)_steps.Count
			});

			foreach (var step in _steps)
			{
				var serialized = step.Serialize();
				serialized.CopyTo(buf, offset);

				offset += step.Size;
			}

			return buf;
		}

		public void Deserialize(byte[] buffer)
		{
			var offset = 0;
			_steps.Clear();

			var packageId = new ArduinoByte();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, packageId);

			if (packageId.Value != (byte)Constants.PacketType.ColorProgramPacketId)
			{
				throw new ApplicationException($"Invalid packet id: {packageId.Value}");
			}

			var steps = new ArduinoInt();
			offset = SerializationHelper.ReadFromBuffer(buffer, offset, steps);

			if (steps.Value < 0 || steps.Value > Constants.MaxColorProgramLength)
			{
				throw new ApplicationException($"Invalid program size id: {steps.Value}");
			}
			
			for (int i = 0; i < steps.Value; i++)
			{
				var step = new ColorProgramStep();
				var stepData = buffer.Skip(offset).Take(step.Size).ToArray(); 

				step.Deserialize(stepData);
				offset += step.Size;
				_steps.Add(step);
			}
		}

		public int Size
		{
			get
			{
				return _size + _steps.Sum(s => s.Size);
			}
		}

		public void Add(ColorProgramStep step)
		{
			if (_steps.Count >= Constants.MaxColorProgramLength)
			{
				throw new InvalidOperationException($"Color program cannot be longer than {Constants.MaxColorProgramLength} steps");
			}

			_steps.Add(step);
		}

		public void RemoveAt(int index)
		{
			_steps.RemoveAt(index);
		}

		public ColorProgramStep[] Steps => _steps.ToArray();

		private readonly List<ColorProgramStep> _steps = new List<ColorProgramStep>();
		private readonly int _size;
	}
}
