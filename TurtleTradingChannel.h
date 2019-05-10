//+------------------------------------------------------------------+
//|                    Converted by: MqlConvert.com                  |
//|                       Site of CyberFX.org                        |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//| TheTurtleTradingChannel.mq4
//| Copyright © Pointzero-indicator.com
//+------------------------------------------------------------------+
#property copyright "Copyright © Pointzero-indicator.com"
#property link      "http://www.pointzero-indicator.com"

//---- indicator settings
//---- drawing the indicator in the main window
#property indicator_chart_window 
//---- two buffers are used for calculation and drawing the indicator
#property indicator_buffers 6
//---- only two plots are used
#property indicator_plots   6
//+----------------------------------------------+
//|  Indicator 1 drawing parameters              |
//+----------------------------------------------+
//---- drawing the indicator 1 as a symbol
#property indicator_type1   STYLE_DOT
//---- red color is used for the indicator bearish line
#property indicator_color1  DodgerBlue
//---- indicator 1 line width is equal to 4
#property indicator_width1  3
//---- displaying the bearish label of the indicator line
#property indicator_label1  "Silver Sell"

//+----------------------------------------------+
//|  Indicator 2 drawing parameters              |
//+----------------------------------------------+
//---- drawing the indicator 2 as a line
#property indicator_type2   STYLE_DOT
//---- lime color is used as the color of the bullish indicator line
#property indicator_color2  Red
//---- indicator 2 line width is equal to 4
#property indicator_width2  3
//---- displaying the bullish label of the indicator line
#property indicator_label2 "Silver Buy"

//+----------------------------------------------+
//|  Indicator 3 drawing parameters              |
//+----------------------------------------------+
//---- drawing the indicator 1 as a symbol
#property indicator_type3   STYLE_DOT
//---- red color is used for the indicator bearish line
#property indicator_color3  DarkSlateGray
//---- indicator 1 line width is equal to 4
#property indicator_width3  1
//---- displaying the bearish label of the indicator line
#property indicator_label3  "Silver Sell"

//+----------------------------------------------+
//|  Indicator 4 drawing parameters              |
//+----------------------------------------------+
//---- drawing the indicator 2 as a line
#property indicator_type4   STYLE_DOT
//---- lime color is used as the color of the bullish indicator line
#property indicator_color4  DarkSlateGray
//---- indicator 2 line width is equal to 4
#property indicator_width4  1
//---- displaying the bullish label of the indicator line
#property indicator_label4 "Silver Buy"

//+----------------------------------------------+
//|  Indicator 5 drawing parameters              |
//+----------------------------------------------+
//---- drawing the indicator 1 as a symbol
#property indicator_type5   STYLE_DOT
//---- red color is used for the indicator bearish line
#property indicator_color5  DodgerBlue
//---- indicator 1 line width is equal to 4
#property indicator_width5  1
//---- displaying the bearish label of the indicator line
#property indicator_label5  "Silver Sell"

//+----------------------------------------------+
//|  Indicator 6 drawing parameters              |
//+----------------------------------------------+
//---- drawing the indicator 2 as a line
#property indicator_type6   STYLE_DOT
//---- lime color is used as the color of the bullish indicator line
#property indicator_color6  Red
//---- indicator 2 line width is equal to 4
#property indicator_width6  1
//---- displaying the bullish label of the indicator line
#property indicator_label6 "Silver Buy"

//---- indicator parameters
input int  TradePeriod         = 20;     // Donchian channel period for trading signals
input int  StopPeriod          = 10;     // Donchian channel period for exit signals
input bool Strict              = false;  // Apply strict entry parameters like the Turtles did
input bool DisplayAlerts       = false;  // You know...

double Ask;
double Bid;
double Bars;
double Open[];
double Close[];
double High[];
double Low[];
datetime Time[];
long Volume[];

//---- indicator buffers
double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double ExtMapBuffer5[];
double ExtMapBuffer6[];
double TrendDirection[];

//---- internal
static datetime TimeStamp;
static int AlertCount = 1;

ENUM_TIMEFRAMES TFMigrate(int tf)
	{
		switch(tf)
			{
				case 0: return(PERIOD_CURRENT);
				case 1: return(PERIOD_M1);
				case 5: return(PERIOD_M5);
				case 15: return(PERIOD_M15);
				case 30: return(PERIOD_M30);
				case 60: return(PERIOD_H1);
				case 240: return(PERIOD_H4);
				case 1440: return(PERIOD_D1);
				case 10080: return(PERIOD_W1);
				case 43200: return(PERIOD_MN1);
      
				case 2: return(PERIOD_M2);
				case 3: return(PERIOD_M3);
				case 4: return(PERIOD_M4);      
				case 6: return(PERIOD_M6);
				case 10: return(PERIOD_M10);
				case 12: return(PERIOD_M12);
				case 16385: return(PERIOD_H1);
				case 16386: return(PERIOD_H2);
				case 16387: return(PERIOD_H3);
				case 16388: return(PERIOD_H4);
				case 16390: return(PERIOD_H6);
				case 16392: return(PERIOD_H8);
				case 16396: return(PERIOD_H12);
				case 16408: return(PERIOD_D1);
				case 32769: return(PERIOD_W1);
				case 49153: return(PERIOD_MN1);      

				default: return(PERIOD_CURRENT);
			}
	}
 int IndicatorCounted()
	{
		if(prev_calculated>0) return(prev_calculated-1);
		if(prev_calculated==0) return(0);
		return(0);
	}
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
void OnInit()
   {
		// One more invisible buffer to store trend direction
			//IndicatorBuffers(7);
   
		// Drawing settings
			PlotIndexSetInteger(0,PLOT_DRAW_TYPE,DRAW_LINE);
			PlotIndexSetInteger(1,PLOT_DRAW_TYPE,DRAW_LINE);
			PlotIndexSetInteger(2,PLOT_DRAW_TYPE,DRAW_LINE);
			PlotIndexSetInteger(3,PLOT_DRAW_TYPE,DRAW_LINE);
			PlotIndexSetInteger(4,PLOT_ARROW,159);
			PlotIndexSetInteger(5,PLOT_ARROW,159);

		
		// Name and labels
			PlotIndexSetString(1,PLOT_LABEL,"Silver Buy");
			PlotIndexSetString(0,PLOT_LABEL,"Upper line");
			PlotIndexSetString(1,PLOT_LABEL,"Lower line");
			PlotIndexSetString(2,PLOT_LABEL,"Longs Stop line");
			PlotIndexSetString(3,PLOT_LABEL,"Shorts Stop line");
			PlotIndexSetString(4,PLOT_LABEL,"Bullish trend change");
			PlotIndexSetString(5,PLOT_LABEL,"Bearish trend change");
			string short_name="SilverTrend_Signal";
			IndicatorSetString(INDICATOR_SHORTNAME,short_name);
		

		// Buffers
			SetIndexBuffer(0,ExtMapBuffer1,INDICATOR_DATA);
			SetIndexBuffer(1,ExtMapBuffer2,INDICATOR_DATA);
			SetIndexBuffer(2,ExtMapBuffer3,INDICATOR_DATA);
			SetIndexBuffer(3,ExtMapBuffer4,INDICATOR_DATA);
			SetIndexBuffer(4,ExtMapBuffer5,INDICATOR_DATA);
			SetIndexBuffer(5,ExtMapBuffer6,INDICATOR_DATA);
			SetIndexBuffer(6,TrendDirection,INDICATOR_DATA);
			
		//---- indexing the elements in the buffer as timeseries
			ArraySetAsSeries(ExtMapBuffer1,true);
			ArraySetAsSeries(ExtMapBuffer2,true);
			ArraySetAsSeries(ExtMapBuffer3,true);
			ArraySetAsSeries(ExtMapBuffer4,true);
			ArraySetAsSeries(ExtMapBuffer5,true);
			ArraySetAsSeries(ExtMapBuffer6,true);
			ArraySetAsSeries(TrendDirection,true);
			
		Set_Values_to_variables();
		//return(0);
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
	Set_Values_to_variables();
	Turtle();
	return(rates_total);
   }


//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int Turtle()
	{
		// More vars here too...
			int start = 0;
			int limit;
			int counted_bars = IndicatorCounted();

		// check for possible errors
			if(counted_bars < 0) 
				return(-1);
        
		// Only check these
			limit = Bars - 1 - counted_bars;
			if(counted_bars==0) limit-=1+1;
     
		// Check the signal foreach bar
			for(int i = limit; i >= start; i--)
				{           
					// Highs and lows
						double rhigh = iHigh(_Symbol,Period(),iHighest(_Symbol, Period(), 2, TradePeriod,i+1));
						double rlow  = iLow(_Symbol,Period(),iLowest(_Symbol, Period(), 1, TradePeriod, i+1));
						double shigh = iHigh(_Symbol,Period(),iHighest(_Symbol, Period(), 2, StopPeriod,i+1));
						double slow  = iLow(_Symbol,Period(),iLowest(_Symbol, Period(), 1, StopPeriod, i+1));
         
					// Candle value
						double CLOSE = iClose(_Symbol,0, i);
						double HIGH = iHigh(_Symbol,0, i);
						double LOW = iLow(_Symbol,0, i);
         
					// Default behavior is to preserve the trend
						TrendDirection[i] = TrendDirection[i+1];
         
					// It might be recalculating bar zero
						ExtMapBuffer1[i] = EMPTY_VALUE;
						ExtMapBuffer2[i] = EMPTY_VALUE;
						ExtMapBuffer3[i] = EMPTY_VALUE;
						ExtMapBuffer4[i] = EMPTY_VALUE;
						ExtMapBuffer5[i] = EMPTY_VALUE;
						ExtMapBuffer6[i] = EMPTY_VALUE;
         
					// Change to uptrend
						if(((CLOSE > rhigh && i > 0) || (HIGH > rhigh && Strict == true)) && TrendDirection[i+1] != 0)
							{
								TrendDirection[i] = 0;
								ExtMapBuffer5[i] = rlow;
								// Change to downtrend
							} 
						else if(((CLOSE < rlow && i > 0) || (LOW < rlow && Strict == true)) && TrendDirection[i+1] != 1) 
							{
								TrendDirection[i] = 1;
								ExtMapBuffer6[i] = rhigh;
							}
					// Draw lines
						if(TrendDirection[i] == 0)
							{
								ExtMapBuffer1[i] = rlow;
								ExtMapBuffer3[i] = slow;
								// Draw lines
							} 
						else if(TrendDirection[i] == 1) 
							{
								ExtMapBuffer2[i] = rhigh;
								ExtMapBuffer4[i] = shigh;
							}
				}
     
		// Alert
			if(TimeStamp != Time[0] && DisplayAlerts == true)
				{
					if(TrendDirection[1] == 1 && TrendDirection[2] == 0 && AlertCount == 0)
						{
							Alert("[Turtle Trading "+ TradePeriod +"-"+ StopPeriod +"]["+ _Symbol +"] SELL");
						} 
					else if (TrendDirection[1] == 0 && TrendDirection[2] == 1 && AlertCount == 0) 
						{
							Alert("[Turtle Trading "+ TradePeriod +"-"+ StopPeriod +"]["+ _Symbol +"] BUY");
						}
					TimeStamp = Time[0];
					AlertCount = 0;
				}
    
		// Bye Bye
		return(0);
	}
//********************************************  Emulation *******************************************
void Set_Values_to_variables()
	{
		MqlTick last_tick;
		SymbolInfoTick(_Symbol,last_tick);
		Ask = last_tick.ask;
		Bid = last_tick.bid;
  
		ArraySetAsSeries(Close,true);
		CopyClose(_Symbol,_Period,0,Bars(_Symbol,_Period),Close);
		ArraySetAsSeries(Open,true);
		CopyOpen(_Symbol,_Period,0,Bars(_Symbol,_Period),Open);
		ArraySetAsSeries(Low,true);
		CopyLow(_Symbol,_Period,0,Bars(_Symbol,_Period),Low);
		ArraySetAsSeries(High,true);
		CopyHigh(_Symbol,_Period,0,Bars(_Symbol,_Period),High);
		ArraySetAsSeries(Time,true);
		CopyTime(_Symbol,_Period,0,Bars(_Symbol,_Period),Time);
		ArraySetAsSeries(Volume,true);
		CopyTickVolume(_Symbol,_Period,0,Bars(_Symbol,_Period),Volume);
	} 
void OnTick()
   {
	Set_Values_to_variables();
   }

//+------------------------------------------------------------------+
//| Get Time for specified bar index                                 |
//+------------------------------------------------------------------+
datetime iTime(const int index,string symbol=NULL,ENUM_TIMEFRAMES timeframe=PERIOD_CURRENT)
	{
		if(symbol==NULL) symbol=Symbol();
		if(timeframe==0) timeframe=Period();
		datetime Time[1];
		datetime time=0;
		int copied=CopyTime(symbol,timeframe,index,1,Time);
		if(copied>0) time=Time[0];
		return(time);
	}
//+------------------------------------------------------------------+
//| Get the High for specified bar index                             |
//+------------------------------------------------------------------+
double iHigh(const int index,string symbol=NULL,ENUM_TIMEFRAMES timeframe=PERIOD_CURRENT)
	{
		if(symbol==NULL) symbol=Symbol();
		if(timeframe==0) timeframe=Period();
		double High[1];
		double high=0;
		int copied=CopyHigh(symbol,timeframe,index,1,High);
		if(copied>0) high=High[0];
		return(high);
	}
//+------------------------------------------------------------------+
//| Get Low for specified bar index                                  |
//+------------------------------------------------------------------+
double iLow(const int index,string symbol=NULL,ENUM_TIMEFRAMES timeframe=PERIOD_CURRENT)
	{
		if(symbol==NULL)symbol=Symbol();
		if(timeframe==0) timeframe=Period();
		double Low[1];
		double low=0;
		int copied=CopyLow(symbol,timeframe,index,1,Low);
		if(copied>0) low=Low[0];
		return(low);
	}
//+------------------------------------------------------------------+
//| Get Close for specified bar index                                |
//+------------------------------------------------------------------+
double iClose(const int index,string symbol=NULL,ENUM_TIMEFRAMES timeframe=PERIOD_CURRENT)
	{
		if(symbol==NULL) symbol=Symbol();
		if(timeframe==0) timeframe=Period();
		double Close[1];
		double close=0;
		int copied=CopyClose(symbol,timeframe,index,1,Close);
		if(copied>0) close=Close[0];
		return(close);
	}
//+------------------------------------------------------------------+
//| Get iHighest for specified bar index                             |
//+------------------------------------------------------------------+		
int iHighest(string symbol,
                 int tf,
                 int type,
                 int count=WHOLE_ARRAY,
                 int start=0)
	{
		if(start < 0) return(-1);
		ENUM_TIMEFRAMES timeframe = TFMigrate(tf);
		if(count <= 0) count = Bars(symbol,timeframe);
		if(type <= MODE_OPEN)
			{
				double Open[];
				ArraySetAsSeries(Open,true);
				CopyOpen(symbol,timeframe,start,count,Open);
				return(ArrayMaximum(Open,0,count)+start);
			}
		if(type==MODE_LOW)
			{
				double Low[];
				ArraySetAsSeries(Low,true);
				CopyLow(symbol,timeframe,start,count,Low);
				return(ArrayMaximum(Low,0,count)+start);
			}
		if(type==MODE_HIGH)
			{
				double High[];
				ArraySetAsSeries(High,true);
				CopyHigh(symbol,timeframe,start,count,High);
				return(ArrayMaximum(High,0,count)+start);
			}
		if(type==MODE_CLOSE)
			{
				double Close[];
				ArraySetAsSeries(Close,true);
				CopyClose(symbol,timeframe,start,count,Close);
				return(ArrayMaximum(Close,0,count)+start);
			}
		if(type==MODE_VOLUME)
			{
				long Volume[];
				ArraySetAsSeries(Volume,true);
				CopyTickVolume(symbol,timeframe,start,count,Volume);
				return(ArrayMaximum(Volume,0,count)+start);
			}
		if(type >= MODE_TIME)
			{
				datetime Time[];
				ArraySetAsSeries(Time,true);
				CopyTime(symbol,timeframe,start,count,Time);
				return(ArrayMaximum(Time,0,count)+start);
				//---
			}
		return(0);
	}	
//+------------------------------------------------------------------+
//| Get iLowest for specified bar index                              |
//+------------------------------------------------------------------+
int iLowest(string symbol,
                int tf,
                int type,
                int count=WHOLE_ARRAY,
                int start=0)
	{
		if(start < 0) return(-1);
		ENUM_TIMEFRAMES timeframe=TFMigrate(tf);
		if(count <= 0) count = Bars(symbol,timeframe);
		if(type<=MODE_OPEN)
			{
				double Open[];
				ArraySetAsSeries(Open,true);
				CopyOpen(symbol,timeframe,start,count,Open);
				return(ArrayMinimum(Open,0,count)+start);
			}
		if(type==MODE_LOW)
			{
				double Low[];
				ArraySetAsSeries(Low,true);
				CopyLow(symbol,timeframe,start,count,Low);
				return(ArrayMinimum(Low,0,count)+start);
			}
		if(type==MODE_HIGH)
			{
				double High[];
				ArraySetAsSeries(High,true);
				CopyHigh(symbol,timeframe,start,count,High);
				return(ArrayMinimum(High,0,count)+start);
			}
		if(type==MODE_CLOSE)
			{
				double Close[];
				ArraySetAsSeries(Close,true);
				CopyClose(symbol,timeframe,start,count,Close);
				return(ArrayMinimum(Close,0,count)+start);
			}
		if(type==MODE_VOLUME)
			{
				long Volume[];
				ArraySetAsSeries(Volume,true);
				CopyTickVolume(symbol,timeframe,start,count,Volume);
				return(ArrayMinimum(Volume,0,count)+start);
			}
		if(type>=MODE_TIME)
			{
				datetime Time[];
				ArraySetAsSeries(Time,true);
				CopyTime(symbol,timeframe,start,count,Time);
				return(ArrayMinimum(Time,0,count)+start);
			}
		//---
		return(0);
	}	
//+------------------------------------------------------------------+
