//+------------------------------------------------------------------+
//|                                                  Exp_Fishing.mq5 |
//|                               Copyright © 2016, Хлыстов Владимир |
//|                                                cmillion@narod.ru |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2016, http://cmillion.ru"
#property link      "cmillion@narod.ru"
#property version   "1.00"
#property description   "Эксперт открывает позицию после направленного движения цены и производит доливки к существующей позиции при движении цены в профицитном направлении."
//+----------------------------------------------+
//  Торговые алгоритмы                           | 
//+----------------------------------------------+
#include <TradeAlgorithms.mqh>
//+----------------------------------------------+
//|  Перечисление для вариантов расчёта лота     |
//+----------------------------------------------+
/*enum MarginMode  - перечисление объявлено в файле TradeAlgorithms.mqh
  {
   FREEMARGIN=0,     //MM от свободных средств на счёте
   BALANCE,          //MM от баланса средств на счёте
   LOSSFREEMARGIN,   //MM по убыткам от свободных средств на счёте
   LOSSBALANCE,      //MM по убыткам от баланса средств на счёте
   LOT               //Лот без изменения
  }; */
//+----------------------------------------------+
//| Входные параметры индикатора эксперта        |
//+----------------------------------------------+
input double MM=0.1;              //Доля финансовых ресурсов от депозита в доливочной сделке
input MarginMode MMMode=LOT;      //способ определения размера лота
input uint    PriceStep=300;     //шаг ценовой сетки в пунктах для доливки или открывания позиции
input uint  PosTotal=10;          //количество доливочных сделок
input uint    StopLoss_=1000;     //стоплосс в пунктах
input uint    TakeProfit_=2000;   //тейкпрофит в пунктах
input int    Deviation_=10;       //макс. отклонение цены в пунктах
//+----------------------------------------------+
//---- Объявление целых переменных для хранения периода графика в секундах 
int TimeShiftSec;
//---- Объявление целых переменных для хендлов индикаторов
int InpInd_Handle;
//---- объявление целых переменных начала отсчета данных
int min_rates_total;
double dPriceStep;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---- инициализация переменной для хранения периода графика в секундах  
   TimeShiftSec=PeriodSeconds(PERIOD_CURRENT);
//---- Инициализация переменных начала отсчёта данных
   min_rates_total=2;
   dPriceStep=PriceStep*_Point;
//--- завершение инициализации
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//----
   GlobalVariableDel_(Symbol());
//----
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---- проверка количества баров на достаточность для расчёта
   if(Bars(Symbol(),PERIOD_CURRENT)<min_rates_total) return;

//---- Объявление статических переменных
   static bool Recount=true;
   static bool ReBUY_Open=false,ReSELL_Open=false;
   static datetime UpSignalTime,DnSignalTime;
   static CIsNewBar NB;
   static string RePosComment="";
   bool BUY_Open=false,SELL_Open=false;
//---- определяем сигнал для открывания первой позиции
   if(!PositionSelect(Symbol()))
     {
      double iClose[1],iOpen[1];
      if(CopyClose(Symbol(),PERIOD_CURRENT,0,1,iClose)<=0) return;
      if(CopyOpen(Symbol(),PERIOD_CURRENT,0,1,iOpen)<=0) return;
      double dPrice=iClose[0]-iOpen[0];
      if(dPrice>dPriceStep) BUY_Open=true;
      if(-dPrice>dPriceStep) SELL_Open=true;
      UpSignalTime=datetime(SeriesInfoInteger(Symbol(),PERIOD_CURRENT,SERIES_LASTBAR_DATE))+TimeShiftSec;
      DnSignalTime=datetime(SeriesInfoInteger(Symbol(),PERIOD_CURRENT,SERIES_LASTBAR_DATE))+TimeShiftSec;
     }

//---- определяем сигнал для доливки на последнем закрытом баре
   if(NB.IsNewBar(Symbol(),PERIOD_CURRENT) || Recount) // проверка на появление нового бара
     {
      //---- обнулим торговые сигналы
      ReBUY_Open=false;
      ReSELL_Open=false;
      Recount=false;
      UpSignalTime=datetime(SeriesInfoInteger(Symbol(),PERIOD_CURRENT,SERIES_LASTBAR_DATE))+TimeShiftSec;
      DnSignalTime=datetime(SeriesInfoInteger(Symbol(),PERIOD_CURRENT,SERIES_LASTBAR_DATE))+TimeShiftSec;

      //---- Получение сигналов для доливок
      if(PositionSelect(Symbol())) //Проверка на на наличие открытой позиции
        {
         //---- Получим число доливок в позиции
         string PosComment=PositionGetString(POSITION_COMMENT); //комментарий к позиции
         int OrderTotal;
         double OpenPrice,LastLot;
         if(PosComment=="")
           {
            OrderTotal=0; //число доливок в позиции
            OpenPrice=PositionGetDouble(POSITION_PRICE_OPEN); //Цена открытия последней доливки
            LastLot=PositionGetDouble(POSITION_VOLUME); //Размер лота последней доливки
           }
         else
           {
            OrderTotal=GetTotalFromStringComment(PosComment); //число доливок в позиции
            OpenPrice=GetPriceFromStringComment(PosComment); //Цена открытия последней доливки
            LastLot=GetLotFromStringComment(PosComment); //Размер лота последней доливки
           }

         if(OrderTotal<int(PosTotal)) //проверка числа доливок на лимит числа доливок
           {
            ENUM_POSITION_TYPE PosType=ENUM_POSITION_TYPE(PositionGetInteger(POSITION_TYPE));

            if(PosType==POSITION_TYPE_SELL)
              {
               double Bid=SymbolInfoDouble(Symbol(),SYMBOL_BID);
               double point=SymbolInfoDouble(Symbol(),SYMBOL_POINT);
               if(!Bid || !point) {Recount=true; return;} //нет данных для дальнейшего расчёта
               int len=int((OpenPrice-Bid)/point); //расстояние в пунктах между текущей ценой и ценой открытия
               if(len>int(PriceStep)) //проверка на минимальное расстояние между доливками
                 {
                  ReSELL_Open=true;
                  //---- Информация о новой доливке для комментария к позиции
                  int NewOrderTotal=OrderTotal+1;
                  if(NewOrderTotal<10) RePosComment="  "+string(NewOrderTotal);
                  else if(NewOrderTotal<100) RePosComment=" "+string(NewOrderTotal);
                  else if(NewOrderTotal<1000) RePosComment=string(NewOrderTotal);
                  RePosComment=RePosComment+"/"+DoubleToString(Bid,_Digits);
                 }
              }

            if(PosType==POSITION_TYPE_BUY)
              {
               double Ask=SymbolInfoDouble(Symbol(),SYMBOL_ASK);
               double point=SymbolInfoDouble(Symbol(),SYMBOL_POINT);
               if(!Ask || !point) {Recount=true; return;}  //нет данных для дальнейшего расчёта
               int len=int((Ask-OpenPrice)/point); //расстояние в пунктах между текущей ценой и ценой открытия
               if(len>int(PriceStep)) //проверка на минимальное расстояние между доливками
                 {
                  ReBUY_Open=true;
                  //---- Информация о новой доливке для комментария к позиции
                  int NewOrderTotal=OrderTotal+1;
                  if(NewOrderTotal<10) RePosComment="  "+string(NewOrderTotal);
                  else if(NewOrderTotal<100) RePosComment=" "+string(NewOrderTotal);
                  else if(NewOrderTotal<1000) RePosComment=string(NewOrderTotal);
                  RePosComment=RePosComment+"/"+DoubleToString(Ask,_Digits);
                 }
              }
           }
        }
     }

//+----------------------------------------------+
//| Совершение сделок                            |
//+----------------------------------------------+
//---- Доливаем лонг
   ReBuyPositionOpen_X(ReBUY_Open,Symbol(),UpSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_,RePosComment);

//---- Доливаем шорт
   ReSellPositionOpen_X(ReSELL_Open,Symbol(),DnSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_,RePosComment);
//----

//---- Открываем лонг
   BuyPositionOpen(BUY_Open,Symbol(),UpSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_);

//---- Открываем шорт
   SellPositionOpen(SELL_Open,Symbol(),DnSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_);
//----
  }
//+------------------------------------------------------------------+
//|  Получение числа доливок из комента к открытой позиции           |
//+------------------------------------------------------------------+
int GetTotalFromStringComment(string comment)
  {
//----
   return(int(StringToInteger(StringSubstr(comment,0,3))));
//----
  }
//+------------------------------------------------------------------+
//|  Получение последней цены открытия из комента к открытой позиции |
//+------------------------------------------------------------------+
double GetPriceFromStringComment(string comment)
  {
//----
   return(StringToDouble(StringSubstr(comment,4,7)));
//----
  }
//+------------------------------------------------------------------+
//|  Получение последнего объёма из комента к открытой позиции       |
//+------------------------------------------------------------------+
double GetLotFromStringComment(string comment)
  {
//----
   return(StringToDouble(StringSubstr(comment,12,-1)));
//----
  }
//+------------------------------------------------------------------+
//|  Получение торгового результата последней закрытой позиции       |
//+------------------------------------------------------------------+
double GetLastClosePosRezalt(const string PosSymbol)
  {
//---- Запросим всю торговую историю по счёту
   if(!HistorySelect(0,TimeCurrent())) return(0.0);

//---- Определим размер списка сделок
   uint deals=HistoryDealsTotal();

//--- теперь обработаем каждую сделку 
   for(int pos=int(deals)-1; pos>=0; pos--)
     {
      //---- найдём тикет последней закрытой сделки
      ulong deal_ticket=HistoryDealGetTicket(pos);
      //---- определим символ сделки
      string symbol=HistoryDealGetString(deal_ticket,DEAL_SYMBOL);
      if(symbol!=PosSymbol) continue;
      //---- определим профит последней закрытой сделки
      double last_profit=HistoryDealGetDouble(deal_ticket,DEAL_PROFIT);
      double last_volume=HistoryDealGetDouble(deal_ticket,DEAL_VOLUME);
      if(last_profit>=0) return(last_volume);
      else return(-last_volume);
     }
//----
   return(0.0);
  }
//+------------------------------------------------------------------+
