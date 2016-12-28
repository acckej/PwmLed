namespace LedController.Logic.Interfaces
{
	public interface ISerializableEntity
	{
		byte[] Serialize();

		int Size { get; }
	}
}
