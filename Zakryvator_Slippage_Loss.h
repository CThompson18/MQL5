
//+------------------------------------------------------------------+
//|                                                   Zakryvator.mq4 |
//|                                 Copyright 2015-2016, Assurkov.ru |
//|                                           http://www.assurkov.ru
/*The EA closes orders, opened in the terminal, when certain loss is reached by them in the account currency. Including the orders opened for all other pairs on which the adviser is not installed. It can be put on any currency pair and it will keep track of all open orders.

It is possible to vary the loss by the volume of the order. For example, by default the maximum loss equals 4 (min_001_002=4) for orders with the volume of 0,01-0,02 lots (if the account is in dollars, then $4), and equals 30 (min_from1=30) at order volumes greater than 1 lot.

Attention: possible slippage depending on the market conditions, as well as the broker!

Input Parameters:
◾min_001_002 — The maximum loss per orders with volumes starting from 0,01 (inclusive) to 0,02 lots (inclusive);
◾min_002_005 — The maximum loss per orders with volumes starting from 0,02 (inclusive) to 0,05 lots (inclusive);
◾min_005_01 — The maximum loss per orders with volumes starting from 0,05 (inclusive) to 0,1 lots (inclusive);
◾min_01_03 — The maximum loss per orders with volumes starting from 0,1 (inclusive) to 0,3 lots (inclusive);
◾min_03_05 — The maximum loss per orders with volumes starting from 0,3 (inclusive) to 0,5 lots (inclusive);
◾min_05_1 — The maximum loss per orders with volumes starting from 0,5 (inclusive) to 1 lot (inclusive);
◾min_from1 — The maximum loss per orders with volumes greater than 1 lot;
 */
 |
//+------------------------------------------------------------------+
#property strict
#property copyright "Copyright © 2015-2016, www.assurkov.ru, Alexey Surkov"
#property link      "http://www.assurkov.ru/"
#property version     "1.00"

input double min_001_002 = 4;
input double min_002_005 = 8;
input double min_005_01  = 10;
input double min_01_03   = 15;
input double min_03_05   = 20;
input double min_05_1    = 25;
input double min_from1   = 30;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick()
  {
   int _GetLastError=0,Dig,_OrdersTotal=OrdersTotal();
   for(int z=_OrdersTotal-1; z>=0; z --)
     {
      if(!OrderSelect(z,SELECT_BY_POS))
        {
         _GetLastError=GetLastError();
         Print("OrderSelect( ",z,", SELECT_BY_POS ) - Error #",_GetLastError);
         continue;
        }

      //   min_001_002  
      if(OrderLots()<=0.02 && OrderProfit()<=min_001_002*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_002_005
      if((OrderLots()>0.02 && OrderLots()<=0.05) && OrderProfit()<=min_002_005*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_005_01
      if((OrderLots()>0.05 && OrderLots()<=0.10) && OrderProfit()<=min_005_01*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_01_03
      if((OrderLots()>0.10 && OrderLots()<=0.30) && OrderProfit()<=min_01_03*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_03_05
      if((OrderLots()>0.30 && OrderLots()<=0.50) && OrderProfit()<=min_03_05*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),10,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_05_1
      if((OrderLots()>0.50 && OrderLots()<=1) && OrderProfit()<=min_05_1*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }

      //   min_from1
      if((OrderLots()>1) && OrderProfit()<=min_from1*(-1))
        {
         if(OrderType()==OP_BUY)
           {
            Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
            if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Dig),2,Violet))
               Print("OrderClose error ",GetLastError());
            return;
           }
         else
           {
            if(OrderType()==OP_SELL)
              {
               Dig=(int)MarketInfo(OrderSymbol(),MODE_DIGITS);
               if(!OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Dig),2,Violet))
                  Print("OrderClose error ",GetLastError());
               return;
              }
           }
        }
     }
  }
//+------------------------------------------------------------------+

