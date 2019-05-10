//+------------------------------------------------------------------+
//|                                                MasterWindows.mq5 |
//|                                                 Copyright DC2008 |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "DC2008"
#property link      "http://www.mql5.com"
#property version   "1.00"
//--- include file with class
#include <ClassMasterWindows.mqh>
//--- class declaration
CMasterWindows    MasterWin;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//--- run MasterWin
   MasterWin.Run();
   return(0);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//--- call MasterWin deinitialization
   MasterWin.Deinit();
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//--- call MasterWin OnTick event processing method
   MasterWin.OnEventTick();
  }
//+------------------------------------------------------------------+
//| Expert Event function                                            |
//+------------------------------------------------------------------+
void OnChartEvent(const int id,
                  const long &lparam,
                  const double &dparam,
                  const string &sparam)
  {
//--- call MasterWin OnChartEvent event processing method
   MasterWin.OnEvent(id,lparam,dparam,sparam);
  }
//+------------------------------------------------------------------+
