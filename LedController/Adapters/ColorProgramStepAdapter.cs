using System.Collections.Generic;
using Android.Graphics;
using Android.Views;
using Android.Widget;
using LedController.Logic.Entities;

namespace LedController.Adapters
{
	public class ColorProgramStepAdapter : BaseAdapter<ColorProgramStep>
	{
		public delegate void StepButtonClicked(int position);

		public delegate void ColorBoxClicked(int position);

		public event StepButtonClicked OnStepButtonClicked;
		public event ColorBoxClicked OnColorBoxClicked;
		public ColorProgramStepAdapter(IList<ColorProgramStep> steps)
		{
			_steps = new List<ColorProgramStep>();
			_steps.AddRange(steps);
		}

		public override long GetItemId(int position)
		{
			return position;
		}

		public void SetColor(Color color, int position)
		{
			var item = _steps[position];
			item.Blue = color.B;
			item.Red = color.R;
			item.Green = color.G;

			NotifyDataSetChanged();
		}

		public void Remove(int position)
		{
			_steps.RemoveAt(position);
			NotifyDataSetChanged();
		}

		public void SetProgram(ColorProgram program)
		{
			_steps.Clear();
			_steps.AddRange(program.Steps);
			NotifyDataSetChanged();
		}

		public void Add(ColorProgramStep step)
		{
			_steps.Add(step);
			NotifyDataSetChanged();
		}

		public override View GetView(int position, View convertView, ViewGroup parent)
		{
			var view = LayoutInflater.From(parent.Context).Inflate(Resource.Layout.ColorProgramRow, parent, false);

			var delay = view.FindViewById<EditText>(Resource.Id.txtDelay);
			var text = view.FindViewById<TextView>(Resource.Id.lblDelay);
			var deleteButton = view.FindViewById<Button>(Resource.Id.btnDelete);

			deleteButton.Click += (sender, args) =>
			{
				OnStepButtonClicked?.Invoke(position);
			};

			var step = _steps[position];

			delay.Text = step.Delay.ToString();
			text.SetBackgroundColor(new Color(step.Red, step.Green, step.Blue));

			text.Click += (sender, args) =>
			{
				OnColorBoxClicked?.Invoke(position);
			};

			return view;
		}

		public ColorProgramStep[] Steps => _steps.ToArray();

		public override int Count => _steps.Count;

		public override ColorProgramStep this[int position] => _steps[position];

		private readonly List<ColorProgramStep> _steps;
	}
}