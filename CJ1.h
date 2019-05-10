// property indicator
//********************************************************************
#property copyright ""
#property link      ""

#property indicator_chart_window
#property indicator_buffers 8
#property indicator_color1 Aqua
#property indicator_color2 Aqua
#property indicator_color3 Lime
#property indicator_color4 Red
#property indicator_color5 Lime
#property indicator_color6 Red
#property indicator_color7 Yellow
#property indicator_color8 Magenta

//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
//Multi-function variables
//*********************************************************************
//---- input parameters
extern string glbStartDate="2016.01.01 00:00";
extern double glbSignal_Risk = 9.1;
extern int glbATR_Period = 250;
extern double glbATR_Multiplier = 5.2;
extern double glbStop_ATR_Multiplier = 3.0;
extern int glbPullback_Pips = 20;
extern int glbPullback_Timeout = 25;
extern int glbUsed_Bar=1;

extern int glbCountBars = 300;
extern color glbText_Color = Red;
extern int glbFont_Size = 10;

int glbSignal_Timeframe = 0;
int glbATR_Timeframe = 0;
string glbIndicator_Name="New_System2";
//BuySell orders.
//*********************************************************************
extern int glbSlippage = 1;
extern double glbLot_Size = .10;
string com = "CD_EA";

//---- buffers
//*********************************************************************
double glbSellArray[];
double glbBuyArray[];
double semn1[];
double semn2[];
double glbStopLoss[];
double glbTakeProfit[];
double glbPlaceBuy[];
double glbPlaceSell[];

//DataDisplay function global variables
//*********************************************************************
extern int glb_right_edge_shift = 175;
extern int glb_upper_edge_shift = 30;
string glb_ea_name = "CD Financials";
extern int glbMagic_Number = 1234;
extern double glbLot_Size_Percent = .75;

double glbSell_Value = 0.000;
double glbBuy_Value = 0.000;

int init()
  {
//---- indicator line
	IndicatorBuffers(8);
	SetIndexStyle(0,DRAW_ARROW,5,2);
	SetIndexArrow(0, 233);
	SetIndexBuffer(0,semn1);
	SetIndexStyle(1,DRAW_ARROW,5,2);
	SetIndexArrow(1, 234);
	SetIndexBuffer(1,semn2);   

	SetIndexStyle(2,DRAW_LINE,0,2);
	SetIndexBuffer(2,glbTakeProfit);
	SetIndexStyle(3,DRAW_LINE,0,2);
	SetIndexBuffer(3,glbStopLoss);   

	SetIndexStyle(4,DRAW_ARROW,STYLE_DOT,2);
	SetIndexBuffer(4,glbPlaceBuy);
	SetIndexArrow(4, 158);
	SetIndexStyle(5,DRAW_ARROW,STYLE_DOT,2);
	SetIndexBuffer(5,glbPlaceSell); 
	SetIndexArrow(5, 158);  


	SetIndexStyle(6,DRAW_ARROW,5,2);
	SetIndexArrow(6, 240);
	SetIndexBuffer(6,glbSellArray);
	SetIndexStyle(7,DRAW_ARROW,5,2);
	SetIndexArrow(7, 239);
	SetIndexBuffer(7,glbBuyArray);

//----
   return(0);
  }
//+------------------------------------------------------------------+
//| ASCTrend1sig                                                     |
//+------------------------------------------------------------------+
int start()
	{
		// if (glbCountBars>=1000) glbCountBars=950;
		SetIndexDrawBegin(0,Bars-glbCountBars+11+1);
		SetIndexDrawBegin(1,Bars-glbCountBars+11+1);
		int i,shift,counted_bars=IndicatorCounted();
		int Counter,i1,value10,value11;
		double x1,x2;
		double value2,value3;
		double TrueCount,Range,AvgRange,MRO1,MRO2;
		double Table_value2[75000];
	   
		value10 = 3 + glbSignal_Risk * 2;
		x1 = 67 + glbSignal_Risk;
		x2 = 33 - glbSignal_Risk;
		value11 = value10;
		//----
		if(Bars <= 11 + 1) return(0);
		//---- initial zero
		if(counted_bars < 11 + 1)
			{
				for(i = 1;i <= 0;i++) glbSellArray[glbCountBars - i] = 0.0;
				for(i = 1;i <= 0;i++) glbBuyArray[glbCountBars - i] = 0.0;
			}
		//----
		shift = glbCountBars - 11 - 1;
		while(shift >= 0)
			{
				Counter = shift;
				Range = 0.0;
				AvgRange = 0.0;
				for (Counter = shift; Counter <= shift + 9; Counter++) AvgRange = AvgRange + MathAbs(High[Counter] - Low[Counter]);
			
				Range = AvgRange / 10;
				Counter = shift;
				TrueCount = 0;
				while (Counter < shift + 9 && TrueCount < 1)
					{
						if (MathAbs(Open[Counter] - Close[Counter + 1]) >= Range * 2.0) TrueCount = TrueCount + 1;
						Counter = Counter + 1;
					}
				if (TrueCount >= 1) 
				   {
				      MRO1=Counter;
				   } 
				   else 
				   {
				      MRO1 = - 1;
				   }
				Counter = shift;
				TrueCount = 0;
				while (Counter < shift + 6 && TrueCount < 1)
					{
						if (MathAbs(Close[Counter + 3] - Close[Counter]) >= Range * 4.6) TrueCount = TrueCount + 1;
						Counter = Counter + 1;
					}
				if (TrueCount >= 1) 
				   {
				      MRO2=Counter;
				   } 
				   else 
				   {
				      MRO2 = - 1;
				   }
				if (MRO1 >- 1) 
				   {
				      value11 = 3;
				   } 
				   else 
				   {
				      value11 = value10;
				   }
				if (MRO2 > - 1) 
				   {
				      value11 = 4;
				   } 
				   else 
				   {
				      value11 = value10;
				   }
				value2 = 100 - MathAbs(iWPR(NULL,0,value11,shift)); // PercentR(value11=9)
				Table_value2[shift] = value2;
				glbSellArray[shift] = 0;
				glbBuyArray[shift] = 0;
				value3 = 0;
				if (value2 < x2)
					{
						i1 = 1;
						while (Table_value2[shift+i1] >= x2 && Table_value2[shift + i1] <= x1)
							{
								i1++;
							}
						if (Table_value2[shift + i1] > x1) 
							{
								value3=High[shift] + Range * 0.5;
								glbSellArray[shift] = value3;
								glbSell_Value = value3;
								history();
							} 
					}
				if (value2 > x1)
					{
						i1 = 1;
						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1)
							{
								i1++;
							}
						if (Table_value2[shift + i1] < x2) 
							{
								value3=Low[shift] - Range * 0.5;
								glbBuyArray[shift] = value3;
								glbBuy_Value = value3;
								history();
							}
					}      
				shift--;
			}

	   return(0);
	  }
//+------------------------------------------------------------------+

int deinit()
  {
   return(0);
  }
  
void history()
   {
		int mult = 1;
		int last_s = 0;
		int last_ty =0;
		int trades = 0;
		double last_p = 0;
		double SUM = 0;
		bool stop_pb = false;
		bool stop_ps = false;
		datetime last_o = 0;
	
		if (((StringFind(Symbol(),"JPY",0) != - 1 || StringFind(Symbol(),"jpy",0) != - 1) && MarketInfo(Symbol(),MODE_POINT) < 0.01) ||
			((StringFind(Symbol(),"JPY",0) == - 1 || StringFind(Symbol(),"jpy",0) == - 1) && MarketInfo(Symbol(),MODE_POINT) < 0.0001))
			mult=10;

		/*string fn=File_Name+"_"+Symbol()+".sql";
		string fs="";
		FileDelete(fn);*/
		int CountBars = iBarShift(Symbol(),0,StrToTime(glbStartDate),false);   
		//int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
	
		Print(DoubleToStr(NormalizeDouble(Point,Digits) * mult, Digits));
   
		for (int kk = CountBars - 1;kk >= 0;kk--)
			{
				semn1[kk] = EMPTY_VALUE;
				semn2[kk] = EMPTY_VALUE;
				glbTakeProfit[kk] = EMPTY_VALUE;
				glbStopLoss[kk] = EMPTY_VALUE;
				glbPlaceBuy[kk] = EMPTY_VALUE;
				glbPlaceSell[kk] = EMPTY_VALUE;
      
				double patr = iATR(Symbol(),glbATR_Timeframe,glbATR_Period,kk);
	 
				//double Sell_Value = iCustom(Symbol(),glbSignal_Timeframe,"dantrend",glbSignal_Risk,CountBars,1,kk+1);
				//double Buy_Value = iCustom(Symbol(),glbSignal_Timeframe,"dantrend",glbSignal_Risk,CountBars,0,kk+1);

				if (glbSell_Value != 0 && (last_s != 1 || (glbTakeProfit[kk+1] == EMPTY_VALUE && glbPlaceBuy[kk + 1] == EMPTY_VALUE)))
					{
						last_s = 1;
						semn1[kk] = Open[kk];
						glbPlaceBuy[kk] = Open[kk] - glbPullback_Pips * NormalizeDouble(Point,Digits) * mult;
						if (glbTakeProfit[kk + 1] != EMPTY_VALUE) 
							{
								glbBuyArray[kk] = Open[kk];
								double s = ((NormalizeDouble(last_p,Digits) - NormalizeDouble(Open[kk], Digits))/Point)/mult;
								SUM += s;
								Print("Sum Sell: "+s+ " last_p:"+last_p+" Open:"+Open[kk]);
								trades ++;
								/*
								fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(glbTakeProfit[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-glbTakeProfit[kk+1])/Point)/mult,2)+");\n";
								FileWriteString(handle,fs,StringLen(fs));
								FileFlush(handle);               
								*/							
							}
						glbStopLoss[kk + 1] = EMPTY_VALUE;
						glbTakeProfit[kk + 1] = EMPTY_VALUE;
						glbPlaceSell[kk + 1] = EMPTY_VALUE;
					}
				if (glbBuy_Value != 0 && (last_s != 2 || (glbTakeProfit[kk + 1] == EMPTY_VALUE && glbPlaceSell[kk + 1] == EMPTY_VALUE)))
					{
						last_s = 2;
						semn2[kk] = Open[kk];
						glbPlaceSell[kk] = Open[kk] + glbPullback_Pips * NormalizeDouble(Point,Digits) * mult;
						if (glbTakeProfit[kk + 1] != EMPTY_VALUE) 
							{
								glbBuyArray[kk] = Open[kk];
								s = ((NormalizeDouble(Open[kk], Digits) - NormalizeDouble(last_p,Digits))/Point)/mult;
								SUM += s;
								Print("Sum Buy: "+s+ " last_p:"+last_p+" Open:"+Open[kk]);
								trades ++;
								/*
								fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) 
								VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(glbTakeProfit[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-glbTakeProfit[kk+1])/Point)/mult,2)+");\n";
								FileWriteString(handle,fs,StringLen(fs));
								FileFlush(handle);                
								*/
							}         
						glbStopLoss[kk + 1] = EMPTY_VALUE;         
						glbTakeProfit[kk + 1] = EMPTY_VALUE;         
						glbPlaceBuy[kk + 1] = EMPTY_VALUE;         
					}
      
				if (glbPlaceBuy[kk + 1] != EMPTY_VALUE && ! stop_pb) glbPlaceBuy[kk] = glbPlaceBuy[kk + 1];
					if(stop_pb) stop_pb=false;
				if (glbPlaceSell[kk + 1] != EMPTY_VALUE && ! stop_ps) glbPlaceSell[kk] = glbPlaceSell[kk + 1];
					if(stop_ps) stop_ps = false;
		  
				if (High[kk] >= glbPlaceBuy[kk] && Low[kk] < glbPlaceBuy[kk] && glbPlaceBuy[kk] != EMPTY_VALUE) 
					{
						glbTakeProfit[kk] = glbPlaceBuy[kk] + patr * glbATR_Multiplier;
						glbStopLoss[kk] = glbPlaceBuy[kk] - patr * glbStop_ATR_Multiplier;
						last_o = Time[kk];
						last_p = glbPlaceBuy[kk];
						glbSellArray[kk] = last_p;
						last_ty = 1;
						//glbPlaceBuy[kk] = EMPTY_VALUE;    
						stop_pb = true;     
					}

				if (Low[kk] <= glbPlaceSell[kk] && High[kk] > glbPlaceSell[kk] && glbPlaceSell[kk] != EMPTY_VALUE) 
					{
						glbTakeProfit[kk] = glbPlaceSell[kk] - patr * glbATR_Multiplier;
						glbStopLoss[kk] = glbPlaceSell[kk] + patr * glbStop_ATR_Multiplier;
						last_o = Time[kk];
						last_p = glbPlaceSell[kk];
						glbSellArray[kk] = last_p;
						last_ty = 2; 
						//glbPlaceSell[kk] = EMPTY_VALUE;     
						stop_ps = true;    
					}
				if (glbPlaceBuy[kk + glbPullback_Timeout] == glbPlaceBuy[kk]) glbPlaceBuy[kk] = EMPTY_VALUE;
				if (glbPlaceSell[kk + glbPullback_Timeout] == glbPlaceSell[kk]) glbPlaceSell[kk] = EMPTY_VALUE;
				if (glbTakeProfit[kk + 1] != EMPTY_VALUE) glbTakeProfit[kk] = glbTakeProfit[kk + 1];
				if (glbStopLoss[kk + 1] != EMPTY_VALUE) glbStopLoss[kk] = glbStopLoss[kk + 1];
				if (High[kk] >= glbTakeProfit[kk] && Low[kk] < glbTakeProfit[kk]) 
					{
						s = (MathAbs(NormalizeDouble(last_p,Digits) - NormalizeDouble(glbTakeProfit[kk], Digits))/Point)/mult;
						SUM += s;
						Print("TP: "+s+ " last_p:"+last_p+" TP:"+glbTakeProfit[kk]);
						trades ++;
						/*
						fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(glbTakeProfit[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-glbTakeProfit[kk+1])/Point)/mult,2)+");\n";
						FileWriteString(handle,fs,StringLen(fs));
						FileFlush(handle);               
								*/						
						glbBuyArray[kk] = glbTakeProfit[kk];
						glbTakeProfit[kk] = EMPTY_VALUE;
						glbStopLoss[kk] = EMPTY_VALUE;
			 
					}     
				if (High[kk] >= glbStopLoss[kk] && Low[kk] <= glbStopLoss[kk]) 
					{
						s = (MathAbs(NormalizeDouble(last_p,Digits) - NormalizeDouble(glbStopLoss[kk], Digits))/Point)/mult;
						SUM = SUM -s;
						Print("SL: -"+s+ " last_p:"+last_p+" SL:"+glbStopLoss[kk]);
						trades ++;
						glbBuyArray[kk] = glbStopLoss[kk];
						/*
						fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(glbStopLoss[kk],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", -"+DoubleToStr((MathAbs(last_p-glbStopLoss[kk])/Point)/mult,2)+");\n";
						FileWriteString(handle,fs,StringLen(fs));
						FileFlush(handle);    
						*/
						glbStopLoss[kk] = EMPTY_VALUE;
						glbTakeProfit[kk] = EMPTY_VALUE;
					}           
		  
			} 
			/*
			FileFlush(handle);      
			FileClose(handle);
			*/
			Comment("Total Profit: "+DoubleToStr(SUM,2)+"\nTotal number of trades: "+trades);
			return(0);

	}