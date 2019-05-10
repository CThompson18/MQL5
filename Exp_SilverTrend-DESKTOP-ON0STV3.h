//+------------------------------------------------------------------+
//|                                Exp_SilverTrend_Signal_ReOpen.mq5 |
//|                               Copyright © 2016, Nikolay Kositsin |
//|                              Khabarovsk,   farria@mail.redcom.ru |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2016, Nikolay Kositsin"
#property link      "farria@mail.redcom.ru"
#property version   "1.10"
//---- Включение индикатора в код эксперта как ресурса
#resource "\\Indicators\\SilverTrend.ex5"
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
input double MM=0.1;              //Доля финансовых ресурсов от депозита в сделке
input MarginMode MMMode=LOT;      //способ определения размера лота
input int    StopLoss_=1000;      //стоплосс в пунктах
input int    TakeProfit_=2000;    //тейкпрофит в пунктах
input int    Deviation_=10;       //макс. отклонение цены в пунктах
input uint   PriceStep=300;       //шаг ценовой сетки в пунктах для доливки
input uint   PosTotal=10;         //количество доливочных сделок
input bool   BuyPosOpen=true;     //Разрешение для входа в лонг
input bool   SellPosOpen=true;    //Разрешение для входа в шорт
input bool   BuyPosClose=true;    //Разрешение для выхода из лонгов
input bool   SellPosClose=true;   //Разрешение для выхода из шортов
//+----------------------------------------------+
//| Входные параметры индикатора                 |
//+----------------------------------------------+
input ENUM_TIMEFRAMES InpInd_Timeframe=PERIOD_H4; //таймфрейм индикатора SilverTrend_Signal
input double RISK = 9.25;
input double SSP = 41.95;
input double VariableRange = 2.0;  
input uint SignalBar=1;                           //номер бара для получения сигнала входа
//+----------------------------------------------+

int TimeShiftSec;
//---- Объявление целых переменных для хендлов индикаторов
int InpInd_Handle;
//---- объявление целых переменных начала отсчета данных
int min_rates_total;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---- получение хендла индикатора SilverTrend_Signal
   InpInd_Handle=iCustom(Symbol(),InpInd_Timeframe,"::Indicators\\SilverTrend" RISK,,SSP,VariableRange,0);
   if(InpInd_Handle==INVALID_HANDLE)
     {
      Print(" Не удалось получить хендл индикатора SilverTrend_Signal");
      return(INIT_FAILED);
     }

//---- инициализация переменной для хранения периода графика в секундах  
   TimeShiftSec=PeriodSeconds(InpInd_Timeframe);

//---- инициализация переменных начала отсчёта данных
   min_rates_total=SSP;
   min_rates_total+=int(1+SignalBar);
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
   if(BarsCalculated(InpInd_Handle)<min_rates_total) return;

//---- подгрузка истории для нормальной работы функций IsNewBar() и SeriesInfoInteger()  
   LoadHistory(TimeCurrent()-PeriodSeconds(InpInd_Timeframe)-1,Symbol(),InpInd_Timeframe);

//---- Объявление локальных переменных
   double DnValue[1],UpValue[1];
//---- Объявление статических переменных
   static bool Recount=true;
   static bool BUY_Open=false,BUY_Close=false;
   static bool SELL_Open=false,SELL_Close=false;
   static bool ReBUY_Open=false,ReSELL_Open=false;
   static datetime UpSignalTime,DnSignalTime;
   static CIsNewBar NB;
   static string RePosComment="";
//---- Определение сигналов для сделок
   if(!SignalBar || NB.IsNewBar(Symbol(),InpInd_Timeframe) || Recount) // проверка на появление нового бара
     {
      //---- обнулим торговые сигналы
      BUY_Open=false;
      SELL_Open=false;
      BUY_Close=false;
      SELL_Close=false;
      ReBUY_Open=false;
      ReSELL_Open=false;
      Recount=false;

      //---- копируем вновь появившиеся данные в массивы
      if(CopyBuffer(InpInd_Handle,1,SignalBar,1,UpValue)<=0) {Recount=true; return;}
      if(CopyBuffer(InpInd_Handle,0,SignalBar,1,DnValue)<=0) {Recount=true; return;}

      //---- Получим сигналы для покупки
      if(UpValue[0] && UpValue[0]!=EMPTY_VALUE)
        {
         if(BuyPosOpen) BUY_Open=true;
         if(SellPosClose)SELL_Close=true;
         UpSignalTime=datetime(SeriesInfoInteger(Symbol(),InpInd_Timeframe,SERIES_LASTBAR_DATE))+TimeShiftSec;
        }

      //---- Получим сигналы для продажи
      if(DnValue[0] && DnValue[0]!=EMPTY_VALUE)
        {
         if(SellPosOpen) SELL_Open=true;
         if(BuyPosClose) BUY_Close=true;
         DnSignalTime=datetime(SeriesInfoInteger(Symbol(),InpInd_Timeframe,SERIES_LASTBAR_DATE))+TimeShiftSec;
        }

      //---- Поиск последнего направления торговли для получения сигналов для закрывания позиций
      //if(!MQL5InfoInteger(MQL5_TESTING) && !MQL5InfoInteger(MQL5_OPTIMIZATION)) //если режим торговли в тестере "Произвольная задержка"
      if(((BuyPosOpen && BuyPosClose) || (SellPosOpen && SellPosClose)) && (!BUY_Close && !SELL_Close))
        {
         int Bars_=Bars(Symbol(),InpInd_Timeframe);

         for(int bar=int(SignalBar+1); bar<Bars_; bar++)
           {
            if(SellPosClose)
              {
               if(CopyBuffer(InpInd_Handle,1,bar,1,UpValue)<=0) {Recount=true; return;}
               if(UpValue[0]!=0 && UpValue[0]!=EMPTY_VALUE)
                 {
                  SELL_Close=true;
                  break;
                 }
              }

            if(BuyPosClose)
              {
               if(CopyBuffer(InpInd_Handle,0,bar,1,DnValue)<=0) {Recount=true; return;}
               if(DnValue[0]!=0 && DnValue[0]!=EMPTY_VALUE)
                 {
                  BUY_Close=true;
                  break;
                 }
              }
           }
        }

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
//---- Совершение сделок
//---- Закрываем лонг
   BuyPositionClose(BUY_Close,Symbol(),Deviation_);

//---- Закрываем шорт  
   SellPositionClose(SELL_Close,Symbol(),Deviation_);
  
//---- Доливаем лонг
   ReBuyPositionOpen_X(ReBUY_Open,Symbol(),UpSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_,RePosComment);

//---- Доливаем шорт
   ReSellPositionOpen_X(ReSELL_Open,Symbol(),DnSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_,RePosComment);

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
