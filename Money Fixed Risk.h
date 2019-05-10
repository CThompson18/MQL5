//+------------------------------------------------------------------+
//|                                             Money Fixed Risk.mq5 |
//|                              Copyright © 2016, Vladimir Karputov |
//|                                           http://wmua.ru/slesar/ |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2016, Vladimir Karputov"
#property link      "http://wmua.ru/slesar/"
#property version   "1.001"
#include <Trade\Trade.mqh>
#include <Trade\SymbolInfo.mqh>  
#include <Trade\AccountInfo.mqh>
#include <Expert\Money\MoneyFixedRisk.mqh>
CTrade         m_trade;                      // trading object
CSymbolInfo    m_symbol;                     // symbol info object
CAccountInfo   m_account;                    // account info wrapper
CMoneyFixedRisk m_money;
//---
input ushort   InpStopLoss=25;               // StopLoss (in pips)
input double   PercentRisk=10;               // % risk
//---
double ExtStopLoss=0.0;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
   m_symbol.Name(Symbol());
   m_symbol.Refresh();
   if(!RefreshRates())
     {
      Print("Error RefreshRates. Bid=",DoubleToString(m_symbol.Bid(),Digits()),
            ", Ask=",DoubleToString(m_symbol.Ask(),Digits()));
      return(INIT_FAILED);
     }
//--- tuning for 3 or 5 digits
   int digits_adjust=1;
   if(m_symbol.Digits()==3 || m_symbol.Digits()==5)
      digits_adjust=10;
   ExtStopLoss=InpStopLoss*m_symbol.Point()*digits_adjust;
//---
   if(!m_money.Init(GetPointer(m_symbol),Period(),m_symbol.Point()*digits_adjust))
      return(INIT_FAILED);
   m_money.Percent(PercentRisk); // 10% risk
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//---

  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---
   if(!RefreshRates())
      return;
//--- protection against the return value of "zero"
   if(m_symbol.Ask()==0 || m_symbol.Bid()==0)
      return;

   static long count=-21;
   if(count%980==0) // we pass 980 tics
     {
      //--- getting lot size for open long position (CMoneyFixedRisk)
      double sl=0.0;
      double check_open_long_lot=0.0;
      //--- variant #1: StopLoss=0.0
      sl=0.0;
      check_open_long_lot=m_money.CheckOpenLong(m_symbol.Ask(),sl);
      Print("sl=0.0",
            ", CheckOpenLong: ",DoubleToString(check_open_long_lot,2),
            ", Balance: ",    DoubleToString(m_account.Balance(),2),
            ", Equity: ",     DoubleToString(m_account.Equity(),2),
            ", FreeMargin: ", DoubleToString(m_account.FreeMargin(),2));
      //--- variant #2: StopLoss!=0.0
      sl=m_symbol.Bid()-ExtStopLoss;
      check_open_long_lot=m_money.CheckOpenLong(m_symbol.Ask(),sl);
      Print("sl=",DoubleToString(sl,m_symbol.Digits()),
            ", CheckOpenLong: ",DoubleToString(check_open_long_lot,2),
            ", Balance: ",    DoubleToString(m_account.Balance(),2),
            ", Equity: ",     DoubleToString(m_account.Equity(),2),
            ", FreeMargin: ", DoubleToString(m_account.FreeMargin(),2));
      if(check_open_long_lot==0.0)
         return;

      //--- check volume before OrderSend to avoid "not enough money" error (CTrade)
      double chek_volime_lot=m_trade.CheckVolume(m_symbol.Name(),check_open_long_lot,m_symbol.Ask(),ORDER_TYPE_BUY);

      if(chek_volime_lot!=0.0)
         if(chek_volime_lot>=check_open_long_lot)
           {
            if(m_trade.Buy(chek_volime_lot,NULL,m_symbol.Ask(),m_symbol.Bid()-ExtStopLoss,m_symbol.Bid()+ExtStopLoss))
              {
               if(m_trade.ResultDeal()==0)
                  count--;
              }
            else
               count--;
           }
      else
         Print("CMoneyFixedRisk lot = ",DoubleToString(check_open_long_lot,2),
               ", CTrade lot = ",DoubleToString(chek_volime_lot,2));
      //---
     }
   count++;
  }
//+------------------------------------------------------------------+
//| Refreshes the symbol quotes data                                 |
//+------------------------------------------------------------------+
bool RefreshRates()
  {
//--- refresh rates
   if(!m_symbol.RefreshRates())
      return(false);
//--- protection against the return value of "zero"
   if(m_symbol.Ask()==0 || m_symbol.Bid()==0)
      return(false);
//---
   return(true);
  }
//+------------------------------------------------------------------+
