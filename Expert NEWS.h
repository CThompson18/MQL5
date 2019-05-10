//+------------------------------------------------------------------+
//|                         Expert NEWS(barabashkakvn's edition).mq5 | 
//|                              Copyright © 2014, Khlystov Vladimir |
//|                                         http://cmillion.narod.ru |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2014, cmillion@narod.ru"
#property link      "http://cmillion.ru"
#include <Trade\PositionInfo.mqh>
#include <Trade\Trade.mqh>
#include <Trade\SymbolInfo.mqh>  
#include <Trade\AccountInfo.mqh>
#include<Trade\OrderInfo.mqh>
CPositionInfo  m_position;                   // trade position object
CTrade         m_trade;                      // trading object
CSymbolInfo    m_symbol;                     // symbol info object
CAccountInfo   m_account;                    // account info wrapper
COrderInfo     m_order;                      // pending orders object
//--- input parameters
input ushort   InpStoploss          = 10;    //стоплосс, если 0 то не изменяется
input ushort   InpTakeprofit        = 50;    //тейкпрофит, если 0 то не изменяется
input ushort   InpTrailingStop      = 10;    //длинна тралла, если 0 то нет тралла
input ushort   InpTrailingStart     = 0;     //когда включать тралл, например после достижения 40 п прибыл
input ushort   InpStepTrall         = 2;     //шаг тралла - перемещать стоплосс не ближе чем
input ushort   InpNoLoss            = 0;     //перевод в безубыток при заданном кол-ве пунктов прибыли, если 0 то нет перевода в безубыток
input ushort   InpMinProfitNoLoss   = 0;     //минимальная прибыль при переводе в безубыток
input ulong    Magic                = 77;    //магик
input ushort   InpStep              = 10;    //расстояние от цены
input double   Lot                  = 0.1;   //лот
input ushort   TimeModify           = 300;   //кол-во секунд раньше которого запрещено изменять ордер (max 65 535)
input ushort   InpSlippage          = 30;    //Максимально допустимое отклонение цены для рыночных ордеров (ордеров на покупку или продажу).
//---
int  STOPLEVEL=0;
datetime TimeBarB=0,TimeBarS=0;
//---
double   ExtStoploss          = 0;
double   ExtTakeprofit        = 0;
double   ExtTrailingStop      = 0;
double   ExtTrailingStart     = 0;
double   ExtStepTrall         = 0;
double   ExtNoLoss            = 0;
double   ExtMinProfitNoLoss   = 0;
double   ExtStep              = 0;
ulong    ExtSlippage          = 0;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//if(m_account.MarginMode()!=ACCOUNT_MARGIN_MODE_RETAIL_HEDGING)
//  {
//   Print("Hedging only!");
//   return(INIT_FAILED);
//  }
//---
   m_symbol.Name(Symbol());                  // sets symbol name
   m_trade.SetExpertMagicNumber(Magic);      // sets magic number

   if(!RefreshRates())
     {
      Print("Error RefreshRates.",
            " Bid=",DoubleToString(m_symbol.Bid(),Digits()),
            ", Ask=",DoubleToString(m_symbol.Ask(),Digits()));
      return(INIT_FAILED);
     }
//--- tuning for 3 or 5 digits
   int digits_adjust=1;
   if(m_symbol.Digits()==3 || m_symbol.Digits()==5)
      digits_adjust=10;

   ExtStoploss          = InpStoploss        *digits_adjust;
   ExtTakeprofit        = InpTakeprofit      *digits_adjust;
   ExtTrailingStop      = InpTrailingStop    *digits_adjust;
   ExtTrailingStart     = InpTrailingStart   *digits_adjust;
   ExtStepTrall         = InpStepTrall       *digits_adjust;
   ExtNoLoss            = InpNoLoss          *digits_adjust;
   ExtMinProfitNoLoss   = InpMinProfitNoLoss *digits_adjust;
   ExtStep              = InpStep            *digits_adjust;
   ExtSlippage          = InpSlippage        *digits_adjust;

   m_trade.SetDeviationInPoints(ExtSlippage);    // sets deviation
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
   STOPLEVEL=m_symbol.StopsLevel();
   double PSL,StLo,PriceB=0,PriceS=0,PPO=0,SL=0,TP=0;
   int b=0,s=0;
   ENUM_POSITION_TYPE  PosType;
   ulong TicketB=0,TicketS=0;

   if(!RefreshRates())
      return;

   for(int i=PositionsTotal()-1;i>=0;i--)
     {
      if(m_position.SelectByIndex(i))
        {
         if(m_position.Symbol()==Symbol() && Magic==m_position.Magic())
           {
            PosType=m_position.PositionType();
            PSL = NormalizeDouble(m_position.StopLoss(),Digits());
            PPO = NormalizeDouble(m_position.PriceOpen(),Digits());
            SL=PSL;
            if(PosType==POSITION_TYPE_BUY)
              {
               b++;
               if(PSL<PPO && ExtNoLoss!=0)
                 {
                  StLo=NormalizeDouble(PPO+ExtMinProfitNoLoss*Point(),Digits());
                  if(StLo>PSL && StLo<=NormalizeDouble(m_symbol.Bid()-STOPLEVEL*Point(),Digits()))
                     SL=StLo;
                 }

               if(ExtTrailingStop>=STOPLEVEL && ExtTrailingStop!=0 && (m_symbol.Bid()-PPO)/Point()>=ExtTrailingStart)
                 {
                  StLo=NormalizeDouble(m_symbol.Bid()-ExtTrailingStop *Point(),Digits());
                  if(StLo>=PPO && StLo>PSL+ExtStepTrall*Point())
                     SL=StLo;
                 }

               if(SL>PSL)
                 {
                  if(!m_trade.PositionModify(m_position.Ticket(),SL,TP))
                     Print("Error ",m_trade.ResultRetcode(),
                           "(",m_trade.ResultRetcodeDescription(),"), Order Modify Buy SL ",PSL,"-> ",SL);
                  else
                     Print("Order Buy Modify SL ",PSL,"-> ",SL);
                 }
              }
            if(PosType==POSITION_TYPE_SELL)
              {
               s++;
               if((PSL>PPO || PSL==0) && ExtNoLoss!=0)
                 {
                  StLo=NormalizeDouble(PPO-ExtMinProfitNoLoss*Point(),Digits());
                  if((StLo<PSL || PSL==0) && StLo>=NormalizeDouble(m_symbol.Ask()+STOPLEVEL*Point(),Digits()))
                     SL=StLo;
                 }

               if(ExtTrailingStop>=STOPLEVEL && ExtTrailingStop!=0 && (PPO-m_symbol.Ask())/Point()>=ExtTrailingStart)
                 {
                  StLo=NormalizeDouble(m_symbol.Ask()+ExtTrailingStop *Point(),Digits());
                  if(StLo<=PPO && (StLo<PSL-ExtStepTrall*Point() || PSL==0))
                     SL=StLo;
                 }

               if((SL<PSL || PSL==0) && SL!=0)
                 {
                  if(!m_trade.PositionModify(m_position.Ticket(),SL,TP))
                     Print("Error ",m_trade.ResultRetcode(),
                           "(",m_trade.ResultRetcodeDescription(),"), Order Modify Sell SL ",PSL,"-> ",SL);
                  else
                     Print("Order Sell Modify SL ",PSL,"-> ",SL);
                 }
              }
           }
        }
     }

   for(int i=OrdersTotal()-1;i>=0;i--)
     {
      if(m_order.SelectByIndex(i))
        {
         if(m_order.Symbol()==Symbol() && Magic==m_order.Magic())
           {
            if(m_order.OrderType()==ORDER_TYPE_BUY_STOP)
              {
               PriceB=PPO; TicketB=m_order.Ticket();
              }
            if(m_order.OrderType()==ORDER_TYPE_SELL_STOP)
              {
               PriceS=PPO; TicketS=m_order.Ticket();
              }
           }
        }
     }

   if(b+TicketB==0)
     {
      if(ExtStoploss>=STOPLEVEL && ExtStoploss!=0)
         SL=NormalizeDouble(m_symbol.Bid()-ExtStoploss*Point(),Digits());
      else
         SL=0;

      if(ExtTakeprofit>=STOPLEVEL && ExtTakeprofit!=0)
         TP=NormalizeDouble(m_symbol.Ask()+ExtTakeprofit*Point(),Digits());
      else
         TP=0;

      if(m_trade.BuyStop(Lot,NormalizeDouble(m_symbol.Ask()+ExtStep*Point(),Digits()),
         Symbol(),SL,TP,0,0,"news"))
         TimeBarB=TimeCurrent();
     }
   if(s+TicketS==0)
     {
      if(ExtStoploss>=STOPLEVEL && ExtStoploss!=0)
         SL=NormalizeDouble(m_symbol.Ask()+ExtStoploss*Point(),Digits());
      else SL=0;

      if(ExtTakeprofit>=STOPLEVEL && ExtTakeprofit!=0)
         TP=NormalizeDouble(m_symbol.Bid()-ExtTakeprofit*Point(),Digits());
      else TP=0;

      if(m_trade.SellStop(Lot,NormalizeDouble(m_symbol.Bid()-ExtStep*Point(),Digits()),
         Symbol(),SL,TP,0,0,"news"))
         TimeBarS=TimeCurrent();
     }

   if(TicketB!=0)
     {
      if(m_order.Select(TicketB))
        {
         double order_sl=m_order.StopLoss();
         double order_tp=m_order.TakeProfit();
         double order_price=m_order.PriceOpen();
         if(TimeBarB<TimeCurrent()-TimeModify && 
            MathAbs(NormalizeDouble(m_symbol.Ask()+ExtStep*Point(),Digits())-PriceB)/Point()>ExtStepTrall)
           {
            if(ExtStoploss>=STOPLEVEL && ExtStoploss!=0)
               SL=NormalizeDouble(m_symbol.Bid()-ExtStoploss*Point(),Digits());
            else
               SL=0;

            if(ExtTakeprofit>=STOPLEVEL && ExtTakeprofit!=0)
               TP=NormalizeDouble(m_symbol.Ask()+ExtTakeprofit*Point(),Digits());
            else
               TP=0;

            double price=NormalizeDouble(m_symbol.Ask()+ExtStep*Point(),Digits());
            //--- protection against "[no changes]"
            if(!CompareDoubles(price,order_price))
               if(m_trade.OrderModify(TicketB,price,SL,TP,0,0))
                  TimeBarB=TimeCurrent();
           }
        }
     }
   if(TicketS!=0)
     {
      if(m_order.Select(TicketS))
        {
         double order_sl=m_order.StopLoss();
         double order_tp=m_order.TakeProfit();
         double order_price=m_order.PriceOpen();
         if(TimeBarS<TimeCurrent()-TimeModify && 
            MathAbs(NormalizeDouble(m_symbol.Bid()-ExtStep*Point(),Digits())-PriceS)/Point()>ExtStepTrall)
           {
            if(ExtStoploss>=STOPLEVEL && ExtStoploss!=0)
               SL=NormalizeDouble(m_symbol.Ask()+ExtStoploss*Point(),Digits());
            else SL=0;

            if(ExtTakeprofit>=STOPLEVEL && ExtTakeprofit!=0)
               TP=NormalizeDouble(m_symbol.Bid()-ExtTakeprofit*Point(),Digits());
            else TP=0;

            double price=NormalizeDouble(m_symbol.Bid()-ExtStep*Point(),Digits());
            //--- protection against "[no changes]"
            if(!CompareDoubles(price,order_price))
               if(m_trade.OrderModify(TicketS,price,SL,TP,0,0))
                  TimeBarS=TimeCurrent();
           }
        }
     }
   return;
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
//|                                                                  |
//+------------------------------------------------------------------+
bool CompareDoubles(double number1,double number2)
  {
   if(NormalizeDouble(number1-number2,Digits()-1)==0)
      return(true);
   else
      return(false);
  }
//--------------------------------------------------------------------
