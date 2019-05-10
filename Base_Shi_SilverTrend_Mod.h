//+------------------------------------------------------------------+
//|                                     Base_Shi_SilverTrend_Mod.mq5 |
//|                                  Copyright 2017,CJThompson Corp. |
//|                                               https://www.CJ.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2017,CJThompson Corp."
#property link      "https://www.CJ.com"
#property version   "1.00"

#property description "Base_Shi_SilverTrend_Mod"
//---- indicator version
#property version   "1.00"
//---- drawing the indicator in the main window
#property indicator_chart_window
//---- two buffers are used for calculation and drawing the indicator
#property indicator_buffers 2
//---- only two plots are used
#property indicator_plots   2
//+----------------------------------------------+
//|  Bearish indicator drawing parameters        |
//+----------------------------------------------+
//---- drawing the indicator 1 as a symbol
#property indicator_type1   DRAW_ARROW
//---- magenta color is used as the color of the bearish indicator line
#property indicator_color1  Magenta
//---- thickness of the indicator 1 line is equal to 4
#property indicator_width1  4
//---- bullish indicator label display
#property indicator_label1  "ASCtrend Sell"
//+----------------------------------------------+
//|  Bullish indicator drawing parameters        |
//+----------------------------------------------+
//---- drawing the indicator 2 as a symbol
#property indicator_type2   DRAW_ARROW
//---- blue color is used as the color of a bullish candlestick
#property indicator_color2  Blue
//---- thickness of the indicator 2 line is equal to 4
#property indicator_width2  4
//---- bearish indicator label display
#property indicator_label2 "ASCtrend Buy"

#define RESET 0 // The constant for getting the command for the indicator recalculation back to the terminal
//+----------------------------------------------+
//|  Indicator input parameters                  |
//+----------------------------------------------+
input double RISK=10.25;
//+----------------------------------------------+

//---- declaration of dynamic arrays that further
// will be used as indicator buffers
double SellBuffer[];
double BuyBuffer[];
//----
extern int     AllBars=0;//Äëÿ ñêîëüêè áàðîâ ñ÷èòàòü. 0 - äëÿ âñåõ.
extern int     Otstup=30;//Îòñòóï.
extern double  Per=9;//Ïåðèîä.
int            SH,NB,i,UD;
double         R,SHMax,SHMin;
int  StartBars;
#define  SH_BUY   1
#define  SH_SELL  -1

double Ask;
double Bid;
double Bars;
double Open[];
double Close[];
double High[];
double Low[];
datetime Time[];
long Volume[];
int prev_calculated;
   int IndicatorCounted()
  {
   if(prev_calculated>0) return(prev_calculated-1);
   if(prev_calculated==0) return(0);
   return(0);
  }
void Set_Values_to_variables()
{
   MqlTick last_tick;
   SymbolInfoTick(_Symbol,last_tick);
   Ask=last_tick.ask;
   Bid=last_tick.bid;
  
   ArraySetAsSeries(Close,true);
   CopyClose(_Symbol,_Period,0,Bars(_Symbol,_Period),Close);
   ArraySetAsSeries(Open,true);
   CopyOpen(_Symbol,_Period,0,Bars(_Symbol,_Period),Open);
   ArraySetAsSeries(Low,true);
   CopyLow(_Symbol,_Period,0,Bars(_Symbol,_Period),Low);
   ArraySetAsSeries(High,true);
   CopyHigh(_Symbol,_Period,0,Bars(_Symbol,_Period),High);
   ArraySetAsSeries(Time,true);
   CopyTime(_Symbol,_Period,0,Bars(_Symbol,_Period),Time);
   ArraySetAsSeries(Volume,true);
   CopyTickVolume(_Symbol,_Period,0,Bars(_Symbol,_Period),Volume);
}  
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int OnInit()
  {
      Set_Values_to_variables();
      Bars=Bars(_Symbol,_Period);
//--- indicator buffers mapping
//---- set dynamic array as an indicator buffer
   SetIndexBuffer(0,SellBuffer,INDICATOR_DATA);
//---- shifting the start of drawing the indicator 1
   PlotIndexSetInteger(0,PLOT_DRAW_BEGIN,StartBars);
//---- create a label to display in DataWindow
   PlotIndexSetString(0,PLOT_LABEL,"ASCtrend Sell");
//---- indicator symbol
   PlotIndexSetInteger(0,PLOT_ARROW,108);
//---- indexing the elements in the buffer as timeseries
   ArraySetAsSeries(SellBuffer,true);
//---- setting values of the indicator that won't be visible on a chart
   PlotIndexSetDouble(0,PLOT_EMPTY_VALUE,0);

//---- set dynamic array as an indicator buffer
   SetIndexBuffer(1,BuyBuffer,INDICATOR_DATA);
//---- shifting the start of drawing the indicator 2
   PlotIndexSetInteger(1,PLOT_DRAW_BEGIN,StartBars);
//---- create a label to display in DataWindow
   PlotIndexSetString(1,PLOT_LABEL,"ASCtrend Buy");
//---- indicator symbol
   PlotIndexSetInteger(1,PLOT_ARROW,108);
//---- indexing the elements in the buffer as timeseries
   ArraySetAsSeries(BuyBuffer,true);
//---- setting values of the indicator that won't be visible on a chart
   PlotIndexSetDouble(1,PLOT_EMPTY_VALUE,0);

//---- Setting the format of accuracy of displaying the indicator
   IndicatorSetInteger(INDICATOR_DIGITS,_Digits);
//---- name for the data window and the label for tooltips
   string short_name="Base_Shi_SilverTrend_Mod";
   IndicatorSetString(INDICATOR_SHORTNAME,short_name);
   ArrayInitialize(SellBuffer,0.0);//Çàáü¸ì îáà áóôåðà íîëèêàìè. Èíà÷å áóäåò ìóñîð ïðè ñìåíå òàéìôðåéìà.
   ArrayInitialize(BuyBuffer,0.0);
//----     
//---
   return(INIT_SUCCEEDED);
  }
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
//---
   Set_Values_to_variables();
   int CB=IndicatorCounted();
   
   if(CB<0) return(-1); else if(CB==0) NB=Bars; //else NB=Bars(_Symbol,_Period);
   /*for(int n=0;n<=NB;n++)
      {
         BuyBuffer[n]=0.00;
         SellBuffer[n]=0.00;
      }*/
   for (SH=1;SH<NB;SH++)//Ïðî÷¸ñûâàåì ãðàôèê îò 1 äî NB
   {
      for (R=0,i=SH;i<SH+10;i++) {R+=(10+SH-i)*(High[i]-Low[i]);}      
      R/=55;

      //SHMax = High[Highest(NULL,0,MODE_HIGH,Per,SH)];
      SHMax = ArrayMaximum(high,0,SH);
      SHMin = ArrayMinimum(Low,0,SH);
      //SHMin = Low[Lowest(NULL,0,MODE_LOW,Per,SH)];
      if (Close[SH]<SHMin+(SHMax-SHMin)*Otstup/100 && UD!=SH_SELL) { BuyBuffer[SH]=Low[SH]-R*0.5; UD=SH_SELL; }
      if (Close[SH]>SHMax-(SHMax-SHMin)*Otstup/100 && UD!=SH_BUY) { SellBuffer[SH]=High[SH]+R*0.5; UD=SH_BUY; }
   }
//--- return value of prev_calculated for next call
   return(rates_total);
  }
//+------------------------------------------------------------------+
