//+------------------------------------------------------------------+
//|                                                      X-Meter.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window
#property indicator_buffers 2
#property indicator_color1 Lime
#property indicator_color2 Red

extern int       Periods=121;
extern int       Shift=0;
extern int       ApplyTo=0;

extern int       Atr_Period=50;
extern int       Count_Bars=1000;

//---- buffers
double ExtMapBuffer1[];
double ExtMapBuffer2[];

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_HISTOGRAM);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexStyle(1,DRAW_HISTOGRAM);
   SetIndexBuffer(1,ExtMapBuffer2);

//----
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
   int l=0;
   if (Count_Bars>Bars) {l=Bars;}else{l=Count_Bars;}
   for (int i=l;i>=0;i--){
      ExtMapBuffer1[i]=0;
      ExtMapBuffer2[i]=0;
   
      double t=0,a=0;
      t=iCustom(Symbol(),0,"TMA",Periods,Shift,ApplyTo,0,i);
      a=iATR(Symbol(),0,Atr_Period,i);
      if (a==0) a=1;
      
      double xmeter=(Close[i]-t)/a;   
      if (xmeter >0) {
         ExtMapBuffer1[i]=xmeter;
      }else{
         ExtMapBuffer2[i]=xmeter;
      }
   }   
   return(0);
  }
//+------------------------------------------------------------------+