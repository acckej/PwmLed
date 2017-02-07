namespace LedController
{
	public static class Constants
	{
		public static class SpeedColor
		{
			public const int DefaultWheelCircle = 2350;
			public const int DefaultMaxSpeed = 41670;
			public const int MaxSpeed = 100000;
			
			public const int MinSpeed = 500;
			
			public const int CoefficientsSliderMultiplier = 10000;
			
			public const int DefaultMuRed = 10420;
			public const int DefaultMuGreen = 20830;
			public const int DefaultMuBlue = 0;

			public const int DefaultSigmaRed = 10420;
			public const int DefaultSigmaGreen = 10402;
			public const int DefaultSigmaBlue = 20830;

			public const int DefaultColorChangePeriod = 1000;
			public const int MinColorChangePeriod = 100;
			public const int MaxColorChangePeriod = 4000;

			public const int MinNotMovingDelay = 1000;
			public const int MaxNotMovingDelay = 10000;
			public const int DefaultNotMovingDelay = 6000;

			public const int MinIdleDelay = 100;
			public const int MaxIdleDelay = 2000;
			public const int DefaultIdleDelay = 300;

			public const int MinBlinkDelay = 100;
			public const int MaxBlinkDelay = 2000;
			public const int DefaultBlinkDelay = 1000;

			public const double Threshold = 2;
		}

		public static class Telemetry
		{
			public const int VoltageMultiplier = 100;
			public const float SpeedMultiplier = (float) 3.6;
			public const int UpdatePeriod = 500;
		}
	}
}