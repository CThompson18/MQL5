//+------------------------------------------------------------------+
//|                                                   new_system.mq4 |
//|                                                                  |
//+------------------------------------------------------------------+
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

extern string StartDate="2016.01.01 00:00";

// Dan's TrendSignal settings
extern double Signal_RISK=15;
int Signal_Timeframe = 0;

//Atr settings TakeProfiit
extern int ATR_Period = 250;
extern double ATR_Multiplier = 5.2;
extern double Stop_ATR_Multiplier = 3.0;
int ATR_Timeframe = 0;

//Atr settings Pullback
extern int Pullback_Pips = 20;
extern int Pullback_Timeout = 25;

extern int Used_Bar=1;

string Indicator_Name="New_System2";


double val1[];
double val2[];
double semn1[];
double semn2[];
double sl[];
double tp[];
double pb[];
double ps[];

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
	SetIndexBuffer(2,tp);
	SetIndexStyle(3,DRAW_LINE,0,2);
	SetIndexBuffer(3,sl);   

	SetIndexStyle(4,DRAW_ARROW,STYLE_DOT,2);
	SetIndexBuffer(4,pb);
	SetIndexArrow(4, 158);
	SetIndexStyle(5,DRAW_ARROW,STYLE_DOT,2);
	SetIndexBuffer(5,ps); 
	SetIndexArrow(5, 158);  


	SetIndexStyle(6,DRAW_ARROW,5,2);
	SetIndexArrow(6, 240);
	SetIndexBuffer(6,val1);
	SetIndexStyle(7,DRAW_ARROW,5,2);
	SetIndexArrow(7, 239);
	SetIndexBuffer(7,val2);
	//----
   return(0);
  }

int deinit()
  {
   return(0);
  }
int last_s = 0;

datetime last_o=0;
double last_p=0;
int last_ty =0;
bool stop_pb=false;
bool stop_ps=false;
int start()
  {    
	int mult=1;
	if ( ((StringFind(Symbol(),"JPY",0)!=-1 || StringFind(Symbol(),"jpy",0)!=-1) && MarketInfo(Symbol(),MODE_POINT)<0.01) ||
        ((StringFind(Symbol(),"JPY",0)==-1 || StringFind(Symbol(),"jpy",0)==-1) && MarketInfo(Symbol(),MODE_POINT)<0.0001))
		mult=10;

	/*string fn=File_Name+"_"+Symbol()+".sql";
	string fs="";
	FileDelete(fn);*/
	int CountBars = iBarShift(Symbol(),0,StrToTime(StartDate),false);   
	//int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
	double SUM = 0;
	int trades = 0;
   
	Print(DoubleToStr(NormalizeDouble(Point,Digits)*mult, Digits));
   
	for (int kk=CountBars-1;kk>=0;kk--)
		{
			semn1[kk]=EMPTY_VALUE;
			semn2[kk]=EMPTY_VALUE;
			tp[kk] = EMPTY_VALUE;
			sl[kk] = EMPTY_VALUE;
			pb[kk] = EMPTY_VALUE;
			ps[kk] = EMPTY_VALUE;
      
			double patr=iATR(Symbol(),ATR_Timeframe,ATR_Period,kk);
 
			double dan_up = iCustom(Symbol(),Signal_Timeframe,"dantrend",Signal_RISK,CountBars,1,kk+1);
			double dan_dn = iCustom(Symbol(),Signal_Timeframe,"dantrend",Signal_RISK,CountBars,0,kk+1);

			if (dan_up != 0 && (last_s!=1 || (tp[kk+1]==EMPTY_VALUE && pb[kk+1]==EMPTY_VALUE)))
				{
					last_s=1;
					semn1[kk] = Open[kk];
					pb[kk] = Open[kk] - Pullback_Pips*NormalizeDouble(Point,Digits)*mult;
					if (tp[kk+1]!=EMPTY_VALUE) 
						{
							val2[kk] = Open[kk];
							double s = ((NormalizeDouble(last_p,Digits) - NormalizeDouble(Open[kk], Digits))/Point)/mult;
							SUM+=s;
							Print("Sum Sell: "+s+ " last_p:"+last_p+" Open:"+Open[kk]);
							trades ++;
							/*
							fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(tp[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-tp[kk+1])/Point)/mult,2)+");\n";
							FileWriteString(handle,fs,StringLen(fs));
							FileFlush(handle);               
							*/							
						}
					sl[kk+1]=EMPTY_VALUE;
					tp[kk+1]=EMPTY_VALUE;
					ps[kk+1]=EMPTY_VALUE;
				}
			if (dan_dn != 0 && (last_s!=2 || (tp[kk+1]==EMPTY_VALUE && ps[kk+1]==EMPTY_VALUE)))
				{
					last_s=2;
					semn2[kk] = Open[kk];
					ps[kk] = Open[kk] + Pullback_Pips*NormalizeDouble(Point,Digits)*mult;
					if (tp[kk+1]!=EMPTY_VALUE) 
						{
							val2[kk] = Open[kk];
							s = ((NormalizeDouble(Open[kk], Digits)- NormalizeDouble(last_p,Digits))/Point)/mult;
							SUM+=s;
							Print("Sum Buy: "+s+ " last_p:"+last_p+" Open:"+Open[kk]);
							trades ++;
							/*
							fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) 
							VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(tp[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-tp[kk+1])/Point)/mult,2)+");\n";
							FileWriteString(handle,fs,StringLen(fs));
							FileFlush(handle);                
							*/
						}         
					sl[kk+1]=EMPTY_VALUE;         
					tp[kk+1]=EMPTY_VALUE;         
					pb[kk+1]=EMPTY_VALUE;         
				}
      
			if (pb[kk+1] != EMPTY_VALUE && !stop_pb) pb[kk] = pb[kk+1];
				if(stop_pb) stop_pb=false;
			if (ps[kk+1] != EMPTY_VALUE && !stop_ps) ps[kk] = ps[kk+1];
				if(stop_ps) stop_ps=false;
      
			if (High[kk]>=pb[kk] && Low[kk]<pb[kk] && pb[kk]!=EMPTY_VALUE) 
				{
					tp[kk] = pb[kk] + patr*ATR_Multiplier;
					sl[kk] = pb[kk] - patr*Stop_ATR_Multiplier;
					last_o = Time[kk];
					last_p = pb[kk];
					val1[kk] = last_p;
					last_ty = 1;
					//pb[kk] = EMPTY_VALUE;    
					stop_pb=true;     
				}

			if (Low[kk]<=ps[kk] && High[kk]>ps[kk] && ps[kk]!=EMPTY_VALUE) 
				{
					tp[kk] = ps[kk] - patr*ATR_Multiplier;
					sl[kk] = ps[kk] + patr*Stop_ATR_Multiplier;
					last_o = Time[kk];
					last_p = ps[kk];
					val1[kk] = last_p;
					last_ty = 2; 
					//ps[kk] = EMPTY_VALUE;     
					stop_ps=true;    
				}
			if (pb[kk+Pullback_Timeout] == pb[kk]) pb[kk]=EMPTY_VALUE;
			if (ps[kk+Pullback_Timeout] == ps[kk]) ps[kk]=EMPTY_VALUE;
			if (tp[kk+1] != EMPTY_VALUE) tp[kk] = tp[kk+1];
			if (sl[kk+1] != EMPTY_VALUE) sl[kk] = sl[kk+1];
			if (High[kk]>=tp[kk] && Low[kk]<tp[kk]) 
				{
					s = (MathAbs(NormalizeDouble(last_p,Digits) - NormalizeDouble(tp[kk], Digits))/Point)/mult;
					SUM+=s;
					Print("TP: "+s+ " last_p:"+last_p+" TP:"+tp[kk]);
					trades ++;
					/*
					fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(tp[kk+1],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", "+DoubleToStr((MathAbs(last_p-tp[kk+1])/Point)/mult,2)+");\n";
					FileWriteString(handle,fs,StringLen(fs));
					FileFlush(handle);               
							*/						
					val2[kk] = tp[kk];
					tp[kk] = EMPTY_VALUE;
					sl[kk] = EMPTY_VALUE;
         
				}     
			if (High[kk]>=sl[kk] && Low[kk]<=sl[kk]) 
				{
					s = (MathAbs(NormalizeDouble(last_p,Digits) - NormalizeDouble(sl[kk], Digits))/Point)/mult;
					SUM=SUM-s;
					Print("SL: -"+s+ " last_p:"+last_p+" SL:"+sl[kk]);
					trades ++;
					val2[kk] = sl[kk];
					/*
					fs="INSERT INTO trades (currency, openprice, closeprice, opentime, closetime, type, profit) VALUES (\'"+Symbol()+"\',"+DoubleToStr(last_p,Digits)+", "+DoubleToStr(sl[kk],Digits)+", \'"+TimeToStr(last_o)+"\', \'"+TimeToStr(Time[kk])+"\', "+last_ty+", -"+DoubleToStr((MathAbs(last_p-sl[kk])/Point)/mult,2)+");\n";
					FileWriteString(handle,fs,StringLen(fs));
					FileFlush(handle);    
					*/
					sl[kk] = EMPTY_VALUE;
					tp[kk] = EMPTY_VALUE;
				}           
      
		} 
	/*
	FileFlush(handle);      
	FileClose(handle);
	*/
	Comment("Total Profit: "+DoubleToStr(SUM,2)+"\nTotal number of trades: "+trades);
	return(0);
  }
