//+------------------------------------------------------------------+ 
2 //|                                                   SymbolInfo.mqh | 
3 //|                   Copyright 2009-2013, MetaQuotes Software Corp. | 
4 //|                                              http://www.mql5.com | 
5 //+------------------------------------------------------------------+ 
6 #include <Object.mqh> 
7 //+------------------------------------------------------------------+ 
8 //| Class CSymbolInfo.                                               | 
9 //| Appointment: Class for access to symbol info.                    | 
10 //|              Derives from class CObject.                         | 
11 //+------------------------------------------------------------------+ 
12 class CSymbolInfo : public CObject 
13   { 
14 protected: 
15    string            m_name;               // symbol name 
16    MqlTick           m_tick;               // structure of tick; 
17    double            m_point;              // symbol point 
18    double            m_tick_value;         // symbol tick value 
19    double            m_tick_value_profit;  // symbol tick value profit 
20    double            m_tick_value_loss;    // symbol tick value loss 
21    double            m_tick_size;          // symbol tick size 
22    double            m_contract_size;      // symbol contract size 
23    double            m_lots_min;           // symbol lots min 
24    double            m_lots_max;           // symbol lots max 
25    double            m_lots_step;          // symbol lots step 
26    double            m_lots_limit;         // symbol lots limit 
27    double            m_swap_long;          // symbol swap long 
28    double            m_swap_short;         // symbol swap short 
29    int               m_digits;             // symbol digits 
30    int               m_order_mode;         // symbol valid orders 
31    ENUM_SYMBOL_TRADE_EXECUTION m_trade_execution;    // symbol trade execution 
32    ENUM_SYMBOL_CALC_MODE m_trade_calcmode;     // symbol trade calcmode 
33    ENUM_SYMBOL_TRADE_MODE m_trade_mode;         // symbol trade mode 
34    ENUM_SYMBOL_SWAP_MODE m_swap_mode;          // symbol swap mode 
35    ENUM_DAY_OF_WEEK  m_swap3;              // symbol swap3 
36    double            m_margin_initial;     // symbol margin initial 
37    double            m_margin_maintenance; // symbol margin maintenance 
38    double            m_margin_long;        // symbol margin long position 
39    double            m_margin_short;       // symbol margin short position 
40    double            m_margin_limit;       // symbol margin limit order 
41    double            m_margin_stop;        // symbol margin stop order 
42    double            m_margin_stoplimit;   // symbol margin stoplimit order 
43    int               m_trade_time_flags;   // symbol trade time flags 
44    int               m_trade_fill_flags;   // symbol trade fill flags 
45 
 
46 public: 
47                      CSymbolInfo(void); 
48                     ~CSymbolInfo(void); 
49    //--- methods of access to protected data 
50    string            Name(void) const { return(m_name); } 
51    bool              Name(const string name); 
52    bool              Refresh(void); 
53    bool              RefreshRates(void); 
54    //--- fast access methods to the integer symbol propertyes 
55    bool              Select(void) const; 
56    bool              Select(const bool select); 
57    bool              IsSynchronized(void) const; 
58    //--- volumes 
59    ulong             Volume(void) const { return(m_tick.volume); } 
60    ulong             VolumeHigh(void) const; 
61    ulong             VolumeLow(void) const; 
62    //--- miscellaneous 
63    datetime          Time(void) const { return(m_tick.time); } 
64    int               Spread(void) const; 
65    bool              SpreadFloat(void) const; 
66    int               TicksBookDepth(void) const; 
67    //--- trade levels 
68    int               StopsLevel(void) const; 
69    int               FreezeLevel(void) const; 
70    //--- fast access methods to the double symbol propertyes 
71    //--- bid parameters 
72    double            Bid(void) const { return(m_tick.bid); } 
73    double            BidHigh(void) const; 
74    double            BidLow(void) const; 
75    //--- ask parameters 
76    double            Ask(void) const { return(m_tick.ask); } 
77    double            AskHigh(void) const; 
78    double            AskLow(void) const; 
79    //--- last parameters 
80    double            Last(void) const { return(m_tick.last); } 
81    double            LastHigh(void) const; 
82    double            LastLow(void) const; 
83    //--- fast access methods to the mix symbol propertyes 
84    int               OrderMode(void) const { return(m_order_mode); } 
85    //--- terms of trade 
86    ENUM_SYMBOL_CALC_MODE TradeCalcMode(void) const { return(m_trade_calcmode); } 
87    string            TradeCalcModeDescription(void) const; 
88    ENUM_SYMBOL_TRADE_MODE TradeMode(void) const { return(m_trade_mode); } 
89    string            TradeModeDescription(void) const; 
90    //--- execution terms of trade 
91    ENUM_SYMBOL_TRADE_EXECUTION TradeExecution(void) const { return(m_trade_execution); } 
92    string            TradeExecutionDescription(void) const; 
93    //--- swap terms of trade 
94    ENUM_SYMBOL_SWAP_MODE SwapMode(void) const { return(m_swap_mode); } 
95    string            SwapModeDescription(void) const; 
96    ENUM_DAY_OF_WEEK  SwapRollover3days(void) const { return(m_swap3); } 
97    string            SwapRollover3daysDescription(void) const; 
98    //--- dates for futures 
99    datetime          StartTime(void) const; 
100    datetime          ExpirationTime(void) const; 
101    //--- margin parameters 
102    double            MarginInitial(void)                const { return(m_margin_initial);     } 
103    double            MarginMaintenance(void)            const { return(m_margin_maintenance); } 
104    double            MarginLong(void)                   const { return(m_margin_long);        } 
105    double            MarginShort(void)                  const { return(m_margin_short);       } 
106    double            MarginLimit(void)                  const { return(m_margin_limit);       } 
107    double            MarginStop(void)                   const { return(m_margin_stop);        } 
108    double            MarginStopLimit(void)              const { return(m_margin_stoplimit);   } 
109    //--- trade flags parameters 
110    int               TradeTimeFlags(void)               const { return(m_trade_time_flags);   } 
111    int               TradeFillFlags(void)               const { return(m_trade_fill_flags);   } 
112    //--- tick parameters 
113    int               Digits(void)                       const { return(m_digits);             } 
114    double            Point(void)                        const { return(m_point);              } 
115    double            TickValue(void)                    const { return(m_tick_value);         } 
116    double            TickValueProfit(void)              const { return(m_tick_value_profit);  } 
117    double            TickValueLoss(void)                const { return(m_tick_value_loss);    } 
118    double            TickSize(void)                     const { return(m_tick_size);          } 
119    //--- lots parameters 
120    double            ContractSize(void)                 const { return(m_contract_size);      } 
121    double            LotsMin(void)                      const { return(m_lots_min);           } 
122    double            LotsMax(void)                      const { return(m_lots_max);           } 
123    double            LotsStep(void)                     const { return(m_lots_step);          } 
124    double            LotsLimit(void)                    const { return(m_lots_limit);         } 
125    //--- swaps 
126    double            SwapLong(void)                     const { return(m_swap_long);          } 
127    double            SwapShort(void)                    const { return(m_swap_short);         } 
128    //--- fast access methods to the string symbol propertyes 
129    string            CurrencyBase(void) const; 
130    string            CurrencyProfit(void) const; 
131    string            CurrencyMargin(void) const; 
132    string            Bank(void) const; 
133    string            Description(void) const; 
134    string            Path(void) const; 
135    //--- session information 
136    long              SessionDeals(void) const; 
137    long              SessionBuyOrders(void) const; 
138    long              SessionSellOrders(void) const; 
139    double            SessionTurnover(void) const; 
140    double            SessionInterest(void) const; 
141    double            SessionBuyOrdersVolume(void) const; 
142    double            SessionSellOrdersVolume(void) const; 
143    double            SessionOpen(void) const; 
144    double            SessionClose(void) const; 
145    double            SessionAW(void) const; 
146    double            SessionPriceSettlement(void) const; 
147    double            SessionPriceLimitMin(void) const; 
148    double            SessionPriceLimitMax(void) const; 
149    //--- access methods to the API MQL5 functions 
150    bool              InfoInteger(const ENUM_SYMBOL_INFO_INTEGER prop_id,long &var) const; 
151    bool              InfoDouble(const ENUM_SYMBOL_INFO_DOUBLE prop_id,double &var) const; 
152    bool              InfoString(const ENUM_SYMBOL_INFO_STRING prop_id,string &var) const; 
153    //--- service methods 
154    double            NormalizePrice(const double price) const; 
155    bool              CheckMarketWatch(void); 
156   }; 
157 //+------------------------------------------------------------------+ 
158 //| Constructor                                                      | 
159 //+------------------------------------------------------------------+ 
160 CSymbolInfo::CSymbolInfo(void) : m_name(""), 
161                                  m_point(0.0), 
162                                  m_tick_value(0.0), 
163                                  m_tick_value_profit(0.0), 
164                                  m_tick_value_loss(0.0), 
165                                  m_tick_size(0.0), 
166                                  m_contract_size(0.0), 
167                                  m_lots_min(0.0), 
168                                  m_lots_max(0.0), 
169                                  m_lots_step(0.0), 
170                                  m_swap_long(0.0), 
171                                  m_swap_short(0.0), 
172                                  m_digits(0), 
173                                  m_order_mode(0), 
174                                  m_trade_execution(0), 
175                                  m_trade_calcmode(0), 
176                                  m_trade_mode(0), 
177                                  m_swap_mode(0), 
178                                  m_swap3(0), 
179                                  m_margin_initial(0.0), 
180                                  m_margin_maintenance(0.0), 
181                                  m_margin_long(0.0), 
182                                  m_margin_short(0.0), 
183                                  m_margin_limit(0.0), 
184                                  m_margin_stop(0.0), 
185                                  m_margin_stoplimit(0.0), 
186                                  m_trade_time_flags(0), 
187                                  m_trade_fill_flags(0) 
188   { 
189   } 
190 //+------------------------------------------------------------------+ 
191 //| Destructor                                                       | 
192 //+------------------------------------------------------------------+ 
193 CSymbolInfo::~CSymbolInfo(void) 
194   { 
195   } 
196 //+------------------------------------------------------------------+ 
197 //| Set name                                                         | 
198 //+------------------------------------------------------------------+ 
199 bool CSymbolInfo::Name(const string name) 
200   { 
201    m_name=name; 
202 //--- 
203    if(!CheckMarketWatch()) 
204       return(false); 
205 //--- 
206    if(!Refresh()) 
207      { 
208       m_name=""; 
209       Print(__FUNCTION__+": invalid data of symbol '"+name+"'"); 
210       return(false); 
211      } 
212 //--- succeed 
213    return(true); 
214   } 
215 //+------------------------------------------------------------------+ 
216 //| Refresh cached data                                              | 
217 //+------------------------------------------------------------------+ 
218 bool CSymbolInfo::Refresh(void) 
219   { 
220    long tmp=0; 
221 //--- 
222    if(!SymbolInfoDouble(m_name,SYMBOL_POINT,m_point)) 
223       return(false); 
224    if(!SymbolInfoDouble(m_name,SYMBOL_TRADE_TICK_VALUE,m_tick_value)) 
225       return(false); 
226    if(!SymbolInfoDouble(m_name,SYMBOL_TRADE_TICK_VALUE_PROFIT,m_tick_value_profit)) 
227       return(false); 
228    if(!SymbolInfoDouble(m_name,SYMBOL_TRADE_TICK_VALUE_LOSS,m_tick_value_loss)) 
229       return(false); 
230    if(!SymbolInfoDouble(m_name,SYMBOL_TRADE_TICK_SIZE,m_tick_size)) 
231       return(false); 
232    if(!SymbolInfoDouble(m_name,SYMBOL_TRADE_CONTRACT_SIZE,m_contract_size)) 
233       return(false); 
234    if(!SymbolInfoDouble(m_name,SYMBOL_VOLUME_MIN,m_lots_min)) 
235       return(false); 
236    if(!SymbolInfoDouble(m_name,SYMBOL_VOLUME_MAX,m_lots_max)) 
237       return(false); 
238    if(!SymbolInfoDouble(m_name,SYMBOL_VOLUME_STEP,m_lots_step)) 
239       return(false); 
240    if(!SymbolInfoDouble(m_name,SYMBOL_VOLUME_LIMIT,m_lots_limit)) 
241       return(false); 
242    if(!SymbolInfoDouble(m_name,SYMBOL_SWAP_LONG,m_swap_long)) 
243       return(false); 
244    if(!SymbolInfoDouble(m_name,SYMBOL_SWAP_SHORT,m_swap_short)) 
245       return(false); 
246    if(!SymbolInfoInteger(m_name,SYMBOL_DIGITS,tmp)) 
247       return(false); 
248    m_digits=(int)tmp; 
249    if(!SymbolInfoInteger(m_name,SYMBOL_ORDER_MODE,tmp)) 
250       return(false); 
251    m_order_mode=(int)tmp; 
252    if(!SymbolInfoInteger(m_name,SYMBOL_TRADE_EXEMODE,tmp)) 
253       return(false); 
254    m_trade_execution=(ENUM_SYMBOL_TRADE_EXECUTION)tmp; 
255    if(!SymbolInfoInteger(m_name,SYMBOL_TRADE_CALC_MODE,tmp)) 
256       return(false); 
257    m_trade_calcmode=(ENUM_SYMBOL_CALC_MODE)tmp; 
258    if(!SymbolInfoInteger(m_name,SYMBOL_TRADE_MODE,tmp)) 
259       return(false); 
260    m_trade_mode=(ENUM_SYMBOL_TRADE_MODE)tmp; 
261    if(!SymbolInfoInteger(m_name,SYMBOL_SWAP_MODE,tmp)) 
262       return(false); 
263    m_swap_mode=(ENUM_SYMBOL_SWAP_MODE)tmp; 
264    if(!SymbolInfoInteger(m_name,SYMBOL_SWAP_ROLLOVER3DAYS,tmp)) 
265       return(false); 
266    m_swap3=(ENUM_DAY_OF_WEEK)tmp; 
267    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_INITIAL,m_margin_initial)) 
268       return(false); 
269    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_MAINTENANCE,m_margin_maintenance)) 
270       return(false); 
271    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_LONG,m_margin_long)) 
272       return(false); 
273    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_SHORT,m_margin_short)) 
274       return(false); 
275    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_LIMIT,m_margin_limit)) 
276       return(false); 
277    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_STOP,m_margin_stop)) 
278       return(false); 
279    if(!SymbolInfoDouble(m_name,SYMBOL_MARGIN_STOPLIMIT,m_margin_stoplimit)) 
280       return(false); 
281    if(!SymbolInfoInteger(m_name,SYMBOL_EXPIRATION_MODE,tmp)) 
282       return(false); 
283    m_trade_time_flags=(int)tmp; 
284    if(!SymbolInfoInteger(m_name,SYMBOL_FILLING_MODE,tmp)) 
285       return(false); 
286    m_trade_fill_flags=(int)tmp; 
287 //--- succeed 
288    return(true); 
289   } 
290 //+------------------------------------------------------------------+ 
291 //| Refresh cached data                                              | 
292 //+------------------------------------------------------------------+ 
293 bool CSymbolInfo::RefreshRates(void) 
294   { 
295    return(SymbolInfoTick(m_name,m_tick)); 
296   } 
297 //+------------------------------------------------------------------+ 
298 //| Get the property value "SYMBOL_SELECT"                           | 
299 //+------------------------------------------------------------------+ 
300 bool CSymbolInfo::Select(void) const 
301   { 
302    return((bool)SymbolInfoInteger(m_name,SYMBOL_SELECT)); 
303   } 
304 //+------------------------------------------------------------------+ 
305 //| Set the property value "SYMBOL_SELECT"                           | 
306 //+------------------------------------------------------------------+ 
307 bool CSymbolInfo::Select(const bool select) 
308   { 
309    return(SymbolSelect(m_name,select)); 
310   } 
311 //+------------------------------------------------------------------+ 
312 //| Check synchronize symbol                                         | 
313 //+------------------------------------------------------------------+ 
314 bool CSymbolInfo::IsSynchronized(void) const 
315   { 
316    return(SymbolIsSynchronized(m_name)); 
317   } 
318 //+------------------------------------------------------------------+ 
319 //| Get the property value "SYMBOL_VOLUMEHIGH"                       | 
320 //+------------------------------------------------------------------+ 
321 ulong CSymbolInfo::VolumeHigh(void) const 
322   { 
323    return(SymbolInfoInteger(m_name,SYMBOL_VOLUMEHIGH)); 
324   } 
325 //+------------------------------------------------------------------+ 
326 //| Get the property value "SYMBOL_VOLUMELOW"                        | 
327 //+------------------------------------------------------------------+ 
328 ulong CSymbolInfo::VolumeLow(void) const 
329   { 
330    return(SymbolInfoInteger(m_name,SYMBOL_VOLUMELOW)); 
331   } 
332 //+------------------------------------------------------------------+ 
333 //| Get the property value "SYMBOL_SPREAD"                           | 
334 //+------------------------------------------------------------------+ 
335 int CSymbolInfo::Spread(void) const 
336   { 
337    return((int)SymbolInfoInteger(m_name,SYMBOL_SPREAD)); 
338   } 
339 //+------------------------------------------------------------------+ 
340 //| Get the property value "SYMBOL_SPREAD_FLOAT"                     | 
341 //+------------------------------------------------------------------+ 
342 bool CSymbolInfo::SpreadFloat(void) const 
343   { 
344    return((bool)SymbolInfoInteger(m_name,SYMBOL_SPREAD_FLOAT)); 
345   } 
346 //+------------------------------------------------------------------+ 
347 //| Get the property value "SYMBOL_TICKS_BOOKDEPTH"                  | 
348 //+------------------------------------------------------------------+ 
349 int CSymbolInfo::TicksBookDepth(void) const 
350   { 
351    return((int)SymbolInfoInteger(m_name,SYMBOL_TICKS_BOOKDEPTH)); 
352   } 
353 //+------------------------------------------------------------------+ 
354 //| Get the property value "SYMBOL_TRADE_STOPS_LEVEL"                | 
355 //+------------------------------------------------------------------+ 
356 int CSymbolInfo::StopsLevel(void) const 
357   { 
358    return((int)SymbolInfoInteger(m_name,SYMBOL_TRADE_STOPS_LEVEL)); 
359   } 
360 //+------------------------------------------------------------------+ 
361 //| Get the property value "SYMBOL_TRADE_FREEZE_LEVEL"               | 
362 //+------------------------------------------------------------------+ 
363 int CSymbolInfo::FreezeLevel(void) const 
364   { 
365    return((int)SymbolInfoInteger(m_name,SYMBOL_TRADE_FREEZE_LEVEL)); 
366   } 
367 //+------------------------------------------------------------------+ 
368 //| Get the property value "SYMBOL_BIDHIGH"                          | 
369 //+------------------------------------------------------------------+ 
370 double CSymbolInfo::BidHigh(void) const 
371   { 
372    return(SymbolInfoDouble(m_name,SYMBOL_BIDHIGH)); 
373   } 
374 //+------------------------------------------------------------------+ 
375 //| Get the property value "SYMBOL_BIDLOW"                           | 
376 //+------------------------------------------------------------------+ 
377 double CSymbolInfo::BidLow(void) const 
378   { 
379    return(SymbolInfoDouble(m_name,SYMBOL_BIDLOW)); 
380   } 
381 //+------------------------------------------------------------------+ 
382 //| Get the property value "SYMBOL_ASKHIGH"                          | 
383 //+------------------------------------------------------------------+ 
384 double CSymbolInfo::AskHigh(void) const 
385   { 
386    return(SymbolInfoDouble(m_name,SYMBOL_ASKHIGH)); 
387   } 
388 //+------------------------------------------------------------------+ 
389 //| Get the property value "SYMBOL_ASKLOW"                           | 
390 //+------------------------------------------------------------------+ 
391 double CSymbolInfo::AskLow(void) const 
392   { 
393    return(SymbolInfoDouble(m_name,SYMBOL_ASKLOW)); 
394   } 
395 //+------------------------------------------------------------------+ 
396 //| Get the property value "SYMBOL_LASTHIGH"                         | 
397 //+------------------------------------------------------------------+ 
398 double CSymbolInfo::LastHigh(void) const 
399   { 
400    return(SymbolInfoDouble(m_name,SYMBOL_LASTHIGH)); 
401   } 
402 //+------------------------------------------------------------------+ 
403 //| Get the property value "SYMBOL_LASTLOW"                          | 
404 //+------------------------------------------------------------------+ 
405 double CSymbolInfo::LastLow(void) const 
406   { 
407    return(SymbolInfoDouble(m_name,SYMBOL_LASTLOW)); 
408   } 
409 //+------------------------------------------------------------------+ 
410 //| Get the property value "SYMBOL_TRADE_CALC_MODE" as string        | 
411 //+------------------------------------------------------------------+ 
412 string CSymbolInfo::TradeCalcModeDescription(void) const 
413   { 
414    string str; 
415 //--- 
416    switch(m_trade_calcmode) 
417      { 
418       case SYMBOL_CALC_MODE_FOREX: 
419          str="Calculation of profit and margin for Forex"; 
420          break; 
421       case SYMBOL_CALC_MODE_CFD: 
422          str="Calculation of collateral and earnings for CFD"; 
423          break; 
424       case SYMBOL_CALC_MODE_FUTURES: 
425          str="Calculation of collateral and profits for futures"; 
426          break; 
427       case SYMBOL_CALC_MODE_CFDINDEX: 
428          str="Calculation of collateral and earnings for CFD on indices"; 
429          break; 
430       case SYMBOL_CALC_MODE_CFDLEVERAGE: 
431          str="Calculation of collateral and earnings for the CFD when trading with leverage"; 
432          break; 
433       case SYMBOL_CALC_MODE_EXCH_STOCKS: 
434          str="Calculation for exchange stocks"; 
435          break; 
436       case SYMBOL_CALC_MODE_EXCH_FUTURES: 
437          str="Calculation for exchange futures"; 
438          break; 
439       case SYMBOL_CALC_MODE_EXCH_FUTURES_FORTS: 
440          str="Calculation for FORTS futures"; 
441          break; 
442       default: 
443          str="Unknown calculation mode"; 
444      } 
445 //--- result 
446    return(str); 
447   } 
448 //+------------------------------------------------------------------+ 
449 //| Get the property value "SYMBOL_TRADE_MODE" as string             | 
450 //+------------------------------------------------------------------+ 
451 string CSymbolInfo::TradeModeDescription(void) const 
452   { 
453    string str; 
454 //--- 
455    switch(m_trade_mode) 
456      { 
457       case SYMBOL_TRADE_MODE_DISABLED : str="Disabled";           break; 
458       case SYMBOL_TRADE_MODE_LONGONLY : str="Long only";          break; 
459       case SYMBOL_TRADE_MODE_SHORTONLY: str="Short only";         break; 
460       case SYMBOL_TRADE_MODE_CLOSEONLY: str="Close only";         break; 
461       case SYMBOL_TRADE_MODE_FULL     : str="Full access";        break; 
462       default                         : str="Unknown trade mode"; 
463      } 
464 //--- result 
465    return(str); 
466   } 
467 //+------------------------------------------------------------------+ 
468 //| Get the property value "SYMBOL_TRADE_EXEMODE" as string          | 
469 //+------------------------------------------------------------------+ 
470 string CSymbolInfo::TradeExecutionDescription(void) const 
471   { 
472    string str; 
473 //--- 
474    switch(m_trade_execution) 
475      { 
476       case SYMBOL_TRADE_EXECUTION_REQUEST : str="Trading on request";                break; 
477       case SYMBOL_TRADE_EXECUTION_INSTANT : str="Trading on live streaming prices";  break; 
478       case SYMBOL_TRADE_EXECUTION_MARKET  : str="Execution of orders on the market"; break; 
479       case SYMBOL_TRADE_EXECUTION_EXCHANGE: str="Exchange execution";                break; 
480       default:                              str="Unknown trade execution"; 
481      } 
482 //--- result 
483    return(str); 
484   } 
485 //+------------------------------------------------------------------+ 
486 //| Get the property value "SYMBOL_SWAP_MODE" as string              | 
487 //+------------------------------------------------------------------+ 
488 string CSymbolInfo::SwapModeDescription(void) const 
489   { 
490    string str; 
491 //--- 
492    switch(m_swap_mode) 
493      { 
494       case SYMBOL_SWAP_MODE_DISABLED: 
495          str="No swaps"; 
496          break; 
497       case SYMBOL_SWAP_MODE_POINTS: 
498          str="Swaps are calculated in points"; 
499          break; 
500       case SYMBOL_SWAP_MODE_CURRENCY_SYMBOL: 
501          str="Swaps are calculated in base currency"; 
502          break; 
503       case SYMBOL_SWAP_MODE_CURRENCY_MARGIN: 
504          str="Swaps are calculated in margin currency"; 
505          break; 
506       case SYMBOL_SWAP_MODE_CURRENCY_DEPOSIT: 
507          str="Swaps are calculated in deposit currency"; 
508          break; 
509       case SYMBOL_SWAP_MODE_INTEREST_CURRENT: 
510          str="Swaps are calculated as annual interest using the current price"; 
511          break; 
512       case SYMBOL_SWAP_MODE_INTEREST_OPEN: 
513          str="Swaps are calculated as annual interest using the open price"; 
514          break; 
515       case SYMBOL_SWAP_MODE_REOPEN_CURRENT: 
516          str="Swaps are charged by reopening positions at the close price"; 
517          break; 
518       case SYMBOL_SWAP_MODE_REOPEN_BID: 
519          str="Swaps are charged by reopening positions at the Bid price"; 
520          break; 
521       default: 
522          str="Unknown swap mode"; 
523      } 
524 //--- result 
525    return(str); 
526   } 
527 //+------------------------------------------------------------------+ 
528 //| Get the property value "SYMBOL_SWAP_ROLLOVER3DAYS" as string     | 
529 //+------------------------------------------------------------------+ 
530 string CSymbolInfo::SwapRollover3daysDescription(void) const 
531   { 
532    string str; 
533 //--- 
534    switch(m_swap3) 
535      { 
536       case SUNDAY   : str="Sunday";    break; 
537       case MONDAY   : str="Monday";    break; 
538       case TUESDAY  : str="Tuesday";   break; 
539       case WEDNESDAY: str="Wednesday"; break; 
540       case THURSDAY : str="Thursday";  break; 
541       case FRIDAY   : str="Friday";    break; 
542       case SATURDAY : str="Saturday";  break; 
543       default       : str="Unknown"; 
544      } 
545 //--- result 
546    return(str); 
547   } 
548 //+------------------------------------------------------------------+ 
549 //| Get the property value "SYMBOL_START_TIME"                       | 
550 //+------------------------------------------------------------------+ 
551 datetime CSymbolInfo::StartTime(void) const 
552   { 
553    return((datetime)SymbolInfoInteger(m_name,SYMBOL_START_TIME)); 
554   } 
555 //+------------------------------------------------------------------+ 
556 //| Get the property value "SYMBOL_EXPIRATION_TIME"                  | 
557 //+------------------------------------------------------------------+ 
558 datetime CSymbolInfo::ExpirationTime(void) const 
559   { 
560    return((datetime)SymbolInfoInteger(m_name,SYMBOL_EXPIRATION_TIME)); 
561   } 
562 //+------------------------------------------------------------------+ 
563 //| Get the property value "SYMBOL_CURRENCY_BASE"                    | 
564 //+------------------------------------------------------------------+ 
565 string CSymbolInfo::CurrencyBase(void) const 
566   { 
567    return(SymbolInfoString(m_name,SYMBOL_CURRENCY_BASE)); 
568   } 
569 //+------------------------------------------------------------------+ 
570 //| Get the property value "SYMBOL_CURRENCY_PROFIT"                  | 
571 //+------------------------------------------------------------------+ 
572 string CSymbolInfo::CurrencyProfit(void) const 
573   { 
574    return(SymbolInfoString(m_name,SYMBOL_CURRENCY_PROFIT)); 
575   } 
576 //+------------------------------------------------------------------+ 
577 //| Get the property value "SYMBOL_CURRENCY_MARGIN"                  | 
578 //+------------------------------------------------------------------+ 
579 string CSymbolInfo::CurrencyMargin(void) const 
580   { 
581    return(SymbolInfoString(m_name,SYMBOL_CURRENCY_MARGIN)); 
582   } 
583 //+------------------------------------------------------------------+ 
584 //| Get the property value "SYMBOL_BANK"                             | 
585 //+------------------------------------------------------------------+ 
586 string CSymbolInfo::Bank(void) const 
587   { 
588    return(SymbolInfoString(m_name,SYMBOL_BANK)); 
589   } 
590 //+------------------------------------------------------------------+ 
591 //| Get the property value "SYMBOL_DESCRIPTION"                      | 
592 //+------------------------------------------------------------------+ 
593 string CSymbolInfo::Description(void) const 
594   { 
595    return(SymbolInfoString(m_name,SYMBOL_DESCRIPTION)); 
596   } 
597 //+------------------------------------------------------------------+ 
598 //| Get the property value "SYMBOL_PATH"                             | 
599 //+------------------------------------------------------------------+ 
600 string CSymbolInfo::Path(void) const 
601   { 
602    return(SymbolInfoString(m_name,SYMBOL_PATH)); 
603   } 
604 //+------------------------------------------------------------------+ 
605 //| Get the property value "SYMBOL_SESSION_DEALS"                    | 
606 //+------------------------------------------------------------------+ 
607 long CSymbolInfo::SessionDeals(void) const 
608   { 
609    return(SymbolInfoInteger(m_name,SYMBOL_SESSION_DEALS)); 
610   } 
611 //+------------------------------------------------------------------+ 
612 //| Get the property value "SYMBOL_SESSION_BUY_ORDERS"               | 
613 //+------------------------------------------------------------------+ 
614 long CSymbolInfo::SessionBuyOrders(void) const 
615   { 
616    return(SymbolInfoInteger(m_name,SYMBOL_SESSION_BUY_ORDERS)); 
617   } 
618 //+------------------------------------------------------------------+ 
619 //| Get the property value "SYMBOL_SESSION_SELL_ORDERS"              | 
620 //+------------------------------------------------------------------+ 
621 long CSymbolInfo::SessionSellOrders(void) const 
622   { 
623    return(SymbolInfoInteger(m_name,SYMBOL_SESSION_SELL_ORDERS)); 
624   } 
625 //+------------------------------------------------------------------+ 
626 //| Get the property value "SYMBOL_SESSION_TURNOVER"                 | 
627 //+------------------------------------------------------------------+ 
628 double CSymbolInfo::SessionTurnover(void) const 
629   { 
630    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_TURNOVER)); 
631   } 
632 //+------------------------------------------------------------------+ 
633 //| Get the property value "SYMBOL_SESSION_INTEREST"                 | 
634 //+------------------------------------------------------------------+ 
635 double CSymbolInfo::SessionInterest(void) const 
636   { 
637    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_INTEREST)); 
638   } 
639 //+------------------------------------------------------------------+ 
640 //| Get the property value "SYMBOL_SESSION_BUY_ORDERS_VOLUME"        | 
641 //+------------------------------------------------------------------+ 
642 double CSymbolInfo::SessionBuyOrdersVolume(void) const 
643   { 
644    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_BUY_ORDERS_VOLUME)); 
645   } 
646 //+------------------------------------------------------------------+ 
647 //| Get the property value "SYMBOL_SESSION_SELL_ORDERS_VOLUME"       | 
648 //+------------------------------------------------------------------+ 
649 double CSymbolInfo::SessionSellOrdersVolume(void) const 
650   { 
651    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_SELL_ORDERS_VOLUME)); 
652   } 
653 //+------------------------------------------------------------------+ 
654 //| Get the property value "SYMBOL_SESSION_OPEN"                     | 
655 //+------------------------------------------------------------------+ 
656 double CSymbolInfo::SessionOpen(void) const 
657   { 
658    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_OPEN)); 
659   } 
660 //+------------------------------------------------------------------+ 
661 //| Get the property value "SYMBOL_SESSION_CLOSE"                    | 
662 //+------------------------------------------------------------------+ 
663 double CSymbolInfo::SessionClose(void) const 
664   { 
665    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_CLOSE)); 
666   } 
667 //+------------------------------------------------------------------+ 
668 //| Get the property value "SYMBOL_SESSION_AW"                       | 
669 //+------------------------------------------------------------------+ 
670 double CSymbolInfo::SessionAW(void) const 
671   { 
672    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_AW)); 
673   } 
674 //+------------------------------------------------------------------+ 
675 //| Get the property value "SYMBOL_SESSION_PRICE_SETTLEMENT"         | 
676 //+------------------------------------------------------------------+ 
677 double CSymbolInfo::SessionPriceSettlement(void) const 
678   { 
679    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_PRICE_SETTLEMENT)); 
680   } 
681 //+------------------------------------------------------------------+ 
682 //| Get the property value "SYMBOL_SESSION_PRICE_LIMIT_MIN"          | 
683 //+------------------------------------------------------------------+ 
684 double CSymbolInfo::SessionPriceLimitMin(void) const 
685   { 
686    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_PRICE_LIMIT_MIN)); 
687   } 
688 //+------------------------------------------------------------------+ 
689 //| Get the property value "SYMBOL_SESSION_PRICE_LIMIT_MAX"          | 
690 //+------------------------------------------------------------------+ 
691 double CSymbolInfo::SessionPriceLimitMax(void) const 
692   { 
693    return(SymbolInfoDouble(m_name,SYMBOL_SESSION_PRICE_LIMIT_MAX)); 
694   } 
695 //+------------------------------------------------------------------+ 
696 //| Access functions SymbolInfoInteger(...)                          | 
697 //+------------------------------------------------------------------+ 
698 bool CSymbolInfo::InfoInteger(const ENUM_SYMBOL_INFO_INTEGER prop_id,long &var) const 
699   { 
700    return(SymbolInfoInteger(m_name,prop_id,var)); 
701   } 
702 //+------------------------------------------------------------------+ 
703 //| Access functions SymbolInfoDouble(...)                           | 
704 //+------------------------------------------------------------------+ 
705 bool CSymbolInfo::InfoDouble(const ENUM_SYMBOL_INFO_DOUBLE prop_id,double &var) const 
706   { 
707    return(SymbolInfoDouble(m_name,prop_id,var)); 
708   } 
709 //+------------------------------------------------------------------+ 
710 //| Access functions SymbolInfoString(...)                           | 
711 //+------------------------------------------------------------------+ 
712 bool CSymbolInfo::InfoString(const ENUM_SYMBOL_INFO_STRING prop_id,string &var) const 
713   { 
714    return(SymbolInfoString(m_name,prop_id,var)); 
715   } 
716 //+------------------------------------------------------------------+ 
717 //| Normalize price                                                  | 
718 //+------------------------------------------------------------------+ 
719 double CSymbolInfo::NormalizePrice(const double price) const 
720   { 
721    if(m_tick_size!=0) 
722       return(NormalizeDouble(MathRound(price/m_tick_size)*m_tick_size,m_digits)); 
723 //--- 
724    return(NormalizeDouble(price,m_digits)); 
725   } 
726 //+------------------------------------------------------------------+ 
727 //| Checks if symbol is selected in the MarketWatch                  | 
728 //| and adds symbol to the MarketWatch, if necessary                 | 
729 //+------------------------------------------------------------------+ 
730 bool CSymbolInfo::CheckMarketWatch(void) 
731   { 
732 //--- check if symbol is selected in the MarketWatch 
733    if(!Select()) 
734      { 
735       if(GetLastError()==ERR_MARKET_UNKNOWN_SYMBOL) 
736         { 
737          printf(__FUNCTION__+": Unknown symbol '%s'",m_name); 
738          return(false); 
739         } 
740       if(!Select(true)) 
741         { 
742          printf(__FUNCTION__+": Error adding symbol %d",GetLastError()); 
743          return(false); 
744         } 
745      } 
746 //--- succeed 
747    return(true); 
748   } 
749 //+------------------------------------------------------------------+ 
