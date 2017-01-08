using System;
using LedController.Logic.Entities;

namespace LedController.Logic
{
	public static class CommandDispatcher
	{
		public static CommandResult GetCommandResultFromByteArray(byte[] resultData)
		{
			if (resultData == null)
			{
				throw new ApplicationException("Command result is null");
			}

			if (resultData.Length == 0)
			{
				throw new ApplicationException("Command result is empty");
			}

			var result = new CommandResult(new DataPacketFactory());
			result.Deserialize(resultData);

			return result;
		}
	}
}
