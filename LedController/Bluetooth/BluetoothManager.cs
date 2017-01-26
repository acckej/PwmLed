using System;
using System.Collections.Generic;
using System.Linq;
using Android.Bluetooth;
using Java.Util;

namespace LedController.Bluetooth
{
	public class BluetoothManager : IDisposable
	{
		public const string DeviceName = "HC-06";
		private static BluetoothManager _current;

		private readonly BluetoothDevice _device;
		private readonly BluetoothSocket _socket;
		private bool _disposed;
		

		private BluetoothManager()
		{
			var adapter = BluetoothAdapter.DefaultAdapter;
			if (adapter == null)
			{
				throw new Exception("No Bluetooth adapter found.");
			}

			if (!adapter.IsEnabled)
			{
				throw new Exception("Bluetooth adapter is not enabled.");
			}

			_device = (from bd in adapter.BondedDevices
					   where bd.Name == DeviceName
					   select bd).FirstOrDefault();

			if (_device == null)
			{
				throw new Exception("Named device not found.");
			}

			_socket = _device.CreateRfcommSocketToServiceRecord(UUID.FromString("00001101-0000-1000-8000-00805f9b34fb"));
			_socket.Connect();
		}

		public byte[] SendCommandAndGetResponse(byte[] command)
		{
			SendData(command);
			return GetResponse();
		}

		public void SendData(byte[] command)
		{
			if (_socket == null || !_socket.IsConnected)
			{
				throw new InvalidOperationException("Socket is not connected");
			}

			_socket.OutputStream.Write(command, 0, command.Length);
		}

		public byte[] GetResponse()
		{
			if (_socket == null || !_socket.IsConnected)
			{
				throw new InvalidOperationException("Socket is not connected");
			}

			var result = new List<byte>();
			var buffer = new byte[512];
			int ln;

			do
			{
				ln = _socket.InputStream.Read(buffer, 0, buffer.Length);
				result.AddRange(buffer.Take(ln));
			} while (ln == buffer.Length);

			return result.ToArray();
		}

		public static BluetoothManager Current => _current == null || _current._disposed ? (_current = new BluetoothManager()) : _current;

		public void Dispose()
		{
			_disposed = true;
			_socket.Close();
			_socket.Dispose();
			_device.Dispose();
		}
	}
}