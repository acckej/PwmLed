namespace LedController.Logic.Types
{
	public interface IArduinoType<T>
	{
		int Size { get; }

		T Value { get; set; }

		byte[] GetBytes();

		void FromBytes(byte[] buf, int index);
	}
}
