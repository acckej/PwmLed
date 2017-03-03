using System.Collections.Generic;
using LedController.Logic.Entities;

namespace LedController.Adapters
{
	public class PresetsManager
	{
		public static Dictionary<string, ColorProgramStep[]> DefaultColorProgramSteps = new Dictionary<string, ColorProgramStep[]>();

		static PresetsManager()
		{
			DefaultColorProgramSteps.Add("Police", new []
			{
				new ColorProgramStep
				{
					Delay = 500,
					Green = 0,
					Blue = 0,
					Red = 255
				},
				new ColorProgramStep
				{
					Delay = 500,
					Green = 0,
					Blue = 255,
					Red = 0
				}
			});

			DefaultColorProgramSteps.Add("Red green", new[]
			{
				new ColorProgramStep
				{
					Delay = 500,
					Green = 0,
					Blue = 0,
					Red = 255
				},
				new ColorProgramStep
				{
					Delay = 500,
					Green = 255,
					Blue = 0,
					Red = 0
				}
			});

			DefaultColorProgramSteps.Add("Blue green orange", new[]
			{
				new ColorProgramStep
				{
					Delay = 300,
					Green = 0,
					Blue = 255,
					Red = 0
				},
				new ColorProgramStep
				{
					Delay = 300,
					Green = 255,
					Blue = 0,
					Red = 0
				},
				new ColorProgramStep
				{
					Delay = 300,
					Green = 148,
					Blue = 28,
					Red = 227
				}
			});

			DefaultColorProgramSteps.Add("Pumpkin", new[]
			{
				new ColorProgramStep
				{
					Delay = 500,
					Green = 148,
					Blue = 28,
					Red = 227
				},
				new ColorProgramStep
				{
					Delay = 500,
					Green = 244,
					Blue = 11,
					Red = 239
				}
			});

			DefaultColorProgramSteps.Add("Green flash", new[]
			{
				new ColorProgramStep
				{
					Delay = 1000,
					Green = 255,
					Blue = 0,
					Red = 0
				},
				new ColorProgramStep
				{
					Delay = 1000,
					Green = 0,
					Blue = 0,
					Red = 0
				}
			});

			DefaultColorProgramSteps.Add("Default", new[]
			{
				new ColorProgramStep
			{
				Blue = 23,
				Red = 123,
				Green = 234,
				Delay = 1000
			},
				new ColorProgramStep
				{
					Blue = 123,
					Red = 223,
					Green = 134,
					Delay = 500
				}
			});
		}
	}
}