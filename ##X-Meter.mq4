//+------------------------------------------------------------------+
//|                                                 4Xlounge X Meter |
//|         Copyright © 2008, Dan Campbell, d.d.campbell@comcast.net |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2008, Dan Campbell"
#property link      ""

#property indicator_chart_window
#property indicator_buffers 5
#property indicator_color1 Yellow
#property indicator_color2 Red
#property indicator_color3 Red
#property indicator_color4 Lime
#property indicator_color5 Lime

extern int       Periods=60;
extern int       Shift=5;
extern int       ApplyTo=0;

extern int       Atr_Period=50;

extern double    Bands_Distance=15;
extern double    Bands_Width=2;
extern color     Up_Color=C'87,87,87';
extern color     Dn_Color=C'87,87,87';

extern int       Count_Bars=500;

//---- buffers
double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double ExtMapBuffer5[];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_LINE);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexStyle(1,DRAW_LINE);
   SetIndexBuffer(1,ExtMapBuffer2);
   SetIndexStyle(2,DRAW_LINE);
   SetIndexBuffer(2,ExtMapBuffer3);
   SetIndexStyle(3,DRAW_LINE);
   SetIndexBuffer(3,ExtMapBuffer4);
   SetIndexStyle(4,DRAW_LINE);
   SetIndexBuffer(4,ExtMapBuffer5);
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   for (int i=Count_Bars;i>=0;i--){
      ObjectDelete("X-meter1"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0));
      ObjectDelete("X-meter2"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0));   
   }
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
int start()
  {
   int l=0;
   if (Count_Bars>Bars) {l=Bars;}else{l=Count_Bars;}
   for (int i=l;i>=0;i--){
      ExtMapBuffer1[i]=EMPTY_VALUE;
      ExtMapBuffer2[i]=EMPTY_VALUE;
      ExtMapBuffer3[i]=EMPTY_VALUE;
      ExtMapBuffer4[i]=EMPTY_VALUE;
      ExtMapBuffer5[i]=EMPTY_VALUE;
      double atr=iATR(Symbol(),0,Atr_Period,i);
      ExtMapBuffer1[i]=iCustom(Symbol(),0,"TMA",Periods,Shift,ApplyTo,0,i);
      ExtMapBuffer2[i]=ExtMapBuffer1[i]+atr*Bands_Distance;
      ExtMapBuffer4[i]=ExtMapBuffer1[i]-atr*Bands_Distance;
      ExtMapBuffer3[i]=ExtMapBuffer2[i]+atr*Bands_Width;
      ExtMapBuffer5[i]=ExtMapBuffer4[i]-atr*Bands_Width;
      if (i!=l){
         ObjectDelete("X-meter1"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0));
         ObjectCreate("X-meter1"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0),16,0,Time[i+1],ExtMapBuffer2[i],Time[i],ExtMapBuffer3[i]);
         ObjectSet("X-meter1"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0),6,Up_Color);
         ObjectDelete("X-meter2"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0));
         ObjectCreate("X-meter2"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0),16,0,Time[i+1],ExtMapBuffer4[i],Time[i],ExtMapBuffer5[i]);
         ObjectSet("X-meter2"+Symbol()+Period()+DoubleToStr(Periods,0)+DoubleToStr(Atr_Period,0)+DoubleToStr(i,0),6,Dn_Color);
      }         
   }   
   return(0);
  }
//+------------------------------------------------------------------+