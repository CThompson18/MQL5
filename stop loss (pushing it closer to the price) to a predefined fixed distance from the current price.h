//+------------------------------------------------------------------+
//|                                              Exp_TralingStop.mq5 |
//|                               Copyright © 2016, Nikolay Kositsin | 
//|                              Khabarovsk,   farria@mail.redcom.ru | 
//+------------------------------------------------------------------+
#property copyright "Copyright © 2015, Nikolay Kositsin"
#property link      "farria@mail.redcom.ru"
#property version   "1.00"
#property description   "Эксперт производит перестановку стоплосса на фиксированное в настройках эксперта расстояние от текущей цены."
//+----------------------------------------------+
//  Торговые алгоритмы                           | 
//+----------------------------------------------+
#include <TradeAlgorithms.mqh>
//+----------------------------------------------+
//| Входные параметры эксперта                   |
//+----------------------------------------------+
input uint   TralingStop=500;     //стоплосс позиции в пунктах
input uint    Deviation_=10;      //макс. отклонение цены в пунктах
//+----------------------------------------------+
//---- объявление целых переменных начала отсчета данных
int min_rates_total;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---- Инициализация переменных начала отсчёта данных
   min_rates_total=2;
//----
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

//---- Объявление и обнуление переменных сигналов трейлинстопов   
   bool BUY_tral=false;
   bool SELL_tral=false;
   double NewStop=0.0;
//---- Инициализация сигналов трейлинстопов и перевода позиции в безубыток
   if(PositionSelect(Symbol())) //Проверка на на наличие открытой позиции
     {
      ENUM_POSITION_TYPE PosType=ENUM_POSITION_TYPE(PositionGetInteger(POSITION_TYPE));
      double LastStop=PositionGetDouble(POSITION_SL);
      double point=SymbolInfoDouble(Symbol(),SYMBOL_POINT);
      if(!point) return; //нет данных для дальнейшего расчёта
      
      if(PosType==POSITION_TYPE_SELL)
        {        
         double Bid=SymbolInfoDouble(Symbol(),SYMBOL_BID);        
         if(!Bid) return; //нет данных для дальнейшего расчёта

         //---- Получим сигналы движения стоплосса шорта по тралу
         int point_lastloss=int((LastStop-Bid)/point);
         if(point_lastloss>int(TralingStop))
           {
            NewStop=Bid+TralingStop*point;
            SELL_tral=true;
           }
        }

      if(PosType==POSITION_TYPE_BUY)
        {
         double Ask=SymbolInfoDouble(Symbol(),SYMBOL_ASK);
         if(!Ask) return;  //нет данных для дальнейшего расчёта

         //---- Получим сигналы движения стоплосса лонга по тралу
         int point_lastloss=int((Ask-LastStop)/point);
         if(point_lastloss>int(TralingStop))
           {
            NewStop=Ask-TralingStop*point;
            BUY_tral=true;
           }
        }
     }
//+----------------------------------------------+
//| Совершение сделок                            |
//+----------------------------------------------+
//---- Модифицируем лонг
   dBuyPositionModify(BUY_tral,Symbol(),Deviation_,NewStop,0.0);

//---- Модифицируем  шорт
   dSellPositionModify(SELL_tral,Symbol(),Deviation_,NewStop,0.0);
//----
  }
//+------------------------------------------------------------------+
