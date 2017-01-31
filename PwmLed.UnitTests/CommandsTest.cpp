#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CommandLibrary/CommandResult.h"
#include "../CommandLibrary/SystemInformation.h"
#include "TestHelper.h"
#include "../CommandLibrary/ColorProgram.h"
#include "../CommandLibrary/SpeedColorProgramSettings.h"
#include "../CommandLibrary/DataSerializationHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PwmLedUnitTests
{		
	TEST_CLASS(CommandsTest)
	{
	public:
		
		TEST_METHOD(CommandResultTest)
		{
			char* message = "test error message";
			auto result = new CommandResult(GetSystemInformationCommandId, message, strlen(message), true);

			auto length = result->GetDataSize();
			auto buf = new char[length];
			
			result->WriteDataToBuffer(buf);

			auto tempBuf = buf;
			auto packetId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuf);
			auto dataSize = DataSerializationHelper<ArduinoSize>::GetDataFromArray(&tempBuf);
			auto commandId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuf);
			auto hasError = DataSerializationHelper<bool>::GetDataFromArray(&tempBuf);
			auto messageLength = DataSerializationHelper<ArduinoSize>::GetDataFromArray(&tempBuf);

			std::string msg(tempBuf, messageLength);

			Assert::AreEqual(message, msg.data());
			Assert::AreEqual(CommandResultPacketId, packetId);
			Assert::AreEqual(length, dataSize);
			Assert::AreEqual(GetSystemInformationCommandId, commandId);
			Assert::AreEqual(true, hasError);

			delete buf;
			delete result;
		}

		TEST_METHOD(CommandResultTest_EntityData)
		{
			SystemInformation info(22, 33);
			auto message = new char[info.GetDataSize()];
			info.WriteDataToBuffer(message);

			auto result = new CommandResult(GetSystemInformationCommandId, message, info.GetDataSize(), false);

			auto length = result->GetDataSize();
			auto buf = new char[length];

			result->WriteDataToBuffer(buf);

			auto tempBuf = buf;
			auto packetId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuf);
			auto dataSize = DataSerializationHelper<ArduinoSize>::GetDataFromArray(&tempBuf);
			auto commandId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuf);
			auto hasError = DataSerializationHelper<bool>::GetDataFromArray(&tempBuf);
			auto messageLength = DataSerializationHelper<ArduinoSize>::GetDataFromArray(&tempBuf);

			Assert::AreEqual(info.GetDataSize(), messageLength);
			Assert::AreEqual(CommandResultPacketId, packetId);
			Assert::AreEqual(length, dataSize);
			Assert::AreEqual(GetSystemInformationCommandId, commandId);
			Assert::AreEqual(false, hasError);

			delete buf;
			delete result;
		}

		TEST_METHOD(ColorProgramDeserializationTest)
		{
			auto input = "070300040002030108000607059001C8FA64";
			auto filledBuf = new char[strlen(input) / 2];
			TestHelper::Hex2bin(input, filledBuf);
			
			ColorProgram program;

			program.FillFromBuffer(filledBuf);

			Assert::AreEqual(3, static_cast<int>(program.GetNumberOfSteps()));

			auto counter = static_cast<ArduinoInt>(0);

			do
			{
				auto step = program.GetNextStep();
				counter++;
			} while (!program.IsLastStep() && counter <= NumberOfStepsMax);

			Assert::AreEqual(3, static_cast<int>(counter));

			delete filledBuf;
		}

		TEST_METHOD(ColorProgramTest)
		{			
			auto filledBuf = TestHelper::GetFilledColorProgram();

			ColorProgram program;

			program.FillFromBuffer(filledBuf);

			Assert::AreEqual(TestHelper::StepsCount, program.GetNumberOfSteps());

			auto counter = static_cast<ArduinoInt>(0);

			do
			{
				auto step = program.GetNextStep();
				counter++;
			} while (!program.IsLastStep() && counter <= NumberOfStepsMax);

			Assert::AreEqual(TestHelper::StepsCount, counter);

			delete filledBuf;
		}

		TEST_METHOD(SpeedColorProgramTest)
		{
			SpeedColorProgramSettings settings;

			auto dataSize = settings.GetDataSize();
			auto buf = new char[dataSize];

			settings.SetData(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
			settings.WriteDataToBuffer(buf);

			SpeedColorProgramSettings settings2;

			settings2.FillFromBuffer(buf);

			Assert::AreEqual(settings.GetBlinkDelay(), settings2.GetBlinkDelay());
			Assert::AreEqual(settings.GetDataSize(), settings2.GetDataSize());
			Assert::AreEqual(settings.GetColorChangePeriod(), settings2.GetColorChangePeriod());
			Assert::AreEqual(settings.GetDistance(), settings2.GetDistance());
			Assert::AreEqual(settings.GetIdleDelay(), settings2.GetIdleDelay());
			Assert::AreEqual(settings.GetMuBlue(), settings2.GetMuBlue());
			Assert::AreEqual(settings.GetMuGreen(), settings2.GetMuGreen());
			Assert::AreEqual(settings.GetMuRed(), settings2.GetMuRed());
			Assert::AreEqual(settings.GetNotMovingDelay(), settings2.GetNotMovingDelay());
			Assert::AreEqual(settings.GetSigmaBlue(), settings2.GetSigmaBlue());
			Assert::AreEqual(settings.GetSigmaRed(), settings2.GetSigmaRed());
			Assert::AreEqual(settings.GetSigmaGreen(), settings2.GetSigmaGreen());

			delete buf;
		}

		TEST_METHOD(SystemInformationTest)
		{
			SystemInformation info(1030, 2050);

			auto dataSize = info.GetDataSize();
			auto buf = new char[dataSize];

			Assert::AreEqual(21, static_cast<int>(dataSize));
			info.WriteDataToBuffer(buf);

			auto tempBuf = buf;

			auto packetId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuf);
			auto size = DataSerializationHelper<size_t>::GetDataFromArray(&tempBuf);
			auto voltage = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&tempBuf);
			auto speed = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&tempBuf);

			delete buf;

			Assert::AreEqual(SystemInformationPacketId, packetId);
			Assert::AreEqual(21, static_cast<int>(size));
			Assert::AreEqual(static_cast<ArduinoDouble>(1030), speed);
			Assert::AreEqual(static_cast<ArduinoDouble>(2050), voltage);
		}

		TEST_METHOD(HelperTest)
		{
			unsigned char buf[10]{1, 2, 3, 4, 5, 100, 7, 80, 90, 10};
			auto str = TestHelper::HexChar(buf, 10);

			Assert::IsNotNull(str);
			Assert::AreEqual(20, static_cast<int>(strlen(str)));
			delete str;
		}
	};
}