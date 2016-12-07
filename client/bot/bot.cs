using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

class Program
{
	static Random rng = new Random();
	static string ip;
	static short port;

	static void Main( string[] args )
	{
		if (args.Length < 3)
			return;

		ip = args[0];
		port = short.Parse( args[1] );
		int botN = int.Parse(args[2]);

		Thread[] thrd = new Thread[botN];

		for (int i = 0; i < botN; i++)
		{
			thrd[i] = new Thread( BotThread );
			thrd[i].Start( );

			Thread.Sleep( 50 );

			Console.WriteLine( "{0}/{1} connected...", i + 1, botN );
		}

		for (int i = 0; i < thrd.Length; i++)
			thrd[i].Join( );
	}

	static void BotThread( )
	{
		byte[] recvBuffer = new byte[1 << 12];

		try
		{
			TcpClient client = new TcpClient();
			client.Connect( new IPEndPoint( IPAddress.Parse( ip ), port ) );

			// Move around randomly
			while (true)
			{
				Thread.Sleep( rng.Next( 200, 1200 ) );

				byte[] buf = new byte[1024];
				BinaryWriter writer = new BinaryWriter(new MemoryStream(buf));
				writer.Write( (short)(2 + 1) );

				writer.Write( (short)5 );
				writer.Write( (sbyte)rng.Next() % 4 );

				client.Client.Send( buf, (int)2 + 2 + 1, SocketFlags.None );
				client.Client.Receive( recvBuffer );
			}
		}
		catch (Exception e)
		{
			return;
		}
	}
}
