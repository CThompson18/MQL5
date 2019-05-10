//+------------------------------------------------------------------+
//|                                                    rsi_color.mq4 |
//+------------------------------------------------------------------+

#property indicator_separate_window
#property indicator_minimum 1
#property indicator_maximum 100
#property indicator_buffers 5
#property indicator_color1 Gold
#property indicator_color2 Lime
#property indicator_color3 Green
#property indicator_color4 Red
#property indicator_color5 FireBrick
#property indicator_level1 80
#property indicator_level2 70
#property indicator_level3 30
#property indicator_level4 20
extern int RSI_Period=14;
extern int RSI_Price=PRICE_CLOSE;

extern int CountBars=500;

extern double UP_Level1=80;
extern double UP_Level2=75;
extern double DN_Level1=20;
extern double DN_Level2=25;
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
   IndicatorShortName("4Xlounge_RSI_Levels");
//----
   return(0);
  }
int deinit()
  {
   ObjectDelete("rsi_line"+Symbol()+Period());
   return(0);
  }
int start()
  {
   int w=WindowFind("RSI_Color_Levels");
   if (CountBars>Bars){int limit=Bars-RSI_Period;}else{limit=CountBars;}
   for (int i=limit;i>=0;i--){
      ExtMapBuffer1[i]=EMPTY_VALUE;
      ExtMapBuffer2[i]=EMPTY_VALUE;
      ExtMapBuffer3[i]=EMPTY_VALUE;
      ExtMapBuffer4[i]=EMPTY_VALUE;
      ExtMapBuffer5[i]=EMPTY_VALUE;   
      double rsi0=iRSI(Symbol(),0,RSI_Period,RSI_Price,i);
      double rsi1=iRSI(Symbol(),0,RSI_Period,RSI_Price,i+1);
      if (rsi0>UP_Level1) {
         ExtMapBuffer2[i]=rsi0;
         if (ExtMapBuffer2[i+1]==EMPTY_VALUE) ExtMapBuffer2[i+1]=rsi1;
      }else 
      if (rsi0>UP_Level2 && rsi0<=UP_Level1) {
         ExtMapBuffer3[i]=rsi0;
         if (ExtMapBuffer3[i+1]==EMPTY_VALUE) ExtMapBuffer3[i+1]=rsi1;
      }else  
      if (rsi0<DN_Level1) {
         ExtMapBuffer4[i]=rsi0;
         if (ExtMapBuffer4[i+1]==EMPTY_VALUE) ExtMapBuffer4[i+1]=rsi1;
      }else       
      if (rsi0>=DN_Level1 && rsi0<DN_Level2) {
         ExtMapBuffer5[i]=rsi0;
         if (ExtMapBuffer5[i+1]==EMPTY_VALUE) ExtMapBuffer5[i+1]=rsi1;
      }else {
         ExtMapBuffer1[i]=rsi0;
         if (ExtMapBuffer1[i+1]==EMPTY_VALUE) ExtMapBuffer1[i+1]=rsi1;        
      } 
      ObjectDelete("rsi_line"+Symbol()+Period());
  }
   return(0);
  }

