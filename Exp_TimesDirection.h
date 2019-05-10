//+------------------------------------------------------------------+
//|                                           Exp_TimesDirection.mq5 |
//|                               Copyright © 2016, Nikolay Kositsin | 
//|                              Khabarovsk,   farria@mail.redcom.ru | 
//+------------------------------------------------------------------+
#property copyright "Copyright © 2016, Nikolay Kositsin"
#property link      "farria@mail.redcom.ru"
#property version   "1.00"
//+-------------------------------------------------+
//  Торговые алгоритмы                              | 
//+-------------------------------------------------+
#include <TradeAlgorithms.mqh>
//+-------------------------------------------------+
//|  Перечисление для вариантов расчёта лота        |
//+-------------------------------------------------+
/*enum MarginMode  - перечисление объявлено в файле TradeAlgorithms.mqh
  {
   FREEMARGIN=0,     //MM от свободных средств на счёте
   BALANCE,          //MM от баланса средств на счёте
   LOSSFREEMARGIN,   //MM по убыткам от свободных средств на счёте
   LOSSBALANCE,      //MM по убыткам от баланса средств на счёте
   LOT               //Лот без изменения
  }; */
//+-------------------------------------------------+
//|  Перечисление для вариантов торговли            |
//+-------------------------------------------------+
enum TradeMode
  {
   ENUM_SELL,     //Только покупки
   ENUM_BUY       //Только продажи
  };
//+-------------------------------------------------+
//| Входные параметры индикатора эксперта           |
//+-------------------------------------------------+
input double MM=0.1;              //Доля финансовых ресурсов от депозита в сделке
input MarginMode MMMode=LOT;      //способ определения размера лота
input int    StopLoss_=1000;      //стоплосс в пунктах
input int    TakeProfit_=2000;    //тейкпрофит в пунктах
input int    Deviation_=10;       //макс. отклонение цены в пунктах
input TradeMode Trade=ENUM_SELL;  //Направление торговли
input datetime  OpenTime=D'1970.01.01 00:00';      //Время открывания позиции
input datetime  CloseTime=D'3000.01.01 00:00';     //Время закрывания позиции
input uint TradeInterval =60;                      //Торговый интервал в секундах
//+-------------------------------------------------+
//---- Объявление целых переменных для хендлов индикаторов
int InpInd_Handle;
//---- объявление целых переменных начала отсчета данных
int min_rates_total;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---- Инициализация переменных начала отсчёта данных
   min_rates_total=1;
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

//---- Объявление локальных переменных
   datetime SignalTime=NULL;
   bool CloseSignal,OpenSignal;

//---- Получение торговых сигналов   
   if(TimeCurrent()>=OpenTime && TimeCurrent()<OpenTime+TradeInterval  && !PositionSelect(Symbol()))
     {
      OpenSignal=true;
      if(Trade==ENUM_BUY) Print(__FUNCTION__,": ", "Сработал сигнал на покупку для открывания позиции!");
      else Print(__FUNCTION__,": ", "Сработал сигнал на продажу для открывания позиции!");
      Print(__FUNCTION__,": ", "Время открывания позиции: ",TimeToString(TimeCurrent(),TIME_DATE|TIME_SECONDS));
     }
   else OpenSignal=false;
   if(TimeCurrent()>=CloseTime && TimeCurrent()<CloseTime+TradeInterval && PositionSelect(Symbol()))
     {
      CloseSignal=true;
      if(Trade==ENUM_BUY) Print(__FUNCTION__,": ", "Сработал сигнал на продажу для закрывания позиции!");
      else Print(__FUNCTION__,": ", "Сработал сигнал на покупку для закрывания позиции!");
      Print(__FUNCTION__,": ", "Время закрывания позиции: ",TimeToString(TimeCurrent(),TIME_DATE|TIME_SECONDS));
     }
   else CloseSignal=false;

//+-------------------------------------------------+
//| Совершение сделок                               |
//+-------------------------------------------------+
   if(Trade==ENUM_BUY)
     {
      //---- Закрываем лонг
      BuyPositionClose(CloseSignal,Symbol(),Deviation_);
      //---- Открываем лонг
      BuyPositionOpen(OpenSignal,Symbol(),SignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_);
     }
   else
     {
      //---- Закрываем шорт   
      SellPositionClose(CloseSignal,Symbol(),Deviation_);
      //---- Открываем шорт
      SellPositionOpen(OpenSignal,Symbol(),SignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_);
     }
//----
  }
//+------------------------------------------------------------------+
