/*SamplePlugInVC.h*/
#pragma once
using namespace System;
using namespace AmiBroker;
using namespace AmiBroker::PlugIn;
using namespace AmiBroker::Utils;

namespace AmiBroker
{
	namespace Samples
	{
		public ref class BasicSamples : IndicatorBase
		{
		public:
			void BasicSampleVC1();
			ATArray^ BasicSampleVC2();
			ATArray^ BasicSampleVC3(ATArray^ array, float period);
	        float BasicSampleVC4();
			ATArray^ BasicSampleVC5(float period);
			float BasicSampleVC6(float emaPeriod);
			void BasicSamples::BasicSampleVC7();
			void BasicSampleVC8();
			void BasicSampleVC9();
			float BasicSampleVC10(float a, float b);
			float BasicSampleVC11(float a, float b);
		};
	}
}
