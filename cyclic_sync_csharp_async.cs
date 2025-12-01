// Remi Andruccioli
// December 2025
//
// Here is an implementation that relies on async/await and Channels.
//
// This source code is under public domain.

using System;
using System.Threading.Channels;
using System.Threading.Tasks;

public class Program
{
  public const int MaxLoop = 5;

  public static async Task Main(string[] args)
  {
    var channelA = Channel.CreateBounded<int>(1);
    var channelB = Channel.CreateBounded<int>(1);

    await channelB.Writer.WriteAsync(0);

    var taskA = A(channelA.Writer, channelB.Reader);
    var taskB = B(channelA.Reader, channelB.Writer);

    await taskA;
    await taskB;
  }

  public static async Task A(ChannelWriter<int> channelA, ChannelReader<int> channelB)
  {
    for (var i = 0; i < MaxLoop; ++i)
    {
      await channelB.ReadAsync();
      Console.WriteLine("A");
      await channelA.WriteAsync(0);
    }
  }

  public static async Task B(ChannelReader<int> channelA, ChannelWriter<int> channelB)
  {
    for (var i = 0; i < MaxLoop; ++i)
    {
      await channelA.ReadAsync();
      Console.WriteLine("B");
      await channelB.WriteAsync(0);
    }
  }
}

