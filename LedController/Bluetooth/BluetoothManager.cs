using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Android.Bluetooth;
using Android.Util;
using Java.Util;

namespace LedController.Bluetooth
{
	public class BluetoothManager : IDisposable
	{
		public const string DeviceName = "Kurka";
		public const int Timeout = 2000;
		public const int DataChunkWaitingTimeout = 4000;
		public const int WorkerThreadWaitingInterval = 100;
		public const int MinimumBytesNeeded = 2;

		private static BluetoothManager _current;

		private readonly BluetoothDevice _device;
		private readonly BluetoothSocket _socket;
		private readonly List<byte> _accumulator = new List<byte>();
		private Task _workerTask;
		private readonly ManualResetEvent _signal = new ManualResetEvent(false);
		private readonly CancellationTokenSource _cancel = new CancellationTokenSource();
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

		public byte[] SendCommandAndGetResponse(byte[] command, Func<byte[], int> getExpectedLength)
		{
			SendData(command);
			//Thread.Sleep(3000);
			return GetResponseWithSeparateThread(getExpectedLength); // GetResponse(getExpectedLength); 
		}

		public void SendData(byte[] command)
		{
			if (_socket == null || !_socket.IsConnected)
			{
				throw new InvalidOperationException("Socket is not connected");
			}

			lock (_accumulator)
			{
				_accumulator.Clear();
			}
			_signal.Reset();

			_socket.OutputStream.Write(command, 0, command.Length);
			_socket.OutputStream.Flush();
		}



		public byte[] GetResponse(Func<byte[], int> getExpectedLength)
		{
			if (_socket == null || !_socket.IsConnected)
			{
				throw new InvalidOperationException("Socket is not connected");
			}

			var result = new List<byte>();
			var buffer = new byte[512];
			var ln = 0;
			var iterationsCounter = 1;

			for (int i = 0; i < iterationsCounter; i++)
			{
				var wait = _socket.InputStream.BeginRead(buffer, 0, buffer.Length, delegate { }, null);
				wait.AsyncWaitHandle.WaitOne(Timeout);
				if (i == 0)
				{
					ln = getExpectedLength(buffer);

					if (ln == 0)
					{
						return new byte[0];
					}

					iterationsCounter = ln/buffer.Length;
				}

				result.AddRange(i == iterationsCounter ? buffer.Take(ln - i * buffer.Length) : buffer);
			}

			return result.ToArray();
		}

		public byte[] GetResponseWithSeparateThread(Func<byte[], int> getExpectedLength)
		{
			if (_socket == null || !_socket.IsConnected)
			{
				throw new InvalidOperationException("Socket is not connected");
			}

			if (_workerTask == null || _workerTask.Status != TaskStatus.Running)
			{
				_workerTask = new Task(() => WorkerThread(_cancel.Token, getExpectedLength));
				_workerTask.Start();
			}

			_signal.WaitOne(DataChunkWaitingTimeout);
			
			lock (_accumulator)
			{
				return _accumulator.ToArray();
			}
		}

		private void WorkerThread(CancellationToken cancel, Func<byte[], int> getExpectedLength)
		{
			try
			{
				var expectedLength = 0;
				do
				{
					var buffer = new byte[512];
					var ln = _socket.InputStream.Read(buffer, 0, buffer.Length);
					
					lock (_accumulator)
					{
						Log.Debug("WorkerThread", $"accumulator1 {_accumulator.Count}");
						if (_accumulator.Count == 0)
						{
							expectedLength = 0;
						}

						Log.Debug("WorkerThread", $"ln {ln}");
					
						_accumulator.AddRange(buffer.Take(ln));
						if (_accumulator.Count >= MinimumBytesNeeded && (expectedLength == 0 || expectedLength == -1))
						{
							expectedLength = getExpectedLength(_accumulator.ToArray());
						}
						Log.Debug("WorkerThread", $"expectedLength {expectedLength}");
						if (expectedLength > 0 && _accumulator.Count >= expectedLength)
						{
							_signal.Set();
						}
						Log.Debug("WorkerThread", $"accumulator2 {_accumulator.Count}");
					}
				}
				while (!cancel.WaitHandle.WaitOne(WorkerThreadWaitingInterval));
			}
			catch (Exception ex)
			{
				Log.Error("BluetoothManager", ex.ToString());
			}
		}

		public byte[] GetResponse()
		{
			if (_socket == null || !_socket.IsConnected)
			{
				throw new InvalidOperationException("Socket is not connected");
			}

			var buffer = new byte[512];
			var wait = _socket.InputStream.BeginRead(buffer, 0, buffer.Length, delegate { }, null);
			wait.AsyncWaitHandle.WaitOne(Timeout);

			return buffer;
		}

		public static BluetoothManager Current => _current == null || _current._disposed ? (_current = new BluetoothManager()) : _current;

		public void Dispose()
		{
			_disposed = true;
			_socket.Close();
			_socket.Dispose();
			_device.Dispose();

			if (_workerTask != null && _workerTask.Status == TaskStatus.Running)
			{
				_cancel.Cancel();

				for (int i = 0; i < 100; i++)
				{
					Thread.Sleep(100);
					if (_workerTask.Status != TaskStatus.Running)
					{
						break;
					}
				}
				_workerTask.Dispose();
			}
		}
	}
}