#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CommandLibrary/EepromHelper.h"
#include "../CommandLibrary/EepromProvider.h"
#include "TestHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PwmLedUnitTests
{
	TEST_CLASS(CommandDispatcherTests)
	{
	public:

		TEST_METHOD(SaveRestoreSpeedColorFromEepromTest)
		{
			SpeedColorProgramSettings data;
			data.SetData(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

			auto saved = EepromHelper::SaveSpeedColorSettingsToEeprom(&data);
			auto buffer = EepromProvider::GetBuffer();

			Assert::IsNotNull(saved);
			Assert::AreEqual(data.GetDataSize(), static_cast<ArduinoSize>(buffer[SPEEDCOLOR_FLAG_ADDRESS]));

			auto result = EepromHelper::RestoreSpeedColorFromEeprom();

			Assert::IsNotNull(result);
		}

		TEST_METHOD(SaveRestoreColorProgramFromEepromTest)
		{
			auto filledBuf = TestHelper::GetFilledColorProgram();
			ColorProgram program;
			program.FillFromBuffer(filledBuf);

			auto saved = EepromHelper::SaveColorProgramToEeprom(&program);
			auto buffer = EepromProvider::GetBuffer();

			Assert::IsNotNull(saved);
			Assert::AreEqual(program.GetDataSize(), static_cast<ArduinoSize>(buffer[COLORPROGRAM_FLAG_ADDRESS]));

			auto result = EepromHelper::RestoreColorProgramFromEeprom();
			Assert::IsNotNull(result);
		}
	};
	
}
