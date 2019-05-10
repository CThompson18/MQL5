//+------------------------------------------------------------------+
//|                                                   new_system.mq4 |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""
// property indicator
//********************************************************************
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

//Global Multi-function variables
//*********************************************************************
extern string glbStartDate = "2016.01.01 00:00";
extern double glbSignal_Risk = 15;
extern int glbATR_Period = 250;
extern double glbATR_Multiplier = 5.2;
extern double glbStop_ATR_Multiplier = 3.0;
extern int glbPullback_Pips = 20;
extern int glbPullback_Timeout = 25;
extern int glbUsed_Bar = 1;

//Global variables
//*********************************************************************
int ATR_Timeframe = 0;
int Signal_Timeframe = 0;
string Indicator_Name="New_System2";
int last_s = 0;
datetime last_o = 0;
double last_p = 0;
int last_ty = 0;
bool stop_pb = false;
bool stop_ps = false;

//Arrays
//*********************************************************************
double glbSellArray[];
double glbBuyArray[];
double semn1[];
double semn2[];
double glbStopLoss[];
double glbTakeProfit[];
double glbPB[];
double glbPS[];

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
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
   SetIndexBuffer(4,glbPB);
   SetIndexArrow(4, 158);
   SetIndexStyle(5,DRAW_ARROW,STYLE_DOT,2);
   SetIndexBuffer(5,glbPS); 
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
//| Custom indicator De-initialization function                      |
//+------------------------------------------------------------------+
int deinit()
  {
   return(0);
  }
  

int start()
	{    
		int mult = 1;
		int trades = 0;
		double SUM = 0;
		/*string fn=File_Name+"_"+Symbol()+".sql";
		string fs="";
		FileDelete(fn);*/
		int CountBars = iBarShift(Symbol(),0,StrToTime(glbStartDate),false);   
		//int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
		
		if (((StringFind(Symbol(),"JPY",0) != - 1 || StringFind(Symbol(),"jpy",0) != - 1) && MarketInfo(Symbol(),MODE_POINT) < 0.01) ||
			((StringFind(Symbol(),"JPY",0) == - 1 || StringFind(Symbol(),"jpy",0) == - 1) && MarketInfo(Symbol(),MODE_POINT) < 0.0001))
			mult = 10;
			
		Print(DoubleToStr(NormalizeDouble(Point,Digits)*mult, Digits));
   
		for (int kk=CountBars-1;kk>=0;kk--)
			{
				semn1[kk] = EMPTY_VALUE;
				semn2[kk] = EMPTY_VALUE;
				glbTakeProfit[kk] = EMPTY_VALUE;
				glbStopLoss[kk] = EMPTY_VALUE;
				glbPB[kk] = EMPTY_VALUE;
				glbPS[kk] = EMPTY_VALUE;
      
			double patr=iATR(Symbol(),ATR_Timeframe,glbATR_Period,kk);
			double dan_up = iCustom(Symbol(),Signal_Timeframe,"dantrend",glbSignal_Risk,CountBars,1,kk+1);
			double dan_dn = iCustom(Symbol(),Signal_Timeframe,"dantrend",glbSignal_Risk,CountBars,0,kk+1);

			if (dan_up != 0 && (last_s != 1 || (glbTakeProfit[kk + 1] == EMPTY_VALUE && glbPB[kk + 1] == EMPTY_VALUE)))
				{
					last_s=1;
					semn1[kk] = Open[kk];
					glbPB[kk] = Open[kk] - glbPullback_Pips * NormalizeDouble(Point,Digits) * mult;
					if (glbTakeProfit[kk+1]!=EMPTY_VALUE) 
						{
							glbBuyArray[kk] = Open[kk];
							double s = ((NormalizeDouble(last_p,Digits) - NormalizeDouble(Open[kk], Digits)) / Point) / mult;
							SUM += s;
							Print("Sum Sell: "+s+ " last_p:"+last_p+" Open:"+Open[kk]);
							trades ++;
						}
					glbStopLoss[kk+1] = EMPTY_VALUE;
					glbTakeProfit[kk+1] = EMPTY_VALUE;
					glbPS[kk+1] = EMPTY_VALUE;
				}
			if (dan_dn != 0 && (last_s != 2 || (glbTakeProfit[kk + 1] == EMPTY_VALUE && glbPS[kk + 1] == EMPTY_VALUE)))
				{
					last_s = 2;
					semn2[kk] = Open[kk];
					glbPS[kk] = Open[kk] + glbPullback_Pips * NormalizeDouble(Point,Digits) * mult;
				if (glbTakeProfit[kk + 1] != EMPTY_VALUE) 
					{
						glbBuyArray[kk] = Open[kk];
						s = ((NormalizeDouble(Open[kk], Digits) - NormalizeDouble(last_p,Digits)) / Point) / mult;
						SUM += s;
						Print("Sum Buy: "+s+ " last_p:"+last_p+" Open:"+Open[kk]);
						trades ++;
						/*
						fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(glbTakeProfit[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-glbTakeProfit[kk+1])/Point)/mult,2)+");\n";
						FileWriteString(handle,fs,StringLen(fs));
						FileFlush(handle);                
						*/
					}         
					glbStopLoss[kk + 1] = EMPTY_VALUE;         
					glbTakeProfit[kk + 1] = EMPTY_VALUE;         
					glbPB[kk + 1] = EMPTY_VALUE;         
				}
      
			if (glbPB[kk + 1] != EMPTY_VALUE && !stop_pb) glbPB[kk] = glbPB[kk + 1];
			if(stop_pb) stop_pb = false;
			if (glbPS[kk + 1] != EMPTY_VALUE && !stop_ps) glbPS[kk] = glbPS[kk + 1];
			if(stop_ps) stop_ps = false;
      
			if (High[kk] >= glbPB[kk] && Low[kk] < glbPB[kk] && glbPB[kk] != EMPTY_VALUE) 
				{
					glbTakeProfit[kk] = glbPB[kk] + patr * glbATR_Multiplier;
					glbStopLoss[kk] = glbPB[kk] - patr * glbStop_ATR_Multiplier;
					last_o = Time[kk];
					last_p = glbPB[kk];
					glbSellArray[kk] = last_p;
					last_ty = 1;
					//glbPB[kk] = EMPTY_VALUE;    
					stop_pb = true;     
				}

			if (Low[kk]<=glbPS[kk] && High[kk]>glbPS[kk] && glbPS[kk]!=EMPTY_VALUE) 
				{
					glbTakeProfit[kk] = glbPS[kk] - patr * glbATR_Multiplier;
					glbStopLoss[kk] = glbPS[kk] + patr * glbStop_ATR_Multiplier;
					last_o = Time[kk];
					last_p = glbPS[kk];
					glbSellArray[kk] = last_p;
					last_ty = 2; 
					//glbPS[kk] = EMPTY_VALUE;     
					stop_ps = true;    
				}
			if (glbPB[kk+glbPullback_Timeout] == glbPB[kk]) glbPB[kk] = EMPTY_VALUE;
			if (glbPS[kk+glbPullback_Timeout] == glbPS[kk]) glbPS[kk] = EMPTY_VALUE;
			if (glbTakeProfit[kk+1] != EMPTY_VALUE) glbTakeProfit[kk] = glbTakeProfit[kk + 1];
			if (glbStopLoss[kk+1] != EMPTY_VALUE) glbStopLoss[kk] = glbStopLoss[kk + 1];
			if (High[kk] >= glbTakeProfit[kk] && Low[kk] < glbTakeProfit[kk]) 
				{
					s = (MathAbs(NormalizeDouble(last_p,Digits) - NormalizeDouble(glbTakeProfit[kk], Digits)) / Point) / mult;
					SUM += s;
					Print("TP: "+s+ " last_p:"+last_p+" TP:"+glbTakeProfit[kk]);
					trades ++;
					glbBuyArray[kk] = glbTakeProfit[kk];
					glbTakeProfit[kk] = EMPTY_VALUE;
					glbStopLoss[kk] = EMPTY_VALUE;
         
				}     
			if (High[kk] >= glbStopLoss[kk] && Low[kk] <= glbStopLoss[kk]) 
				{
					s = (MathAbs(NormalizeDouble(last_p,Digits) - NormalizeDouble(glbStopLoss[kk], Digits)) / Point) / mult;
					SUM = SUM - s;
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

