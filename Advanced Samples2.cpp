// This is the main DLL file.
#include "stdafx.h"
#include "Advanced Samples2.h"

namespace AmiBroker
{
	namespace Samples
	{
		/// <summary>
		/// AdvancedSampleVC2:
		/// - how to use ABHost
		/// - how to make a STATIC function available to AFL scipts
		/// 
		/// Using ABHost
		/// ------------
		/// ABHost type is the managed equivalent of AmiBroker's site interface. See ADK for details of its methods.
		/// Developers should only use CallFunction and GetStockArray functions as needed. All other functions should be avoided 
		/// as code gets more prone to errors. They are not required if ATVar, ATArray, ABVars, ATAfl and AFxxx classes are used properly.
		/// 
		/// Creating a static AFL plug-in method
		/// ------------------------------------
		/// Creating a static .Net plug-in function has 5 requirements:
		/// - The class of the method must be public (see the above class declaration: public static class AdvancedSamples2)
		/// - The method must be tagged with the [ABMethod] attribute.
		/// - The method must be a public static member of the class.
		/// - The method must return a void, ATArray, string or float value (other types are not accepted for return values).
		/// - The method may have parameters of ATArray, string or float values in this order of types (other order of parameter types and other parameter types are not accepted).
		/// See the AdvancedSample2() method declaration. The first parameter is of type ATArray, the second parameter is of type float!
		/// </summary>
		[ABMethod]
		void AdvancedSamples2::AdvancedSampleVC2(float period)
		{
			// getting price arrays (static function!)
			// This class is not inherited from IndicatorBase!
			ATArray^ close = ABHost::GetStockArray(StockField::Close);
			ATArray^ high = ABHost::GetStockArray(StockField::High);
			ATArray^ low = ABHost::GetStockArray(StockField::Low);

			// calculate typical price from bar price data
			ATArray^ myTypicalPrice = (high + low + 2 * close) / 4;

			ATArray^ mySlowMa;

			// calculate slow average of typical price
			//mySlowMa = AFAvg::Ma(myTypicalPrice, period);              // the easy way using AFAvg
			ATVar result = ABHost::CallFunction("MA"                     // the hard way using ABHost
				, ATVar(myTypicalPrice)
				, ATVar(period));
			mySlowMa = result.GetArray();

			// set MySlowMa AFL variable
			// ATAfl::SaveTo("MySlowMa", mySlowMa);                      // the easy way using ATAfl
			ABHost::SetVariable("MySlowMa", ATVar(mySlowMa));        // the hard way using ABHost
		}

		/// <summary>
		/// AdvancedSampleVC3:
		/// - how to declare static plug-in functions with parameters
		/// - how to use [ABMethod] and [ABParameter] method attributes
		/// NOTE: Only this the advanced mode provides the possibility of using default values (like RSI, stochk, etc.) 
		/// and array or float input parameters (like color parameter of Plot)
		/// 
		/// Advanced mode of declaring parameters
		/// -------------------------------------
		/// Advanced mode requires the following method signature:
		///     public static ATVar <your function name>(ATArgList args)
		/// The method signature is strictly checked at plug-in load.
		/// If method signature is different, your function is not be made availale for AFL scripts.
		/// 
		/// Each ABParameter attribute declares a parameter that AFL will pass to this function.
		/// The first parameter of the ABParameter attribute is the ordinal number. Ordinals tell the order
		/// of passed tarameters. They must start at 0 and must be numbered continuously.
		/// The Type parameters tell what kind of data we expect from AFL. The Type of ABParameters has also 
		/// some AmiBroker restiriction. The order of parameters must adhere to the following order of parameter types.
		///     ABParameterType::Array
		///     ABParameterType::String
		///     ABParameterType::Float or ABParameterType::FloatOrArray
		///     ABParameterType::Default
		/// Exceptions are NOT caught in advanced mode. Each plug-in method must implement its own exception handler.
		/// See next sample for more datails.
		/// 
		/// NOTE: If you use ABParameterType::FloatOrArray type for any of the parameters, 
		/// all ABParameterType::Float parameters may receive ATArray^ parameters as well! 
		/// In such case you should check parameter types in you code!
		/// </summary>
		[ABMethod]
		[ABParameter(0, Type = ABParameterType::Array, Description = "Array to calc Ema")]
		[ABParameter(1, Type = ABParameterType::Float, Description = "Ema period")]
		[ABParameter(2, Type = ABParameterType::Float, Description = "Ma period")]
		ATVar AdvancedSamples2::AdvancedSampleVC3(ATArgList args)
		{
			try
			{
				// reading parameters
				ATArray^ myEmaArray = args[0].GetArray();
				float myEmaPeriod = args[1].GetFloat();
				float myMaPeriod = args[2].GetFloat();

				// getting price arrays
				ATArray^ close = ABHost::GetStockArray(StockField::Close);
				ATArray^ high = ABHost::GetStockArray(StockField::High);
				ATArray^ low = ABHost::GetStockArray(StockField::Low);

				// calculate typical price from bar price data
				ATArray^ myTypicalPrice = (high + low + 2 * close) / 4;

				// calculate slow average of typical price
				ATArray^ mySlowMa = AFAvg::Ma(myTypicalPrice, myMaPeriod);

				// plotting average typical price
				AFGraph::Plot(mySlowMa, "SlowMa", Color::Red, Style::Thick);

				// calculate fast average of close price
				ATArray^ myFastEma = AFAvg::Ema(myEmaArray, myEmaPeriod);

				// plotting fast average close price with alternating color
				AFGraph::Plot(myFastEma, "FastEma",
					AFTools::Iif(mySlowMa < myFastEma, Color::DarkGreen, Color::Brown),
					Style::Line);

				// generate signals and set trade prices
				ATArray^ buy = AFTools::Cross(mySlowMa, myFastEma);
				ABVars::Buy.Set(buy);
				ABVars::BuyPrice.Set(gcnew ATArray(close));

				ATArray^ shrt = AFTools::Cross(myFastEma, mySlowMa);
				ABVars::Short.Set(shrt);
				//ABVars::ShortPrice.Set(gcnew ATArray(close)); // same as the next line
				ATAfl::SaveTo("ShortPrice", gcnew ATArray(close));

				AFGraph::PlotShapes(buy * (float)Shape::UpArrow, Color::Green, 0, close);
				AFGraph::PlotShapes(shrt * (float)Shape::DownArrow, Color::Red, 0, close);

				// indicate success
				return ATVar::Ok;
			}
			catch (Exception^ e)
			{
				YException::Show("Error while executing AdvancedSampleVC3 indicator.", e);

				// indicate failure
				return ATVar::Fail;
			}
		}

		/// <summary>
		/// AdvancedSampleVC4:
		/// - how to use parameters with default/optional values
		/// 
		/// In this sample AB's MA function is called. It has two parameters:
		///     Array to calc with.
		///     Period to calc the average.
		/// In our DefAvgVC function the period parameter has a default value. So it is not required in AFL.
		/// </summary>
		[ABMethod(Name = "DefAvgVC")]
		[ABParameter(0, Type = ABParameterType::Array, Description = "Array to calculate DefAvg")]
		[ABParameter(1, Type = ABParameterType::Default, Description = "Period of DefAvg", Default = 20)]
		ATVar AdvancedSamples2::AdvancedSampleVC4(ATArgList args)
		{
			// read parameters
			ATArray^ array = args[0].GetArray();

			// if period is not supplied in the AFL script, AB will pass the default value
			float period = args[1].GetFloat();

			// now we call AB's built-in MA function
			ATArray^ result = AFAvg::Ma(array, period);

			// return result of the MA call
			return ATVar(result);
		}

		/// <summary>
		/// AdvancedSampleVC5:
		/// - how to call any built-in or 3rd party plugin methods
		/// 
		/// Any built-in functions or 3rd party plug-in functions can be called from your plug-in.
		/// This sample calls AmiBroker's built in MA function by calling CallFunction method of site interface
		/// to calculate the moving average. The same way any 3rd party plug-in function can be called.
		///
		/// DO NOT CALL built-in AmiBroker methods in this way! Use AFxxx static classes instead.
		/// If you use AFxxx classes, your code can run on all future versions of AmiBroker!
		///
		/// </summary>
		[ABMethod(Name = "CallMaVC")]
		[ABParameter(0, Type = ABParameterType::Array, Description = "Array to calculate Ma")]
		[ABParameter(1, Type = ABParameterType::Default, Description = "Period of Ma", Default = 20)]
		ATVar AdvancedSamples2::AdvancedSampleVC5(ATArgList args)
		{
			// read parameters
			ATArray^ array = args[0].GetArray();

			// if period is not supplied in the AFL script, AB will pass the default value
			float period = args[1].GetFloat();

			ATVar result = ABHost::CallFunction("MA", ATVar(array), ATVar(period));

			return result;
		}

		[ABMethod(Name = "AdvancedLoopSampleVC")]
		[ABParameter(0, Type = ABParameterType::Float, Description = "Period")]
		ATVar AdvancedSamples2::AdvancedSampleVC6(ATArgList args)
		{
			int maPeriod = (int)args[0].GetFloat();
			ATArray^ close = ABHost::GetStockArray(StockField::Close);

			// allocate memory for result array
			ATArray^ myMa = gcnew ATArray();

			// set null values at the beginning of the array
			for (int i = 0; i < maPeriod - 1 && i < myMa->Length; i++)
				myMa[i] = ATFloat::Null;

			// calculate the average for each bar
			for (int i = maPeriod - 1; i < myMa->Length; i++)
			{
				// clear temporary result
				float tempSum = 0.0f;

				// inner loop to sum bar data
				for (int j = 0; j < maPeriod; j++)
					tempSum = tempSum + close[i - j];

				// save calculated value to result array element
				myMa[i] = tempSum / maPeriod;
			}

			// returning result to AFL  script
			return ATVar(myMa);
		}
	}
}
