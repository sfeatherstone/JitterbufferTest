using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Threading;
using CSNamedPipe;

namespace Clock
{

  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
        var clock = new Clock3CS.ClockFace();

        NamedPipeServer PServer1 = new NamedPipeServer(@"\\.\pipe\myNamedPipe1", 0);
        PServer1.Start(new NamedPipeServer.MessageRecieved(clock.Animate));


      Application.Run(clock);

      PServer1.StopServer();


    }
  }
}
