//+------------------------------------------------------------------+
//|                                  Opening and Сlosing on time.mq5 |
//|                              Copyright © 2016, Vladimir Karputov |
//|                                           http://wmua.ru/slesar/ |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2016, Vladimir Karputov"
#property link      "http://wmua.ru/slesar/"
#property version   "1.000"

#include <Trade\PositionInfo.mqh>
#include <Trade\Trade.mqh>
CPositionInfo  m_position;                            // trade position object
CTrade         m_trade;                               // trading object
//--- input parameters
input datetime m_time_open=D'1980.07.19 13:00:00';    // opening time (only HH:mm are considered)
input datetime m_time_close=D'1980.07.19 13:01:00';   // closing time (only HH:mm are considered)
input string   m_symbol="EURUSD";                     // symbol
input double   m_lots=1.0;                            // volume transaction
ushort   m_sl=0;                                // sell stop
ushort   m_tp=0;                                // take profit 
input bool     m_buy=true;                            // true -> Buy, false -> Sell
input ulong    m_magic=19624;                         // magic number
//---
bool IF_POSITION_ALREADY_OPEN=false;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---
   if(m_time_open==m_time_close)
      return(INIT_PARAMETERS_INCORRECT);

   if(m_lots<=0.0)
     {
      Print("The \"volume transaction\" can't be smaller or equal to zero");
      return(INIT_PARAMETERS_INCORRECT);
     }

   string err_text="";
   if(!CheckVolumeValue(m_lots,err_text))
     {
      Print(err_text);
      return(INIT_PARAMETERS_INCORRECT);
     }

   if(!SymbolsFind(m_symbol))
     {
      Print("The ",m_symbol," symbol isn't found in MarketWatch");
      return(INIT_PARAMETERS_INCORRECT);
     }

   m_trade.SetExpertMagicNumber(m_magic);
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
   MqlDateTime str_time_current,str_time_open,str_time_close;
   TimeToStruct(TimeCurrent(),str_time_current);
   TimeToStruct(m_time_open,str_time_open);
   TimeToStruct(m_time_close,str_time_close);

   if(!IF_POSITION_ALREADY_OPEN)
     {
      if(str_time_current.hour==str_time_open.hour)
         if(str_time_current.min==str_time_open.min)
           {
            if(m_buy)
              {
               if(!CheckMoneyForTrade(Symbol(),m_lots,ORDER_TYPE_BUY))
                  return;
              }
            else
              {
               if(!CheckMoneyForTrade(Symbol(),m_lots,ORDER_TYPE_SELL))
                  return;
              }
            //--- let's open the position
            if(m_buy)
              {
               m_trade.Buy(m_lots,Symbol());
              }
            else
              {
               m_trade.Sell(m_lots,Symbol());
              }
            IF_POSITION_ALREADY_OPEN=true;
           }
     }
   else
     {
      if(str_time_current.hour==str_time_close.hour)
         if(str_time_current.min==str_time_close.min)
           {
            //--- let's close the position
            for(int i=PositionsTotal()-1;i>=0;i--)
              {
               m_position.SelectByIndex(i);
               if(m_position.Magic()==m_magic)
                 {
                  m_trade.PositionClose(m_position.Ticket());
                 }
              }
            IF_POSITION_ALREADY_OPEN=false;
           }
     }
  }
//+------------------------------------------------------------------+
//| Check the correctness of the order volume                        |
//+------------------------------------------------------------------+
bool CheckVolumeValue(double volume,string &error_description)
  {
//--- minimal allowed volume for trade operations
   double min_volume=SymbolInfoDouble(Symbol(),SYMBOL_VOLUME_MIN);
   if(volume<min_volume)
     {
      error_description=StringFormat("Volume is less than the minimal allowed SYMBOL_VOLUME_MIN=%.2f",min_volume);
      return(false);
     }

//--- maximal allowed volume of trade operations
   double max_volume=SymbolInfoDouble(Symbol(),SYMBOL_VOLUME_MAX);
   if(volume>max_volume)
     {
      error_description=StringFormat("Volume is greater than the maximal allowed SYMBOL_VOLUME_MAX=%.2f",max_volume);
      return(false);
     }

//--- get minimal step of volume changing
   double volume_step=SymbolInfoDouble(Symbol(),SYMBOL_VOLUME_STEP);

   int ratio=(int)MathRound(volume/volume_step);
   if(MathAbs(ratio*volume_step-volume)>0.0000001)
     {
      error_description=StringFormat("Volume is not a multiple of the minimal step SYMBOL_VOLUME_STEP=%.2f, the closest correct volume is %.2f",
                                     volume_step,ratio*volume_step);
      return(false);
     }
   error_description="Correct volume value";
   return(true);
  }
//+------------------------------------------------------------------+
//| Symbols Find                                                     |
//+------------------------------------------------------------------+
bool SymbolsFind(const string name_symbol)
  {
   int m_total=SymbolsTotal(false);
   for(int i=0;i<m_total;i++)
     {
      if(SymbolName(i,false)==name_symbol)
        {
         return(true);
         break;
        }
     }
//---
   return(false);
  }
//+------------------------------------------------------------------+
//| Check Money For Trade                                            |
//+------------------------------------------------------------------+
bool CheckMoneyForTrade(string symb,double lots,ENUM_ORDER_TYPE type)
  {
//--- Getting the opening price
   MqlTick mqltick;
   SymbolInfoTick(symb,mqltick);
   double price=mqltick.ask;
   if(type==ORDER_TYPE_SELL)
      price=mqltick.bid;
//--- values of the required and free margin
   double margin,free_margin=AccountInfoDouble(ACCOUNT_MARGIN_FREE);
//--- call of the checking function
   if(!OrderCalcMargin(type,symb,lots,price,margin))
     {
      //--- something went wrong, report and return false
      Print("Error in ",__FUNCTION__," code=",GetLastError());
      return(false);
     }
//--- if there are insufficient funds to perform the operation
   if(margin>free_margin)
     {
      //--- report the error and return false
      Print("Not enough money for ",EnumToString(type)," ",lots," ",symb," Error code=",GetLastError());
      return(false);
     }
//--- checking successful
   return(true);
  }
//+------------------------------------------------------------------+
