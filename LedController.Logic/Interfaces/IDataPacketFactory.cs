namespace LedController.Logic.Interfaces
{
	public interface IDataPacketFactory
	{
		IDeserializableEntity GetEntityFromBuffer(byte[] buffer);
	}
}
