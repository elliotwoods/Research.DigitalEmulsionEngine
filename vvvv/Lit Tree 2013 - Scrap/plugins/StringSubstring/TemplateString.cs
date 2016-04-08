#region usings
using System;
using System.ComponentModel.Composition;

using VVVV.PluginInterfaces.V1;
using VVVV.PluginInterfaces.V2;
using VVVV.Utils.VColor;
using VVVV.Utils.VMath;

using VVVV.Core.Logging;
#endregion usings

namespace VVVV.Nodes
{
	#region PluginInfo
	[PluginInfo(Name = "Substring", Category = "String", Help = "Basic template with one string in/out", Tags = "")]
	#endregion PluginInfo
	public class StringSubstringNode : IPluginEvaluate
	{
		#region fields & pins
		[Input("Input", DefaultString = "hello c#")]
		ISpread<string> FInput;

		[Input("Start")]
		ISpread<int> FInStart;
		
		[Input("Length")]
		ISpread<int> FInLength;
		
		[Output("Output")]
		ISpread<string> FOutput;

		[Import()]
		ILogger FLogger;
		#endregion fields & pins

		//called when data for any output pin is requested
		public void Evaluate(int SpreadMax)
		{
			FOutput.SliceCount = SpreadMax;

			for (int i = 0; i < SpreadMax; i++)
			{
				var start = FInStart[0];
				if (start < 0)
					start += FInput[i].Length;
				FOutput[i] = FInput[i].Substring(start, FInLength[0]);
			}

			//FLogger.Log(LogType.Debug, "Logging to Renderer (TTY)");
		}
	}
}
