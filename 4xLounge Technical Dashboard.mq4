//+------------------------------------------------------------------+
//|                                 4xLounge Technical Dashboard.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window

extern int XMeter_Periods=121;
extern int XMeter_Shift=5;
extern int XMeter_ApplyTo=0;
extern int XMeter_Atr_Period=50;

extern int RSI_Period=14;
extern int RSI_Price=0;
extern double RSI_Value=50;

extern int MA1_Period=60;
extern int MA1_Mode=MODE_SMA;
extern int MA1_Price=PRICE_CLOSE;

extern int MA2_Period=90;
extern int MA2_Mode=MODE_EMA;
extern int MA2_Price=PRICE_CLOSE;

extern int MA3_Period=120;
extern int MA3_Mode=MODE_EMA;
extern int MA3_Price=PRICE_CLOSE;

extern int CCI_Period=14;
extern int CCI_Price=PRICE_CLOSE;
extern double CCI_Value=0;

extern int BBands_Period=20;
extern int BBands_Deviation=2;
extern int BBands_Price=PRICE_CLOSE;

extern int Stoch_K_Period=5;
extern int Stoch_D_Period=3;
extern int Stoch_Slowing_Period=3;
extern int Stoch_Mode=MODE_SMA;
extern int Stoch_Price=0;
extern double Stoch_Value=50;

extern int ATR_Period=100;

extern int MACD_FastEMA=12;
extern int MACD_SlowEMA=26;
extern int MACD_SignalSMA=9;
extern double MACD_Value=0;

extern int Start_X=40;
extern int X_Spacing=160;
extern int Start_Y=30;
extern int Y_Spacing=40;
extern int Text_Size=20;
extern int Trade_Bar=0;
extern color UP=Green;
extern color DN=Red;

extern int DayStartHour=22; //I think 22 is 5 pm EST  .. the server usualy is GMT.

double last_bid=0;
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
//----
   IndicatorShortName("4xLounge Technical Dashboard");
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
//----

 

//----
   return(0);
  }

//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+



int start()
  {
   color c=White;
   string c1="",s="";
   int tf[7]={1,5,15,30,60,240,1440};
   double rsi[7],ma1[7],ma2[7],ma3[7],cci[7],bbu[7],bbd[7],stochm[7],stochs[7],atr[7],atr1[7],macdf[7],macds[7],xmt[7],tma[7];
   for (int kk=0;kk<7;kk++){      
      tma[kk]=iCustom(Symbol(),tf[kk],"TMA",XMeter_Periods,XMeter_Shift,XMeter_ApplyTo,0,Trade_Bar);
      xmt[kk]=(Bid-tma[kk])/iATR(Symbol(),tf[kk],XMeter_Atr_Period,Trade_Bar);
      rsi[kk]=iRSI(Symbol(),tf[kk],RSI_Period,RSI_Price,Trade_Bar);
      ma1[kk]=iMA(Symbol(),tf[kk],MA1_Period,0,MA1_Mode,MA1_Price,Trade_Bar);
      ma2[kk]=iMA(Symbol(),tf[kk],MA2_Period,0,MA2_Mode,MA2_Price,Trade_Bar);
      ma3[kk]=iMA(Symbol(),tf[kk],MA3_Period,0,MA3_Mode,MA3_Price,Trade_Bar);
      cci[kk]=iCCI(Symbol(),tf[kk],CCI_Period,CCI_Price,Trade_Bar);
      bbu[kk]=iBands(Symbol(),tf[kk],BBands_Period,BBands_Deviation,0,BBands_Price,MODE_UPPER,Trade_Bar);
      bbd[kk]=iBands(Symbol(),tf[kk],BBands_Period,BBands_Deviation,0,BBands_Price,MODE_LOWER,Trade_Bar);
      stochm[kk]=iStochastic(Symbol(),tf[kk],Stoch_K_Period,Stoch_D_Period,Stoch_Slowing_Period,Stoch_Mode,Stoch_Price,MODE_MAIN,Trade_Bar);
      stochs[kk]=iStochastic(Symbol(),tf[kk],Stoch_K_Period,Stoch_D_Period,Stoch_Slowing_Period,Stoch_Mode,Stoch_Price,MODE_SIGNAL,Trade_Bar);
      atr[kk]=iATR(Symbol(),tf[kk],ATR_Period,1);
      atr1[kk]=iATR(Symbol(),tf[kk],ATR_Period,2);
      macdf[kk]=iCustom(Symbol(),tf[kk],"MACD_Lines",MACD_FastEMA,MACD_SlowEMA,MACD_SignalSMA,0,Trade_Bar);
      macds[kk]=iCustom(Symbol(),tf[kk],"MACD_Lines",MACD_FastEMA,MACD_SlowEMA,MACD_SignalSMA,1,Trade_Bar);
   }   
   
   
   int w=WindowFind("4xLounge Technical Dashboard");   
   
   string col[8]={"","1min","5min","15min","30min","1hour","4hour","daily"};
   col[0]=Symbol();
   
   for (kk=0;kk<8;kk++){
      ObjectDelete("col"+kk+Period()+Symbol());
      ObjectCreate("col"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("col"+kk+Period()+Symbol(),col[kk],Text_Size, "Arial Bold", White);
      ObjectSet("col"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("col"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+30+(X_Spacing*kk));
      ObjectSet("col"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y);
   }

   string row[14]={"X Meter","RSI","60MA","90MA","120MA","CCI","B Bands","Stoch","ATR","MACD","","Daily Stats","Weekly Stats","Current"};

   for (kk=0;kk<14;kk++){
      ObjectDelete("row"+kk+Period()+Symbol());
      ObjectCreate("row"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("row"+kk+Period()+Symbol(),row[kk],Text_Size, "Arial Bold", Aqua);
      ObjectSet("row"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("row"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+30);
      ObjectSet("row"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+(kk*Y_Spacing)+Text_Size*2);
   }

   for (kk=0;kk<7;kk++){
   
      ObjectDelete("xmt"+kk+Period()+Symbol());
      ObjectCreate("xmt"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("xmt"+kk+Period()+Symbol(),DoubleToStr(xmt[kk],1),Text_Size, "Arial Bold", White);
      ObjectSet("xmt"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("xmt"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("xmt"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2);
      if (Bid>tma[kk]) {
         ObjectSet("xmt"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
      }else{
         ObjectSet("xmt"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
      }   
   
      ObjectDelete("rsi"+kk+Period()+Symbol());
      ObjectCreate("rsi"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("rsi"+kk+Period()+Symbol(),DoubleToStr(rsi[kk],0),Text_Size, "Arial Bold", White);
      ObjectSet("rsi"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("rsi"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("rsi"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Y_Spacing+Text_Size*2);
      if (rsi[kk]>RSI_Value) {
         ObjectSet("rsi"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
      }else{
         ObjectSet("rsi"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
      }

      ObjectDelete("ma1"+kk+Period()+Symbol());
      ObjectCreate("ma1"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("ma1"+kk+Period()+Symbol(),DoubleToStr((ma1[kk]-Bid)/Point,0),Text_Size, "Arial Bold", White);
      ObjectSet("ma1"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("ma1"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("ma1"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*2);
      if (ma1[kk]>Bid) {
         ObjectSet("ma1"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
      }else{
         ObjectSet("ma1"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
      }

      ObjectDelete("ma2"+kk+Period()+Symbol());
      ObjectCreate("ma2"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("ma2"+kk+Period()+Symbol(),DoubleToStr((ma2[kk]-Bid)/Point,0),Text_Size, "Arial Bold", White);
      ObjectSet("ma2"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("ma2"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("ma2"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*3);
      if (ma2[kk]>Bid) {
         ObjectSet("ma2"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
      }else{
         ObjectSet("ma2"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
      }

      ObjectDelete("ma3"+kk+Period()+Symbol());
      ObjectCreate("ma3"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("ma3"+kk+Period()+Symbol(),DoubleToStr((ma3[kk]-Bid)/Point,0),Text_Size, "Arial Bold", White);
      ObjectSet("ma3"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("ma3"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("ma3"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*4);
      if (ma3[kk]>Bid) {
         ObjectSet("ma3"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
      }else{
         ObjectSet("ma3"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
      }

      ObjectDelete("cci"+kk+Period()+Symbol());
      ObjectCreate("cci"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("cci"+kk+Period()+Symbol(),DoubleToStr(cci[kk],0),Text_Size, "Arial Bold", White);
      ObjectSet("cci"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("cci"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("cci"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*5);
      if (cci[kk]>CCI_Value) {
         ObjectSet("cci"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
      }else{
         ObjectSet("cci"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
      }


      if (bbu[kk]-Bid>0) {
         c=UP;
         c1="5";
         s="+";
      }else{
         c=DN;
         c1="6";
         s="-";
      }
      ObjectDelete("bbu"+kk+Period()+Symbol());
      ObjectCreate("bbu"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("bbu"+kk+Period()+Symbol(),"U"+s+DoubleToStr(MathAbs((bbu[kk]-Bid)/Point),0),Text_Size-5, "Arial Bold", c);
      ObjectSet("bbu"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("bbu"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("bbu"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*6);
      ObjectDelete("bbua"+kk+Period()+Symbol());
      ObjectCreate("bbua"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("bbua"+kk+Period()+Symbol(),c1,Text_Size-5, "Webdings", c);
      ObjectSet("bbua"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("bbua"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+50);
      ObjectSet("bbua"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*6);

      if (bbd[kk]-Bid>0) {
         c=UP;
         c1="5";
         s="+";
      }else{
         c=DN;
         c1="6";
         s="-";
      }
      ObjectDelete("bbd"+kk+Period()+Symbol());
      ObjectCreate("bbd"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("bbd"+kk+Period()+Symbol(),"D"+s+DoubleToStr(MathAbs((bbd[kk]-Bid)/Point),0),Text_Size-5, "Arial Bold", c);
      ObjectSet("bbd"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("bbd"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+80);
      ObjectSet("bbd"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*6);
      ObjectDelete("bbda"+kk+Period()+Symbol());
      ObjectCreate("bbda"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("bbda"+kk+Period()+Symbol(),c1,Text_Size-5, "Webdings", c);
      ObjectSet("bbda"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("bbda"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+130);
      ObjectSet("bbda"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*6);


      if (stochm[kk]>Stoch_Value) {
         c=UP;
         c1="5";         
      }else{
         c=DN;
         c1="6";
      }
      ObjectDelete("stm"+kk+Period()+Symbol());
      ObjectCreate("stm"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("stm"+kk+Period()+Symbol(),"K"+DoubleToStr(stochm[kk],1),Text_Size-5, "Arial Bold", c);
      ObjectSet("stm"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("stm"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("stm"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*7);
      ObjectDelete("stma"+kk+Period()+Symbol());
      ObjectCreate("stma"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("stma"+kk+Period()+Symbol(),c1,Text_Size-5, "Webdings", c);
      ObjectSet("stma"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("stma"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+50);
      ObjectSet("stma"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*7);

      if (stochs[kk]>Stoch_Value) {
         c=UP;
         c1="5";         
      }else{
         c=DN;
         c1="6";
      }
      ObjectDelete("sts"+kk+Period()+Symbol());
      ObjectCreate("sts"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("sts"+kk+Period()+Symbol(),"D"+DoubleToStr(stochs[kk],1),Text_Size-5, "Arial Bold", c);
      ObjectSet("sts"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("sts"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+80);
      ObjectSet("sts"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*7);
      ObjectDelete("stsa"+kk+Period()+Symbol());
      ObjectCreate("stsa"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("stsa"+kk+Period()+Symbol(),c1,Text_Size-5, "Webdings", c);
      ObjectSet("stsa"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("stsa"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+130);
      ObjectSet("stsa"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*7);




      ObjectDelete("atr"+kk+Period()+Symbol());
      ObjectCreate("atr"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("atr"+kk+Period()+Symbol(),DoubleToStr(atr[kk]/Point,1),Text_Size, "Arial Bold", White);
      ObjectSet("atr"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("atr"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("atr"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*8);
      ObjectDelete("atra"+kk+Period()+Symbol());
      ObjectCreate("atra"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      if (atr[kk]>=atr1[kk]) {
         ObjectSet("atr"+kk+Period()+Symbol(),OBJPROP_COLOR, UP);
         c=UP;
         c1="5";
      }else{
         ObjectSet("atr"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
         ObjectSet("atra"+kk+Period()+Symbol(),OBJPROP_COLOR, DN);
         c=DN;
         c1="6";
      }      
      ObjectSetText("atra"+kk+Period()+Symbol(),c1,Text_Size, "Webdings", c);
      ObjectSet("atra"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("atra"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+60);
      ObjectSet("atra"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*8);
      
      
      if (macdf[kk]>MACD_Value) {
         c=UP;
         c1="5";         
      }else{
         c=DN;
         c1="6";
      }      
      ObjectDelete("macdf"+kk+Period()+Symbol());
      ObjectCreate("macdf"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("macdf"+kk+Period()+Symbol(),"F"+DoubleToStr(macdf[kk]/Point,0),Text_Size-5, "Arial Bold", c);
      ObjectSet("macdf"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("macdf"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk));
      ObjectSet("macdf"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*9);
      ObjectDelete("macdfa"+kk+Period()+Symbol());
      ObjectCreate("macdfa"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("macdfa"+kk+Period()+Symbol(),c1,Text_Size-5, "Webdings", c);
      ObjectSet("macdfa"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("macdfa"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+50);
      ObjectSet("macdfa"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*9);

      if (macds[kk]>MACD_Value) {
         c=UP;
         c1="5";         
      }else{
         c=DN;
         c1="6";
      }      
      ObjectDelete("macds"+kk+Period()+Symbol());
      ObjectCreate("macds"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("macds"+kk+Period()+Symbol(),"S"+DoubleToStr(macds[kk]/Point,0),Text_Size-5, "Arial Bold", c);
      ObjectSet("macds"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("macds"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+80);
      ObjectSet("macds"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*9);
      ObjectDelete("macdsa"+kk+Period()+Symbol());
      ObjectCreate("macdsa"+kk+Period()+Symbol(),OBJ_LABEL,w,0,0);
      ObjectSetText("macdsa"+kk+Period()+Symbol(),c1,Text_Size-5, "Webdings", c);
      ObjectSet("macdsa"+kk+Period()+Symbol(),OBJPROP_CORNER, 0);
      ObjectSet("macdsa"+kk+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+30+(X_Spacing*kk)+130);
      ObjectSet("macdsa"+kk+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*9);
      
   }
    
   for (kk=0;kk<Bars;kk++){
      if (TimeHour(Time[kk])>=DayStartHour && TimeHour(Time[kk+1])<DayStartHour) break;
   }
   double dhigh=iHigh(Symbol(),0,iHighest(Symbol(),0,MODE_HIGH,kk,0));
   double dlow=iLow(Symbol(),0,iLowest(Symbol(),0,MODE_LOW,kk,0));

   ObjectDelete("dhigh"+Period()+Symbol());
   ObjectCreate("dhigh"+Period()+Symbol(),OBJ_LABEL,w,0,0);
   ObjectSetText("dhigh"+Period()+Symbol(),"H= "+DoubleToStr(dhigh,Digits),Text_Size, "Arial Bold", White);
   ObjectSet("dhigh"+Period()+Symbol(),OBJPROP_CORNER, 0);
   ObjectSet("dhigh"+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+50);
   ObjectSet("dhigh"+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*11);

   ObjectDelete("dlow"+Period()+Symbol());
   ObjectCreate("dlow"+Period()+Symbol(),OBJ_LABEL,w,0,0);
   ObjectSetText("dlow"+Period()+Symbol(),"L= "+DoubleToStr(dlow,Digits),Text_Size, "Arial Bold", White);
   ObjectSet("dlow"+Period()+Symbol(),OBJPROP_CORNER, 0);
   ObjectSet("dlow"+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+250);
   ObjectSet("dlow"+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*11);

    
   for (kk=0;kk<Bars;kk++){
      if (TimeDayOfWeek(Time[kk])==0 && TimeDayOfWeek(Time[kk+1])!=0) break;
   }   
   double whigh=iHigh(Symbol(),0,iHighest(Symbol(),0,MODE_HIGH,kk,0));
   double wlow=iLow(Symbol(),0,iLowest(Symbol(),0,MODE_LOW,kk,0));
   
   ObjectDelete("whigh"+Period()+Symbol());
   ObjectCreate("whigh"+Period()+Symbol(),OBJ_LABEL,w,0,0);
   ObjectSetText("whigh"+Period()+Symbol(),"H= "+DoubleToStr(whigh,Digits),Text_Size, "Arial Bold", White);
   ObjectSet("whigh"+Period()+Symbol(),OBJPROP_CORNER, 0);
   ObjectSet("whigh"+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+50);
   ObjectSet("whigh"+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*12);

   ObjectDelete("wlow"+Period()+Symbol());
   ObjectCreate("wlow"+Period()+Symbol(),OBJ_LABEL,w,0,0);
   ObjectSetText("wlow"+Period()+Symbol(),"L= "+DoubleToStr(wlow,Digits),Text_Size, "Arial Bold", White);
   ObjectSet("wlow"+Period()+Symbol(),OBJPROP_CORNER, 0);
   ObjectSet("wlow"+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+250);
   ObjectSet("wlow"+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*12);
   
      
   if (Bid>last_bid) {
      c=UP;
      c1="5";         
   }else if (Bid<last_bid) {
      c=DN;
      c1="6";
   }else{
      c1="";
      c=White;
   }
   ObjectDelete("cur"+Period()+Symbol());
   ObjectCreate("cur"+Period()+Symbol(),OBJ_LABEL,w,0,0);
   ObjectSetText("cur"+Period()+Symbol(),DoubleToStr(Bid,Digits),Text_Size, "Arial Bold", c);
   ObjectSet("cur"+Period()+Symbol(),OBJPROP_CORNER, 0);
   ObjectSet("cur"+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+50);
   ObjectSet("cur"+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*13);
   ObjectDelete("cura"+Period()+Symbol());
   ObjectCreate("cura"+Period()+Symbol(),OBJ_LABEL,w,0,0);
   ObjectSetText("cura"+Period()+Symbol(),c1,Text_Size, "Webdings", c);
   ObjectSet("cura"+Period()+Symbol(),OBJPROP_CORNER, 0);
   ObjectSet("cura"+Period()+Symbol(),OBJPROP_XDISTANCE, Start_X+X_Spacing+140);
   ObjectSet("cura"+Period()+Symbol(),OBJPROP_YDISTANCE, Start_Y+Text_Size*2+Y_Spacing*13);

   return(0);
  }


