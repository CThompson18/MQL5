//+------------------------------------------------------------------+
//|                                Exp_SilverTrend_Signal_ReOpen.mq5 |
//|                               Copyright © 2016, Nikolay Kositsin | 
//|                              Khabarovsk,   farria@mail.redcom.ru | 
//+------------------------------------------------------------------+
#property copyright "Copyright © 2016, Nikolay Kositsin"
#property link      "farria@mail.redcom.ru"
#property version   "1.10"
//---- Включение индикатора в код эксперта как ресурса
//---- Inclusion of the indicator in the code of the expert as a resource
#resource "\\Indicators\\SilverTrend_Signal.ex5"
//+----------------------------------------------+
//  Торговые алгоритмы   
//  Trading Algorithms          | 
//+----------------------------------------------+
#include <TradeAlgorithms.mqh>
//+----------------------------------------------+
//|  Перечисление для вариантов расчёта лота 
//| Enumeration for options for calculating the lot
//+----------------------------------------------+
/*enum MarginMode  - перечисление объявлено в файле TradeAlgorithms.mqh
		//-enumeration declared in file TradeAlgorithms.mqh
  {
   FREEMARGIN=0,     //MM от свободных средств на счёте
		     //MM from free funds on the account
   BALANCE,          //MM от баланса средств на счёте
		     //MM from the balance of funds on the account
   LOSSFREEMARGIN,   //MM по убыткам от свободных средств на счёте
		     //MM on losses from available funds on the account 
   LOSSBALANCE,      //MM по убыткам от баланса средств на счёте
		     //MM on losses from the balance of funds on the account
   LOT               //Лот без изменения
		     //Lot Unchanged
  }; */
//+----------------------------------------------+
//| Входные параметры индикатора эксперта        |
//| Input parameters of the expert indicator	 |
//+----------------------------------------------+
input double MM=0.1;              //Доля финансовых ресурсов от депозита в сделке
				  //The share of financial resources from the deposit in the transaction
input MarginMode MMMode=LOT;      //способ определения размера лота
				  //method of determining the size of the lot
input int    StopLoss_=1000;      //стоплосс в пунктах
				  //stop-loss in points
input int    TakeProfit_=2000;    //тейкпрофит в пунк
				  //take-profit in points
input int    Deviation_=10;       //макс. отклонение цены в пунктах
				  //Max. price deviation in points
input uint   PriceStep=300;       //шаг ценовой сетки в пунктах для доливки
				  //step of the price grid in points for refilling
input uint   PosTotal=10;         //количество доливочных сделок
				  //number of topping deals
input bool   BuyPosOpen=true;     //Разрешение для входа в лонг
				  //The permission to enter the long
input bool   SellPosOpen=true;    //Разрешение для входа в шорт
				  //Permission to enter shorts
input bool   BuyPosClose=true;    //Разрешение для выхода из лонгов
				  //Permission to leave long
input bool   SellPosClose=true;   //Разрешение для выхода из шортов
				  //Permission to leave shorts
//+----------------------------------------------+
//| Входные параметры индикатора                 |
//| Input parameters of the indicator		 |
//+----------------------------------------------+
input ENUM_TIMEFRAMES InpInd_Timeframe=PERIOD_H4; //таймфрейм индикатора SilverTrend_Signal
						  //timeframe indicator SilverTrend_Signal
input int SSP=9;
input int RISK=3;   
input uint SignalBar=1;                           //номер бара для получения сигнала входа
						  //bar number for receiving the input signal
//+----------------------------------------------+

int TimeShiftSec;
//---- Объявление целых переменных для хендлов индикаторов
//---- Declaring Integer Variables for Indicator Handles
int InpInd_Handle;
//---- объявление целых переменных начала отсчета данных
//---- declaration of entire variables of the origin of data
int min_rates_total;
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---- получение хендла индикатора SilverTrend_Signal
//---- getting the SilverTrend_Signal indicator handle
   InpInd_Handle=iCustom(Symbol(),InpInd_Timeframe,"::Indicators\\SilverTrend_Signal",SSP,RISK,0);
   if(InpInd_Handle==INVALID_HANDLE)
     {
      Print(" Failed to get indicator handle SilverTrend_Signal");
      return(INIT_FAILED);
     }

//---- инициализация переменной для хранения периода графика в секундах  
//---- initialization of the variable to store the period of the graph in seconds
   TimeShiftSec=PeriodSeconds(InpInd_Timeframe);

//---- инициализация переменных начала отсчёта данных
//---- initialization of the data reference start variables
   min_rates_total=SSP;
   min_rates_total+=int(1+SignalBar);
//--- завершение инициализации
//--- completion of initialization
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
//---- checking the number of bars for sufficiency for calculation
   if(BarsCalculated(InpInd_Handle)<min_rates_total) return;

//---- подгрузка истории для нормальной работы функций IsNewBar() и SeriesInfoInteger()  
//---- history loading for the normal operation of the functions IsNewBar () and SeriesInfoInteger ()
   LoadHistory(TimeCurrent()-PeriodSeconds(InpInd_Timeframe)-1,Symbol(),InpInd_Timeframe);

//---- Объявление локальных переменных
//---- Declaring local variables
   double DnValue[1],UpValue[1];
//---- Объявление статических переменных
//---- Declaring Static Variables
   static bool Recount=true;
   static bool BUY_Open=false,BUY_Close=false;
   static bool SELL_Open=false,SELL_Close=false;
   static bool ReBUY_Open=false,ReSELL_Open=false;
   static datetime UpSignalTime,DnSignalTime;
   static CIsNewBar NB;
   static string RePosComment="";
//---- Определение сигналов для сделок
//---- Definition of signals for transactions
   if(!SignalBar || NB.IsNewBar(Symbol(),InpInd_Timeframe) || Recount) // check for a new bar
     {
      //---- обнулим торговые сигналы
      //---- reset trading signals
      BUY_Open=false;
      SELL_Open=false;
      BUY_Close=false;
      SELL_Close=false;
      ReBUY_Open=false;
      ReSELL_Open=false;
      Recount=false;

      //---- копируем вновь появившиеся данные в массивы
      //---- copy newly appeared data into arrays
      if(CopyBuffer(InpInd_Handle,1,SignalBar,1,UpValue)<=0) {Recount=true; return;}
      if(CopyBuffer(InpInd_Handle,0,SignalBar,1,DnValue)<=0) {Recount=true; return;}

      //---- Получим сигналы для покупки
      //---- We will receive signals for purchase
      if(UpValue[0] && UpValue[0]!=EMPTY_VALUE)
        {
         if(BuyPosOpen) BUY_Open=true;
         if(SellPosClose)SELL_Close=true;
         UpSignalTime=datetime(SeriesInfoInteger(Symbol(),InpInd_Timeframe,SERIES_LASTBAR_DATE))+TimeShiftSec;
        }

      //---- Получим сигналы для продажи
      //---- We will receive signals for sale
      if(DnValue[0] && DnValue[0]!=EMPTY_VALUE)
        {
         if(SellPosOpen) SELL_Open=true;
         if(BuyPosClose) BUY_Close=true;
         DnSignalTime=datetime(SeriesInfoInteger(Symbol(),InpInd_Timeframe,SERIES_LASTBAR_DATE))+TimeShiftSec;
        }

      //---- Поиск последнего направления торговли для получения сигналов для закрывания позиций
      //---- Search for the last trade direction to receive signals for closing positions
      //if(!MQL5InfoInteger(MQL5_TESTING) && !MQL5InfoInteger(MQL5_OPTIMIZATION)) //If the trading mode in the tester "Arbitrary delay" 
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
      //---- Getting signals for refills
      if(PositionSelect(Symbol())) //Проверка на на наличие открытой позиции
        {
         //---- Получим число доливок в позиции
	 //---- Get the number of refills in the position
         string PosComment=PositionGetString(POSITION_COMMENT); //комментарий к позиции
         int OrderTotal;
         double OpenPrice,LastLot;
         if(PosComment=="")
           {
            OrderTotal=0; //число доливок в позиции
            OpenPrice=PositionGetDouble(POSITION_PRICE_OPEN); //Цена открытия последней доливки
							      //The price of opening the last refill
            LastLot=PositionGetDouble(POSITION_VOLUME); //Размер лота последней доливки
							//Lot size of the last refill
           }
         else
           {
            OrderTotal=GetTotalFromStringComment(PosComment); //число доливок в позиции
							      //number of refills in position
            OpenPrice=GetPriceFromStringComment(PosComment); //Цена открытия последней доливки
							     //The price of opening the last refill
            LastLot=GetLotFromStringComment(PosComment); //Размер лота последней доливки
							 //Lot size of the last refill
           }

         if(OrderTotal<int(PosTotal)) //проверка числа доливок на лимит числа доливок
				      //checking the number of refills for the limit of the number of refills
           {
            ENUM_POSITION_TYPE PosType=ENUM_POSITION_TYPE(PositionGetInteger(POSITION_TYPE));

            if(PosType==POSITION_TYPE_SELL)
              {
               double Bid=SymbolInfoDouble(Symbol(),SYMBOL_BID);
               double point=SymbolInfoDouble(Symbol(),SYMBOL_POINT);
               if(!Bid || !point) {Recount=true; return;} //нет данных для дальнейшего расчёта
							  //no data for further calculation
               int len=int((OpenPrice-Bid)/point); //расстояние в пунктах между текущей ценой и ценой открытия
						   //the distance in points between the current price and the opening price
               if(len>int(PriceStep)) //проверка на минимальное расстояние между доливками
				      //check for minimum distance between refills
                 {
                  ReSELL_Open=true;
                  //---- Информация о новой доливке для комментария к позиции
		  //---- Information about a new top-up for commenting on a position
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
							   //no data for further calculation
               int len=int((Ask-OpenPrice)/point); //расстояние в пунктах между текущей ценой и ценой открытия
						   //the distance in points between the current price and the opening price
               if(len>int(PriceStep)) //проверка на минимальное расстояние между доливками
				      //check for minimum distance between refills
                 {
                  ReBUY_Open=true;
                  //---- Информация о новой доливке для комментария к позиции
		  //---- Information about a new top-up for commenting on a position
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
//---- Transaction execution
//---- Закрываем лонг
//---- Close Long
   BuyPositionClose(BUY_Close,Symbol(),Deviation_);

//---- Закрываем шорт 
//---- Close the shorts
   SellPositionClose(SELL_Close,Symbol(),Deviation_);
   
//---- Доливаем лонг
//---- Add long
   ReBuyPositionOpen_X(ReBUY_Open,Symbol(),UpSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_,RePosComment);

//---- Доливаем шорт
//---- Add short
   ReSellPositionOpen_X(ReSELL_Open,Symbol(),DnSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_,RePosComment);

//---- Открываем лонг
//---- Open long
   BuyPositionOpen(BUY_Open,Symbol(),UpSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_);

//---- Открываем шорт
//---- Opoen short
   SellPositionOpen(SELL_Open,Symbol(),DnSignalTime,MM,MMMode,Deviation_,StopLoss_,TakeProfit_);
//----
  }
//+------------------------------------------------------------------+
//|  Получение числа доливок из комента к открытой позиции           |
//| Getting the number of refills from a comment to an open position |
//+------------------------------------------------------------------+
int GetTotalFromStringComment(string comment)
  {
//----
   return(int(StringToInteger(StringSubstr(comment,0,3))));
//----
  }
//+------------------------------------------------------------------+
//|  Получение последней цены открытия из комента к открытой позиции |
//| Receiving the last open price from a quote to an open position   |
//+------------------------------------------------------------------+
double GetPriceFromStringComment(string comment)
  {
//----
   return(StringToDouble(StringSubstr(comment,4,7)));
//----
  }
//+------------------------------------------------------------------+
//|  Получение последнего объёма из комента к открытой позиции       |
//| Receiving the last volume from a comment to an open position     |
//+------------------------------------------------------------------+
double GetLotFromStringComment(string comment)
  {
//----
   return(StringToDouble(StringSubstr(comment,12,-1)));
//----
  }
//+------------------------------------------------------------------+
//|  Получение торгового результата последней закрытой позиции       |
//| Obtaining the trading result of the last closed position	     |
//+------------------------------------------------------------------+
double GetLastClosePosRezalt(const string PosSymbol)
  {
//---- Запросим всю торговую историю по счёту
//---- We request the entire trade history on the account
   if(!HistorySelect(0,TimeCurrent())) return(0.0);

//---- Определим размер списка сделок
//---- Define the size of the list of transactions
   uint deals=HistoryDealsTotal();

//--- теперь обработаем каждую сделку 
//--- now we will process each transaction
   for(int pos=int(deals)-1; pos>=0; pos--)
     {
      //---- найдём тикет последней закрытой сделки
      //---- find the ticket of the last closed deal
      ulong deal_ticket=HistoryDealGetTicket(pos);
      //---- определим символ сделки
      //---- define the symbol of the transaction
      string symbol=HistoryDealGetString(deal_ticket,DEAL_SYMBOL);
      if(symbol!=PosSymbol) continue;
      //---- определим профит последней закрытой сделки
      //---- determine the profit of the last closed deal
      double last_profit=HistoryDealGetDouble(deal_ticket,DEAL_PROFIT);
      double last_volume=HistoryDealGetDouble(deal_ticket,DEAL_VOLUME);
      if(last_profit>=0) return(last_volume);
      else return(-last_volume);
     }
//----
   return(0.0);
  }
//+------------------------------------------------------------------+