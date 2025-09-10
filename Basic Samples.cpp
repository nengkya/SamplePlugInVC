// This is the main DLL file.
#include "stdafx.h"
#include "Basic Samples.h"

namespace AmiBroker
{
    /// You should be familiar with AmiBroker's concept of array processing to understand these samples.
    /// You should also have some degree of understanding of object oriented programming and .NET programming.
    ///
    /// These samples use a class that is inherited from IndicatorBase class which 
    /// hides the complexities of AmiBroker's site interface (See ADK) and provides
    /// a programming environment that resembles to AFL a lot.
    /// 
    /// Discover the .NET types to create AmiBroker AFL plug-ins
    /// --------------------------------------------------------
    /// All .NET plug-ins use AmiBroker.PlugInHost.dll as a reference assembly. This contains
    /// all the classes and structures that are needed for plug-in developement.
    /// Use Visual Studio's Object Browser window to get acquanited to the types 
    /// in the AmiBroker namespace provided by AmiBroker.PlugInHost.dll. 
    /// AmiBroker.PlugInHost.dll is located in <Amibroker folder>\.NET for AmiBroker\Assemblies folder.
    ///
	namespace Samples
	{
        /// <summary>
        /// BasicSampleVC1:
        /// - how to code an AFL plug-in method in .NET
        /// - how to call an AFL plug-in method from an AFL script
        ///
        /// Creating a parameterless AFL plug-in method
        /// ---------------------------------------
        /// Creating a parameterless .NET plug-in function is quite easy. There are 4 simple requirements:
        /// - The class (BasicSamples) of the method must be public and must be inherited from IndicatorBase (see the above class declaration: public class BasicSamples : IndicatorBase)
        /// - The method must be tagged with the [ABMethod] attribute.
        /// - The method must be a public member of the class.
        /// - The method has no input parameters and no return value (void).
        /// See the BasicSampleVC1() method declaration.
        /// 
        /// Calling built-in AFL methods from .NET
        /// ---------------------------------------
        /// AFAvg, AFComposites, AFDate, AFForeign, AFGraph, AFHL, AFInd, AFInfo, AFMath,
        /// AFMisc, AFPattern, AFStat, AFTimeFrame, AFTools classes have static members 
        /// to execute built-in AmiBroker functions.
        /// 
        /// E.g.: AFTimeFrame.TimeFrameSet executes the built-in TimeFrameSet function of AmiBroker. 
        /// .NET functions are called the same as AB's functions. However, they usually have many forms (overloaded methods.)
        /// AFTimeFrame.TimeFrameSet has two forms. It can have a parameter of float:
        ///     AFTimeFrame.TimeFrameSet(5 * 60);
        /// OR a parameter of type TFInterval:
        ///     AFTimeFrame.TimeFrameSet(TFInterval.In5Minute);
        /// The two forms are equivalent in results. However the second form is more strict (type safe)
        /// in the values that can be supplied as parameter.
        /// </summary>
		[ABMethod]
		void BasicSamples::BasicSampleVC1()
		{
			// calculate the moving average of close price by calling the built-in MA function
			ATArray^ mySlowMa = AFAvg::Ma(Close, 20);

			// plotting the moving average
			AFGraph::Plot(mySlowMa, "mySlowMa", Color::Red, Style::Thick);
		}
	
        /// <summary>
        /// BasicSampleVC2:
        /// - how to return a single value from an AFl plug-in method to the calling AFL script
        /// - how to use ATArray objects in expressions
        /// 
        /// Returning value to the calling AFL script
        /// -----------------------------------------
        /// Creating a parameterless .NET plug-in function that returns a value has 5 simple requirements:
        /// - The class of the method must be public and inherited from IndicatorBase (see the above class declaration: public class BasicSamples : IndicatorBase)
        /// - The method must be tagged with the [ABMethod] attribute.
        /// - The method must be a public member of the class.
        /// - The method has no input parameters.
        /// - The method must return an ATArray, string or float value (other types are not accepted for return values).
        /// See the BasicSampleVC2() method declaration.
        /// 
        /// Using time series data
        /// ---------------------------------------
        /// Objects of ATArray type are used heavily in all plug-in methods.
        /// ATArray objects hold time series (array values in AFL scritps) data.
        /// ATArray objects can be part of expressions just like array values in AFL. 
        /// All arithmetric, boolean and comparision operators are available.
        /// 
        /// High, Low, Buy, Sell, etc., (predefined AFL variables) are of ATArray type and are public members
        /// of IndicatorBase class. They can be used just like in any AFL expressions.
        /// 
        /// See this line: 
        ///     ATArray myTypicalPrice = (this.High + this.Low + 2 * this.Close) / 4;
        /// It calculates the typical price of the 3 time series (High, Low, Close)
        /// and assigns the resulting time series to a new ATArray object (myTypicalPrice).
        /// ATArray operators regard null values. Here are all the available operators:
        ///  +, -, *, /, %, <, <=, >, >=, ==, !=, &, |, ^, ++, -- 
        /// 
        /// ATArray objects can appear in experssions mixed with float (scalar) values (e.g.: 2 * Close).
        /// In this case the result is always an ATArray object!
        /// 
        /// The item in ATArray at the selected index can be printed to strings easily using the ToString method;
        /// Title = "myTypicalPrice=" + myTypicalPrice;
        /// </summary>
		[ABMethod]
		ATArray^ BasicSamples::BasicSampleVC2()
		{
			// calculate bar avg price
			ATArray^ myTypicalPrice = (High + Low + 2 * Close) / 4;

			// calculate the moving average of typical price by calling the built-in MA function
			ATArray^ mySlowMa = AFAvg::Ma(myTypicalPrice, 20);

			// returning result to AFL  script
			return mySlowMa;
		}

	
		/// <summary>
        /// BasicSampleVC3:
        /// - how get input parameters from AFL scripts
        /// 
        /// Accepting input parameters from the calling AFL script
        /// ------------------------------------------------------
        /// Creating a .NET plug-in function has 5 requirements:
        /// - The class of the method must be public and inherited from IndicatorBase (see the above class declaration: public class BasicSamples : IndicatorBase)
        /// - The method must be tagged with the [ABMethod] attribute.
        /// - The method must be a public member of the class.
        /// - The method must return a void, ATArray, string or float value (other types are not accepted for return values).
        /// - The method may have parameters of ATArray, string or float values in this order of types (other order of parameter types and other parameter types are not accepted).
        /// See the BasicSampleVC3() method declaration. The first parameter is of type ATArray, the second parameter is of type float!
        /// </summary>
		[ABMethod]
		ATArray^ BasicSamples::BasicSampleVC3(ATArray^ array, float period)
		{
			ATArray^ myEma = AFAvg::Ema(array, period);
			return myEma;
		}
	
        /// <summary>
        /// BasicSampleVC4:
        /// - how to read and write global AFL script variables
        /// - how to use ATVAr struct
        /// - how to return multiple values to AFL scripts by setting AFL script variables
        /// 
        /// Reading and writing AFL variables
        /// ----------------------------------
        /// ATAfl type has static methods (ReadFrom, SaveTo) to access AFL variables. ReadFrom reads an AFl variable and returns an ATVar struct.
        /// ATVar struct represents an AmiBroker variant which may have an array, string, float or an object value (or may have no value at all). 
        /// To get the value from the variant one of the GetArray, GetString, GetFloat or GetObject methods of the ATVar object must be called.
        ///
        /// Passing many values between AFL and .NET plug-ins
        /// -------------------------------------------------
        /// If more values are needed to be returned to the calling AFL script, 
        /// use ATAfl objects (AFL global variables) to pass all results to AFL and
        /// use the function return value to indicate the failure or the success.
        /// </summary>
        [ABMethod]
		float BasicSamples::BasicSampleVC4()
        {
            // reading the variant of the Period AFL variable intialized by the AFL script
			ATVar^ emaPeriodVar = ATAfl::ReadFrom("EmaPeriod");

            // reading the float value from the variant
            // AFL variable MUST be initialized with data of correct type
            // uninitialized variable or mismaching data types will cause exception
            // if default value is provided for the GetFloat method, it returns the default if the variable was not initialized.
			float emaPeriod = emaPeriodVar->GetFloat();

            // same as above on a single line
			float maPeriod = ATAfl::ReadFrom("MaPeriod").GetFloat();

            // if ma period is faster then ema period then this indicator produces invalid result
            if (maPeriod < emaPeriod)
                // indicate invalid result
				return ATFloat::False;

            // calculating the average close price
			ATArray^ myEma = AFAvg::Ema(Close, emaPeriod);

            // saving result to MyEma AFL variable
			ATAfl::SaveTo("MyEma", myEma);

            // calculating the average close price
			ATArray^ myMa = AFAvg::Ma(Close, maPeriod);

            // saving result to MyMa AFL variable
			ATAfl::SaveTo("MyMa", myMa);

            // calculate the % difference of the two averages
            ATArray^ myDiff = (myMa - myEma) * 100.0f;

            // setting AFL variable value
			ATAfl::SaveTo("MyDiff", myDiff);

            // indicate success to the calling AFL script
			return ATFloat::True;
        }
	
        /// <summary>
        /// BasicSampleVC5:
        /// - how to do fast loops
        /// - how to access elements of an array
        /// 
        /// Performance advantage of .NET
        /// -----------------------------
        /// Because of compiled code and integer index arithmetric 
        /// .NET loops are much faster than AFL script loops (2-40 times faster)
        /// 
        /// This indicator has 2 more optimized versions in Unsafe Samples. Those are for 
        /// the experienced C/C# programmers.
        /// 
        /// Compare the speed of calculating MA in 4 different ways. 
        /// The following samples all calculate MA:
        /// Basic - "Sample5 Loop-AFL.afl"	                 ~2.5s
        /// Basic - "Sample5 Loop.afl"						 ~0.21s
        /// Unsafe - "Sample1 Loop - Unsafe.afl"              ~0.081s
        /// Unsafe - "Sample2 Loop - Unsafe Optimized.afl"    ~0.075s
        /// (Vista 64 bit / AmiBroker 5.20 32 bit / Intel E8400 / 4GB / 497780 bars)
        /// (If you test them, these sample indicators use all bars from your current database!)
        /// 
        /// Setting AFL method name
        /// -----------------------
        /// When multiple plug-ins are used then different plug-ins (dlls) may have plug-in methods with the same name.
        /// In such case neither method will be available from AFL scripts.
        /// In such case either one of the method names must be changed or the ABMethod attribute has to be parameterized
        /// to change the AFL method name.
        /// </summary>
        [ABMethod(Name = "LoopSampleVC")]
		ATArray^ BasicSamples::BasicSampleVC5(float period)
        {
            int maPeriod = (int)period;

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
                    tempSum = tempSum + Close[i - j];

                // save calculated value to result array element
                myMa[i] = tempSum / maPeriod;
            }

            // returning result to AFL  script
            return myMa;
        }

		/// <summary>
		/// BasicSampleVC6:
		/// - how to handle .NET exceptions
		/// 
		/// Default exception handling
		/// --------------------------
		/// Developers do NOT have to use try-catch blocks to catch .NET exceptions and prevent AmiBroker from crashing.
		/// All exceptions are catched and reported to the instance log file automatically (and to Trace window of AmiBroker). 
		/// 
		/// Recommended exception handling
		/// ------------------------------
		/// Besides the default exception handling it is still a good practice to surround your code with 
		/// try-catch block and handle all exceptions gracefully. 
		/// E.g.: in case of an exception this indicator returns False and the calling AFL script knows that there was an issue.
		/// NOTE: If you return a single, non boolean value to AFL script, it is recommended to return ATVar.None in error condition. This cause an error in AFL script as well when returned value gets used.
		/// </summary>
		[ABMethod]
		float BasicSamples::BasicSampleVC6(float emaPeriod)
		{
			try
			{
				ATArray^ myEma = AFAvg::Ema(Close, emaPeriod);

				throw gcnew ApplicationException("Exception thrown by user code to demo exception handling.");

				// the following code branch will never execute because of thrown exception
				// this way "MyEma" AFL variable will not exist when control returns to
				// the caller AFL script

				// disable warning of unreachable code
#pragma warning(push)
#pragma warning(disable : 4101)

				// save result
				ATAfl::SaveTo("MyEma", myEma);

				// indicate success to the calling AFL script
				return ATFloat::True;
#pragma warning(pop)

			}
			
			catch (Exception^ e)
			{
				// present error message on indicator panel and the Log-Trace window
				YException::Show("Error while executing BasicSampleVC6 indicator.", e);

				// indicate failure to the calling AFL script
				return ATFloat::False;
			}
		}

		/// <summary>
		/// BasicSampleVC7:
		/// - how to use AFL script objects
		/// 
		/// It is possible to use all AFL objects (Backtester, Signal, Stats, Trade) in .NET.
		/// The following function calculates the expectency % of the system by using the
		/// Backtester and the Trade objects.
		/// </summary>
		[ABMethod]
		void BasicSamples::BasicSampleVC7()
		{
			if (AFMisc::StatusAction() == Action::Portfolio)
			{
				Backtester^ bo = AFTools::GetBacktesterObject();
				bo->Backtest();

				double totalProfitPercent = 0;
				int tradeNum = 0;

				// iterate through all closed trades
				for (Trade^ trade = bo->GetFirstTrade(); trade != nullptr; trade = bo->GetNextTrade())
				{
					// sum up profit % 
					totalProfitPercent = totalProfitPercent + trade->GetPercentProfit();
					tradeNum++;
				}

				// iterate through open positions
				for (Trade^ trade = bo->GetFirstOpenPos(); trade != nullptr; trade = bo->GetNextOpenPos())
				{
					// sum up profit % 
					totalProfitPercent = totalProfitPercent + trade->GetPercentProfit();
					tradeNum++;
				}

				// calc expectency %
				float expectancy = (float)(totalProfitPercent / tradeNum);

				// Adding custom metric to the report
				bo->AddCustomMetric("Expectancy %", expectancy, 0, 0, 4);
			}
		}

		/// <summary>
		/// BasicSampleVC8:
		/// - how to use advanced backtester interface
		/// 
		/// This function modifies the exit prices by the expected slippage (Slippage of Long and Short are usually different in size.)
		/// The function uses the Backtester and the Signal object to modifiy the ExitPrices.
		/// </summary>
		[ABMethod]
		void BasicSamples::BasicSampleVC8()
		{
			if (AFMisc::StatusAction() == Action::Portfolio)
			{
				float slippageLong = ATAfl::ReadFrom("SlippageLong").GetFloat();
				float slippageShort = ATAfl::ReadFrom("SlippageShort").GetFloat();
				float tickSize = ATAfl::ReadFrom("SlippageTickSize").GetFloat();

				if (tickSize == ATFloat::Null || tickSize == 0.0f)
					throw gcnew Exception("TickSize must be set for the ticker!");

				Backtester^ bo = AFTools::GetBacktesterObject();

				bo->PreProcess();

				for (int bar = 0; bar < BasicSamples::BarCount; bar++)
				{
					for (Signal^ sig = bo->GetFirstSignal(bar); sig != nullptr; sig = bo->GetNextSignal(bar))
					{
						if (!sig->IsEntry())     // if exit signal
						{
							if (sig->IsLong())
								sig->Price = sig->Price - slippageLong * tickSize;
							else
								sig->Price = sig->Price + slippageShort * tickSize;
						}
					}
					bo->ProcessTradeSignals(bar);
				}

				bo->PostProcess();
			}
		}

		/// <summary>
		/// BasicSampleVC9:
		/// - how to use AmiBroker's predefined variables
		/// 
		/// This function add expectency custom metrics to the report by using the Stats object.
		/// </summary>
		[ABMethod]
		void BasicSamples::BasicSampleVC9()
		{
			float MaPeriod = 20;
			ATArray^ myTypicalPrice = (High + Low + 2 * Close) / 4;

			ATArray^ myMa = AFAvg::Ma(myTypicalPrice, MaPeriod);
			AFGraph::Plot(myMa, "MyMa", Color::Blue, Style::Thick);

			ATArray^ myFastEma = AFAvg::Ema(Close, 5);
			AFGraph::Plot(myFastEma, "Ema5", AFTools::Iif(myMa < myFastEma, Color::DarkGreen, Color::Brown), Style::Line);

			AFGraph::PlotOHLC(Open, High, Low, Close, "Close", Color::Red, Style::Candle);

			Buy = AFTools::Cross(myFastEma, myMa);
			Cover = gcnew ATArray(Buy);               //makes a copy of the original array as it may change at backtest
			Short = AFTools::Cross(myMa, myFastEma);
			Sell = gcnew ATArray(Short);

			BuyPrice = gcnew ATArray(Open);
			ShortPrice = gcnew ATArray(Open);
			SellPrice = gcnew ATArray(Close);
			CoverPrice = gcnew ATArray(Close);

			// Be carefull! The following line has correct syntax. It would probably even work in this sample in most cases.
			// However, it does not copy the "Open" array to the BuyPrice and ShortPrice arrays. Instead it sets 
			// the AFL variables to the same array (it changes the memory pointers!). So if you change array values in one of them,
			// all of them will reflect the changes!
			//           BuyPrice = ShortPrice = Open;
			// Now BuyPrice and ShortPrice point to Open array

			// As backtester might change the Buy, Sell, etc. values, each variable must have a different copy of the same 
			// trade/price data arrays.
			// If you initialize an ATArray object by the constructor (gcnew ATArray...) it allocates memory for the array
			// and initializes it with the parameter of th contructor)
			//           BuyPrice = gcnew ATArray(Open);
			// Now BuyPrice has a copy of the Open array

			AFGraph::PlotShapes(AFTools::Iif(Buy, Shape::UpArrow, Shape::None), Color::Green, 0, Open);
			AFGraph::PlotShapes(AFTools::Iif(Sell, Shape::DownArrow, Shape::None), Color::Red, 0, Close);

			AFGraph::PlotShapes(AFTools::Iif(Short, Shape::HollowDownArrow, Shape::None), Color::Red, 0, Open);
			AFGraph::PlotShapes(AFTools::Iif(Cover, Shape::HollowUpArrow, Shape::None), Color::Green, 0, Close);

			AFTools::SetOption(AFTools::UseCustomBacktestProc, ATFloat::True);

			if (AFMisc::StatusAction() == Action::Portfolio)
			{
				Backtester^ bo = AFTools::GetBacktesterObject();
				bo->Backtest();

				Stats^ st = bo->GetPerformanceStats(StatType::All);

				// calc expectency
				float expectancy = (st->GetValue(Stats::WinnersPercent) * st->GetValue("WinnersAvgProfit") +
					st->GetValue(Stats::LosersAvgLoss) * st->GetValue(Stats::LosersPercent)) / 100;

				bo->AddCustomMetric("Expectancy", expectancy, 0, 0, 2);
			}
		}

		/// <summary>
		/// BasicSampleVC10:
		/// - how to do "AFL-like scalar operations" that respects NULL values using ATFloat struct
		/// NOTE: C#'s float type does NOT treat AmiBroker null values in AFL's fasion! This is why you need ATFloat in some case!
		/// 
		/// This function shows how to use variables and expressions of ATFloat type.
		/// ATFloat type provides constants, member methods, casting and operators. These help to copy the behaviour of AmiBroker logical operators and boolean evaluation of float values
		/// 
		/// ATFloat provides contant values (True, False, Null, Ok, Fail). E.g.:
		/// return ATFloat.True;
		/// 
		/// float can be casted implicitly to/from ATFloat. E.g.:
		/// float a = 1.0f;
		/// ATFloat ata = a;                // no need for explicit type casting
		/// 
		/// ATFloats can be casted explicitly to/from most numeric and boolean built-in types. E.g.:
		/// ATFloat a = ATFloat.True;
		/// bool result = (bool)a;          // explicit casting. It throws exception if a has a Null value!
		/// bool? nResult = (bool?)a;       // explicit casting. It does NOT throw exception even if a has a Null value!
		/// 
		/// ATFloat provides logical, arithmetric, relation, etc, operators that pespects AmiBroker Nulls. E.g.:
		/// ATFloat a = ATFloat.True;
		/// ATFloat b = ATFloat.False;
		/// ATFloat result = a & b;         // False (ATFloat.False = 0)
		/// 
		/// An ATFloat expressions can be used in some places of boolean expression. E.g.:
		/// ATFloat ata = a;
		/// ATFloat atb = b;
		/// if (ata & atb)
		/// {...}
		/// </summary>
		/// <returns></returns>
		[ABMethod]
		float BasicSamples::BasicSampleVC10(float a, float b)
		{
			return (ATFloat)a & b;   // logical AND operator that respects Nulls
		}

		/// <summary>
		/// BasicSampleVC11:
		/// - how to use AmiBroker's float values in boolean expressions
		/// 
		/// ATFloat.IsTrue(a) returns boolean true if a != 0 and a is not Null and a is not NaN
		/// ATFloat.IsFalse(a) returns boolean true if a == 0
		/// ATFloat.IsNull(a) returns boolean true if a is Null
		/// ATFloat.IsNaN(a) returns boolean true if a is NaN
		/// ATFloat.ABDateNumToDateTime(a) returns a DateTime value representing a float value (YYYYMMDD - 19000000). 
		/// ATFloat.DateTimeToABDateNum(d) converts a DateTime to a float value.
		/// Simliarly use ATFloat.ABDateTimeToDateTime(a) and ATFloat.DateTimeToABDateTime(d)
		/// Simliarly use ATFloat.ABTimeNumToTimeSpan(a), ATFloat.TimeSpanToABTimeNum(t)
		/// Simliarly use ATFloat.DateTimeToABTimeNum(d)
		/// 
		/// </summary>
		/// <returns></returns>
		[ABMethod]
		float BasicSamples::BasicSampleVC11(float a, float b)
		{
			// You can use ATFloat's static members to test any float's boolean value
			if (ATFloat::IsTrue(a) & ATFloat::IsTrue(b))
				return ATFloat::True;

			return ATFloat::False;
		}
	}
}
