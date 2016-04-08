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
	[PluginInfo(Name = "Graycode", Category = "Value", Help = "Basic template with one value in/out", Tags = "")]
	#endregion PluginInfo
	public class ValueGraycodeNode : IPluginEvaluate
	{
		#region fields & pins
		[Input("Binary", DefaultValue = 1.0)]
		IDiffSpread<int> FInput;

		[Output("Graycode")]
		ISpread<int> FOutput;

		[Output("Inverse")]
		ISpread<int> FInverse;
		
		#endregion fields & pins

		//called when data for any output pin is requested
		public void Evaluate(int SpreadMax)
		{
			if (FInput.IsChanged)
			{
				FOutput.SliceCount = SpreadMax;
				FInverse.SliceCount = SpreadMax;
				
				for (int i = 0; i < SpreadMax; i++)
				{
					var binary = FInput[i];
					var graycode = binary ^ (binary >> 1);
					
					FOutput[i] = graycode;
					FInverse[graycode] = i;
				}	
			}
		}
	}
}
