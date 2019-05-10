
//+------------------------------------------------------------------+ 
2 //|                                                        Trade.mqh | 
3 //|                   Copyright 2009-2013, MetaQuotes Software Corp. | 
4 //|                                              http://www.mql5.com | 
5 //+------------------------------------------------------------------+ 
6 #include <Object.mqh> 
7 #include "SymbolInfo.mqh" 
8 #include "OrderInfo.mqh" 
9 #include "HistoryOrderInfo.mqh" 
10 #include "PositionInfo.mqh" 
11 #include "DealInfo.mqh" 
12 //+------------------------------------------------------------------+ 
13 //| enumerations                                                     | 
14 //+------------------------------------------------------------------+ 
15 enum ENUM_LOG_LEVELS 
16   { 
17    LOG_LEVEL_NO    =0, 
18    LOG_LEVEL_ERRORS=1, 
19    LOG_LEVEL_ALL   =2 
20   }; 
21 //+------------------------------------------------------------------+ 
22 //| Class CTrade.                                                    | 
23 //| Appointment: Class trade operations.                             | 
24 //|              Derives from class CObject.                         | 
25 //+------------------------------------------------------------------+ 
26 class CTrade : public CObject 
27   { 
28 protected: 
29    MqlTradeRequest   m_request;         // request data 
30    MqlTradeResult    m_result;          // result data 
31    MqlTradeCheckResult m_check_result;  // result check data 
32    bool              m_async_mode;      // trade mode 
33    ulong             m_magic;           // expert magic number 
34    ulong             m_deviation;       // deviation default 
35    ENUM_ORDER_TYPE_FILLING m_type_filling; 
36    //--- 
37    ENUM_LOG_LEVELS   m_log_level; 
38 
 
39 public: 
40                      CTrade(void); 
41                     ~CTrade(void); 
42    //--- methods of access to protected data 
43    void              LogLevel(const ENUM_LOG_LEVELS log_level)   { m_log_level=log_level;               } 
44    void              Request(MqlTradeRequest &request) const; 
45    ENUM_TRADE_REQUEST_ACTIONS RequestAction(void)          const { return(m_request.action);            } 
46    string            RequestActionDescription(void)        const; 
47    ulong             RequestMagic(void)                    const { return(m_request.magic);             } 
48    ulong             RequestOrder(void)                    const { return(m_request.order);             } 
49    string            RequestSymbol(void)                   const { return(m_request.symbol);            } 
50    double            RequestVolume(void)                   const { return(m_request.volume);            } 
51    double            RequestPrice(void)                    const { return(m_request.price);             } 
52    double            RequestStopLimit(void)                const { return(m_request.stoplimit);         } 
53    double            RequestSL(void)                       const { return(m_request.sl);                } 
54    double            RequestTP(void)                       const { return(m_request.tp);                } 
55    ulong             RequestDeviation(void)                const { return(m_request.deviation);         } 
56    ENUM_ORDER_TYPE   RequestType(void)                     const { return(m_request.type);              } 
57    string            RequestTypeDescription(void) const; 
58    ENUM_ORDER_TYPE_FILLING RequestTypeFilling(void)        const { return(m_request.type_filling);      } 
59    string            RequestTypeFillingDescription(void)   const; 
60    ENUM_ORDER_TYPE_TIME RequestTypeTime(void)              const { return(m_request.type_time);         } 
61    string            RequestTypeTimeDescription(void)      const; 
62    datetime          RequestExpiration(void)               const { return(m_request.expiration);        } 
63    string            RequestComment(void)                  const { return(m_request.comment);           } 
64    //--- 
65    void              Result(MqlTradeResult &result)        const; 
66    uint              ResultRetcode(void)                   const { return(m_result.retcode);            } 
67    string            ResultRetcodeDescription(void)        const; 
68    ulong             ResultDeal(void)                      const { return(m_result.deal);               } 
69    ulong             ResultOrder(void)                     const { return(m_result.order);              } 
70    double            ResultVolume(void)                    const { return(m_result.volume);             } 
71    double            ResultPrice(void)                     const { return(m_result.price);              } 
72    double            ResultBid(void)                       const { return(m_result.bid);                } 
73    double            ResultAsk(void)                       const { return(m_result.ask);                } 
74    string            ResultComment(void)                   const { return(m_result.comment);            } 
75    //--- 
76    void              CheckResult(MqlTradeCheckResult &check_result) const; 
77    uint              CheckResultRetcode(void)              const { return(m_check_result.retcode);      } 
78    string            CheckResultRetcodeDescription(void)   const; 
79    double            CheckResultBalance(void)              const { return(m_check_result.balance);      } 
80    double            CheckResultEquity(void)               const { return(m_check_result.equity);       } 
81    double            CheckResultProfit(void)               const { return(m_check_result.profit);       } 
82    double            CheckResultMargin(void)               const { return(m_check_result.margin);       } 
83    double            CheckResultMarginFree(void)           const { return(m_check_result.margin_free);  } 
84    double            CheckResultMarginLevel(void)          const { return(m_check_result.margin_level); } 
85    string            CheckResultComment(void)              const { return(m_check_result.comment);      } 
86    //--- trade methods 
87    void              SetAsyncMode(const bool mode)               { m_async_mode=mode;                   } 
88    void              SetExpertMagicNumber(const ulong magic)     { m_magic=magic;                       } 
89    void              SetDeviationInPoints(const ulong deviation) { m_deviation=deviation;               } 
90    void              SetTypeFilling(const ENUM_ORDER_TYPE_FILLING filling) { m_type_filling=filling;    } 
91    //--- methods for working with positions 
92    bool              PositionOpen(const string symbol,const ENUM_ORDER_TYPE order_type,const double volume, 
93                                   const double price,const double sl,const double tp,const string comment=""); 
94    bool              PositionModify(const string symbol,const double sl,const double tp); 
95    bool              PositionClose(const string symbol,const ulong deviation=ULONG_MAX); 
96    //--- methods for working with pending orders 
97    bool              OrderOpen(const string symbol,const ENUM_ORDER_TYPE order_type,const double volume, 
98                                const double limit_price,const double price,const double sl,const double tp, 
99                                ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0, 
100                                const string comment=""); 
101    bool              OrderModify(const ulong ticket,const double price,const double sl,const double tp, 
102                                  const ENUM_ORDER_TYPE_TIME type_time,const datetime expiration,const double stoplimit=0.0); 
103    bool              OrderDelete(const ulong ticket); 
104    //--- additions methods 
105    bool              Buy(const double volume,const string symbol=NULL,double price=0.0,const double sl=0.0,const double tp=0.0,const string comment=""); 
106    bool              Sell(const double volume,const string symbol=NULL,double price=0.0,const double sl=0.0,const double tp=0.0,const string comment=""); 
107    bool              BuyLimit(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
108                               const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment=""); 
109    bool              BuyStop(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
110                              const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment=""); 
111    bool              SellLimit(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
112                                const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment=""); 
113    bool              SellStop(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
114                               const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment=""); 
115    //--- method check 
116    virtual bool      OrderCheck(const MqlTradeRequest &request,MqlTradeCheckResult &check_result); 
117    virtual bool      OrderSend(const MqlTradeRequest &request,MqlTradeResult &result); 
118    //--- info methods 
119    void              PrintRequest(void) const; 
120    void              PrintResult(void) const; 
121    //--- positions 
122    string            FormatPositionType(string &str,const uint type) const; 
123    //--- orders 
124    string            FormatOrderType(string &str,const uint type) const; 
125    string            FormatOrderStatus(string &str,const uint status) const; 
126    string            FormatOrderTypeFilling(string &str,const uint type) const; 
127    string            FormatOrderTypeTime(string &str,const uint type) const; 
128    string            FormatOrderPrice(string &str,const double price_order,const double price_trigger,const uint digits) const; 
129    //--- trade request 
130    string            FormatRequest(string &str,const MqlTradeRequest &request) const; 
131    string            FormatRequestResult(string &str,const MqlTradeRequest &request,const MqlTradeResult &result) const; 
132 
 
133 protected: 
134    bool              FillingCheck(const string symbol); 
135    bool              ExpirationCheck(const string symbol); 
136    bool              OrderTypeCheck(const string symbol); 
137    void              ClearStructures(void); 
138    bool              IsStopped(const string function); 
139   }; 
140 //+------------------------------------------------------------------+ 
141 //| Constructor                                                      | 
142 //+------------------------------------------------------------------+ 
143 CTrade::CTrade(void) : m_async_mode(false), 
144                        m_magic(0), 
145                        m_deviation(10), 
146                        m_type_filling(ORDER_FILLING_FOK), 
147                        m_log_level(LOG_LEVEL_ERRORS) 
148 
 
149   { 
150 //--- initialize protected data 
151    ClearStructures(); 
152 //--- check programm mode 
153    if(MQL5InfoInteger(MQL5_OPTIMIZATION)) 
154       m_log_level=LOG_LEVEL_NO; 
155    if(MQL5InfoInteger(MQL5_TESTING)) 
156       m_log_level=LOG_LEVEL_ALL; 
157   } 
158 //+------------------------------------------------------------------+ 
159 //| Destructor                                                       | 
160 //+------------------------------------------------------------------+ 
161 CTrade::~CTrade(void) 
162   { 
163   } 
164 //+------------------------------------------------------------------+ 
165 //| Get the request structure                                        | 
166 //+------------------------------------------------------------------+ 
167 void CTrade::Request(MqlTradeRequest &request) const 
168   { 
169    request.action      =m_request.action; 
170    request.magic       =m_request.magic; 
171    request.order       =m_request.order; 
172    request.symbol      =m_request.symbol; 
173    request.volume      =m_request.volume; 
174    request.price       =m_request.price; 
175    request.stoplimit   =m_request.stoplimit; 
176    request.sl          =m_request.sl; 
177    request.tp          =m_request.tp; 
178    request.deviation   =m_request.deviation; 
179    request.type        =m_request.type; 
180    request.type_filling=m_request.type_filling; 
181    request.type_time   =m_request.type_time; 
182    request.expiration  =m_request.expiration; 
183    request.comment     =m_request.comment; 
184   } 
185 //+------------------------------------------------------------------+ 
186 //| Get the trade action as string                                   | 
187 //+------------------------------------------------------------------+ 
188 string CTrade::RequestActionDescription(void) const 
189   { 
190    string str; 
191 //--- 
192    FormatRequest(str,m_request); 
193 //--- 
194    return(str); 
195   } 
196 //+------------------------------------------------------------------+ 
197 //| Get the order type as string                                     | 
198 //+------------------------------------------------------------------+ 
199 string CTrade::RequestTypeDescription(void) const 
200   { 
201    string str; 
202 //--- 
203    FormatOrderType(str,(uint)m_request.order); 
204 //--- 
205    return(str); 
206   } 
207 //+------------------------------------------------------------------+ 
208 //| Get the order type filling as string                             | 
209 //+------------------------------------------------------------------+ 
210 string CTrade::RequestTypeFillingDescription(void) const 
211   { 
212    string str; 
213 //--- 
214    FormatOrderTypeFilling(str,m_request.type_filling); 
215 //--- 
216    return(str); 
217   } 
218 //+------------------------------------------------------------------+ 
219 //| Get the order type time as string                                | 
220 //+------------------------------------------------------------------+ 
221 string CTrade::RequestTypeTimeDescription(void) const 
222   { 
223    string str; 
224 //--- 
225    FormatOrderTypeTime(str,m_request.type_time); 
226 //--- 
227    return(str); 
228   } 
229 //+------------------------------------------------------------------+ 
230 //| Get the result structure                                         | 
231 //+------------------------------------------------------------------+ 
232 void CTrade::Result(MqlTradeResult &result) const 
233   { 
234    result.retcode   =m_result.retcode; 
235    result.deal      =m_result.deal; 
236    result.order     =m_result.order; 
237    result.volume    =m_result.volume; 
238    result.price     =m_result.price; 
239    result.bid       =m_result.bid; 
240    result.ask       =m_result.ask; 
241    result.comment   =m_result.comment; 
242    result.request_id=m_result.request_id; 
243   } 
244 //+------------------------------------------------------------------+ 
245 //| Get the retcode value as string                                  | 
246 //+------------------------------------------------------------------+ 
247 string CTrade::ResultRetcodeDescription(void) const 
248   { 
249    string str; 
250 //--- 
251    FormatRequestResult(str,m_request,m_result); 
252 //--- 
253    return(str); 
254   } 
255 //+------------------------------------------------------------------+ 
256 //| Get the check result structure                                   | 
257 //+------------------------------------------------------------------+ 
258 void CTrade::CheckResult(MqlTradeCheckResult &check_result) const 
259   { 
260 //--- copy structure 
261    check_result.retcode     =m_check_result.retcode; 
262    check_result.balance     =m_check_result.balance; 
263    check_result.equity      =m_check_result.equity; 
264    check_result.profit      =m_check_result.profit; 
265    check_result.margin      =m_check_result.margin; 
266    check_result.margin_free =m_check_result.margin_free; 
267    check_result.margin_level=m_check_result.margin_level; 
268    check_result.comment     =m_check_result.comment; 
269   } 
270 //+------------------------------------------------------------------+ 
271 //| Get the check retcode value as string                            | 
272 //+------------------------------------------------------------------+ 
273 string CTrade::CheckResultRetcodeDescription(void) const 
274   { 
275    string str; 
276    MqlTradeResult result; 
277 //--- 
278    result.retcode=m_check_result.retcode; 
279    FormatRequestResult(str,m_request,result); 
280 //--- 
281    return(str); 
282   } 
283 //+------------------------------------------------------------------+ 
284 //| Open position                                                    | 
285 //+------------------------------------------------------------------+ 
286 bool CTrade::PositionOpen(const string symbol,const ENUM_ORDER_TYPE order_type,const double volume, 
287                           const double price,const double sl,const double tp,const string comment) 
288   { 
289 //--- check stopped 
290    if(IsStopped(__FUNCTION__)) 
291       return(false); 
292 //--- clean 
293    ClearStructures(); 
294 //--- check 
295    if(order_type!=ORDER_TYPE_BUY && order_type!=ORDER_TYPE_SELL) 
296      { 
297       m_result.retcode=TRADE_RETCODE_INVALID; 
298       m_result.comment="Invalid order type"; 
299       return(false); 
300      } 
301 //--- setting request 
302    m_request.action      =TRADE_ACTION_DEAL; 
303    m_request.symbol      =symbol; 
304    m_request.magic       =m_magic; 
305    m_request.volume      =volume; 
306    m_request.type        =order_type; 
307    m_request.price       =price; 
308    m_request.sl          =sl; 
309    m_request.tp          =tp; 
310    m_request.deviation   =m_deviation; 
311 //--- check order type 
312    if(!OrderTypeCheck(symbol)) 
313       return(false); 
314 //--- check filling 
315    if(!FillingCheck(symbol)) 
316       return(false); 
317    m_request.comment=comment; 
318 //--- action and return the result 
319    return(OrderSend(m_request,m_result)); 
320   } 
321 //+------------------------------------------------------------------+ 
322 //| Modify specified opened position                                 | 
323 //+------------------------------------------------------------------+ 
324 bool CTrade::PositionModify(const string symbol,const double sl,const double tp) 
325   { 
326 //--- check stopped 
327    if(IsStopped(__FUNCTION__)) 
328       return(false); 
329 //--- clean 
330    ClearStructures(); 
331 //--- setting request 
332    m_request.action=TRADE_ACTION_SLTP; 
333    m_request.symbol=symbol; 
334    m_request.magic =m_magic; 
335    m_request.sl    =sl; 
336    m_request.tp    =tp; 
337 //--- action and return the result 
338    return(OrderSend(m_request,m_result)); 
339   } 
340 //+------------------------------------------------------------------+ 
341 //| Close specified opened position                                  | 
342 //+------------------------------------------------------------------+ 
343 bool CTrade::PositionClose(const string symbol,const ulong deviation) 
344   { 
345    bool   partial_close=false; 
346    int    retry_count  =10; 
347    uint   retcode      =TRADE_RETCODE_REJECT; 
348 //--- check stopped 
349    if(IsStopped(__FUNCTION__)) 
350       return(false); 
351 //--- clean 
352    ClearStructures(); 
353    do 
354      { 
355       //--- check 
356       if(PositionSelect(symbol)) 
357         { 
358          if((ENUM_POSITION_TYPE)PositionGetInteger(POSITION_TYPE)==POSITION_TYPE_BUY) 
359            { 
360             //--- prepare request for close BUY position 
361             m_request.type =ORDER_TYPE_SELL; 
362             m_request.price=SymbolInfoDouble(symbol,SYMBOL_BID); 
363            } 
364          else 
365            { 
366             //--- prepare request for close SELL position 
367             m_request.type =ORDER_TYPE_BUY; 
368             m_request.price=SymbolInfoDouble(symbol,SYMBOL_ASK); 
369            } 
370         } 
371       else 
372         { 
373          //--- position not found 
374          m_result.retcode=retcode; 
375          return(false); 
376         } 
377       //--- setting request 
378       m_request.action      =TRADE_ACTION_DEAL; 
379       m_request.symbol      =symbol; 
380       m_request.magic       =m_magic; 
381       m_request.deviation   =(deviation==ULONG_MAX) ? m_deviation : deviation; 
382       //--- check filling 
383       if(!FillingCheck(symbol)) 
384          return(false); 
385       m_request.volume=PositionGetDouble(POSITION_VOLUME); 
386       //--- check volume 
387       double max_volume=SymbolInfoDouble(symbol,SYMBOL_VOLUME_MAX); 
388       if(m_request.volume>max_volume) 
389         { 
390          m_request.volume=max_volume; 
391          partial_close=true; 
392         } 
393       else 
394          partial_close=false; 
395       //--- order send 
396       if(!OrderSend(m_request,m_result)) 
397         { 
398          if(--retry_count!=0) continue; 
399          if(retcode==TRADE_RETCODE_DONE_PARTIAL) 
400             m_result.retcode=retcode; 
401          return(false); 
402         } 
403       //--- WARNING. If position volume exceeds the maximum volume allowed for deal, 
404       //--- and when the asynchronous trade mode is on, for safety reasons, position is closed not completely, 
405       //--- but partially. It is decreased by the maximum volume allowed for deal. 
406       if(m_async_mode) 
407          break; 
408       retcode=TRADE_RETCODE_DONE_PARTIAL; 
409       if(partial_close) 
410          Sleep(1000); 
411      } 
412    while(partial_close); 
413 //--- succeed 
414    return(true); 
415   } 
416 //+------------------------------------------------------------------+ 
417 //| Installation pending order                                       | 
418 //+------------------------------------------------------------------+ 
419 bool CTrade::OrderOpen(const string symbol,const ENUM_ORDER_TYPE order_type,const double volume,const double limit_price, 
420                        const double price,const double sl,const double tp, 
421                        ENUM_ORDER_TYPE_TIME type_time,const datetime expiration,const string comment) 
422   { 
423 //--- check stopped 
424    if(IsStopped(__FUNCTION__)) 
425       return(false); 
426 //--- clean 
427    ClearStructures(); 
428 //--- check order type 
429    if(order_type==ORDER_TYPE_BUY || order_type==ORDER_TYPE_SELL) 
430      { 
431       m_result.retcode=TRADE_RETCODE_INVALID; 
432       m_result.comment="Invalid order type"; 
433       return(false); 
434      } 
435 //--- check order expiration 
436    if(type_time==ORDER_TIME_GTC && expiration==0) 
437      { 
438       int exp=(int)SymbolInfoInteger(symbol,SYMBOL_EXPIRATION_MODE); 
439       if((exp&SYMBOL_EXPIRATION_GTC)!=SYMBOL_EXPIRATION_GTC) 
440         { 
441          //--- if you place order for an unlimited time and if placing of such orders is prohibited 
442          //--- try to place order with expiration at the end of the day 
443          if((exp&SYMBOL_EXPIRATION_DAY)!=SYMBOL_EXPIRATION_DAY) 
444            { 
445             //--- if even this is not possible - error 
446             Print(__FUNCTION__,": Error: Unable to place order without explicitly specified expiration time"); 
447             m_result.retcode=TRADE_RETCODE_INVALID_EXPIRATION; 
448             m_result.comment="Invalid expiration type"; 
449             return(false); 
450            } 
451          type_time=ORDER_TIME_DAY; 
452         } 
453      } 
454 //--- setting request 
455    m_request.action      =TRADE_ACTION_PENDING; 
456    m_request.symbol      =symbol; 
457    m_request.magic       =m_magic; 
458    m_request.volume      =volume; 
459    m_request.type        =order_type; 
460    m_request.stoplimit   =limit_price; 
461    m_request.price       =price; 
462    m_request.sl          =sl; 
463    m_request.tp          =tp; 
464    m_request.type_filling=m_type_filling; 
465    m_request.type_time   =type_time; 
466    m_request.expiration  =expiration; 
467 //--- check order type 
468    if(!OrderTypeCheck(symbol)) 
469       return(false); 
470 //--- check filling 
471    if(!FillingCheck(symbol)) 
472      { 
473       m_result.retcode=TRADE_RETCODE_INVALID_FILL; 
474       Print(__FUNCTION__+": Invalid filling type"); 
475       return(false); 
476      } 
477 //--- check expiration 
478    if(!ExpirationCheck(symbol)) 
479      { 
480       m_result.retcode=TRADE_RETCODE_INVALID_EXPIRATION; 
481       Print(__FUNCTION__+": Invalid expiration type"); 
482       return(false); 
483      } 
484    m_request.comment=comment; 
485 //--- action and return the result 
486    return(OrderSend(m_request,m_result)); 
487   } 
488 //+------------------------------------------------------------------+ 
489 //| Modify specified pending order                                   | 
490 //+------------------------------------------------------------------+ 
491 bool CTrade::OrderModify(const ulong ticket,const double price,const double sl,const double tp, 
492                          const ENUM_ORDER_TYPE_TIME type_time,const datetime expiration,const double stoplimit) 
493   { 
494 //--- check stopped 
495    if(IsStopped(__FUNCTION__)) 
496       return(false); 
497 //--- clean 
498    ClearStructures(); 
499 //--- setting request 
500    m_request.action      =TRADE_ACTION_MODIFY; 
501    m_request.magic       =m_magic; 
502    m_request.order       =ticket; 
503    m_request.price       =price; 
504    m_request.stoplimit   =stoplimit; 
505    m_request.sl          =sl; 
506    m_request.tp          =tp; 
507    m_request.type_time   =type_time; 
508    m_request.expiration  =expiration; 
509 //--- action and return the result 
510    return(OrderSend(m_request,m_result)); 
511   } 
512 //+------------------------------------------------------------------+ 
513 //| Delete specified pending order                                   | 
514 //+------------------------------------------------------------------+ 
515 bool CTrade::OrderDelete(const ulong ticket) 
516   { 
517 //--- check stopped 
518    if(IsStopped(__FUNCTION__)) 
519       return(false); 
520 //--- clean 
521    ClearStructures(); 
522 //--- setting request 
523    m_request.action    =TRADE_ACTION_REMOVE; 
524    m_request.magic     =m_magic; 
525    m_request.order     =ticket; 
526 //--- action and return the result 
527    return(OrderSend(m_request,m_result)); 
528   } 
529 //+------------------------------------------------------------------+ 
530 //| Output full information of request to log                        | 
531 //+------------------------------------------------------------------+ 
532 void CTrade::PrintRequest(void) const 
533   { 
534    if(m_log_level<LOG_LEVEL_ALL) 
535       return; 
536 //--- 
537    string str; 
538    printf("%s",FormatRequest(str,m_request)); 
539   } 
540 //+------------------------------------------------------------------+ 
541 //| Output full information of result to log                         | 
542 //+------------------------------------------------------------------+ 
543 void CTrade::PrintResult(void) const 
544   { 
545    if(m_log_level<LOG_LEVEL_ALL) 
546       return; 
547 //--- 
548    string str; 
549    printf("%s",FormatRequestResult(str,m_request,m_result)); 
550   } 
551 //+------------------------------------------------------------------+ 
552 //| Clear structures m_request,m_result and m_check_result           | 
553 //+------------------------------------------------------------------+ 
554 void CTrade::ClearStructures(void) 
555   { 
556    ZeroMemory(m_request); 
557    ZeroMemory(m_result); 
558    ZeroMemory(m_check_result); 
559   } 
560 //+------------------------------------------------------------------+ 
561 //| Checks forced shutdown of MQL5-program                           | 
562 //+------------------------------------------------------------------+ 
563 bool CTrade::IsStopped(const string function) 
564   { 
565    if(!IsStopped()) 
566       return(false); 
567 //--- MQL5 program is stopped 
568    printf("%s: MQL5 program is stopped. Trading is disabled",function); 
569    m_result.retcode=TRADE_RETCODE_CLIENT_DISABLES_AT; 
570    return(true); 
571   } 
572 //+------------------------------------------------------------------+ 
573 //| Buy operation                                                    | 
574 //+------------------------------------------------------------------+ 
575 bool CTrade::Buy(const double volume,const string symbol=NULL,double price=0.0,const double sl=0.0,const double tp=0.0,const string comment="") 
576   { 
577    CSymbolInfo sym; 
578 //--- check volume 
579    if(volume<=0.0) 
580      { 
581       m_result.retcode=TRADE_RETCODE_INVALID_VOLUME; 
582       return(false); 
583      } 
584 //--- check symbol 
585    sym.Name((symbol==NULL)?Symbol():symbol); 
586 //--- check price 
587    if(price==0.0) 
588      { 
589       sym.RefreshRates(); 
590       price=sym.Ask(); 
591      } 
592 //--- 
593    return(PositionOpen(sym.Name(),ORDER_TYPE_BUY,volume,price,sl,tp,comment)); 
594   } 
595 //+------------------------------------------------------------------+ 
596 //| Sell operation                                                   | 
597 //+------------------------------------------------------------------+ 
598 bool CTrade::Sell(const double volume,const string symbol=NULL,double price=0.0,const double sl=0.0,const double tp=0.0,const string comment="") 
599   { 
600    CSymbolInfo sym; 
601 //--- check volume 
602    if(volume<=0.0) 
603      { 
604       m_result.retcode=TRADE_RETCODE_INVALID_VOLUME; 
605       return(false); 
606      } 
607 //--- check symbol 
608    sym.Name((symbol==NULL)?Symbol():symbol); 
609 //--- check price 
610    if(price==0.0) 
611      { 
612       sym.RefreshRates(); 
613       price=sym.Bid(); 
614      } 
615 //--- 
616    return(PositionOpen(sym.Name(),ORDER_TYPE_SELL,volume,price,sl,tp,comment)); 
617   } 
618 //+------------------------------------------------------------------+ 
619 //| Send BUY_LIMIT order                                             | 
620 //+------------------------------------------------------------------+ 
621 bool CTrade::BuyLimit(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
622                       const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment="") 
623   { 
624    string sym; 
625 //--- check volume 
626    if(volume<=0.0) 
627      { 
628       m_result.retcode=TRADE_RETCODE_INVALID_VOLUME; 
629       return(false); 
630      } 
631 //--- check price 
632    if(price==0.0) 
633      { 
634       m_result.retcode=TRADE_RETCODE_INVALID_PRICE; 
635       return(false); 
636      } 
637 //--- check symbol 
638    sym=(symbol==NULL)?Symbol():symbol; 
639 //--- send "BUY_LIMIT" order 
640    return(OrderOpen(sym,ORDER_TYPE_BUY_LIMIT,volume,0.0,price,sl,tp,type_time,expiration,comment)); 
641   } 
642 //+------------------------------------------------------------------+ 
643 //| Send BUY_STOP order                                              | 
644 //+------------------------------------------------------------------+ 
645 bool CTrade::BuyStop(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
646                      const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment="") 
647   { 
648    string sym; 
649 //--- check volume 
650    if(volume<=0.0) 
651      { 
652       m_result.retcode=TRADE_RETCODE_INVALID_VOLUME; 
653       return(false); 
654      } 
655 //--- check price 
656    if(price==0.0) 
657      { 
658       m_result.retcode=TRADE_RETCODE_INVALID_PRICE; 
659       return(false); 
660      } 
661 //--- check symbol 
662    sym=(symbol==NULL)?Symbol():symbol; 
663 //--- send "BUY_STOP" order 
664    return(OrderOpen(sym,ORDER_TYPE_BUY_STOP,volume,0.0,price,sl,tp,type_time,expiration,comment)); 
665   } 
666 //+------------------------------------------------------------------+ 
667 //| Send SELL_LIMIT order                                            | 
668 //+------------------------------------------------------------------+ 
669 bool CTrade::SellLimit(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
670                        const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment="") 
671   { 
672    string sym; 
673 //--- check volume 
674    if(volume<=0.0) 
675      { 
676       m_result.retcode=TRADE_RETCODE_INVALID_VOLUME; 
677       return(false); 
678      } 
679 //--- check price 
680    if(price==0.0) 
681      { 
682       m_result.retcode=TRADE_RETCODE_INVALID_PRICE; 
683       return(false); 
684      } 
685 //--- check symbol 
686    sym=(symbol==NULL)?Symbol():symbol; 
687 //--- send "SELL_LIMIT" order 
688    return(OrderOpen(sym,ORDER_TYPE_SELL_LIMIT,volume,0.0,price,sl,tp,type_time,expiration,comment)); 
689   } 
690 //+------------------------------------------------------------------+ 
691 //| Send SELL_STOP order                                             | 
692 //+------------------------------------------------------------------+ 
693 bool CTrade::SellStop(const double volume,const double price,const string symbol=NULL,const double sl=0.0,const double tp=0.0, 
694                       const ENUM_ORDER_TYPE_TIME type_time=ORDER_TIME_GTC,const datetime expiration=0,const string comment="") 
695   { 
696    string sym; 
697 //--- check volume 
698    if(volume<=0.0) 
699      { 
700       m_result.retcode=TRADE_RETCODE_INVALID_VOLUME; 
701       return(false); 
702      } 
703 //--- check price 
704    if(price==0.0) 
705      { 
706       m_result.retcode=TRADE_RETCODE_INVALID_PRICE; 
707       return(false); 
708      } 
709 //--- check symbol 
710    sym=(symbol==NULL)?Symbol():symbol; 
711 //--- send "SELL_STOP" order 
712    return(OrderOpen(sym,ORDER_TYPE_SELL_STOP,volume,0.0,price,sl,tp,type_time,expiration,comment)); 
713   } 
714 //+------------------------------------------------------------------+ 
715 //| Converts the position type to text                               | 
716 //+------------------------------------------------------------------+ 
717 string CTrade::FormatPositionType(string &str,const uint type) const 
718   { 
719 //--- clean 
720    str=""; 
721 //--- see the type 
722    switch(type) 
723      { 
724       case POSITION_TYPE_BUY : str="buy";  break; 
725       case POSITION_TYPE_SELL: str="sell"; break; 
726 
 
727       default: 
728          str="unknown position type "+(string)type; 
729          break; 
730      } 
731 //--- return the result 
732    return(str); 
733   } 
734 //+------------------------------------------------------------------+ 
735 //| Converts the order type to text                                  | 
736 //+------------------------------------------------------------------+ 
737 string CTrade::FormatOrderType(string &str,const uint type) const 
738   { 
739 //--- clean 
740    str=""; 
741 //--- see the type 
742    switch(type) 
743      { 
744       case ORDER_TYPE_BUY            : str="buy";             break; 
745       case ORDER_TYPE_SELL           : str="sell";            break; 
746       case ORDER_TYPE_BUY_LIMIT      : str="buy limit";       break; 
747       case ORDER_TYPE_SELL_LIMIT     : str="sell limit";      break; 
748       case ORDER_TYPE_BUY_STOP       : str="buy stop";        break; 
749       case ORDER_TYPE_SELL_STOP      : str="sell stop";       break; 
750       case ORDER_TYPE_BUY_STOP_LIMIT : str="buy stop limit";  break; 
751       case ORDER_TYPE_SELL_STOP_LIMIT: str="sell stop limit"; break; 
752 
 
753       default: 
754          str="unknown order type "+(string)type; 
755          break; 
756      } 
757 //--- return the result 
758    return(str); 
759   } 
760 //+------------------------------------------------------------------+ 
761 //| Converts the order filling type to text                          | 
762 //+------------------------------------------------------------------+ 
763 string CTrade::FormatOrderTypeFilling(string &str,const uint type) const 
764   { 
765 //--- clean 
766    str=""; 
767 //--- see the type 
768    switch(type) 
769      { 
770       case ORDER_FILLING_RETURN: str="return remainder"; break; 
771       case ORDER_FILLING_IOC   : str="cancel remainder"; break; 
772       case ORDER_FILLING_FOK   : str="fill or kill";     break; 
773 
 
774       default: 
775          str="unknown type filling "+(string)type; 
776          break; 
777      } 
778 //--- return the result 
779    return(str); 
780   } 
781 //+------------------------------------------------------------------+ 
782 //| Converts the type of order by expiration to text                 | 
783 //+------------------------------------------------------------------+ 
784 string CTrade::FormatOrderTypeTime(string &str,const uint type) const 
785   { 
786 //--- clean 
787    str=""; 
788 //--- see the type 
789    switch(type) 
790      { 
791       case ORDER_TIME_GTC          : str="gtc";           break; 
792       case ORDER_TIME_DAY          : str="day";           break; 
793       case ORDER_TIME_SPECIFIED    : str="specified";     break; 
794       case ORDER_TIME_SPECIFIED_DAY: str="specified day"; break; 
795 
 
796       default: 
797          str="unknown type time "+(string)type; 
798          break; 
799      } 
800 //--- return the result 
801    return(str); 
802   } 
803 //+------------------------------------------------------------------+ 
804 //| Converts the order prices to text                                | 
805 //+------------------------------------------------------------------+ 
806 string CTrade::FormatOrderPrice(string &str,const double price_order,const double price_trigger,const uint digits) const 
807   { 
808    string price,trigger; 
809 //--- clean 
810    str=""; 
811 //--- Is there its trigger price? 
812    if(price_trigger) 
813      { 
814       price  =DoubleToString(price_order,digits); 
815       trigger=DoubleToString(price_trigger,digits); 
816       str    =StringFormat("%s (%s)",price,trigger); 
817      } 
818    else 
819       str=DoubleToString(price_order,digits); 
820 //--- return the result 
821    return(str); 
822   } 
823 //+------------------------------------------------------------------+ 
824 //| Converts the parameters of a trade request to text               | 
825 //+------------------------------------------------------------------+ 
826 string CTrade::FormatRequest(string &str,const MqlTradeRequest &request) const 
827   { 
828    string      type,price,price_new; 
829    string      tmp; 
830    CSymbolInfo symbol; 
831 //--- clean 
832    str=""; 
833 //--- set up 
834    int digits=5; 
835    if(request.symbol!=NULL) 
836      { 
837       if(symbol.Name(request.symbol)) 
838          digits=symbol.Digits(); 
839      } 
840 //--- see what is wanted 
841    switch(request.action) 
842      { 
843       //--- instant execution of a deal 
844       case TRADE_ACTION_DEAL: 
845          switch(symbol.TradeExecution()) 
846            { 
847             //--- request execution 
848             case SYMBOL_TRADE_EXECUTION_REQUEST: 
849                str=StringFormat("request %s %s %s at %s", 
850                                 FormatOrderType(type,request.type), 
851                                 DoubleToString(request.volume,2), 
852                                 request.symbol, 
853                                 DoubleToString(request.price,digits)); 
854             //--- Is there SL or TP? 
855             if(request.sl!=0.0) 
856               { 
857                tmp=StringFormat(" sl: %s",DoubleToString(request.sl,digits)); 
858                str+=tmp; 
859               } 
860             if(request.tp!=0.0) 
861               { 
862                tmp=StringFormat(" tp: %s",DoubleToString(request.tp,digits)); 
863                str+=tmp; 
864               } 
865             break; 
866             //--- instant execution 
867             case SYMBOL_TRADE_EXECUTION_INSTANT: 
868                str=StringFormat("instant %s %s %s at %s", 
869                                 FormatOrderType(type,request.type), 
870                                 DoubleToString(request.volume,2), 
871                                 request.symbol, 
872                                 DoubleToString(request.price,digits)); 
873             //--- Is there SL or TP? 
874             if(request.sl!=0.0) 
875               { 
876                tmp=StringFormat(" sl: %s",DoubleToString(request.sl,digits)); 
877                str+=tmp; 
878               } 
879             if(request.tp!=0.0) 
880               { 
881                tmp=StringFormat(" tp: %s",DoubleToString(request.tp,digits)); 
882                str+=tmp; 
883               } 
884             break; 
885             //--- market execution 
886             case SYMBOL_TRADE_EXECUTION_MARKET: 
887                str=StringFormat("market %s %s %s", 
888                                 FormatOrderType(type,request.type), 
889                                 DoubleToString(request.volume,2), 
890                                 request.symbol); 
891             //--- Is there SL or TP? 
892             if(request.sl!=0.0) 
893               { 
894                tmp=StringFormat(" sl: %s",DoubleToString(request.sl,digits)); 
895                str+=tmp; 
896               } 
897             if(request.tp!=0.0) 
898               { 
899                tmp=StringFormat(" tp: %s",DoubleToString(request.tp,digits)); 
900                str+=tmp; 
901               } 
902             break; 
903             //--- exchange execution 
904             case SYMBOL_TRADE_EXECUTION_EXCHANGE: 
905                str=StringFormat("exchange %s %s %s", 
906                                 FormatOrderType(type,request.type), 
907                                 DoubleToString(request.volume,2), 
908                                 request.symbol); 
909             //--- Is there SL or TP? 
910             if(request.sl!=0.0) 
911               { 
912                tmp=StringFormat(" sl: %s",DoubleToString(request.sl,digits)); 
913                str+=tmp; 
914               } 
915             if(request.tp!=0.0) 
916               { 
917                tmp=StringFormat(" tp: %s",DoubleToString(request.tp,digits)); 
918                str+=tmp; 
919               } 
920             break; 
921            } 
922          break; 
923          //--- setting a pending order 
924       case TRADE_ACTION_PENDING: 
925          str=StringFormat("%s %s %s at %s", 
926                           FormatOrderType(type,request.type), 
927                           DoubleToString(request.volume,2), 
928                           request.symbol, 
929                           FormatOrderPrice(price,request.price,request.stoplimit,digits)); 
930       //--- Is there SL or TP? 
931       if(request.sl!=0.0) 
932         { 
933          tmp=StringFormat(" sl: %s",DoubleToString(request.sl,digits)); 
934          str+=tmp; 
935         } 
936       if(request.tp!=0.0) 
937         { 
938          tmp=StringFormat(" tp: %s",DoubleToString(request.tp,digits)); 
939          str+=tmp; 
940         } 
941       break; 
942 
 
943       //--- Setting SL/TP 
944       case TRADE_ACTION_SLTP: 
945          str=StringFormat("modify %s (sl: %s, tp: %s)", 
946                           request.symbol, 
947                           DoubleToString(request.sl,digits), 
948                           DoubleToString(request.tp,digits)); 
949       break; 
950 
 
951       //--- modifying a pending order 
952       case TRADE_ACTION_MODIFY: 
953          str=StringFormat("modify #%I64u at %s (sl: %s tp: %s)", 
954                           request.order, 
955                           FormatOrderPrice(price_new,request.price,request.stoplimit,digits), 
956                           DoubleToString(request.sl,digits), 
957                           DoubleToString(request.tp,digits)); 
958       break; 
959 
 
960       //--- deleting a pending order 
961       case TRADE_ACTION_REMOVE: 
962          str=StringFormat("cancel #%I64u",request.order); 
963          break; 
964 
 
965       default: 
966          str="unknown action "+(string)request.action; 
967          break; 
968      } 
969 //--- return the result 
970    return(str); 
971   } 
972 //+------------------------------------------------------------------+ 
973 //| Converts the result of a request to text                         | 
974 //+------------------------------------------------------------------+ 
975 string CTrade::FormatRequestResult(string &str,const MqlTradeRequest &request,const MqlTradeResult &result) const 
976   { 
977    CSymbolInfo symbol; 
978 //--- clean 
979    str=""; 
980 //--- set up 
981    int digits=5; 
982    if(request.symbol!=NULL) 
983      { 
984       if(symbol.Name(request.symbol)) 
985          digits=symbol.Digits(); 
986      } 
987 //--- see the response code 
988    switch(result.retcode) 
989      { 
990       case TRADE_RETCODE_REQUOTE: 
991          str=StringFormat("requote (%s/%s)", 
992                           DoubleToString(result.bid,digits), 
993                           DoubleToString(result.ask,digits)); 
994       break; 
995 
 
996       case TRADE_RETCODE_DONE: 
997          if(request.action==TRADE_ACTION_DEAL &&  
998             (symbol.TradeExecution()==SYMBOL_TRADE_EXECUTION_REQUEST ||  
999             symbol.TradeExecution()==SYMBOL_TRADE_EXECUTION_INSTANT || 
1000             symbol.TradeExecution()==SYMBOL_TRADE_EXECUTION_MARKET)) 
1001             str=StringFormat("done at %s",DoubleToString(result.price,digits)); 
1002       else 
1003          str="done"; 
1004       break; 
1005 
 
1006       case TRADE_RETCODE_DONE_PARTIAL: 
1007          if(request.action==TRADE_ACTION_DEAL &&  
1008             (symbol.TradeExecution()==SYMBOL_TRADE_EXECUTION_REQUEST ||  
1009             symbol.TradeExecution()==SYMBOL_TRADE_EXECUTION_INSTANT || 
1010             symbol.TradeExecution()==SYMBOL_TRADE_EXECUTION_MARKET)) 
1011             str=StringFormat("done partially %s at %s", 
1012                              DoubleToString(result.volume,2), 
1013                              DoubleToString(result.price,digits)); 
1014       else 
1015          str=StringFormat("done partially %s", 
1016                           DoubleToString(result.volume,2)); 
1017       break; 
1018 
 
1019       case TRADE_RETCODE_REJECT            : str="rejected";                        break; 
1020       case TRADE_RETCODE_CANCEL            : str="canceled";                        break; 
1021       case TRADE_RETCODE_PLACED            : str="placed";                          break; 
1022       case TRADE_RETCODE_ERROR             : str="common error";                    break; 
1023       case TRADE_RETCODE_TIMEOUT           : str="timeout";                         break; 
1024       case TRADE_RETCODE_INVALID           : str="invalid request";                 break; 
1025       case TRADE_RETCODE_INVALID_VOLUME    : str="invalid volume";                  break; 
1026       case TRADE_RETCODE_INVALID_PRICE     : str="invalid price";                   break; 
1027       case TRADE_RETCODE_INVALID_STOPS     : str="invalid stops";                   break; 
1028       case TRADE_RETCODE_TRADE_DISABLED    : str="trade disabled";                  break; 
1029       case TRADE_RETCODE_MARKET_CLOSED     : str="market closed";                   break; 
1030       case TRADE_RETCODE_NO_MONEY          : str="not enough money";                break; 
1031       case TRADE_RETCODE_PRICE_CHANGED     : str="price changed";                   break; 
1032       case TRADE_RETCODE_PRICE_OFF         : str="off quotes";                      break; 
1033       case TRADE_RETCODE_INVALID_EXPIRATION: str="invalid expiration";              break; 
1034       case TRADE_RETCODE_ORDER_CHANGED     : str="order changed";                   break; 
1035       case TRADE_RETCODE_TOO_MANY_REQUESTS : str="too many requests";               break; 
1036       case TRADE_RETCODE_NO_CHANGES        : str="no changes";                      break; 
1037       case TRADE_RETCODE_SERVER_DISABLES_AT: str="auto trading disabled by server"; break; 
1038       case TRADE_RETCODE_CLIENT_DISABLES_AT: str="auto trading disabled by client"; break; 
1039       case TRADE_RETCODE_LOCKED            : str="locked";                          break; 
1040       case TRADE_RETCODE_FROZEN            : str="frozen";                          break; 
1041       case TRADE_RETCODE_INVALID_FILL      : str="invalid fill";                    break; 
1042       case TRADE_RETCODE_CONNECTION        : str="no connection";                   break; 
1043       case TRADE_RETCODE_ONLY_REAL         : str="only real";                       break; 
1044       case TRADE_RETCODE_LIMIT_ORDERS      : str="limit orders";                    break; 
1045       case TRADE_RETCODE_LIMIT_VOLUME      : str="limit volume";                    break; 
1046       case TRADE_RETCODE_POSITION_CLOSED   : str="position closed";                 break; 
1047       case TRADE_RETCODE_INVALID_ORDER     : str="invalid order";                   break; 
1048 
 
1049       default: 
1050          str="unknown retcode "+(string)result.retcode; 
1051          break; 
1052      } 
1053 //--- return the result 
1054    return(str); 
1055   } 
1056 //+------------------------------------------------------------------+ 
1057 //| Checks if the m_request structure is filled correctly            | 
1058 //+------------------------------------------------------------------+ 
1059 bool CTrade::OrderCheck(const MqlTradeRequest &request,MqlTradeCheckResult &check_result) 
1060   { 
1061 //--- action and return the result 
1062    return(::OrderCheck(request,check_result)); 
1063   } 
1064 //+------------------------------------------------------------------+ 
1065 //| Checks and corrects type of filling policy                       | 
1066 //+------------------------------------------------------------------+ 
1067 bool CTrade::FillingCheck(const string symbol) 
1068   { 
1069 //--- get execution mode of orders by symbol 
1070    ENUM_SYMBOL_TRADE_EXECUTION exec=(ENUM_SYMBOL_TRADE_EXECUTION)SymbolInfoInteger(symbol,SYMBOL_TRADE_EXEMODE); 
1071 //--- check execution mode 
1072    if(exec==SYMBOL_TRADE_EXECUTION_REQUEST || exec==SYMBOL_TRADE_EXECUTION_INSTANT) 
1073      { 
1074       //--- neccessary filling type will be placed automatically 
1075       return(true); 
1076      } 
1077 //--- get possible filling policy types by symbol 
1078    uint filling=(uint)SymbolInfoInteger(symbol,SYMBOL_FILLING_MODE); 
1079 //--- check execution mode again 
1080    if(exec==SYMBOL_TRADE_EXECUTION_MARKET) 
1081      { 
1082       //--- for the MARKET execution mode 
1083       //--- analyze order 
1084       if(m_request.action!=TRADE_ACTION_PENDING) 
1085         { 
1086          //--- in case of instant execution order 
1087          //--- if the required filling policy is supported, add it to the request 
1088          if(m_type_filling==ORDER_FILLING_FOK && (filling & SYMBOL_FILLING_FOK)!=0) 
1089            { 
1090             m_request.type_filling=m_type_filling; 
1091             return(true); 
1092            } 
1093          if(m_type_filling==ORDER_FILLING_IOC && (filling & SYMBOL_FILLING_IOC)!=0) 
1094            { 
1095             m_request.type_filling=m_type_filling; 
1096             return(true); 
1097            } 
1098          //--- wrong filling policy, set error code 
1099          m_result.retcode=TRADE_RETCODE_INVALID_FILL; 
1100          return(false); 
1101         } 
1102       return(true); 
1103      } 
1104 //--- EXCHANGE execution mode 
1105    switch(m_type_filling) 
1106      { 
1107       case ORDER_FILLING_FOK: 
1108          //--- analyze order 
1109          if(m_request.action==TRADE_ACTION_PENDING) 
1110            { 
1111             //--- in case of pending order 
1112             //--- add the expiration mode to the request 
1113             if(!ExpirationCheck(symbol)) 
1114                m_request.type_time=ORDER_TIME_DAY; 
1115             //--- stop order? 
1116             if(m_request.type==ORDER_TYPE_BUY_STOP || m_request.type==ORDER_TYPE_SELL_STOP) 
1117               { 
1118                //--- in case of stop order 
1119                //--- add the corresponding filling policy to the request 
1120                m_request.type_filling=ORDER_FILLING_RETURN; 
1121                return(true); 
1122               } 
1123             } 
1124          //--- in case of limit order or instant execution order 
1125          //--- if the required filling policy is supported, add it to the request 
1126          if((filling & SYMBOL_FILLING_FOK)!=0) 
1127            { 
1128             m_request.type_filling=m_type_filling; 
1129             return(true); 
1130            } 
1131          //--- wrong filling policy, set error code 
1132          m_result.retcode=TRADE_RETCODE_INVALID_FILL; 
1133          return(false); 
1134       case ORDER_FILLING_IOC: 
1135          //--- analyze order 
1136          if(m_request.action==TRADE_ACTION_PENDING) 
1137            { 
1138             //--- in case of pending order 
1139             //--- add the expiration mode to the request 
1140             if(!ExpirationCheck(symbol)) 
1141                m_request.type_time=ORDER_TIME_DAY; 
1142             //--- stop order? 
1143             if(m_request.type==ORDER_TYPE_BUY_STOP || m_request.type==ORDER_TYPE_SELL_STOP) 
1144               { 
1145                //--- in case of stop order 
1146                //--- add the corresponding filling policy to the request 
1147                m_request.type_filling=ORDER_FILLING_RETURN; 
1148                return(true); 
1149               } 
1150            } 
1151          //--- in case of limit order or instant execution order 
1152          //--- if the required filling policy is supported, add it to the request 
1153          if((filling & SYMBOL_FILLING_IOC)!=0) 
1154            { 
1155             m_request.type_filling=m_type_filling; 
1156             return(true); 
1157            } 
1158          //--- wrong filling policy, set error code 
1159          m_result.retcode=TRADE_RETCODE_INVALID_FILL; 
1160          return(false); 
1161       case ORDER_FILLING_RETURN: 
1162          //--- add filling policy to the request 
1163          m_request.type_filling=m_type_filling; 
1164          return(true); 
1165      } 
1166 //--- unknown execution mode, set error code 
1167    m_result.retcode=TRADE_RETCODE_ERROR; 
1168    return(false); 
1169   } 
1170 //+------------------------------------------------------------------+ 
1171 //| Check expiration type of pending order                           | 
1172 //+------------------------------------------------------------------+ 
1173 bool CTrade::ExpirationCheck(const string symbol) 
1174   { 
1175    CSymbolInfo sym; 
1176 //--- check symbol 
1177    if(!sym.Name((symbol==NULL)?Symbol():symbol)) 
1178       return(false); 
1179 //--- get flags 
1180    int flags=sym.TradeTimeFlags(); 
1181 //--- check type 
1182    switch(m_request.type_time) 
1183      { 
1184       case ORDER_TIME_GTC: 
1185          if((flags&SYMBOL_EXPIRATION_GTC)!=0) 
1186             return(true); 
1187          break; 
1188       case ORDER_TIME_DAY: 
1189          if((flags&SYMBOL_EXPIRATION_DAY)!=0) 
1190             return(true); 
1191          break; 
1192       case ORDER_TIME_SPECIFIED: 
1193          if((flags&SYMBOL_EXPIRATION_SPECIFIED)!=0) 
1194             return(true); 
1195          break; 
1196       case ORDER_TIME_SPECIFIED_DAY: 
1197          if((flags&SYMBOL_EXPIRATION_SPECIFIED_DAY)!=0) 
1198             return(true); 
1199          break; 
1200       default: 
1201          Print(__FUNCTION__+": Unknown expiration type"); 
1202          break; 
1203      } 
1204 //--- failed 
1205    return(false); 
1206   } 
1207 //+------------------------------------------------------------------+ 
1208 //| Checks order                                                     | 
1209 //+------------------------------------------------------------------+ 
1210 bool CTrade::OrderTypeCheck(const string symbol) 
1211   { 
1212    bool res=false;    
1213 //--- check symbol 
1214    CSymbolInfo sym; 
1215    if(!sym.Name((symbol==NULL)?Symbol():symbol)) 
1216       return(false); 
1217 //--- get flags of allowed trade orders 
1218    int flags=sym.OrderMode(); 
1219 //--- depending on the type of order in request 
1220    switch(m_request.type) 
1221      { 
1222       case ORDER_TYPE_BUY: 
1223       case ORDER_TYPE_SELL: 
1224          //--- check possibility of execution 
1225          res=((flags&SYMBOL_ORDER_MARKET)!=0); 
1226          break; 
1227       case ORDER_TYPE_BUY_LIMIT: 
1228       case ORDER_TYPE_SELL_LIMIT: 
1229          //--- check possibility of execution 
1230          res=((flags&SYMBOL_ORDER_LIMIT)!=0); 
1231          break; 
1232       case ORDER_TYPE_BUY_STOP: 
1233       case ORDER_TYPE_SELL_STOP: 
1234          //--- check possibility of execution 
1235          res=((flags&SYMBOL_ORDER_STOP)!=0); 
1236          break; 
1237       case ORDER_TYPE_BUY_STOP_LIMIT: 
1238       case ORDER_TYPE_SELL_STOP_LIMIT: 
1239          //--- check possibility of execution 
1240          res=((flags&SYMBOL_ORDER_STOP_LIMIT)!=0); 
1241          break; 
1242       default: 
1243          break; 
1244      } 
1245    if(res) 
1246      { 
1247       //--- trading order is valid 
1248       //--- check if we need and able to set protective orders 
1249       if(m_request.sl!=0.0 || m_request.tp!=0.0) 
1250         { 
1251          if((flags&SYMBOL_ORDER_SL)==0) 
1252             m_request.sl=0.0; 
1253          if((flags&SYMBOL_ORDER_TP)==0) 
1254             m_request.tp=0.0; 
1255         } 
1256      } 
1257    else 
1258      { 
1259       //--- trading order is not valid 
1260       //--- set error 
1261       m_result.retcode=TRADE_RETCODE_INVALID_ORDER; 
1262       Print(__FUNCTION__+": Invalid order type"); 
1263      } 
1264 //--- result 
1265    return(res); 
1266   } 
1267 //+------------------------------------------------------------------+ 
1268 //| Send order                                                       | 
1269 //+------------------------------------------------------------------+ 
1270 bool CTrade::OrderSend(const MqlTradeRequest &request,MqlTradeResult &result) 
1271   { 
1272    bool res; 
1273    string action=""; 
1274    string fmt   =""; 
1275 //--- action 
1276    if(m_async_mode) 
1277       res=OrderSendAsync(request,result); 
1278    else 
1279       res=::OrderSend(request,result); 
1280 //--- check 
1281    if(res) 
1282      { 
1283       if(m_log_level>LOG_LEVEL_ERRORS) 
1284          printf(__FUNCTION__+": %s [%s]",FormatRequest(action,request),FormatRequestResult(fmt,request,result)); 
1285      } 
1286    else 
1287      { 
1288       if(m_log_level>LOG_LEVEL_NO) 
1289          printf(__FUNCTION__+": %s [%s]",FormatRequest(action,request),FormatRequestResult(fmt,request,result)); 
1290      } 
1291 //--- return the result 
1292    return(res); 
1293   } 
1294 //+------------------------------------------------------------------+ 


