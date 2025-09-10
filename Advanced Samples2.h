// SamplePlugInVC.h

#pragma once

using namespace System;
using namespace AmiBroker;
using namespace AmiBroker::PlugIn;
using namespace AmiBroker::Utils;

namespace AmiBroker
{
	namespace Samples
	{
		/// <summary>
		/// This class is static and it is NOT INHERITED from IndicatorBase which hides AmiBroker internals mostly. 
		/// In static classes plug-in developers sometimes need to use AmiBroker's site interface functions.
		/// ABHost type is a managed static class around AmiBroker's site interface. It provides all the 
		/// functionality of the site interface.
		/// 
		/// Using ATVar, ATArray, ATAfl, ABVars and AFxxx types still helps the plug-in developers to create static plug-in methods simply and efficiently.
		/// </summary>
		public ref class AdvancedSamples2 abstract sealed
		{
		public:
			static void AdvancedSampleVC2(float period);
			static ATVar AdvancedSampleVC3(ATArgList args);
			static ATVar AdvancedSampleVC4(ATArgList args);
			static ATVar AdvancedSampleVC5(ATArgList args);
			static ATVar AdvancedSampleVC6(ATArgList args);
		};
	}
}

