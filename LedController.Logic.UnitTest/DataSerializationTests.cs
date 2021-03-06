﻿using LedController.Logic.Entities;
using LedController.Logic.Helper;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace LedController.Logic.UnitTest
{
	[TestClass]
	public class DataSerializationTests
	{
		[TestMethod]
		public void DeserializeGetSysInfoCommandResultTest()
		{
			var factory = new DataPacketFactory();
			var packet = DataOperationsHelper.StringToByteArray("06120002000b00010b0000a00c46004ac447");
			var commandResult = new CommandResult(factory);

			commandResult.Deserialize(packet);

			Assert.AreEqual(Constants.CommandType.GetSystemInformationCommandId, commandResult.CommandType);
			Assert.IsNull(commandResult.Message);
			Assert.IsNotNull(commandResult.Data);
			Assert.IsFalse(commandResult.HasError);

			var info = commandResult.Data as SystemInformation;
			Assert.IsNotNull(info);
			Assert.AreEqual((double) 100500, info.Voltage);
			Assert.AreEqual((double) 9000, info.Speed);
		}

		[TestMethod]
		public void GetTelemetryCommandSerializationTest()
		{
			var cmd = new Command(Constants.CommandType.GetSystemInformationCommandId);
			var serialized = cmd.Serialize();
			Assert.IsNotNull(serialized);
		}

		[TestMethod]
		public void DeserializeGetSpeedColorProgramCommandResultTest()
		{
			var factory = new DataPacketFactory();
			var packet = DataOperationsHelper.StringToByteArray("06320004002b00032b000000803f05000400060000001041000000410000e040030000004041000030410000204100000040");
			var commandResult = new CommandResult(factory);

			commandResult.Deserialize(packet);

			Assert.AreEqual(Constants.CommandType.GetSpeedColorProgramCommandId, commandResult.CommandType);
			Assert.IsNull(commandResult.Message);
			Assert.IsNotNull(commandResult.Data);
			Assert.IsFalse(commandResult.HasError);

			var info = commandResult.Data as SpeedColorProgramSettings;
			Assert.IsNotNull(info);

			Assert.AreEqual(1, info.Distance);
			Assert.AreEqual(9, info.MuBlue);
			Assert.AreEqual(8, info.MuGreen);
			Assert.AreEqual(7, info.MuRed);
			Assert.AreEqual(12, info.SigmaBlue);
			Assert.AreEqual(11, info.SigmaGreen);
			Assert.AreEqual(10, info.SigmaRed);
			Assert.AreEqual(2, info.TopSpeed);

			Assert.AreEqual((short)5, info.BlinkDelay);
			Assert.AreEqual((short)4, info.ColorChangePeriod);
			Assert.AreEqual((short)6, info.IdleDelay);
			Assert.AreEqual((short)3, info.NotMovingDelay);
		}

		[TestMethod]
		public void SerializeColorProgramTest()
		{
			var program = new ColorProgram();

			program.Add(new ColorProgramStep
			{
				Blue = 1,
				Red = 2,
				Green = 3,
				Delay = 4
			});

			program.Add(new ColorProgramStep
			{
				Blue = 5,
				Red = 6,
				Green = 7,
				Delay = 8
			});

			program.Add(new ColorProgramStep
			{
				Blue = 100,
				Red = 200,
				Green = 250,
				Delay = 400
			});

			var serialized = program.Serialize();
			var str = DataOperationsHelper.ByteArrayToString(serialized);

			Assert.IsFalse(string.IsNullOrEmpty(str));
		}

		[TestMethod]
		public void SerializeColorProgramTest2()
		{
			var program = new ColorProgram();

			program.Add(new ColorProgramStep
			{
				Blue = 23,
				Red = 123,
				Green = 234,
				Delay = 1000
			});

			program.Add(new ColorProgramStep
			{
				Blue = 123,
				Red = 223,
				Green = 134,
				Delay = 500
			});

			var ser = program.Serialize();
			Assert.IsNotNull(ser);
			var hex = DataOperationsHelper.ByteArrayToString(ser);
			//070200E8037BEA17F401DF867B
			Assert.AreEqual("070200E8037BEA17F401DF867B", hex);
		}

		[TestMethod]
		public void UploadColorProgramSerializationTest()
		{
			var program = new ColorProgram();

			program.Add(new ColorProgramStep
			{
				Blue = 23,
				Red = 123,
				Green = 234,
				Delay = 1000
			});

			program.Add(new ColorProgramStep
			{
				Blue = 123,
				Red = 223,
				Green = 134,
				Delay = 500
			});

			var cmd = new Command(Constants.CommandType.UploadColorProgramCommandId, program);

			var ser = cmd.Serialize();
			var hex = DataOperationsHelper.ByteArrayToString(ser);
			Assert.AreEqual("08070200E8037BEA17F401DF867B", hex);
		}

		[TestMethod]
		public void DeserializeColorProgramTest()
		{
			//070200E8037BEA17F401DF867B
			//070200e8036478824c046e828c
			const string packet = "070200E8037BEA17F401DF867B";
			var data = DataOperationsHelper.StringToByteArray(packet);

			var program = new ColorProgram();
			program.Deserialize(data);

			Assert.AreEqual(2, program.Steps.Length);
		}
	}
}
