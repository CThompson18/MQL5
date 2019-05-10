//+------------------------------------------------------------------+
//|                                           SilverTrend_Signal.mq5 |
//|                                        Ramdass - Conversion only |
//+------------------------------------------------------------------+
#property copyright "        rewritten by CrazyChart"
#property link      "http://viac.ru/"
//---- indicator version
#property version   "1.00"
//---- drawing the indicator in the main window
#property indicator_chart_window 
//---- two buffers are used for calculation and drawing the indicator
#property indicator_buffers 2
//---- only two plots are used
#property indicator_plots   2
//+----------------------------------------------+
//|  Bearish indicator drawing parameters        |
//+----------------------------------------------+
//---- drawing the indicator 1 as a symbol
#property indicator_type1   DRAW_ARROW
//---- red color is used for the indicator bearish line
#property indicator_color1  Red
//---- indicator 1 line width is equal to 4
#property indicator_width1  4
//---- displaying the bearish label of the indicator line
#property indicator_label1  "Silver Sell"

//+----------------------------------------------+
//|  Bullish indicator drawing parameters        |
//+----------------------------------------------+
//---- drawing the indicator 2 as a line
#property indicator_type2   DRAW_ARROW
//---- lime color is used as the color of the bullish indicator line
#property indicator_color2  Lime
//---- indicator 2 line width is equal to 4
#property indicator_width2  4
//---- displaying the bullish label of the indicator line
#property indicator_label2 "Silver Buy"
//+----------------------------------------------+
//|  Indicator input parameters                  |
//+----------------------------------------------+
input double RISK = 9.25;
input double SSP = 41.95;
input double VRange = 2.0;
input int NumberofAlerts = 0;
//+----------------------------------------------+

//---- declaration of dynamic arrays that
//---- will be used as indicator buffers
double AskBuffer[];
double BidBuffer[];
//----
int K;
int counter=0;
bool old,uptrend_;
//---- declaration of the integer variables for the start of data calculation
int StartBars;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
void OnInit()
	{
		//---- initialization of variables of the start of data calculation
		StartBars = SSP + 1;
		//---- set AskBuffer[] dynamic array as an indicator buffer
		SetIndexBuffer(0,AskBuffer,INDICATOR_DATA);
		//---- shifting the start of drawing the indicator 1
		PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,StartBars);
		//--- create a label to display in DataWindow
		PlotIndexSetString(0,PLOT_LABEL,"Silver Sell");
		//---- indicator symbol
		PlotIndexSetInteger(0,PLOT_ARROW,234);
		//---- indexing the elements in the buffer as timeseries
		ArraySetAsSeries(AskBuffer,true);

		//---- set BidBuffer[] dynamic array as an indicator buffer
		SetIndexBuffer(1,BidBuffer,INDICATOR_DATA);
		//---- shifting the start of drawing the indicator 2
		PlotIndexSetInteger(1,PLOT_DRAW_BEGIN,StartBars);
		//--- create a label to display in DataWindow
		PlotIndexSetString(1,PLOT_LABEL,"Silver Buy");
		//---- indicator symbol
		PlotIndexSetInteger(1,PLOT_ARROW,233);
		//---- indexing the elements in the buffer as timeseries
		ArraySetAsSeries(BidBuffer,true);

		//---- setting the format of accuracy of displaying the indicator
		IndicatorSetInteger(INDICATOR_DIGITS,_Digits);
		//---- name for the data window and the label for tooltips 
		string short_name="SilverTrend_Signal";
		IndicatorSetString(INDICATOR_SHORTNAME,short_name);
		//----   
	}
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
	{
		//---- checking the number of bars to be enough for the calculation
		if(rates_total < StartBars) return(0);

		//---- declarations of local variables 
		int limit;
		double Range,AvgRange,smin,smax,SsMax,SsMin,price;
		bool uptrend;

		//---- calculations of the necessary amount of data to be copied
		//---- and the 'limit' starting index for the bars recalculation loop
		if(prev_calculated > rates_total || prev_calculated <= 0)// checking for the first start of the indicator calculation
			{
				K = 33 - RISK;
				limit = rates_total - StartBars;       // starting index for calculation of all bars
			}
		else
			{
				limit = rates_total - prev_calculated; // starting index for calculation of new bars
			}

		//---- indexing elements in arrays as timeseries  
		ArraySetAsSeries(high,true);
		ArraySetAsSeries(low,true);
		ArraySetAsSeries(close,true);

		//---- restore values of the variables
		uptrend = uptrend_;

		//---- main indicator calculation loop
		for(int bar = limit; bar >= 0; bar--)
			{
				//---- store values of the variables before running at the current bar
				if(rates_total != prev_calculated && bar == 0)
					{
						uptrend_= uptrend;
					}

				Range = 0;
				AvgRange = 0;
				for(int iii = bar; iii <= bar + SSP; iii++) AvgRange = AvgRange + MathAbs(high[iii] - low[iii]);
				Range = AvgRange /(SSP + 1);
				//----
				SsMax=low[bar];
				SsMin=close[bar];

				for(int kkk = bar; kkk <= bar + SSP -1; kkk++)
					{
						price = high[kkk];
						if(SsMax < price) SsMax = price;
						price = low[kkk];
						if(SsMin >= price) SsMin = price;
					}

				smin = SsMin + (SsMax - SsMin) * K / 100;
				smax = SsMax - (SsMax - SsMin) * K / 100;

				AskBuffer[bar]=0;
				BidBuffer[bar]=0;

				if(close[bar] < smin) uptrend = false;
				if(close[bar] > smax) uptrend = true;

				if(uptrend != old && uptrend == true)
					{
						BidBuffer[bar] = low[bar] - Range * ValueRange;
					}
				if(uptrend != old && uptrend == false)
					{
						AskBuffer[bar] = high[bar] + Range * ValueRange;
						//else counter=0;
					}
				if(bar > 0) old = uptrend;
			}
		//----     
		return(rates_total);
	}
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                    ArrayCopy.mq5 |
//|                                                              CJT |
//|                                     https://www.Chucknjill18.com |
//+------------------------------------------------------------------+
#property copyright "CJT"
#property link      "https://www.Chucknjill18.com"
#property version   "1.00"
#property description "The indicator highlights the candlesticks that are local"
#property description "highs and lows. Interval length for finding"
#property description "extreme values should be found using an input parameters."
//--- indicator settings
#property indicator_chart_window
#property indicator_buffers 5
#property indicator_plots   1
//---- plot
#property indicator_label1  "Extremums"
#property indicator_type1   DRAW_COLOR_CANDLES
#property indicator_color1  clrLightSteelBlue,clrRed,clrBlue
#property indicator_style1  STYLE_SOLID
#property indicator_width1  1
//--- predefined constant
#define INDICATOR_EMPTY_VALUE 0.0
//--- input parameters
input int InpNum=4; // Half-interval length
//--- indicator buffers
double ExtOpen[];
double ExtHigh[];
double ExtLow[];
double ExtClose[];
double ExtColor[];
//--- global variables
int    ExtStart=0; // index of the first candlestick that is not an extremum
int    ExtCount=0; // number of non-extremums in the interval
//+------------------------------------------------------------------+
//| Filling out non-extremum candlesticks                            |
//+------------------------------------------------------------------+
void FillCandles(const double &open[],const double &high[],
                 const double &low[],const double &close[])
  {
//--- fill out the candlesticks
   ArrayCopy(ExtOpen,open,ExtStart,ExtStart,ExtCount);
   ArrayCopy(ExtHigh,high,ExtStart,ExtStart,ExtCount);
   ArrayCopy(ExtLow,low,ExtStart,ExtStart,ExtCount);
   ArrayCopy(ExtClose,close,ExtStart,ExtStart,ExtCount);
  }
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- indicator buffers mapping
   SetIndexBuffer(0,ExtOpen);
   SetIndexBuffer(1,ExtHigh);
   SetIndexBuffer(2,ExtLow);
   SetIndexBuffer(3,ExtClose);
   SetIndexBuffer(4,ExtColor,INDICATOR_COLOR_INDEX);
//--- specify the value, which is not displayed
   PlotIndexSetDouble(0,PLOT_EMPTY_VALUE,INDICATOR_EMPTY_VALUE);
//--- specify the names of indicator buffers for displaying in the data window
   PlotIndexSetString(0,PLOT_LABEL,"Open;High;Low;Close");
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//--- set straight indexing in time series
   ArraySetAsSeries(open,false);
   ArraySetAsSeries(high,false);
   ArraySetAsSeries(low,false);
   ArraySetAsSeries(close,false);
//--- variable of the bar calculation start
   int start=prev_calculated;
//--- calculation is not performed for the first InpNum*2 bars
   if(start==0)
     {
      start+=InpNum*2;
      ExtStart=0;
      ExtCount=0;
     }
//--- if the bar has just formed, check the next potential extremum
   if(rates_total-start==1)
      start--;
//--- bar index to be checked for the extremum
   int ext;
//--- indicator value calculation loop
   for(int i=start;i<rates_total-1;i++)
     {
      //--- initially on i bar without drawing
      ExtOpen[i]=0;
      ExtHigh[i]=0;
      ExtLow[i]=0;
      ExtClose[i]=0;
      //--- extremum index for check
      ext=i-InpNum;
      //--- check for the local maximum
      if(IsMax(high,ext))
        {
         //--- highlight an extremum candlestick
         ExtOpen[ext]=open[ext];
         ExtHigh[ext]=high[ext];
         ExtLow[ext]=low[ext];
         ExtClose[ext]=close[ext];
         ExtColor[ext]=1;
         //--- highlight other candles up to the extremum with a neutral color
         FillCandles(open,high,low,close);
         //--- change the variable colors
         ExtStart=ext+1;
         ExtCount=0;
         //--- pass to the next iteration
         continue;
        }
      //--- check for the local minimum
      if(IsMin(low,ext))
        {
         //--- highlight an extremum candlestick
         ExtOpen[ext]=open[ext];
         ExtHigh[ext]=high[ext];
         ExtLow[ext]=low[ext];
         ExtClose[ext]=close[ext];
         ExtColor[ext]=2;
         //--- highlight other candles up to the extremum with a neutral color
         FillCandles(open,high,low,close);
         //--- change variable values
         ExtStart=ext+1;
         ExtCount=0;
         //--- pass to the next iteration
         continue;
        }
      //--- increase the number of non-extremums at the interval
      ExtCount++;
     }
//--- return value of prev_calculated for next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
//| Check if the current array element is a local high               |
//+------------------------------------------------------------------+
bool IsMax(const double &price[],const int ind)
  {
//--- interval start variable
   int i=ind-InpNum;
//--- interval end period
   int finish=ind+InpNum+1;
//--- check for the first half of the interval
   for(;i<ind;i++)
     {
      if(price[ind]<=price[i])
         return(false);
     }
//--- check for the second half of the interval
   for(i=ind+1;i<finish;i++)
     {
      if(price[ind]<=price[i])
         return(false);
     }
//--- this is an extremum
   return(true);
  }
//+------------------------------------------------------------------+
//| Check if the current array element is a local low                |
//+------------------------------------------------------------------+
bool IsMin(const double &price[],const int ind)
  {
//--- interval start variable
   int i=ind-InpNum;
//--- interval end variable
   int finish=ind+InpNum+1;
//--- check for the first half of the interval
   for(;i<ind;i++)
     {
      if(price[ind]>=price[i])
         return(false);
     }
//--- check for the second half of the interval
   for(i=ind+1;i<finish;i++)
     {
      if(price[ind]>=price[i])
         return(false);
     }
//--- this is an extremum
   return(true);
  }
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                         ArraySetAsSeriesTime.mq5 |
//|                                                              CJT |
//|                                     https://www.Chucknjill18.com |
//+------------------------------------------------------------------+
#property copyright "CJT"
#property link      "https://www.Chucknjill18.com"
#property version   "1.00"
#property indicator_chart_window
#property indicator_buffers 1
#property indicator_plots   1
//---- plot Numeration
#property indicator_label1  "Numeration"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrLimeGreen
//--- indicator buffers
double         NumerationBuffer[];
datetime TimeStart = StringToTime("2018.04.10 15:00");
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- indicator buffers mapping
   SetIndexBuffer(0,NumerationBuffer,INDICATOR_DATA);
//--- set indexing for the buffer like in timeseries
   ArraySetAsSeries(NumerationBuffer,true);
//--- set accuracy of showing in DataWindow
   IndicatorSetInteger(INDICATOR_DIGITS,0);
//--- how the name of the indicator array is displayed in DataWindow
   PlotIndexSetString(0,PLOT_LABEL,"Bar #"); 
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//---  we'll store the time of the current zero bar opening
   static datetime currentBarTimeOpen = 0;
//--- revert access to array time[] - do it like in timeseries
   ArraySetAsSeries(time,true);
//--- If time of zero bar differs from the stored one
   if(currentBarTimeOpen!=time[0])
     {
     //--- enumerate all bars from the current to the chart depth
      for(int i=rates_total-1;i>=0;i--) NumerationBuffer[i]=i;
      currentBarTimeOpen=time[0];
     }
//--- return value of prev_calculated for next call
   return(rates_total);
  }
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                   SwingIndex.mq5 |
//|                                                              CJT |
//|                                     https://www.Chucknjill18.com |
//+------------------------------------------------------------------+
#property copyright "CJT"
#property link      "https://www.Chucknjill18.com"
#property version   "1.00"
//---- drawing the indicator in a separate window
#property indicator_separate_window
//---- number of indicator buffers
#property indicator_buffers 1 
//---- only one plot is used
#property indicator_plots   1
//+-----------------------------------+
//|  Indicator drawing parameters     |
//+-----------------------------------+
//---- drawing the indicator as a line
#property indicator_type1   DRAW_LINE
//---- gold color is used for the indicator line
#property indicator_color1 Gold
//---- the indicator line is a continuous curve
#property indicator_style1  STYLE_SOLID
//---- indicator line width is equal to 1
#property indicator_width1  1
//---- displaying the indicator label
#property indicator_label1  "SwingIndex"
//+-----------------------------------+
//|  Indicator input parameters       |
//+-----------------------------------+
input double T=30000; // Scaling ratio
input int Shift=0;    // Horizontal shift of the indicator in bars
//---- declaration of a dynamic array that further 
//---- will be used as an indicator buffer
double SWINGINDEX[];
//---- declaration of the integer variables for the start of data calculation
int StartBars=1;
//+------------------------------------------------------------------+   
//| SwingIndex indicator initialization function                     | 
//+------------------------------------------------------------------+ 
void OnInit()
  {
//---- set SWINGINDEX[] dynamic array as an indicator buffer
   SetIndexBuffer(0,SWINGINDEX,INDICATOR_DATA);
//---- moving the indicator 1 horizontally
   PlotIndexSetInteger(0,PLOT_SHIFT,Shift);
//---- performing the shift of the beginning of the indicator drawing
   PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,StartBars);
//---- setting the indicator values that won't be visible on a chart
   PlotIndexSetDouble(0,PLOT_EMPTY_VALUE,0);

//---- initializations of a variable for the indicator short name
   string shortname;
   StringConcatenate(shortname,"SwingIndex( ",DoubleToString(T,2)," )");
//---- creation of the name to be displayed in a separate sub-window and in a tooltip
   IndicatorSetString(INDICATOR_SHORTNAME,shortname);
//---- determination of accuracy of displaying the indicator values
   IndicatorSetInteger(INDICATOR_DIGITS,_Digits+1);
//---- initialization end
  }
//+------------------------------------------------------------------+ 
//| SwingIndex iteration function                                    | 
//+------------------------------------------------------------------+ 
int OnCalculate(const int rates_total,    // number of bars in history at the current tick
                const int prev_calculated,// number of bars calculated at previous call
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//---- checking the number of bars to be enough for the calculation
   if(rates_total<StartBars) return(0);
//---- declaration of variables with a floating point  
   double O,O1,C,C1,H,L,K,TR,SH1,SI,ER,R;
//---- declaration of integer variables and getting calculated bars
   int first,bar;
//---- calculation of the 'first' starting index for the bars recalculation loop
   if(prev_calculated>rates_total || prev_calculated<=0) // checking for the first start of the indicator calculation
      first=1;                                           // starting index for calculation of all bars
   else first=prev_calculated-1;                         // starting index for calculation of new bars
//---- main indicator calculation loop
   for(bar=first; bar<rates_total && !IsStopped(); bar++)
     {
      O=open[bar];
      O1=open[bar-1];
      C=close[bar];
      C1=close[bar-1];
      H=high[bar];
      L=low[bar];

      K=MathMax(MathAbs(H-C1),MathAbs(L-C1));
      TR=MathMax(MathMax(MathAbs(H-C1),MathAbs(L-C1)),MathAbs(H-L));

      ER=0;
      if(C1 > H) ER = MathAbs(H - C1);
      if(C1 < L) ER = MathAbs(L - C1);

      SH1=MathAbs(C1-O1);
      R=TR-0.5*ER+0.25*SH1;
      SI=0;
      if (R!=0) SI=50*((C-C1)+0.5*(C-O)+0.25*(C1-O1))*(K/T)/R;
      SWINGINDEX[bar]=SI;
     }
//----     
   return(rates_total);
  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                               Demo_iFractals.mq5 |
//|                        Copyright 2011, MetaQuotes Software Corp. |
//|                                              https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2011, MetaQuotes Software Corp."
#property link      "https://www.mql5.com"
#property version   "1.00"
#property description "The indicator demonstrates how to obtain data"
#property description "of indicator buffers for the iFractals technical indicator."
#property description "A symbol and timeframe used for calculation of the indicator,"
#property description "are set by the symbol and period parameters."
#property description "The method of creation of the handle is set through the 'type' parameter (function type)."
 
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
//--- the FractalUp plot
#property indicator_label1  "FractalUp"
#property indicator_type1   DRAW_ARROW
#property indicator_color1  clrAqua
//--- the FractalDown plot
#property indicator_label2  "FractalDown"
#property indicator_type2   DRAW_ARROW
#property indicator_color2  clrWhite
//+------------------------------------------------------------------+
//| Enumeration of the methods of handle creation                    |
//+------------------------------------------------------------------+
enum Creation
  {
   Call_iFractals,         // use iFractals
   Call_IndicatorCreate    // use IndicatorCreate
  };
//--- input parameters
input Creation             type=Call_iFractals;          // type of the function
input string               symbol=" ";                   // symbol 
input ENUM_TIMEFRAMES      period=PERIOD_CURRENT;        // timeframe
//--- indicator buffers
double         FractalUpBuffer[];
double         FractalDownBuffer[];
//--- variable for storing the handle of the iFractals indicator
int    handle;
//--- variable for storing
string name=symbol;
//--- name of the indicator on a chart
string short_name;
//--- we will keep the number of values in the Fractals indicator
int    bars_calculated=0;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- assignment of arrays to indicator buffers
   SetIndexBuffer(0,FractalUpBuffer,INDICATOR_DATA);
   SetIndexBuffer(1,FractalDownBuffer,INDICATOR_DATA);
//--- set codes using a symbol from the Wingdings charset for the PLOT_ARROW property
   PlotIndexSetInteger(0,PLOT_ARROW,218); // arrow up
   PlotIndexSetInteger(1,PLOT_ARROW,217); // arrow down
//--- determine the symbol the indicator is drawn for
   name=symbol;
//--- delete spaces to the right and to the left
   StringTrimRight(name);
   StringTrimLeft(name);
//--- if it results in zero length of the 'name' string
   if(StringLen(name)==0)
     {
      //--- take the symbol of the chart the indicator is attached to
      name=_Symbol;
     }
//--- create handle of the indicator
   if(type==Call_iFractals)
      handle=iFractals(name,period);
   else
      handle=IndicatorCreate(name,period,IND_FRACTALS);
//--- if the handle is not created
   if(handle==INVALID_HANDLE)
     {
      //--- tell about the failure and output the error code
      PrintFormat("Failed to create handle of the iFractals indicator for the symbol %s/%s, error code %d",
                  name,
                  EnumToString(period),
                  GetLastError());
      //--- the indicator is stopped early
      return(INIT_FAILED);
     }
//--- show the symbol/timeframe the Fractals indicator is calculated for
   short_name=StringFormat("iFractals(%s/%s)",name,EnumToString(period));
   IndicatorSetString(INDICATOR_SHORTNAME,short_name);
//--- normal initialization of the indicator
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//--- number of values copied from the iFractals indicator
   int values_to_copy;
//--- determine the number of values calculated in the indicator
   int calculated = BarsCalculated(handle);
   if(calculated <= 0)
     {
      PrintFormat("BarsCalculated() returned %d, error code %d",calculated,GetLastError());
      return(0);
     }
//--- if it is the first start of calculation of the indicator or if the number of values in the iFractals indicator changed
//---or if it is necessary to calculated the indicator for two or more bars (it means something has changed in the price history)
   if(prev_calculated==0 || calculated!=bars_calculated || rates_total > prev_calculated+1)
     {
      //--- if the FractalUpBuffer array is greater than the number of values in the iFractals indicator for symbol/period, then we don't copy everything 
      //--- otherwise, we copy less than the size of indicator buffers
      if(calculated>rates_total) values_to_copy=rates_total;
      else                       values_to_copy=calculated;
     }
   else
     {
      //--- it means that it's not the first time of the indicator calculation, and since the last call of OnCalculate()
      //--- for calculation not more than one bar is added
      values_to_copy =(rates_total - prev_calculated) + 1;
     }
//--- fill the FractalUpBuffer and FractalDownBuffer arrays with values from the Fractals indicator
//--- if FillArrayFromBuffer returns false, it means the information is nor ready yet, quit operation
   if(!FillArraysFromBuffers(FractalUpBuffer,FractalDownBuffer,handle,values_to_copy)) return(0);
//--- form the message
   string comm=StringFormat("%s ==>  Updated value in the indicator %s: %d",
                            TimeToString(TimeCurrent(),TIME_DATE|TIME_SECONDS),
                            short_name,
                            values_to_copy);
//--- display the service message on the chart
   Comment(comm);
//--- memorize the number of values in the Fractals indicator
   bars_calculated=calculated;
//--- return the prev_calculated value for the next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
//| Filling indicator buffers from the iFractals indicator           |
//+------------------------------------------------------------------+
bool FillArraysFromBuffers(double &up_arrows[],        // indicator buffer for up arrows
                           double &down_arrows[],      // indicator buffer for down arrows
                           int ind_handle,             // handle of the iFractals indicator
                           int amount                  // number of copied values
                           )
  {
//--- reset error code
   ResetLastError();
//--- fill a part of the FractalUpBuffer array with values from the indicator buffer that has 0 index
   if(CopyBuffer(ind_handle,0,0,amount,up_arrows)<0)
     {
      //--- if the copying fails, tell the error code
      PrintFormat("Failed to copy data from the iFractals indicator to the FractalUpBuffer array, error code %d",
                  GetLastError());
      //--- quit with zero result - it means that the indicator is considered as not calculated
      return(false);
     }
//--- fill a part of the FractalDownBuffer array with values from the indicator buffer that has index 1
   if(CopyBuffer(ind_handle,1,0,amount,down_arrows)<0)
     {
      //--- if the copying fails, tell the error code
      PrintFormat("Failed to copy data from the iFractals indicator to the FractalDownBuffer array, error code %d",
                  GetLastError());
      //--- quit with zero result - it means that the indicator is considered as not calculated
      return(false);
     }
//--- everything is fine
   return(true);
  }
//+------------------------------------------------------------------+
//| Indicator deinitialization function                              |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//--- clear the chart after deleting the indicator
   Comment("");
  }
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                          candle_size_info_jj.mq5 |
//|                                                              CJT |
//|                                     https://www.Chucknjill18.com |
//+------------------------------------------------------------------+
//--- Description
#property description "Script creates \ "Text \"."
//--- input parameters display window during script startup
#property script_show_inputs


//--- Script parameter input
input int               IHistoryBars   =  25;            // Check Historical Data
input string            InpFont        =  "Tahoma";      // Font Type
input int               InpFontSize    =  10;            // Size of fonts
input color             UpColor        =  clrGold;       // Color Up
input color             DwColor        =  clrRoyalBlue;  // Color Dw
input color             HLColor        =  clrAqua;       // Color Center
input bool              InpBack        =  false;         // Background object
input bool              InpSelection   =  false;         // Highlight to move
input bool              InpHidden      =  true;          // Hide in object list
input bool              WithDigit      =  true;          // Hide in object list
input long              InpZOrder      =  0;             // Priority for mouse click


int digits=0,extradig=0;

double            InpAngle    =     0.0;           // Angle of inclination in degrees
ENUM_ANCHOR_POINT InpAnchor   =     ANCHOR_CENTER; // Type of Anchorage
double   gBars                =     0;
double   gTotalRates          =     0;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnInit()
  {
   digits=MathPow(10,_Digits-extradig);
   if(WithDigit) extradig=1;

  }
//+------------------------------------------------------------------+
//| Move anchor point                                                |
//+------------------------------------------------------------------+
bool TextMove(const long   chart_ID=0,  // Chart ID
              const string name="Text", // object name
              datetime     time=0,      // time anchor point coordinate
              double       price=0)     // price anchor point coordinate
  {
//--- if the position of the point is not set, move to the current bar having the Bid price
   if(!time)
      time=TimeCurrent();
   if(!price)
      price=SymbolInfoDouble(Symbol(),SYMBOL_BID);
//--- resets the error value
   ResetLastError();
//--- move the anchor point
   if(!ObjectMove(chart_ID,name,0,time,price))
     {
      Print(__FUNCTION__,
            ": failed to move anchor point! Error Code = ",GetLastError());
      return(false);
     }
//--- success in execution
   return(true);
  }
//+------------------------------------------------------------------+
//| Check anchor point values and set default values for those empty |
//|                                                                  |
//+------------------------------------------------------------------+
void ChangeTextEmptyPoint(datetime &time,double &price)
  {
//--- if the point time is not set, it will be in the current bar
   if(!time)
      time=TimeCurrent();
//--- if the point price is not set, it will have bid value
   if(!price)
      price=SymbolInfoDouble(Symbol(),SYMBOL_BID);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
   ClearMyObjects();
   Print("Deinit Value Chart, reason = "+IntegerToString(reason));
  }
//+------------------------------------------------------------------+
//| Value Chart                                                      | 
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &Time[],
                const double &Open[],
                const double &High[],
                const double &Low[],
                const double &Close[],
                const long &TickVolume[],
                const long &Volume[],
                const int &Spread[])
  {

//--- define how texts are often displayed
   int scale=(int)ChartGetInteger(0,CHART_SCALE);
   int bars=(int)ChartGetInteger(0,CHART_VISIBLE_BARS)+3;
   double value;
   digits=MathPow(10,_Digits-extradig);

//--- set the step
   int step=1;
   switch(scale)
     {
      case 0:
         step=12;
         break;
      case 1:
         step=6;
         break;
      case 2:
         step=4;
         break;
      case 3:
         step=2;
         break;
     }
   gTotalRates=rates_total;
   gBars=bars;
   for(int i=rates_total-1;i>rates_total-bars;i-=step) 
     {
      if(Close[i]>Open[i])
        {
         // bullish candle
         value=(Close[i]-Open[i])*digits;
         //Print(Close[i]-Open[i], "->", Close[i]-Open[i]*digits);

         TextCreate(0,"Text_"+(string)i+(string)PERIOD_CURRENT,0,Time[i],(Close[i]+Open[i])/2,DoubleToString(value,extradig),InpFont,InpFontSize,
                    UpColor,InpAngle,InpAnchor,InpBack,InpSelection,InpHidden,InpZOrder);

         value=(Open[i]-Low[i]) *digits;
         if(!TextCreate(0,"TextL_"+(string)i+(string)PERIOD_CURRENT,0,Time[i],Low[i],DoubleToString(value,extradig),InpFont,InpFontSize,
            HLColor,InpAngle,InpAnchor,InpBack,InpSelection,InpHidden,InpZOrder)) {       return 0;     }

         value=(High[i]-Close[i]) *digits;
         if(!TextCreate(ChartID(),"TextH_"+(string)i+(string)PERIOD_CURRENT,0,Time[i],High[i],DoubleToString(value,extradig),InpFont,InpFontSize,
            HLColor,InpAngle,InpAnchor,InpBack,InpSelection,InpHidden,InpZOrder)) {       return 0;     }

           } else {

         value=(Open[i]-Close[i]) *digits;
         if(!TextCreate(ChartID(),"Text_"+(string)i+(string)PERIOD_CURRENT,0,Time[i],(Close[i]+Open[i])/2,DoubleToString(value,extradig),InpFont,InpFontSize,
            DwColor,-InpAngle,InpAnchor,InpBack,InpSelection,InpHidden,InpZOrder)) {      return 0;      }

         value=(Close[i]-Low[i]) *digits;
         if(!TextCreate(ChartID(),"TextL_"+(string)i+(string)PERIOD_CURRENT,0,Time[i],Low[i],DoubleToString(value,extradig),InpFont,InpFontSize,
            HLColor,InpAngle,InpAnchor,InpBack,InpSelection,InpHidden,InpZOrder)) {       return 0;     }

         value=(High[i]-Open[i]) *digits;
         if(!TextCreate(ChartID(),"TextH_"+(string)i+(string)PERIOD_CURRENT,0,Time[i],High[i],DoubleToString(value,extradig),InpFont,InpFontSize,
            HLColor,InpAngle,InpAnchor,InpBack,InpSelection,InpHidden,InpZOrder)) {       return 0;     }
        }

     }
   ChartRedraw();
   return 0;
  }
//+------------------------------------------------------------------+
//|  Trace Arrow Function                                            |
//+------------------------------------------------------------------+
void Trace(string name,int sens,double price,datetime time,color couleur)
  {
   ObjectCreate(0,name,OBJ_ARROW,0,time,price);
   if(sens==1)
      ObjectSetInteger(0,name,OBJPROP_ARROWCODE,233);
   if(sens==-1)
      ObjectSetInteger(0,name,OBJPROP_ARROWCODE,234);
   ObjectSetInteger(0,name,OBJPROP_COLOR,couleur);
   ObjectSetInteger(0,name,OBJPROP_WIDTH,5);
  }
//+------------------------------------------------------------------+
//|   Delete Arrow Function                                          |
//+------------------------------------------------------------------+  
void ClearMyObjects() 
  {
   string name;
   int scale=(int)ChartGetInteger(0,CHART_SCALE);
   int bars=(int)ChartGetInteger(0,CHART_VISIBLE_BARS)+3;
   double value;
   digits=MathPow(10,_Digits-extradig);
   int step=1;
   switch(scale)
     {
      case 0:
         step=12;
         break;
      case 1:
         step=6;
         break;
      case 2:
         step=4;
         break;
      case 3:
         step=2;
         break;
     }
   for(int i=gTotalRates-1;i>gTotalRates-bars;i-=step) 
     {
      if(!TextDelete(ChartID(),"Text_"+(string)i+(string)PERIOD_CURRENT)){}
      if(!TextDelete(ChartID(),"TextH_"+(string)i+(string)PERIOD_CURRENT)){}
      if(!TextDelete(ChartID(),"TextL_"+(string)i+(string)PERIOD_CURRENT)){}
      //--- redraw the chart
     }
   ChartRedraw();

  }
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| Change object text                                               |
//+------------------------------------------------------------------+
bool TextChange(const long   chart_ID=0,  // Chart ID
                const string name="Text", // object name
                const string text="Text") // text
  {
//--- resets the error value
   ResetLastError();
//--- change object text
   if(!ObjectSetString(chart_ID,name,OBJPROP_TEXT,text))
     {
      Print(__FUNCTION__,
            ": failed to change text! Error Code = ",GetLastError());
      return(false);
     }
//--- success in execution
   return(true);
  }
//+------------------------------------------------------------------+
//| Delete Text Object                                               |
//+------------------------------------------------------------------+
bool TextDelete(const long   chart_ID=0,  // Graph Id
                const string name="Text") // object name
  {
//--- resets the error value
   ResetLastError();
//--- delete the object
   if(!ObjectDelete(chart_ID,name))
     {
      Print(__FUNCTION__,
            ": failed to delete object \ "Text \"! Error Code = ",GetLastError());
      return(false);
     }
//--- success in execution
   return(true);
  }
//+------------------------------------------------------------------+
//| Creating Text Object                                             |
//+------------------------------------------------------------------+
bool TextCreate(const long              chart_ID=0,               // Chart ID
                const string            name="Text",              // object name
                const int               sub_window=0,             // sub-window index
                datetime                time=0,                   // anchor point of time
                double                  price=0,                  // price anchor point
                const string            text="Text",              // the text itself
                const string            font="Arial",             // fonte
                const int               font_size=10,             // font_size
                const color             clr=clrRed,               // color
                const double            angle=0.0,                // text tilt
                const ENUM_ANCHOR_POINT anchor=ANCHOR_CENTER,     // type of anchorage
                const bool              back=false,               // in the background
                const bool              selection=false,          // highlight to move
                const bool              hidden=true,              // hide in object list
                const long              z_order=0)                // priority to click the mouse
  {

   if(ObjectFind(chart_ID,name)==-1) 
     {

      //--- set the coordinates of anchor points, if they are not defined
      //--- resets the error value
      ResetLastError();
      //--- create Text object
      if(!ObjectCreate(chart_ID,name,OBJ_TEXT,sub_window,time,price)) 
        {
         Print(__FUNCTION__,
               ": failed to create object \ "Text \"! Error Code = ",GetLastError());
         return(false);
        }
      //--- set the text
      ObjectSetString(chart_ID,name,OBJPROP_TEXT,text);
      //--- set the source text
      ObjectSetString(chart_ID,name,OBJPROP_FONT,font);
      //--- set font size
      ObjectSetInteger(chart_ID,name,OBJPROP_FONTSIZE,font_size);
      //--- set the angle of the text
      ObjectSetDouble(chart_ID,name,OBJPROP_ANGLE,angle);
      //--- Anchor type definition
      ObjectSetInteger(chart_ID,name,OBJPROP_ANCHOR,anchor);
      ObjectSetInteger(chart_ID,name,OBJPROP_ALIGN,ALIGN_CENTER);

      //--- set color
      ObjectSetInteger(chart_ID,name,OBJPROP_COLOR,clr);
      //--- foreground (false) or background (true)
      ObjectSetInteger(chart_ID,name,OBJPROP_BACK,back);
      //--- enable (true) or disable (false) the way to move the object with the mouse
      ObjectSetInteger(chart_ID,name,OBJPROP_SELECTABLE,selection);
      ObjectSetInteger(chart_ID,name,OBJPROP_SELECTED,selection);
      //--- hide (true) or display (false) the name of the chart object in the object list 
      ObjectSetInteger(chart_ID,name,OBJPROP_HIDDEN,hidden);
      //--- set the priority to receive the event with a mouse click on the chart
      ObjectSetInteger(chart_ID,name,OBJPROP_ZORDER,z_order);
      ObjectSetDouble(chart_ID,name,OBJPROP_PRICE,price);
      ObjectSetInteger(chart_ID,name,OBJPROP_TIME,time);

      //--- success in execution
      return(true);
        } else {
      ChangeTextEmptyPoint(time,price);

      ObjectSetString(chart_ID,name,OBJPROP_TEXT,text);
      ObjectSetInteger(chart_ID,name,OBJPROP_COLOR,clr);
      ObjectSetInteger(chart_ID,name,OBJPROP_ANCHOR,anchor);
      ObjectSetInteger(chart_ID,name,OBJPROP_ALIGN,ALIGN_CENTER);
      ObjectSetDouble(chart_ID,name,OBJPROP_ANGLE,angle);
      ObjectSetDouble(chart_ID,name,OBJPROP_PRICE,price);
      ObjectSetInteger(chart_ID,name,OBJPROP_TIME,time);
      ObjectSetString(chart_ID,name,OBJPROP_FONT,font);
      //--- set font size
      ObjectSetInteger(chart_ID,name,OBJPROP_FONTSIZE,font_size);
      ObjectSetInteger(chart_ID,name,OBJPROP_YDISTANCE,100);

/*ObjectSetString(chart_ID,name,OBJPROP_FONT,Font);
      ObjectSetInteger(chart_ID,name,OBJPROP_FONTSIZE,Size);*/
      return (true);
     }

  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnChartEvent(const int id,         // Event ID
                  const long& lparam,   // Parameter of type long event
                  const double& dparam, // Parameter of type double event
                  const string& sparam  // Parameter of type string events
                  )
  {
   if(id==CHARTEVENT_CHART_CHANGE)
     {
      ClearMyObjects();
     }

  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                     CTrade2.mq5 |
//|                   Copyright 2009-2017, MetaQuotes Software Corp. |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "CJT"
#property link      "http://www.mql5.com"
//---- indicator settings
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
#property indicator_type1   DRAW_ARROW
#property indicator_type2   DRAW_ARROW
#property indicator_color1  clrRed
#property indicator_color2  clrWhite
#property indicator_label1  "CTrade Down"
#property indicator_label2  "CTrade Up"
#property indicator_width1  4
#property indicator_width2  4
//---- indicator buffers
double BidBuffer[];
double AskBuffer[];

//+----------------------------------------------+
//|  Indicator input parameters                  |
//+----------------------------------------------+
input double   OffSet   =  41.95;              //41.95
input double   Risk     =  9.25;  
//--- 10 pixels upper from high price
int   ArrowShift  =  -5;
//--- Starting of the bar sequence
int   StartBars   =  0;
double K;
bool old,uptrend_;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
void OnInit()
  {
      //---- initialization of variables of the start of data calculation
		StartBars = OffSet + 1;
		//---- set AskBuffer[] dynamic array as an indicator buffer
		SetIndexBuffer(0,AskBuffer,INDICATOR_DATA);
		PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,StartBars);
		PlotIndexSetString(0,PLOT_LABEL,"Silver Sell");
		PlotIndexSetInteger(0,PLOT_ARROW,218);
		ArraySetAsSeries(AskBuffer,true);

		//---- set BidBuffer[] dynamic array as an indicator buffer
		SetIndexBuffer(1,BidBuffer,INDICATOR_DATA);
		PlotIndexSetInteger(1,PLOT_DRAW_BEGIN,StartBars);
		PlotIndexSetString(1,PLOT_LABEL,"Silver Buy");
		PlotIndexSetInteger(1,PLOT_ARROW,217);
		ArraySetAsSeries(BidBuffer,true);
		
		//---- indexing the elements in the buffer as timeseries
		   ArraySetAsSeries(AskBuffer,true);         
         ArraySetAsSeries(BidBuffer,true);
      //---- initialization done
  }
//+------------------------------------------------------------------+
//|  Accelerator/Decelerator Oscillator                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
   {
	//---- checking the number of bars to be enough for the calculation
		if(rates_total < StartBars) return(0);

		//---- declarations of local variables 
		int limit;
		double Range,AvgRange,smin,smax,SsMax,SsMin,price;
		bool uptrend;

		//---- calculations of the necessary amount of data to be copied
		//---- and the 'limit' starting index for the bars recalculation loop
		if(prev_calculated > rates_total || prev_calculated <= 0)// checking for the first start of the indicator calculation
			{
				K = 33 - Risk;
				limit = rates_total - StartBars;       // starting index for calculation of all bars
			}
		else
			{
				limit = rates_total - prev_calculated; // starting index for calculation of new bars
			}

		//---- indexing elements in arrays as timeseries  
		ArraySetAsSeries(high,true);
		ArraySetAsSeries(low,true);
		ArraySetAsSeries(close,true);

		//---- restore values of the variables
		uptrend = uptrend_;

		//---- main indicator calculation loop
		for(int bar = limit; bar >= 0; bar--)
			{
				//---- store values of the variables before running at the current bar
				if(rates_total != prev_calculated && bar == 0)
					{
						uptrend_= uptrend;
					}

				Range = 0;
				AvgRange = 0;
				for(int iii = bar; iii <= bar + OffSet; iii++) 
				AvgRange = AvgRange + MathAbs(high[iii] - low[iii]);
				Range = AvgRange /(OffSet + 1);
				//----
				SsMax=low[bar];
				SsMin=close[bar];
				for(int kkk = bar; kkk <= bar + OffSet -1; kkk++)
					{
						price = high[kkk];
						if(SsMax < price) SsMax = price;
						price = low[kkk];
						if(SsMin >= price) SsMin = price;
					}
            //Comment(StringFormat("Show prices\nSsMin = %G\nSsMax = %G\nSsMin = %G\nK = %d",SsMin,SsMax,SsMin,K,0));  
				smin = SsMin + (SsMax - SsMin) * K / 100;
				smax = SsMax - (SsMax - SsMin) * K / 100;

				AskBuffer[bar]=0;
				BidBuffer[bar]=0;

				if(close[bar] < smin) uptrend = false;
				if(close[bar] > smax) uptrend = true;

				if(uptrend != old && uptrend == true)
					{
						BidBuffer[bar] = low[bar] - Range * 2;
					}
				if(uptrend != old && uptrend == false)
					{
						AskBuffer[bar] = high[bar] + Range * 2;
						//else counter=0;
					}
				if(bar > 0) old = uptrend;
			}
		//----     
		return(rates_total);
     
   }

//+------------------------------------------------------------------+

//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                          WPR.mq5 |
//|                        Copyright 2009, MetaQuotes Software Corp. |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright   "2009, MetaQuotes Software Corp."
#property link        "http://www.mql5.com"
#property description "Larry Williams' Percent Range"
//---- indicator settings
#property indicator_separate_window
#property indicator_level1     -20.0
#property indicator_level2     -80.0
#property indicator_levelstyle STYLE_DOT
#property indicator_levelcolor Silver
#property indicator_levelwidth 1
#property indicator_maximum    0.0
#property indicator_minimum    -100.0
#property indicator_buffers    1
#property indicator_plots      1
#property indicator_type1      DRAW_LINE
#property indicator_color1     DodgerBlue
//---- input parameters
input int InpWPRPeriod=14; // Period
//---- buffers
double    ExtWPRBuffer[];
//--- global variables
int       ExtPeriodWPR;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
void OnInit()
  {
//--- check for input value
   if(InpWPRPeriod<3)
     {
      ExtPeriodWPR=14;
      Print("Incorrect InpWPRPeriod value. Indicator will use value=",ExtPeriodWPR);
     }
   else ExtPeriodWPR=InpWPRPeriod;
//---- name for DataWindow and indicator subwindow label
   IndicatorSetString(INDICATOR_SHORTNAME,"%R"+"("+string(ExtPeriodWPR)+")");
//---- indicator's buffer   
   SetIndexBuffer(0,ExtWPRBuffer);
   PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,ExtPeriodWPR-1);
//--- digits   
   IndicatorSetInteger(INDICATOR_DIGITS,2);
//----
  }
//+------------------------------------------------------------------+
//| Williams’ Percent Range                                          |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,const int prev_calculated,
                const datetime &Time[],
                const double &Open[],
                const double &High[],
                const double &Low[],
                const double &Close[],
                const long &TickVolume[],
                const long &Volume[],
                const int &Spread[])
  {
//---- insufficient data
   if(rates_total<ExtPeriodWPR)
      return(0);
//--- start working
   int i=prev_calculated-1;
//--- correct position
   if(i<ExtPeriodWPR-1) i=ExtPeriodWPR-1;
//---  main cycle
   while(i<rates_total && !IsStopped())
     {
      //--- calculate maximum High
      double dMaxHigh=MaxAr(High,ExtPeriodWPR,i);
      //--- calculate minimum Low
      double dMinLow=MinAr(Low,ExtPeriodWPR,i);
      //--- calculate WPR
      if(dMaxHigh!=dMinLow)
         ExtWPRBuffer[i]=-(dMaxHigh-Close[i])*100/(dMaxHigh-dMinLow);
      else
         ExtWPRBuffer[i]=ExtWPRBuffer[i-1];
      //--- increment i for next iteration
      i++;
     }
   //--- return new prev_calculated value
   return(rates_total);
  }
//+------------------------------------------------------------------+
//| Maximum High                                                     |
//+------------------------------------------------------------------+
double MaxAr(const double &array[],int period,int cur_position)
  {
   double Highest=array[cur_position];
   for(int i=cur_position-1;i>cur_position-period;i--)
     {
      if(Highest<array[i]) Highest=array[i];
     }
   return(Highest);
  }
//+------------------------------------------------------------------+
//| Minimum Low                                                      |
//+------------------------------------------------------------------+
double MinAr(const double &array[],int period,int cur_position)
  {
   double Lowest=array[cur_position];
   for(int i=cur_position-1;i>cur_position-period;i--)
     {
      if(Lowest>array[i]) Lowest=array[i];
     }
   return(Lowest);
  }
//+------------------------------------------------------------------+ 
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                   iCTrade_V1.mq5 |
//|                        Copyright 2011, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2011, MetaQuotes Software Corp."
#property link      "https://www.mql5.com"
#property version   "1.00"
#property description "The indicator demonstrates how to obtain data"
#property description "of indicator buffers for the iFractals technical indicator."
#property description "A symbol and timeframe used for calculation of the indicator,"
#property description "are set by the symbol and period parameters."
#property description "The method of creation of the handle is set through the 'type' parameter (function type)."
 
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
//--- the FractalUp plot
#property indicator_label1  "FractalUp"
#property indicator_type1   DRAW_ARROW
#property indicator_color1  clrBlue
//--- the FractalDown plot
#property indicator_label2  "FractalDown"
#property indicator_type2   DRAW_ARROW
#property indicator_color2  clrRed
//+------------------------------------------------------------------+
//| Enumeration of the methods of handle creation                    |
//+------------------------------------------------------------------+
enum Creation
	{
		Call_iFractals,    		// use iFractals
		Call_IndicatorCreate 	// use IndicatorCreate
	};
//--- input parameters
input Creation             type		= Call_iFractals;  	// type of the function
input string               symbol 	= " ";             	// symbol 
input ENUM_TIMEFRAMES      period 	= PERIOD_CURRENT;  	// timeframe
//--- indicator buffers
double         FractalUpBuffer[];
double         FractalDownBuffer[];
//--- variable for storing the handle of the iFractals indicator
int    handle;
//--- variable for storing
string name = symbol;
//--- name of the indicator on a chart
string short_name;
//--- we will keep the number of values in the Fractals indicator
int    bars_calculated = 0;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
	{
		//--- assignment of arrays to indicator buffers
		   SetIndexBuffer(0,FractalUpBuffer,INDICATOR_DATA);
		   SetIndexBuffer(1,FractalDownBuffer,INDICATOR_DATA);
		//--- set codes using a symbol from the Wingdings charset for the PLOT_ARROW property
		   PlotIndexSetInteger(0,PLOT_ARROW,234); // arrow up
		   PlotIndexSetInteger(1,PLOT_ARROW,233); // arrow down
		//--- determine the symbol the indicator is drawn for
		   name=symbol;
		//--- delete spaces to the right and to the left
		   StringTrimRight(name);
		   StringTrimLeft(name);
		//--- if it results in zero length of the 'name' string
		   if(StringLen(name) == 0)
			 {
			  //--- take the symbol of the chart the indicator is attached to
			  name=_Symbol;
			 }
		//--- create handle of the indicator
		   if(type==Call_iFractals)
			  handle=iFractals(name,period);
		   else
			  handle=IndicatorCreate(name,period,IND_FRACTALS);
		//--- if the handle is not created
		   if(handle==INVALID_HANDLE)
			 {
			  //--- tell about the failure and output the error code
			  PrintFormat("Failed to create handle of the iFractals indicator for the symbol %s/%s, error code %d",
						  name,
						  EnumToString(period),
						  GetLastError());
			  //--- the indicator is stopped early
			  return(INIT_FAILED);
			 }
		//--- show the symbol/timeframe the Fractals indicator is calculated for
		   short_name=StringFormat("iFractals(%s/%s)",name,EnumToString(period));
		   IndicatorSetString(INDICATOR_SHORTNAME,short_name);
		//--- normal initialization of the indicator
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
	{
		//--- number of values copied from the iFractals indicator
			int values_to_copy;
		//--- determine the number of values calculated in the indicator
			int calculated = BarsCalculated(handle);
			if(calculated <= 0) {PrintFormat("BarsCalculated() returned %d, error code %d",calculated,GetLastError());return(0);}
   
		//--- if it is the first start of calculation of the indicator or if the number of values in the iFractals indicator changed
		//---or if it is necessary to calculated the indicator for two or more bars (it means something has changed in the price history)
			if(prev_calculated == 0 || calculated != bars_calculated || rates_total > prev_calculated + 1)
				{
					//--- if the FractalUpBuffer array is greater than the number of values in the iFractals indicator for symbol/period, then we don't copy everything 
					//--- otherwise, we copy less than the size of indicator buffers
					if(calculated > rates_total) 
						values_to_copy = rates_total;
					else                       
						values_to_copy = calculated;
				}
			else
				{
					//--- it means that it's not the first time of the indicator calculation, and since the last call of OnCalculate()
					//--- for calculation not more than one bar is added
						values_to_copy = (rates_total - prev_calculated) + 1;
				}
		//--- fill the FractalUpBuffer and FractalDownBuffer arrays with values from the Fractals indicator
		//--- if FillArrayFromBuffer returns false, it means the information is nor ready yet, quit operation
			if(!FillArraysFromBuffers(FractalUpBuffer,FractalDownBuffer,handle,values_to_copy)) return(0);
		//--- form the message
			string comm = StringFormat("%s ==>  Updated value in the indicator %s: %d",TimeToString(TimeCurrent(),TIME_DATE|TIME_SECONDS),short_name,values_to_copy);
		//--- display the service message on the chart
			Comment(comm);
		//--- memorize the number of values in the Fractals indicator
			bars_calculated = calculated;
		//--- return the prev_calculated value for the next call
		return(rates_total);
	}
//+------------------------------------------------------------------+
//| Filling indicator buffers from the iFractals indicator           |
//+------------------------------------------------------------------+
bool FillArraysFromBuffers(double &up_arrows[],        // indicator buffer for up arrows
                           double &down_arrows[],      // indicator buffer for down arrows
                           int ind_handle,             // handle of the iFractals indicator
                           int amount                  // number of copied values
                           )
	{
		//--- reset error code
			ResetLastError();
		//--- fill a part of the FractalUpBuffer array with values from the indicator buffer that has 0 index
			if(CopyBuffer(ind_handle,0,0,amount,up_arrows) < 0)
				{
					//--- if the copying fails, tell the error code
						PrintFormat("Failed to copy data from the iFractals indicator to the FractalUpBuffer array, error code %d",GetLastError());
					//--- quit with zero result - it means that the indicator is considered as not calculated
						return(false);
				}
		//--- fill a part of the FractalDownBuffer array with values from the indicator buffer that has index 1
		if(CopyBuffer(ind_handle,1,0,amount,down_arrows) < 0)
			{
				//--- if the copying fails, tell the error code
					PrintFormat("Failed to copy data from the iFractals indicator to the FractalDownBuffer array, error code %d",GetLastError());
				//--- quit with zero result - it means that the indicator is considered as not calculated
					return(false);
			}
		//--- everything is fine
		return(true);
	}
//+------------------------------------------------------------------+
//| Indicator deinitialization function                              |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
	{
		//--- clear the chart after deleting the indicator
		Comment("");
	}
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                             MultiSignals_PCH.mq5 |
//+------------------------------------------------------------------+
#property copyright   "Copyright 2012, tol64."
#property link        "https://login.mql5.com/ru/users/tol64"
#property description "email: hello.tol64@gmail.com"
#property version     "1.00"
//#property icon        "\\Images\\logoMultiSignals_PCH_64x64.ico"

// Properties
#property indicator_chart_window
//---
#property indicator_buffers 5
#property indicator_plots   5

// LEVELS
#property indicator_type1   DRAW_LINE
#property indicator_type2   DRAW_LINE
#property indicator_type3   DRAW_LINE
#property indicator_type4   DRAW_LINE
#property indicator_type5   DRAW_LINE

// STYLES
#property indicator_style1 STYLE_SOLID
#property indicator_style2 STYLE_SOLID
#property indicator_style3 STYLE_DOT
#property indicator_style4 STYLE_DOT
#property indicator_style5 STYLE_DOT

// COLORS MAIN LEVELS
#property indicator_color1 clrGold        // H
#property indicator_color2 clrGreenYellow // L
#property indicator_color3 clrSilver
#property indicator_color4 clrSilver
#property indicator_color5 clrSilver

//---------------------------------------------------------------
#property indicator_label1  "Channel upper"
#property indicator_label2  "Channel lower"
#property indicator_label3  "Channel median"
#property indicator_label4  "Channel UPmedian"
#property indicator_label5  "Channel DWmedian"
//---
#define szArr 5

//--- input parameters
input int  Period_PCH=10; // Period
//---
int j=0;
//---
double High_Buffer[],Low_Buffer[],Mid_Buffer[],High_Mid_Buffer[],Low_Mid_Buffer[]; // Buffers of Lines
//---
int ArrayPlotShift[szArr],ArrayPlotDrawBegin[szArr],ArrayPlotEmptyValue[szArr],ArrayPlotLineWidth[szArr];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
void OnInit()
  {
//--- indicator buffers mapping
   SetIndexBuffer(0,High_Buffer,INDICATOR_DATA);
   SetIndexBuffer(1,Low_Buffer,INDICATOR_DATA);
   SetIndexBuffer(2,Mid_Buffer,INDICATOR_DATA);
   SetIndexBuffer(3,High_Mid_Buffer,INDICATOR_DATA);
   SetIndexBuffer(4,Low_Mid_Buffer,INDICATOR_DATA);
   
//---

// Accuracy of values
   IndicatorSetInteger(INDICATOR_DIGITS,_Digits-1);

// Starting point for drawing
   for(j=0; j<szArr; j++) { PlotIndexSetInteger(j,PLOT_DRAW_BEGIN,Period_PCH); }

// Shifting lines along the X-axis
   for(j=0; j<szArr; j++) { PlotIndexSetInteger(j,PLOT_SHIFT,1); }

// Line thickness
   for(j=0; j<szArr; j++) { PlotIndexSetInteger(j,PLOT_LINE_WIDTH,1); }
//---
//--- name for DataWindow and indicator label
   IndicatorSetString(INDICATOR_SHORTNAME,"MultiRanges_PCH ("+string(Period_PCH)+")");
//---
   PlotIndexSetString(0,PLOT_LABEL,"H_PCH (0)");
   PlotIndexSetString(1,PLOT_LABEL,"L_PCH (1)");
   PlotIndexSetString(2,PLOT_LABEL,"M_PCH (2)");
   PlotIndexSetString(3,PLOT_LABEL,"MH_PCH (3)");
   PlotIndexSetString(4,PLOT_LABEL,"ML_PCH (4)");
//---
// Clear dots with empty values
   for(j=0; j<szArr; j++) { PlotIndexSetDouble(j,PLOT_EMPTY_VALUE,EMPTY_VALUE); }
  }
//+------------------------------------------------------------------+
//| Price Channel                                                    |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &Time[],
                const double &Open[],
                const double &High[],
                const double &Low[],
                const double &Close[],
                const long &TickVolume[],
                const long &Volume[],
                const int &Spread[])
  {
   int i=0,limit=0;

//--- check for rates
   if(rates_total<Period_PCH) { return(0); }

//--- preliminary calculations
   if(prev_calculated==0) { limit=Period_PCH; } else { limit=prev_calculated-1; }

//--- the main loop of calculations
   for(i=limit; i<rates_total; i++)
     {
      //+------------------------------------------------------------------+
      //| INDICATORS_BUFFERS                                               |
      //+------------------------------------------------------------------+
      High_Buffer[i]=Highest(High,Period_PCH,i);
      Low_Buffer[i]=Lowest(Low,Period_PCH,i);
      Mid_Buffer[i]=(High_Buffer[i]+Low_Buffer[i])/2.0;
      //---
      High_Mid_Buffer[i]=(Mid_Buffer[i]+High_Buffer[i])/2.0;
      Low_Mid_Buffer[i]=(Low_Buffer[i]+Mid_Buffer[i])/2.0;
     }
//--- OnCalculate done. Return new prev_calculated.
   return(rates_total);
  }
//+------------------------------------------------------------------+
//| get highest value for range                                      |
//+------------------------------------------------------------------+
double Highest(const double &array[],int range,int fromIndex)
  {
   int i=0;
   double res;
//---
   res=array[fromIndex];
//---
   for(i=fromIndex; i>fromIndex-range && i>=0; i--)
     {
      if(res<array[i]) res=array[i];
     }
//---
   return(res);
  }
//+------------------------------------------------------------------+
//| get lowest value for range                                       |
//+------------------------------------------------------------------+
double Lowest(const double &array[],int range,int fromIndex)
  {
   int i=0;
   double res;
//---
   res=array[fromIndex];
//---
   for(i=fromIndex;i>fromIndex-range && i>=0;i--)
     {
      if(res>array[i]) res=array[i];
     }
//---
   return(res);
  }
//+------------------------------------------------------------------+
//| ADJUSTING THE VALUE BASED ON THE NUMBER OF DIGITS IN THE PRICE   |
//| MULTIPLICATION                                                   |
//+------------------------------------------------------------------+
//| [] - int value                                                   |
//+------------------------------------------------------------------+
int vDgtMlt(int value)
  {
   if(_Digits==3 || _Digits==5) { return(value*=10); } else { return(value); }
  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                              X-bars_Fractals.mq5 |
//|                                            Copyright 2011, Rone. |
//|                                            rone.sergey@gmail.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2011, Rone."
#property link      "rone.sergey@gmail.com"
#property version   "1.00"
#property description "The indicator allows to set the number of bars on the left and on the right sides of the fractal separately. Is suitable for "
#property description "both local and global extremums."
//--- indicator buffers
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
//--- plot UpFractals
#property indicator_label1  "Up Fractals"
#property indicator_type1   DRAW_ARROW
#property indicator_color1  clrDodgerBlue
#property indicator_style1  STYLE_SOLID
#property indicator_width1  1
//--- plot DnFractals
#property indicator_label2  "Down Fractals"
#property indicator_type2   DRAW_ARROW
#property indicator_color2  clrTomato
#property indicator_style2  STYLE_SOLID
#property indicator_width2  1
//--- input parameters
input int      InpLeftSide = 3;          // Number of bars to the left of the fractal
input int      InpRightSide = 3;         // Number of bars to the right of the fractal
//--- indicator buffers
double         UpFractalsBuffer[];
double         DnFractalsBuffer[];
//--- global variables
int            minRequiredBars;
int            leftSide, rightSide;
int            maxSide;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit() {
//---
   if ( InpLeftSide < 1 ) {
      leftSide = 2;
      printf("Invalid parameter value \"Number of bars to the left of the fractal\": %d. The value used: %d.",
         InpLeftSide, leftSide);
   } else {
      leftSide = InpLeftSide;
   }
   if ( InpRightSide < 1 ) {
      rightSide = 2;
      printf("Invalid parameter value \"Number of bars to the right of the fractal\": %d. The value used: %d.",
         InpRightSide, rightSide);
   } else {
      rightSide = InpRightSide;
   }
//---
   minRequiredBars = leftSide + rightSide + 1;
   maxSide = int(MathMax(leftSide, rightSide));
//---
   SetIndexBuffer(0, UpFractalsBuffer, INDICATOR_DATA);
   SetIndexBuffer(1, DnFractalsBuffer, INDICATOR_DATA);
//---
   PlotIndexSetInteger(0, PLOT_ARROW, 217);
   PlotIndexSetInteger(1, PLOT_ARROW, 218);
//---
   PlotIndexSetInteger(0, PLOT_ARROW_SHIFT, -10);  
   PlotIndexSetInteger(1, PLOT_ARROW_SHIFT, 10);
//---
   for ( int i = 0; i < 2; i++ ) {
      PlotIndexSetInteger(i, PLOT_DRAW_BEGIN, minRequiredBars);
      PlotIndexSetDouble(i, PLOT_EMPTY_VALUE, 0.0);
   }
//---
   IndicatorSetInteger(INDICATOR_DIGITS, _Digits);
//---
   IndicatorSetString(INDICATOR_SHORTNAME, "X-bars Fractals ("+(string)leftSide+", "+(string)rightSide+")");
//---
   return(0);
}
//+------------------------------------------------------------------+
//| Check if is Up Fractal function                                  |
//+------------------------------------------------------------------+
bool isUpFractal(int bar, int max, const double &High[]) {
//---
   for ( int i = 1; i <= max; i++ ) {
      if ( i <= leftSide && High[bar] < High[bar-i] ) {
         return(false);
      }
      if ( i <= rightSide && High[bar] <= High[bar+i] ) {
         return(false);
      }
   }
//---
   return(true);  
}
//+------------------------------------------------------------------+
//| Check if is Down Fractal function                                |
//+------------------------------------------------------------------+
bool isDnFractal(int bar, int max, const double &Low[]) {
//---
   for ( int i = 1; i <= max; i++ ) {
      if ( i <= leftSide && Low[bar] > Low[bar-i] ) {
         return(false);
      }
      if ( i <= rightSide && Low[bar] >= Low[bar+i] ) {
         return(false);
      }
   }
//---
   return(true);  
}
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
{
//---
   int startBar, lastBar;
//---
   if ( rates_total < minRequiredBars ) {
      Print("Not enough data for calculation");
      return(0);
   }
//---
   if (prev_calculated < minRequiredBars) {
      startBar = leftSide;
      ArrayInitialize(UpFractalsBuffer, 0.0);
      ArrayInitialize(DnFractalsBuffer, 0.0);
   }        
   else {
      startBar = rates_total - minRequiredBars;
   }
//---
   lastBar = rates_total - rightSide;
   for ( int bar = startBar; bar < lastBar && !IsStopped(); bar++ ) {
      //---
      if ( isUpFractal(bar, maxSide, high) ) {
         UpFractalsBuffer[bar] = high[bar];
      } else {
         UpFractalsBuffer[bar] = 0.0;
      }
      //---
      if ( isDnFractal(bar, maxSide, low) ) {
         DnFractalsBuffer[bar] = low[bar];
      } else {
         DnFractalsBuffer[bar] = 0.0;
      }
   }  
//--- return value of prev_calculated for next call
   return(rates_total);
}
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                 indicator_tp.mq5 |
//+------------------------------------------------------------------+
#property copyright "Denis Zyatkevich"
#property description "This indicator calculates TakeProfit levels"
#property description "using the average market volatility. It uses the values"
#property description "of Average True Range (ATR) indicator, calculated"
#property description "on daily price data. Indicator values are calculated"
#property description "using maximal and minimal price values per day."
#property version   "1.00"
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
#property indicator_type1   DRAW_LINE
#property indicator_color1  C'127,191,127'
#property indicator_style1  STYLE_SOLID
#property indicator_label1  "Buy TP"
#property indicator_type2   DRAW_LINE
#property indicator_color2  C'191,127,127'
#property indicator_style2  STYLE_SOLID
#property indicator_label2  "Sell TP"

input int             ATRper       = 5;         //ATR Period
input ENUM_TIMEFRAMES ATRtimeframe = PERIOD_D1; //Indicator timeframe

double bu[],bd[];
int hATR;

void OnInit()
  {
   SetIndexBuffer(0,bu,INDICATOR_DATA);
   SetIndexBuffer(1,bd,INDICATOR_DATA);
   hATR=iATR(NULL,ATRtimeframe,ATRper);
  }

int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime& time[],
                const double& open[],
                const double& high[],
                const double& low[],
                const double& close[],
                const long& tick_volume[],
                const long& volume[],
                const int& spread[])

  {
   int i,day_n,day_t;
   double atr[],h_day,l_day;

   CopyBuffer(hATR,0,0,2,atr);
   ArraySetAsSeries(atr,true);

   for(i=prev_calculated;i<rates_total;i++)
     {
      day_t=time[i]/PeriodSeconds(ATRtimeframe);
      if(day_n<day_t)
        {
         day_n=day_t;
         l_day=low[i];
         h_day=high[i];
        }
        else
        {
         if(high[i]>h_day) h_day=high[i];
         if(low[i]<l_day) l_day=low[i];
        }
      bu[i]=l_day+atr[1];
      bd[i]=h_day-atr[1];
     }
   return(rates_total);
  }

void OnDeinit(const int reason)
  {
   IndicatorRelease(hATR);
  }
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                 PivotCandles.mq5 |
//|                        Copyright 2012, MetaQuotes Software Corp. |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2012, MetaQuotes Software Corp."
#property link      "http://www.mql5.com"
#property version   "1.00"
#property indicator_chart_window

// Use four buffers, while drawing two
#property indicator_buffers 4
#property indicator_plots   2
//--- plot SlowMA
#property indicator_label1  "SlowMA"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrAliceBlue
#property indicator_style1  STYLE_SOLID
#property indicator_width1  1
//--- plot ChartSignal
#property indicator_label2  "ChartSignal"
#property indicator_type2   DRAW_COLOR_ARROW
#property indicator_color2  clrLightSalmon,clrOrangeRed,clrBlack,clrSteelBlue,clrLightBlue
#property indicator_style2  STYLE_SOLID
#property indicator_width2  3

#include <PivotCandlesClass.mqh>
//+------------------------------------------------------------------+
//| Common arrays and structures                                     |
//+------------------------------------------------------------------+
//--- Indicator buffers                                                
double   SMA[];            // Values of the Moving Average
double   Signal[];         // Signal values
double   ChartSignal[];    // Location of signals on the chart
double   SignalColor[];    // Signal color array
//--- Calculation class
CPivotCandlesClass PivotCandlesClass;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- indicator buffers mapping
   SetIndexBuffer(0,SMA,INDICATOR_DATA);
   SetIndexBuffer(1,ChartSignal,INDICATOR_DATA);
   SetIndexBuffer(2,SignalColor,INDICATOR_COLOR_INDEX);
   SetIndexBuffer(3,Signal,INDICATOR_CALCULATIONS);

//--- set 0 as an empty value
   PlotIndexSetDouble(1,PLOT_EMPTY_VALUE,0);

   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
   // If there have not been calculations yet or (!) the new history is uploaded, clean up the calculation object
   if (prev_calculated == 0)
      PivotCandlesClass.CleanupHistory();
   
   int end_calc_edge = rates_total-1;   
   if (prev_calculated >= end_calc_edge)
      return end_calc_edge;
   
   for(int i=prev_calculated; i<end_calc_edge; i++)
     {
      int signal = PivotCandlesClass.AnalizeNewCandle(time[i],open[i],high[i],low[i],close[i],tick_volume[i],volume[i],spread[i]);
      Signal[i] = signal;
      SMA[i] = PivotCandlesClass.MAValue();
      
      // Signals are processed, display them on the chart
      // Set the location of our signals...
      if (signal < 0)
         ChartSignal[i]=high[i];
      else if (signal > 0)
         ChartSignal[i]=low[i];
      else
         ChartSignal[i]=0;
      // .. ...as well as their color
      // Signals have a range of [-2..2], while color indices - [0..4]. Align them 
      SignalColor[i]=signal+2;
     }
   
   // Set the Moving Average value similar to the previous one to prevent it from sharp fall
   SMA[end_calc_edge] = SMA[end_calc_edge-1];

//--- return value of prev_calculated for next call
   return(end_calc_edge);
  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                        YeildHighYeildLowLine.mq5 |
//|                                                              CJT |
//|                                     https://www.Chucknjill18.com |
//+------------------------------------------------------------------+
#property copyright "CJT"
#property link      "https://www.Chucknjill18.com"
#property version   "1.00"
#property indicator_chart_window
//+------------------------------------------------------------------+
//|  Indicator settings                                              |
//+------------------------------------------------------------------+
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots 2
#property indicator_type1 DRAW_LINE
#property indicator_type2 DRAW_LINE
#property indicator_color1 clrAquamarine
#property indicator_color2 clrLightYellow
#property indicator_label1 "up"
#property indicator_label2 "down"
#property indicator_width1 2
#property indicator_width2 2
//+------------------------------------------------------------------+
//|  Indicator input parameters                                      |
//+------------------------------------------------------------------+
input double   OffSet   =  100;              //41.95
input double   Risk     =  9.25;  

//+------------------------------------------------------------------+
//|  Array parameters                                                |
//+------------------------------------------------------------------+
double HighLine[];
double LowLine[];

//+------------------------------------------------------------------+
//|  Global parameters                                               |
//+------------------------------------------------------------------+
int      StartBars   =  0;
double   K           =  0;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
      //---- initialization of variables of the start of data calculation
   		StartBars = OffSet + 1;
   	//--- indicator buffers mapping
         SetIndexBuffer(0,LowLine,INDICATOR_DATA);
         ArraySetAsSeries(LowLine,true);
         PlotIndexSetString(0,PLOT_LABEL,"Low Line");
         PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,StartBars);
         PlotIndexSetInteger(0,PLOT_DRAW_TYPE,DRAW_LINE);
         PlotIndexSetInteger(0,PLOT_LINE_STYLE,STYLE_DASHDOTDOT);
         
   	//--- indicator buffers mapping
         SetIndexBuffer(1,HighLine,INDICATOR_DATA);
         ArraySetAsSeries(HighLine,true);
         PlotIndexSetString(1,PLOT_LABEL,"High Line");
         PlotIndexSetInteger(1,PLOT_DRAW_BEGIN,StartBars);
         PlotIndexSetInteger(1,PLOT_DRAW_TYPE,DRAW_LINE);
         PlotIndexSetInteger(1,PLOT_LINE_STYLE,STYLE_DASHDOTDOT);

   
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//---
         double dHigh,dLow;
         if (Bars(_Symbol,_Period)<rates_total) return(0);
         int i=0;
            for (i=(int)MathMax(prev_calculated -1,0); i < rates_total && !_StopFlag; i++)
            {
               MqlRates rates[]; if (CopyRates(_Symbol,_Period,time[i],2,rates)==-1) break;
                  HighLine[i] = rates[0].high;
                  LowLine[i]  = rates[0].low;
                  dHigh = rates[0].high;
                  dLow = rates[0].low;
                  Comment(StringFormat("Starting Rates\nLabelDisplay = %G\nHigh = %G\nLow = %d",dHigh,dLow,StartBars));
            }

//--- return value of prev_calculated for next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                           Support and Resistance |
//|                                      Copyright © 2010, EarnForex |
//|                                        http://www.earnforex.com/ |
//|                          Based on MT4 indicator by Barry Stander |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2010, EarnForex"
#property link      "http://www.earnforex.com"
#property version   "1.01"
#property description "Blue and red support and resistance levels displayed directly on the chart."
#property description "Based on MT4 indicator by Barry Stander."

#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
#property indicator_color1  Red
#property indicator_type1   DRAW_ARROW
#property indicator_style1  STYLE_SOLID
#property indicator_width1  2
#property indicator_color2  Blue
#property indicator_type2   DRAW_ARROW
#property indicator_style2  STYLE_SOLID
#property indicator_width2  2

//---- buffers
double Resistance[];
double Support[];

void OnInit()
{
   IndicatorSetString(INDICATOR_SHORTNAME, "Support and Resistance");
   IndicatorSetInteger(INDICATOR_DIGITS, _Digits);

   PlotIndexSetInteger(0, PLOT_ARROW, 218);
   PlotIndexSetInteger(1, PLOT_ARROW, 217);

   SetIndexBuffer(0, Resistance);
   PlotIndexSetString(0, PLOT_LABEL, "Resistance");

   SetIndexBuffer(1, Support);
   PlotIndexSetString(1, PLOT_LABEL, "Support");

   ArraySetAsSeries(Resistance, true);
   ArraySetAsSeries(Support, true);
}

//+------------------------------------------------------------------+
//| Custom Support and Resistance                                    |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &High[],
                const double &Low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
{
   ArraySetAsSeries(High, true);
   ArraySetAsSeries(Low, true);

   //Get the values of the Fractals indicator before entering the cycle
   double FractalUpperBuffer[];
   double FractalLowerBuffer[];
   int myFractal = iFractals(NULL, 0);
   CopyBuffer(myFractal, 0, 0, rates_total, FractalUpperBuffer);
   CopyBuffer(myFractal, 1, 0, rates_total, FractalLowerBuffer);
	ArraySetAsSeries(FractalUpperBuffer, true);
	ArraySetAsSeries(FractalLowerBuffer, true);
	
   for (int i = rates_total - 2; i >= 0; i--)
   {
		if (FractalUpperBuffer[i] != EMPTY_VALUE) Resistance[i] = High[i];
    	else Resistance[i] = Resistance[i + 1];
  
 		if (FractalLowerBuffer[i] != EMPTY_VALUE) Support[i] = Low[i];
      else Support[i] = Support[i + 1];
      
      double Maximum = MathMax(High[i],SYMBOL_VOLUME_MAX);
      double Minimum = MathMin(Low[i],SYMBOL_VOLUME_MIN);
      Comment(StringFormat("High and Low\nMaximum = %G\nMinimum = %d",Maximum,Minimum));
   }   
   
   
	return(rates_total);
}

//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                       BB_OutsideCandle_Alert.mq5 |
//|                        Copyright 2018, MetaQuotes Software Corp. |
//|                                                 https://mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2018, MetaQuotes Software Corp."
#property link      "https://mql5.com"
#property version   "1.00"
#property description "Bollinger Bands Outside Candle Alert indicator"
#property description "Candle opening and closing outside the bands for buy/sell signals"
#property description "Optional Inside/Out Mode for open inside and close outside"
#property indicator_chart_window
#property indicator_buffers 10
#property indicator_plots   5
//--- plot SigB
#property indicator_label1  "Long Signal"
#property indicator_type1   DRAW_ARROW
#property indicator_color1  clrBlue
#property indicator_style1  STYLE_SOLID
#property indicator_width1  1
//--- plot SigS
#property indicator_label2  "Short Signal"
#property indicator_type2   DRAW_ARROW
#property indicator_color2  clrRed
#property indicator_style2  STYLE_SOLID
#property indicator_width2  1
//--- plot BandUP
#property indicator_label3  "Upper Band"
#property indicator_type3   DRAW_LINE
#property indicator_color3  clrChocolate
#property indicator_style3  STYLE_SOLID
#property indicator_width3  1
//--- plot BandDN
#property indicator_label4  "Lower Band"
#property indicator_type4   DRAW_LINE
#property indicator_color4  clrForestGreen
#property indicator_style4  STYLE_SOLID
#property indicator_width4  1
//--- plot Signal Candles
#property indicator_label5  "Signal Candle"
#property indicator_type5   DRAW_COLOR_CANDLES
#property indicator_color5  clrMediumSeaGreen,clrDarkOrange,clrDarkGray
#property indicator_style5  STYLE_SOLID
#property indicator_width5  1
//--- enums
enum ENUM_INPUT_YES_NO
  {
   INPUT_YES   =  1, // Yes
   INPUT_NO    =  0  // No
  };
//---
enum ENUM_MODE_OP_CL
  {
   MODE_CANDLE_INSIDE_OUT, // Opening inside, then closing outside 
   MODE_CANDLE_OUTSIDE     // Opening and closing outside
  };
//--- input parameters
input uint              InpPeriodBB    =  12;                  // BB period
input double            InpDeviation   =  2.2;                 // BB deviation
input ENUM_MODE_OP_CL   InpModeCandle  =  MODE_CANDLE_OUTSIDE; // BB line breakdown method
input ENUM_INPUT_YES_NO InpShowBands   =  INPUT_YES;           // Show bands
input ENUM_INPUT_YES_NO InpShowCandles =  INPUT_YES;           // Show signal candles
input ENUM_INPUT_YES_NO InpShowAlerts  =  INPUT_YES;           // Use alerts
//--- indicator buffers
double         BufferSigB[];
double         BufferSigS[];
double         BufferBandUP[];
double         BufferBandDN[];
double         BufferCandleO[];
double         BufferCandleH[];
double         BufferCandleL[];
double         BufferCandleC[];
double         BufferColors[];
double         BufferBB[];
//--- global variables
double         deviation;
int            period_bb;
int            handle_bb;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- set global variables
   period_bb=int(InpPeriodBB<1 ? 1 : InpPeriodBB);
   deviation=InpDeviation;
//--- indicator buffers mapping
   SetIndexBuffer(0,BufferSigB,INDICATOR_DATA);
   SetIndexBuffer(1,BufferSigS,INDICATOR_DATA);
   SetIndexBuffer(2,BufferBandUP,INDICATOR_DATA);
   SetIndexBuffer(3,BufferBandDN,INDICATOR_DATA);
   SetIndexBuffer(4,BufferCandleO,INDICATOR_DATA);
   SetIndexBuffer(5,BufferCandleH,INDICATOR_DATA);
   SetIndexBuffer(6,BufferCandleL,INDICATOR_DATA);
   SetIndexBuffer(7,BufferCandleC,INDICATOR_DATA);
   SetIndexBuffer(8,BufferColors,INDICATOR_COLOR_INDEX);
   SetIndexBuffer(9,BufferBB,INDICATOR_CALCULATIONS);
//--- setting a code from the Wingdings charset as the property of PLOT_ARROW
   PlotIndexSetInteger(0,PLOT_ARROW,233);
   PlotIndexSetInteger(1,PLOT_ARROW,234);
//--- setting indicator parameters
   IndicatorSetString(INDICATOR_SHORTNAME,"BB Outside Candle Alert+("+(string)period_bb+","+DoubleToString(deviation,1)+")");
   IndicatorSetInteger(INDICATOR_DIGITS,Digits());
//--- setting plot buffer parameters
   PlotIndexSetInteger(2,PLOT_DRAW_TYPE,InpShowBands);
   PlotIndexSetInteger(3,PLOT_DRAW_TYPE,InpShowBands);
//--- setting buffer arrays as timeseries
   ArraySetAsSeries(BufferSigB,true);
   ArraySetAsSeries(BufferSigS,true);
   ArraySetAsSeries(BufferBandUP,true);
   ArraySetAsSeries(BufferBandDN,true);
   ArraySetAsSeries(BufferCandleO,true);
   ArraySetAsSeries(BufferCandleH,true);
   ArraySetAsSeries(BufferCandleL,true);
   ArraySetAsSeries(BufferCandleC,true);
   ArraySetAsSeries(BufferColors,true);
   ArraySetAsSeries(BufferBB,true);
//--- create handles
   ResetLastError();
   handle_bb=iBands(NULL,PERIOD_CURRENT,period_bb,0,deviation,PRICE_CLOSE);
   if(handle_bb==INVALID_HANDLE)
     {
      Print("The iBands (",(string)period_bb+","+(string)deviation,") object was not created: Error ",GetLastError());
      return INIT_FAILED;
     }
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//--- Установка массивов буферов как таймсерий
   ArraySetAsSeries(open,true);
   ArraySetAsSeries(high,true);
   ArraySetAsSeries(low,true);
   ArraySetAsSeries(close,true);
   ArraySetAsSeries(time,true);
//--- Проверка количества доступных баров
   if(rates_total<4) return 0;
//--- Проверка и расчёт количества просчитываемых баров
   int limit=rates_total-prev_calculated;
   if(limit>1)
     {
      limit=rates_total-2;
      ArrayInitialize(BufferSigB,EMPTY_VALUE);
      ArrayInitialize(BufferSigS,EMPTY_VALUE);
      ArrayInitialize(BufferBandUP,EMPTY_VALUE);
      ArrayInitialize(BufferBandDN,EMPTY_VALUE);
      ArrayInitialize(BufferCandleO,EMPTY_VALUE);
      ArrayInitialize(BufferCandleH,EMPTY_VALUE);
      ArrayInitialize(BufferCandleL,EMPTY_VALUE);
      ArrayInitialize(BufferCandleC,EMPTY_VALUE);
      ArrayInitialize(BufferColors,2);
      ArrayInitialize(BufferBB,0);
     }
//--- Подготовка данных
   int count=(limit>1 ? rates_total : 1),copied=0;
   copied=CopyBuffer(handle_bb,UPPER_BAND,0,count,BufferBandUP);
   if(copied!=count) return 0;
   copied=CopyBuffer(handle_bb,LOWER_BAND,0,count,BufferBandDN);
   if(copied!=count) return 0;

//--- Расчёт индикатора
   static datetime last_time=0;
   string alert="";
   for(int i=limit; i>=0 && !IsStopped(); i--)
     {
      BufferCandleO[i]=BufferCandleH[i]=BufferCandleL[i]=BufferCandleC[i]=EMPTY_VALUE;
      if(InpModeCandle==MODE_CANDLE_INSIDE_OUT)
        {
         //--- Bearish. Candle opening inside the bands and closing above the upper bollinger band
         if(open[i+1]<BufferBandUP[i+1] && close[i+1]>BufferBandUP[i+1])
           {
            BufferSigS[i]=high[i];//fmax(open[i+1],fmax(open[i],BufferBandUP[i]));
            if(InpShowCandles)
              DrawColorCandle(i+1,open,high,low,close);
            //--- Alert after the last Close of a candle
            if(i==0 && InpShowAlerts && time[0]>last_time)
              {
               Alert(Symbol()+" "+TimeframeToString(Period())+": Bollinger Bands Outside Candle SHORT Signal");
               last_time=TimeCurrent();
              }
           }
         //--- Bullish. Candle opening inside the bands and closing below the lower bollinger band
         if(open[i+1]>BufferBandDN[i+1] && close[i+1]<BufferBandDN[i+1])
           {
            BufferSigB[i]=low[i];//fmin(open[i+1],fmin(open[i],BufferBandDN[i]));
            if(InpShowCandles)
              DrawColorCandle(i+1,open,high,low,close);
            //--- Alert after the last Close of a candle
            if(i==0 && InpShowAlerts && time[0]>last_time)
              {
               Alert(Symbol()+" "+TimeframeToString(Period())+": Bollinger Bands Outside Candle LONG Signal");
               last_time=TimeCurrent();
              }
           }
        }
      else
        {
         //--- Bearish. Candle opening and closing above the upper bollinger band
         if(open[i+1]>close[i+1] && close[i+1]>BufferBandUP[i+1])
           {
            BufferSigS[i]=high[i];//fmax(open[i+1],fmax(open[i],BufferBandUP[i]));
            if(InpShowCandles)
              DrawColorCandle(i+1,open,high,low,close);
            //--- Alert after the last Close of a candle
            if(i==0 && InpShowAlerts && time[0]>last_time)
              {
               Alert(Symbol()+" "+TimeframeToString(Period())+": Bollinger Bands Outside Candle SHORT Signal");
               last_time=TimeCurrent();
              }
           }
         //--- Bullish. Candle opening and closing below the lower bollinger band
         if(open[i+1]<close[i+1] && close[i+1]<BufferBandDN[i+1])
           {
            BufferSigB[i]=low[i];//fmin(open[i+1],fmin(open[i],BufferBandDN[i]));
            if(InpShowCandles)
              DrawColorCandle(i+1,open,high,low,close);
            //--- Alert after the last Close of a candle
            if(i==0 && InpShowAlerts && time[0]>last_time)
              {
               Alert(Symbol()+" "+TimeframeToString(Period())+": Bollinger Bands Outside Candle LONG Signal");
               last_time=TimeCurrent();
              }
           }
        }
     }

//--- return value of prev_calculated for next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
//| Timeframe to string                                              |
//+------------------------------------------------------------------+
string TimeframeToString(const ENUM_TIMEFRAMES timeframe)
  { 
   return StringSubstr(EnumToString(timeframe),7);
  }
//+------------------------------------------------------------------+
//| Draw Color Candle                                                |
//+------------------------------------------------------------------+
void DrawColorCandle(const int shift,const double &open[],const double &high[],const double &low[],const double &close[])
  {
   BufferCandleO[shift]=open[shift];
   BufferCandleH[shift]=high[shift];
   BufferCandleL[shift]=low[shift];
   BufferCandleC[shift]=close[shift];
   BufferColors[shift]=(open[shift]<close[shift] ? 0 : open[shift]>close[shift] ? 1 : 2);
  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
//+------------------------------------------------------------------+
//|                                                 OHLC Channel.mq5 |
//|                              Copyright © 2018, Vladimir Karputov |
//|                                           http://wmua.ru/slesar/ |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2018, Vladimir Karputov"
#property link      "http://wmua.ru/slesar/"
#property version   "1.000"
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_plots   2
//--- plot High
#property indicator_label1  "Up"
#property indicator_type1   DRAW_LINE
#property indicator_color1  clrChartreuse
#property indicator_style1  STYLE_SOLID
#property indicator_width1  2
//--- plot Low
#property indicator_label2  "Down"
#property indicator_type2   DRAW_LINE
#property indicator_color2  clrDeepPink
#property indicator_style2  STYLE_SOLID
#property indicator_width2  2
//---
enum ENUM_PRICE
  {
   open_price     = 0,  // Open
   high_price     = 1,  // High
   low_price      = 2,  // Low
   close_price    = 3,  // Close
  };
//--- input parameters
input ENUM_PRICE  InpUp    = high_price;
input ENUM_PRICE  InpDown  = low_price;
//--- indicator buffers
double            HighBuffer[];
double            LowBuffer[];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- indicator buffers mapping
   SetIndexBuffer(0,HighBuffer,INDICATOR_DATA);
   SetIndexBuffer(1,LowBuffer,INDICATOR_DATA);
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
//--- by default in the MQL5 indicator on the chart: 
//    left bar                | right bar
//    D'1980.07.19 12:30:27'  | D'2018.07.19 12:30:27'
   int limit=prev_calculated-1;
   if(prev_calculated==0)
      limit=0;
   for(int i=limit;i<rates_total;i++)
     {
      switch(InpUp)
        {
         case open_price:
            HighBuffer[i]=open[i];
            break;
         case high_price:
            HighBuffer[i]=high[i];
            break;
         case low_price:
            HighBuffer[i]=low[i];
            break;
         case close_price:
            HighBuffer[i]=close[i];
            break;
        }
      switch(InpDown)
        {
         case open_price:
            LowBuffer[i]=open[i];
            break;
         case high_price:
            LowBuffer[i]=high[i];
            break;
         case low_price:
            LowBuffer[i]=low[i];
            break;
         case close_price:
            LowBuffer[i]=close[i];
            break;
        }
     }
//--- return value of prev_calculated for next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
//#####################################################################################################################
//#####################################################################################################################
