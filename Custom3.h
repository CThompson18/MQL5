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
   
   Trend(rates_total,prev_calculated,high[],low[],close[]);
   
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
//| Custom indicator                                                 |
//+------------------------------------------------------------------+ 
bool Trend(const int rates_total,const int prev_calculated,const double high[],const double low[],const double close[])
{
//---- checking the number of bars to be enough for the calculation
		if(rates_total < InpStartBars) return(0);

		//---- declarations of local variables 
		int limit;
		double Range,AvgRange,smin,smax,SsMax,SsMin,price;
		bool uptrend;

		//---- calculations of the necessary amount of data to be copied
		//---- and the 'limit' starting index for the bars recalculation loop
		if(prev_calculated > rates_total || prev_calculated <= 0)// checking for the first start of the indicator calculation
			{
				K = InpKK - InpRisk;
				limit = rates_total - InpStartBars;       // starting index for calculation of all bars
			}
		else
			{
				limit = rates_total - prev_calculated; // starting index for calculation of new bars
			}

		//---- indexing elements in arrays as timeseries  
		//ArraySetAsSeries(high,true);
		//ArraySetAsSeries(low,true);
		//ArraySetAsSeries(close,true);

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
				for(int iii = bar; iii <= bar + InpOffSet; iii++) 
				AvgRange = AvgRange + MathAbs(high[iii] - low[iii]);
				Range = AvgRange /(InpOffSet + 1);
				//----
				SsMax=low[bar];
				SsMin=close[bar];
				for(int kkk = bar; kkk <= bar + InpOffSet -1; kkk++)
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
		return(true);
     
   }
