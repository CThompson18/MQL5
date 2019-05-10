
//+------------------------------------------------------------------+
//|                                             Exp_TrailingStop.mq5 |
//|                               Copyright © 2017, Nikolay Kositsin | 
//|                              Khabarovsk,   farria@mail.redcom.ru | 
//+------------------------------------------------------------------+
#property copyright "Copyright © 2017, Nikolay Kositsin"
#property link "farria@mail.redcom.ru"
#property version   "1.00"
//+----------------------------------------------+
//| Входные параметры эксперта                   |
//+----------------------------------------------+
input uint   StopLoss_=1000;      //стоплосс в пунктах
input int    TralStart=1000;      //старт трейлинга в пунктах
input uint   TralStep=200;        //шаг трейлинстопа в пунктах
input uint   Deviation_=10;       //макс. отклонение цены в пунктах
//+----------------------------------------------+

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//----
   
//---- завершение инициализации
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//----

//----
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//+------------------------------------------------+
//| Двигаем тралы всех открытых позиций по символу |
//+------------------------------------------------+
   BuyTrailingStopBySymbol(Symbol(),StopLoss_,TralStart,TralStep,Deviation_);
   SellTrailingStopBySymbol(Symbol(),StopLoss_,TralStart,TralStep,Deviation_);
//----
  }
//+------------------------------------------------------------------+
//| Двигаем тралы открытых всех длинных позиций по символу           |
//+------------------------------------------------------------------+
bool BuyTrailingStopBySymbol(string Symb, uint Stop, int StartLevel, uint Step, uint deviation)
  {
//----
   uint deals=PositionsTotal();
//--- теперь обработаем каждую сделку 
   for(int pos=int(deals)-1; pos>=0; pos--)
     {
      //---- найдём тикет последней позиции
      ulong deal_ticket=PositionGetTicket(pos);
      if(!PositionSelectByTicket(deal_ticket)) continue;
      if(PositionGetString(POSITION_SYMBOL)!=Symb) continue;
      if(PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_BUY) continue;
      //----
      double openprice=PositionGetDouble(POSITION_PRICE_OPEN);      
      double point=SymbolInfoDouble(Symbol(),SYMBOL_POINT);
      double Ask=SymbolInfoDouble(Symbol(),SYMBOL_ASK);
      if(!point || !Ask) return(false);
      double diff=Ask-openprice;
      if(diff<=StartLevel*point) continue;  //проверка трейлинга на старт         
      double loss=Ask-Stop*point;
      double loss_prev=PositionGetDouble(POSITION_SL); 
      bool Signal=true;    
      if((loss_prev && loss-loss_prev>Step*point) || !loss_prev) dBuyPositionModify_(Signal,Symb,deviation,loss,NULL);//если цена выросла больше, чем Step, двигаем трал
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Двигаем тралы всех открытых коротких позиций  по символу         |
//+------------------------------------------------------------------+
bool SellTrailingStopBySymbol(string Symb, uint Stop, int StartLevel, uint Step, uint deviation)
  {
//----
   uint deals=PositionsTotal();
//--- теперь обработаем каждую сделку 
   for(int pos=int(deals)-1; pos>=0; pos--)
     {
      //---- найдём тикет последней позиции
      ulong deal_ticket=PositionGetTicket(pos);
      if(!PositionSelectByTicket(deal_ticket)) continue;
      if(PositionGetString(POSITION_SYMBOL)!=Symb) continue;
      if(PositionGetInteger(POSITION_TYPE)!=POSITION_TYPE_SELL) continue;
      //----
      double openprice=PositionGetDouble(POSITION_PRICE_OPEN);      
      double point=SymbolInfoDouble(Symbol(),SYMBOL_POINT);
      double Bid=SymbolInfoDouble(Symbol(),SYMBOL_BID);
      if(!point || !Bid) return(false);
      double diff=openprice-Bid;
      if(diff<=StartLevel*point) continue;  //проверка трейлинга на старт         
      double loss=Bid+Stop*point;
      double loss_prev=PositionGetDouble(POSITION_SL); 
      bool Signal=true;     
      if((loss_prev && loss_prev-loss>Step*point) || !loss_prev) dSellPositionModify_(Signal,Symb,deviation,loss,NULL); //если цена упала больше, чем Step, двигаем трал
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Модифицируем длинную позицию                                     |
//+------------------------------------------------------------------+
bool dBuyPositionModify_
(
bool &Modify_Signal,        // флаг разрешения модификации
const string symbol,        // торговая пара сделки
uint deviation,             // слиппаж
double StopLoss,            // стоплосс в абсолютном значении ценового графика
double Takeprofit           // тейкпрофит в абсолютном значении ценового графика
 )
//dBuyPositionModify(Modify_Signal,symbol,deviation,StopLoss,Takeprofit);
  {
//----
   if(!Modify_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_BUY;

//---- Объявление структур торгового запроса и результата торгового запроса
   MqlTradeRequest request;
   MqlTradeResult result;

//---- Объявление структуры результата проверки торгового запроса 
   MqlTradeCheckResult check;

//---- обнуление структур
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);
//----
   int digit=int(SymbolInfoInteger(symbol,SYMBOL_DIGITS));
   double point=SymbolInfoDouble(symbol,SYMBOL_POINT);
   double Ask=SymbolInfoDouble(symbol,SYMBOL_ASK);
   if(!digit || !point || !Ask) return(true);

//---- Инициализация структуры торгового запроса MqlTradeRequest для открывания BUY позиции
   request.type   = ORDER_TYPE_BUY;
   request.price  = Ask;
   request.action = TRADE_ACTION_SLTP;
   request.symbol = symbol;
   request.position=PositionGetInteger(POSITION_TICKET); 

//---- Определение расстояния до стоплосса в единицах ценового графика
   if(StopLoss)
     {
      int nStopLoss=int((Ask-StopLoss)/point);
      if(nStopLoss<0) return(false);
      if(!StopCorrect(symbol,nStopLoss))return(false);
      double dStopLoss=nStopLoss*point;
      request.sl=NormalizeDouble(request.price-dStopLoss,digit);
      if(request.sl<PositionGetDouble(POSITION_SL)) request.sl=PositionGetDouble(POSITION_SL);
     }
   else request.sl=PositionGetDouble(POSITION_SL);

//---- Определение расстояния до тейкпрофита единицах ценового графика
   if(Takeprofit)
     {
      int nTakeprofit=int((Takeprofit-Ask)/point);
      if(nTakeprofit<0) return(false);
      if(!StopCorrect(symbol,nTakeprofit))return(false);
      double dTakeprofit=nTakeprofit*point;
      request.tp=NormalizeDouble(request.price+dTakeprofit,digit);
      if(request.tp<PositionGetDouble(POSITION_TP)) request.tp=PositionGetDouble(POSITION_TP);
     }
   else request.tp=PositionGetDouble(POSITION_TP);

//----   
   if(request.tp==PositionGetDouble(POSITION_TP) && request.sl==PositionGetDouble(POSITION_SL)) return(true);
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Проверка торгового запроса на корректность
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Неверные данные для структуры торгового запроса!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Модифицируем Buy позицию по ",symbol," ============ >>>");
   Print(comment);

//---- Модифицируем BUY позицию и делаем проверку результата торгового запроса
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): Невозможно модифицировать позицию!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Modify_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Buy позиция по ",symbol," модифицирована ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): Невозможно модифицировать позицию!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| Модифицируем короткую позицию                                    |
//+------------------------------------------------------------------+
bool dSellPositionModify_
(
bool &Modify_Signal,        // флаг разрешения модификации
const string symbol,        // торговая пара сделки
uint deviation,             // слиппаж
double StopLoss,            // стоплосс в абсолютном значении ценового графика
double Takeprofit           // тейкпрофит в абсолютном значении ценового графика
 )
//dSellPositionModify(Modify_Signal,symbol,deviation,StopLoss,Takeprofit);
  {
//----
   if(!Modify_Signal) return(true);

   ENUM_POSITION_TYPE PosType=POSITION_TYPE_SELL;


//---- Объявление структур торгового запроса и результата торгового запроса
   MqlTradeRequest request;
   MqlTradeResult result;

//---- Объявление структуры результата проверки торгового запроса 
   MqlTradeCheckResult check;

//---- обнуление структур
   ZeroMemory(request);
   ZeroMemory(result);
   ZeroMemory(check);
//----
   int digit=int(SymbolInfoInteger(symbol,SYMBOL_DIGITS));
   double point=SymbolInfoDouble(symbol,SYMBOL_POINT);
   double Ask=SymbolInfoDouble(symbol,SYMBOL_ASK);
   if(!digit || !point || !Ask) return(true);

//---- Инициализация структуры торгового запроса MqlTradeRequest для открывания BUY позиции
   request.type   = ORDER_TYPE_SELL;
   request.price  = Ask;
   request.action = TRADE_ACTION_SLTP;
   request.symbol = symbol;
   request.position=PositionGetInteger(POSITION_TICKET); 

//---- Определение расстояния до стоплосса в единицах ценового графика
   if(StopLoss!=0)
     {
      int nStopLoss=int((StopLoss-Ask)/point);
      if(nStopLoss<0) return(false);
      if(!StopCorrect(symbol,nStopLoss))return(false);
      double dStopLoss=nStopLoss*point;
      request.sl=NormalizeDouble(request.price+dStopLoss,digit);
      double laststop=PositionGetDouble(POSITION_SL);
      if(request.sl>laststop && laststop) request.sl=PositionGetDouble(POSITION_SL);
     }
   else request.sl=PositionGetDouble(POSITION_SL);

//---- Определение расстояния до тейкпрофита единицах ценового графика
   if(Takeprofit!=0)
     {
      int nTakeprofit=int((Ask-Takeprofit)/point);
      if(nTakeprofit<0) return(false);
      if(!StopCorrect(symbol,nTakeprofit))return(false);
      double dTakeprofit=nTakeprofit*point;
      request.tp=NormalizeDouble(request.price-dTakeprofit,digit);
      double lasttake=PositionGetDouble(POSITION_TP);
      if(request.tp>lasttake && lasttake) request.tp=PositionGetDouble(POSITION_TP);
     }
   else request.tp=PositionGetDouble(POSITION_TP);

//----   
   if(request.tp==PositionGetDouble(POSITION_TP) && request.sl==PositionGetDouble(POSITION_SL)) return(true);
   request.deviation=deviation;
   request.type_filling=ORDER_FILLING_FOK;

//---- Проверка торгового запроса на корректность
   if(!OrderCheck(request,check))
     {
      Print(__FUNCTION__,"(): Неверные данные для структуры торгового запроса!");
      Print(__FUNCTION__,"(): OrderCheck(): ",ResultRetcodeDescription(check.retcode));
      return(false);
     }

   string comment="";
   StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Модифицируем Sell позицию по ",symbol," ============ >>>");
   Print(comment);

//---- Модифицируем SELL позицию и делаем проверку результата торгового запроса
   if(!OrderSend(request,result) || result.retcode!=TRADE_RETCODE_DONE)
     {
      Print(__FUNCTION__,"(): Невозможно модифицировать позицию!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
      return(false);
     }
   else
   if(result.retcode==TRADE_RETCODE_DONE)
     {
      Modify_Signal=false;
      comment="";
      StringConcatenate(comment,"<<< ============ ",__FUNCTION__,"(): Sell позиция по ",symbol," модифицирована ============ >>>");
      Print(comment);
      PlaySound("ok.wav");
     }
   else
     {
      Print(__FUNCTION__,"(): Невозможно модифицировать позицию!");
      Print(__FUNCTION__,"(): OrderSend(): ",ResultRetcodeDescription(result.retcode));
     }
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| коррекция размера отложенного ордера до допустимого значения     |
//+------------------------------------------------------------------+
bool StopCorrect(string symbol,int &Stop)
  {
//----
   long Extrem_Stop;
   if(!SymbolInfoInteger(symbol,SYMBOL_TRADE_STOPS_LEVEL,Extrem_Stop)) return(false);
   if(Stop<Extrem_Stop) Stop=int(Extrem_Stop);
//----
   return(true);
  }
//+------------------------------------------------------------------+
//| возврат стрингового результата торговой операции по его коду     |
//+------------------------------------------------------------------+
string ResultRetcodeDescription(int retcode)
  {
   string str;
//----
   switch(retcode)
     {
      case TRADE_RETCODE_REQUOTE: str="Реквота"; break;
      case TRADE_RETCODE_REJECT: str="Запрос отвергнут"; break;
      case TRADE_RETCODE_CANCEL: str="Запрос отменен трейдером"; break;
      case TRADE_RETCODE_PLACED: str="Ордер размещен"; break;
      case TRADE_RETCODE_DONE: str="Заявка выполнена"; break;
      case TRADE_RETCODE_DONE_PARTIAL: str="Заявка выполнена частично"; break;
      case TRADE_RETCODE_ERROR: str="Ошибка обработки запроса"; break;
      case TRADE_RETCODE_TIMEOUT: str="Запрос отменен по истечению времени";break;
      case TRADE_RETCODE_INVALID: str="Неправильный запрос"; break;
      case TRADE_RETCODE_INVALID_VOLUME: str="Неправильный объем в запросе"; break;
      case TRADE_RETCODE_INVALID_PRICE: str="Неправильная цена в запросе"; break;
      case TRADE_RETCODE_INVALID_STOPS: str="Неправильные стопы в запросе"; break;
      case TRADE_RETCODE_TRADE_DISABLED: str="Торговля запрещена"; break;
      case TRADE_RETCODE_MARKET_CLOSED: str="Рынок закрыт"; break;
      case TRADE_RETCODE_NO_MONEY: str="Нет достаточных денежных средств для выполнения запроса"; break;
      case TRADE_RETCODE_PRICE_CHANGED: str="Цены изменились"; break;
      case TRADE_RETCODE_PRICE_OFF: str="Отсутствуют котировки для обработки запроса"; break;
      case TRADE_RETCODE_INVALID_EXPIRATION: str="Неверная дата истечения ордера в запросе"; break;
      case TRADE_RETCODE_ORDER_CHANGED: str="Состояние ордера изменилось"; break;
      case TRADE_RETCODE_TOO_MANY_REQUESTS: str="Слишком частые запросы"; break;
      case TRADE_RETCODE_NO_CHANGES: str="В запросе нет изменений"; break;
      case TRADE_RETCODE_SERVER_DISABLES_AT: str="Автотрейдинг запрещен сервером"; break;
      case TRADE_RETCODE_CLIENT_DISABLES_AT: str="Автотрейдинг запрещен клиентским терминалом"; break;
      case TRADE_RETCODE_LOCKED: str="Запрос заблокирован для обработки"; break;
      case TRADE_RETCODE_FROZEN: str="Ордер или позиция заморожены"; break;
      case TRADE_RETCODE_INVALID_FILL: str="Указан неподдерживаемый тип исполнения ордера по остатку"; break;
      case TRADE_RETCODE_CONNECTION: str="Нет соединения с торговым сервером"; break;
      case TRADE_RETCODE_ONLY_REAL: str="Операция разрешена только для реальных счетов"; break;
      case TRADE_RETCODE_LIMIT_ORDERS: str="Достигнут лимит на количество отложенных ордеров"; break;
      case TRADE_RETCODE_LIMIT_VOLUME: str="Достигнут лимит на объем ордеров и позиций для данного символа"; break;
      case TRADE_RETCODE_INVALID_ORDER: str="Неверный или запрещённый тип ордера"; break;
      case TRADE_RETCODE_POSITION_CLOSED: str="Позиция с указанным POSITION_IDENTIFIER уже закрыта"; break;
      case TRADE_RETCODE_INVALID_CLOSE_VOLUME: str="Закрываемый объем превышает текущий объем позиции"; break;
      case TRADE_RETCODE_CLOSE_ORDER_EXIST: str="Для указанной позиции уже есть ордер на закрытие"; break;
      case TRADE_RETCODE_LIMIT_POSITIONS: str="Количество открытых позиций, которое можно одновременно иметь на счете, может быть ограничено настройками сервера"; break;
      //case : str=""; break;
      //case : str=""; break;
      //case : str=""; break;
      //case : str=""; break;
      default: str="Неизвестный результат";
     }
//----
   return(str);
  }
//+------------------------------------------------------------------+

