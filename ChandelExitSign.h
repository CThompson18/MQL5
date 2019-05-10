//+------------------------------------------------------------------+ 
//|                                              ChandelExitSign.mq5 | 
//|                                                       MQLService | 
//|                                           scripts@mqlservice.com | 
//+------------------------------------------------------------------+ 
//---- authorship of indicator
#property copyright "MQLService"
#property link      "http://viac.ru/" 
//#property link      "scripts@mqlservice.com" 
//---- indicator version
#property version   "1.00" 
//---- property indicator in main window
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
#property indicator_label1  "BullsBears Sell" 
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
#property indicator_label2 "BullsBears Buy" 
//+----------------------------------------------+ 
//| Ads contact                                  | 
//+----------------------------------------------+ 
#define RESET 0  
// Constant for returning the command to the terminal to calculate the indicator
//+----------------------------------------------+ 
//| Input parameters of the indicator            | 
//+----------------------------------------------+ 
input 	uint 	RangePeriod = 15; 
input 	uint 	Shift = 1; 
input 	uint 	ATRPeriod = 14; 
input 	uint 	MultipleATR = 4;
input	double	Numerator = 3.00;
input	double 	Denumerator = 8.00;
//+----------------------------------------------+  
//----Declaration of dynamic arrays, which will be used in the future as an indicator buffer
double 	SellBuffer[]; 
double 	BuyBuffer[];  
//---- declaration of price changes for handling indicators
int ATR1_Handle,ATR2_Handle;  
//---- declaration of price changes from the beginning of given account
int min_rates_total; 
//+------------------------------------------------------------------+ 
//| Custom indicator initialization function                         | 
//+------------------------------------------------------------------+   
int OnInit() 
	{	  
		//---- initialization of changes from beginning of given account
		int ATR2Period = 12; 
		min_rates_total = int(MathMax(MathMax(ATRPeriod,RangePeriod + Shift) + 1,ATR2Period)); 

		//---- receiving handle from indicator ATR1
		ATR1_Handle = iATR(NULL,0,ATRPeriod); 
		if(ATR1_Handle==INVALID_HANDLE) 
		   { 
		      Print("Unable to receive ATR1 handle indicator");
		      return(INIT_FAILED); 
		   } 

		//---- receiving handle from indicator ATR2 
		ATR2_Handle = iATR(NULL,0,ATR2Period); 
		if(ATR2_Handle==INVALID_HANDLE) 
		   { 
		      Print("Unable to receive ATR2 handle indicator"); 
		      return(INIT_FAILED); 
		   } 


		//---- set SellBuffer[] dynamic array as an indicator buffer
		SetIndexBuffer(0,SellBuffer,INDICATOR_DATA);
		//---- shifting the start of drawing the indicator 1
		PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,min_rates_total);
		//---- indicator symbol
		PlotIndexSetInteger(0,PLOT_ARROW,234);
		//---- shift indicator 1 horizontally
		PlotIndexSetInteger(0,PLOT_SHIFT,Shift);
		//---- prohibit indicator drawing of null values 
		PlotIndexSetDouble(0,PLOT_EMPTY_VALUE,0.0);
		//---- indexing the elements in the buffer as timeseries
		ArraySetAsSeries(SellBuffer,true);
 

		//---- set BuyBuffer[] dynamic array as an indicator buffer
		SetIndexBuffer(1,BuyBuffer,INDICATOR_DATA);
		//---- shifting the start of drawing the indicator 2
		PlotIndexSetInteger(1,PLOT_DRAW_BEGIN,min_rates_total);
		//---- indicator symbol
		PlotIndexSetInteger(1,PLOT_ARROW,233); 
		//---- shift indicator 1 horizontally 
		PlotIndexSetInteger(1,PLOT_SHIFT,Shift); 
		//---- prohibit indicator drawing of null values
		PlotIndexSetDouble(1,PLOT_EMPTY_VALUE,0.0); 
		//---- indexing the elements in the buffer as timeseries 
		ArraySetAsSeries(BuyBuffer,true); 

 
		//---- Initialize the variable for the short name of the indicator
		string shortname; 
		StringConcatenate(shortname,"ChandelExit(",RangePeriod,", ",ATRPeriod,")");  
		//--- create name for display in a separate window and in a pop-up tip
		IndicatorSetString(INDICATOR_SHORTNAME,shortname);  
		//----Determine the accuracy of the indicator values display
		IndicatorSetInteger(INDICATOR_DIGITS,_Digits);  
		//--- end initialization
		return(INIT_SUCCEEDED); 
	} 
//+------------------------------------------------------------------+ 
//| Custom indicator iteration function                              | 
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
	//---- verify the collection of bars for sufficiency for calculation 
	if(BarsCalculated(ATR1_Handle) < rates_total || BarsCalculated(ATR2_Handle) < rates_total || rates_total < min_rates_total) return(RESET); 
 
	//---- Declare local variables
     static int direction; 
     int to_copy,limit,bar; 
     double ATR1[],ATR2[],HH0,LL0,Up=0.0,Dn=0.0; 
     static double Up1,Dn1; 
 
	//----calculation of starting number limit for cycle of recalculating bars
	if(prev_calculated > rates_total || prev_calculated <= 0)// verify the first start of calculation indicators 								
		{ 
			limit = rates_total - 1 - min_rates_total;   //starting number for calculating all bars
         	direction = 0; 
			Up = Up1 = 0.0; 
			Dn = Dn1 = 0.0; 
		} 
	else limit = rates_total - prev_calculated; // starting number for calculating new bars 
							 
	//--- calculation of necessary copied data
	to_copy = limit + 1; 

	//---- recopy array data
	if(CopyBuffer(ATR1_Handle,0,0,to_copy,ATR1) <= 0) return(RESET); 
	if(CopyBuffer(ATR2_Handle,0,0,to_copy,ATR2) <= 0) return(RESET); 
   
	//--- indication of elements in arrays as in Time Series
	ArraySetAsSeries(ATR1,true); 
	ArraySetAsSeries(ATR2,true); 
	ArraySetAsSeries(high,true); 
	ArraySetAsSeries(low,true); 
	ArraySetAsSeries(close,true); 
 
	//---- establish cycle of calculation indicators
	for(bar = limit; bar >= 0 && !IsStopped(); bar--) 
		{ 
			ATR1[bar] *= MultipleATR; 
			HH0 = high[ArrayMaximum(high,bar + Shift,RangePeriod)] - ATR1[bar]; 
			LL0 = low[ArrayMinimum(low,bar + Shift,RangePeriod)] + ATR1[bar]; 
			if(direction >= 0) 
				{ 
					if(close[bar] < HH0) 
						{ 
							if(bar) direction = -1; 
								Up = LL0; 
								Dn = HH0; 
						} 
					else 
						{ 
							Up = HH0; 
							Dn = LL0; 
						} 
				} 
			else 
				if(direction <= 0) 
					{ 
						if(close[bar] > LL0) 
							{ 
								if(bar) direction =+ 1;
									Dn = LL0;
									Up = HH0;
							}
						else
							{
								Up = LL0;
								Dn = HH0;
							}
					} 

			BuyBuffer[bar] = 0.0; 
			SellBuffer[bar] = 0.0; 
			//--- 
			if(Dn1 <= Up1 && Dn > Up) BuyBuffer[bar] = low[bar] - ATR1[bar] * Numerator / Denumerator; 
			if(Dn1 >= Up1 && Dn < Up) SellBuffer[bar] = high[bar] + ATR1[bar] * Numerator / Denumerator; 

			if(bar) 
				{
					Up1 = Up;
					Dn1 = Dn;
				} 
		} 
		//----     
		return(rates_total); 
	} 
//+------------------------------------------------------------------+
