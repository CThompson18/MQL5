//+------------------------------------------------------------------+
//|                                                 TMA_4xlounge.mq4 |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_chart_window
#property indicator_buffers 2
#property indicator_color1 Green
#property indicator_color2 Red
//---- buffers

extern int       Periods1=7;
extern int       Shift1=0;
extern int       ApplyTo1=0;
extern int       CountBars=1000;
extern int       Bar_Width=5;

double ExtMapBuffer1[];
double ExtMapBuffer2[];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_HISTOGRAM,EMPTY,Bar_Width);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexStyle(1,DRAW_HISTOGRAM,EMPTY,Bar_Width);
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
    if (CountBars>Bars) {l=Bars;}else{l=CountBars;}
    for (int i=l;i>=0;i--){
      double a1=iCustom(Symbol(),0,"TMA", Periods1,Shift1,ApplyTo1,0,i+1);      
      if (Open[i+1]>a1 && Close[i+1]>a1) {
         ExtMapBuffer1[i]=MathMax(Open[i],Close[i]);
         ExtMapBuffer2[i]=MathMin(Open[i],Close[i]);
      }else if (Open[i+1]<a1 && Close[i+1]<a1){
         ExtMapBuffer1[i]=MathMin(Open[i],Close[i]);
         ExtMapBuffer2[i]=MathMax(Open[i],Close[i]);      
      }
    }
    return(0);
  }
//+------------------------------------------------------------------+