//+------------------------------------------------------------------+
//|                                            X-Meter Dashboard.mq4 |
//|                                                                  |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_separate_window
#property indicator_buffers 1

extern int       Periods=121;
extern int       Shift=5;
extern int       ApplyTo=0;

extern int       Atr_Period=50;



extern int Trade_Bar=0;
extern int Precision=1;

double ExtMapBuffer1[];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
   SetIndexStyle(0,DRAW_NONE);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexLabel(0, "X-Meter"); 
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
   return(0);
  }


int timeframe[7]={1,5,15,30,60,240,1440};
//string currency[17]={"EURUSD","GBPUSD","AUDUSD","NZDUSD","USDCHF","USDCAD","USDJPY","GBPJPY","EURJPY","AUDJPY","NZDJPY","CADJPY","GBPCHF","EURCHF","EURGBP","EURCAD","GBPAUD"};

int start()
  {
     for (int j=0;j<7;j++){
         double atr = iATR(Symbol(),timeframe[j],Atr_Period,Trade_Bar);
         double tma = iCustom(Symbol(),timeframe[j],"TMA",Periods,Shift,ApplyTo,0,Trade_Bar);
         if (atr == 0) atr = 1;
         double v1=MathAbs(MarketInfo(Symbol(),MODE_BID)-tma)/atr;
         if (MarketInfo(Symbol(),MODE_BID)<tma)
         {
            ExtMapBuffer1[j] = -1 * NormalizeDouble(v1,2);
         }
         else
         {
            ExtMapBuffer1[j] = NormalizeDouble(v1,2);
         }
      }
      return(0);
  }

