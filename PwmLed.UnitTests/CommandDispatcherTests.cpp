#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CommandLibrary/CommandResult.h"
#include "../CommandLibrary/SystemInformation.h"
#include "TestHelper.h"
#include "../CommandLibrary/ColorProgram.h"
#include "../CommandLibrary/SpeedColorProgramSettings.h"
#include "../CommandLibrary/DataEntityFactory.h"
#include "../CommandLibrary/CommandDispatcher.h"
#include "../CommandLibrary/DataSerializationHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static SpeedColorProgramSettings* _speedColorProgram;
static SystemInformation* _sysInfo;
static ColorProgram* _colorProgram;

namespace PwmLedUnitTests
{
	TEST_CLASS(CommandDispatcherTests)
	{
	public:

		TEST_METHOD(DispatchGetSysInfoTest)
		{
			DataEntityFactory factory;
			CommandDispatcher dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &factory, GetColorProgram);

			_sysInfo = new SystemInformation(100500, 9000);
			//_sysInfo = new SystemInformation(0, 0);
			_speedColorProgram = nullptr;
			
			char buf[sizeof(unsigned char) * 2];
			buf[0] = GetSystemInformationCommandId;
			buf[1] = EmptyDataPacketId;

			auto result = dispatcher.ReceivePacket(buf);

			Assert::IsNotNull(result);
			auto message = result->GetMessage();

			Assert::IsNotNull(message);
			Assert::AreEqual(false, result->HasError());

			auto tempBuf = message;
			auto packetId = DataSerializationHelper<unsigned char>::GetDataFromArray(const_cast<char**>(&tempBuf));

			Assert::AreEqual(SystemInformationPacketId, packetId);
			auto size = DataSerializationHelper<ArduinoSize>::GetDataFromArray(const_cast<char**>(&tempBuf));
			auto speed = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(const_cast<char**>(&tempBuf));
			auto voltage = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(const_cast<char**>(&tempBuf));

			Assert::AreEqual(static_cast<ArduinoSize>(13), size);
			Assert::AreEqual(static_cast<ArduinoDouble>(100500), voltage);
			Assert::AreEqual(static_cast<ArduinoDouble>(9000), speed);
			
			auto dataSize = result->GetDataSize();
			auto resultSerialized = new char[dataSize];
			result->WriteDataToBuffer(resultSerialized);

			auto hexString = TestHelper::HexStr(reinterpret_cast<unsigned char*>(resultSerialized), result->GetDataSize());
			//061800000002000d000000010d00000000a00c46004ac447
			//06120002000b00010b0000a00c46004ac447

			Assert::AreEqual(static_cast<unsigned>(48), hexString.size());

			delete resultSerialized;
			delete _sysInfo;
			delete result;
		}

		TEST_METHOD(DispatchGetSpeedColorProgramTest)
		{
			DataEntityFactory factory;
			CommandDispatcher dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &factory, GetColorProgram);

			_sysInfo = nullptr;
			_speedColorProgram = new SpeedColorProgramSettings();
			_speedColorProgram->SetData(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

			char buf[sizeof(unsigned char) * 2];
			buf[0] = GetSpeedColorProgramCommandId;
			buf[1] = EmptyDataPacketId;
			
			auto result = dispatcher.ReceivePacket(buf);

			Assert::IsNotNull(result);
			auto message = result->GetMessage();

			Assert::IsNotNull(message);
			Assert::AreEqual(false, result->HasError());

			auto tempBuf = message;
			
			SpeedColorProgramSettings settings;
			settings.FillFromBuffer(const_cast<char*>(tempBuf));
			
			Assert::AreEqual(_speedColorProgram->GetDistance(), settings.GetDistance());
			Assert::AreEqual(_speedColorProgram->GetSigmaBlue(), settings.GetSigmaBlue());
			Assert::AreEqual(_speedColorProgram->GetMuRed(), settings.GetMuRed());

			auto resultSerialized = new char[result->GetDataSize()];
			result->WriteDataToBuffer(resultSerialized);

			auto hexString = TestHelper::HexStr(reinterpret_cast<unsigned char*>(resultSerialized), result->GetDataSize());
			//063800000004002d000000032d0000000000803f05000400060000001041000000410000e040030000004041000030410000204100000040
			//06320004002b00032b000000803f05000400060000001041000000410000e040030000004041000030410000204100000040

			Assert::AreEqual(static_cast<unsigned>(112), hexString.size());
			
			delete result;
			delete resultSerialized;
			delete _speedColorProgram;
		}

		TEST_METHOD(DispatchApplyColorProgramTest)
		{
			DataEntityFactory factory;
			CommandDispatcher dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &factory, GetColorProgram);

			_sysInfo = nullptr;
			_speedColorProgram = nullptr;

			auto program = TestHelper::GetFilledColorProgram();
			auto size = TestHelper::ProgramSize + sizeof(UploadColorProgramCommandId);
			auto buf = new char[size];
			memcpy(buf + sizeof(UploadColorProgramCommandId), program, TestHelper::ProgramSize);
			buf[0] = UploadColorProgramCommandId;

			auto hexString = TestHelper::HexStr(reinterpret_cast<unsigned char*>(buf), size);
			//08070200e8036478824c046e828c
			Assert::AreEqual(std::string("08070200e8036478824c046e828c"), hexString);
			
			auto result = dispatcher.ReceivePacket(buf);

			Assert::IsNotNull(result);
			auto message = result->GetMessage();

			Assert::IsNull(message);
			Assert::AreEqual(false, result->HasError());

			delete result;
			delete program;
			delete buf;
		}

		TEST_METHOD(DispatchApplyColorProgramTest2)
		{
			DataEntityFactory factory;
			CommandDispatcher dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &factory, GetColorProgram);

			_sysInfo = nullptr;
			_speedColorProgram = nullptr;

			//08070200E8037BEA17F401DF867B
			auto input = "070200E8037BEA17F401DF867B";
			auto filledBuf = new char[strlen(input) / 2];

			TestHelper::Hex2bin(input, filledBuf);
			ColorProgram* program = new ColorProgram();
			program->FillFromBuffer(filledBuf);

			auto size = TestHelper::ProgramSize + sizeof(UploadColorProgramCommandId);
			auto buf = new char[size];
			memcpy(buf + sizeof(UploadColorProgramCommandId), filledBuf, TestHelper::ProgramSize);
			buf[0] = UploadColorProgramCommandId;

			auto step1 = program->GetNextStep();
			auto step2 = program->GetNextStep();

			auto hexString = TestHelper::HexStr(reinterpret_cast<unsigned char*>(buf), size);
			
			//Assert::AreEqual(std::string("08070200e8036478824c046e828c"), hexString);

			auto result = dispatcher.ReceivePacket(buf);

			Assert::IsNotNull(result);
			auto message = result->GetMessage();

			Assert::IsNull(message);
			Assert::AreEqual(false, result->HasError());
			auto dataSize = result->GetDataSize();

			delete result;
			delete program;
			delete buf;
		}

		TEST_METHOD(DispatchApplySpeedColorProgramTest)
		{
			DataEntityFactory factory;
			CommandDispatcher dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &factory, GetColorProgram);

			_sysInfo = nullptr;
			_speedColorProgram = nullptr;

			SpeedColorProgramSettings settings;
			settings.SetData(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
			auto size = settings.GetDataSize() + sizeof(UploadSpeedColorProgramCommandId);
			auto buf = new char[size];
			buf[0] = UploadSpeedColorProgramCommandId;
			settings.WriteDataToBuffer(buf + sizeof(UploadSpeedColorProgramCommandId));

			auto hexString = TestHelper::HexStr(reinterpret_cast<unsigned char*>(buf), size);
			//05032b000000803f05000400060000001041000000410000e040030000004041000030410000204100000040
			//05032d0000000000803f05000400060000001041000000410000e040030000004041000030410000204100000040
			//Assert::AreEqual(std::string("05032b000000803f05000400060000001041000000410000e040030000004041000030410000204100000040"), hexString);

			auto result = dispatcher.ReceivePacket(buf);

			Assert::IsNotNull(result);
			auto message = result->GetMessage();

			Assert::IsNull(message);
			Assert::AreEqual(false, result->HasError());
						
			delete result;
			delete buf;
		}

		TEST_METHOD(DispatchGetColorProgramTest)
		{
			DataEntityFactory factory;
			CommandDispatcher dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &factory, GetColorProgram);

			_sysInfo = nullptr;
			_speedColorProgram = nullptr;

			auto filled = TestHelper::GetFilledColorProgram();
			ColorProgram* program = new ColorProgram();
			program->FillFromBuffer(filled);
			_colorProgram = program;

			char buf[2]{ GetColorProgramCommandId, 9 };

			auto result = dispatcher.ReceivePacket(buf);

			Assert::IsNotNull(result);
			auto message = result->GetMessage();

			Assert::IsNotNull(message);
			Assert::AreEqual(false, result->HasError());

			auto hexString = TestHelper::HexStr((unsigned char*)result->GetMessage(), result->GetMessageLength());

			delete filled;
			delete result;
		}

	private:
		static SerializableEntityBase* GetSysInfo();
		static void ApplyColorProgram(DeserializableEntityBase*);
		static void ApplySpeedColorProgram(DeserializableEntityBase*);
		static SerializableEntityBase* GetCurrentSpeedColorProgram();
		static SerializableEntityBase* GetColorProgram();
	};

	SerializableEntityBase* CommandDispatcherTests::GetSysInfo() 
	{
		return _sysInfo;
	}

	SerializableEntityBase* CommandDispatcherTests::GetColorProgram()
	{
		return _colorProgram;
	}

	void CommandDispatcherTests::ApplyColorProgram(DeserializableEntityBase* entity)
	{
		auto data = reinterpret_cast<ColorProgram*>(entity);

		Assert::IsNotNull(data);
		Assert::AreEqual(static_cast<ArduinoInt>(2), data->GetNumberOfSteps());
		Assert::AreEqual(static_cast<int>(TestHelper::ProgramSize), static_cast<int>(data->GetDataSize()));	

		delete entity;
	}

	void CommandDispatcherTests::ApplySpeedColorProgram(DeserializableEntityBase* entity)
	{
		auto data = reinterpret_cast<SpeedColorProgramSettings*>(entity);

		Assert::IsNotNull(data);

		delete entity;
	}

	SerializableEntityBase* CommandDispatcherTests::GetCurrentSpeedColorProgram()
	{		
		return _speedColorProgram;
	}
}
