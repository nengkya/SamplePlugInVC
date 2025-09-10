/*this is the main DLL file*/
#include "stdafx.h"
#include "HaGa Sample.h"

namespace AmiBroker
{
    namespace Samples
    {
        [ABMethod]
        void HaGaSampleClass::HaGaSample()
        {
			/*calculate the moving average of close price by calling the built-in MA function*/
            ATArray^ mySlowMa = AFAvg::Ma(Close, 20);

            /*plotting the moving average*/
            AFGraph::Plot(mySlowMa, "mySlowMa", Color::Red, Style::Thick);
        }
    }
}