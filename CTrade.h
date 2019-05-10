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
input double   OffSet   =  41.95;
input double   Risk     =  9.25;  
//--- 10 pixels upper from high price
int   ArrowShift  =  -10;
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

