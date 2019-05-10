//+------------------------------------------------------------------+ 
2 //|                                                 PositionInfo.mqh | 
3 //|                   Copyright 2009-2013, MetaQuotes Software Corp. | 
4 //|                                              http://www.mql5.com | 
5 //+------------------------------------------------------------------+ 
6 #include <Object.mqh> 
7 #include "SymbolInfo.mqh" 
8 //+------------------------------------------------------------------+ 
9 //| Class CPositionInfo.                                             | 
10 //| Appointment: Class for access to position info.                  | 
11 //|              Derives from class CObject.                         | 
12 //+------------------------------------------------------------------+ 
13 class CPositionInfo : public CObject 
14   { 
15 protected: 
16    ENUM_POSITION_TYPE m_type; 
17    double            m_volume; 
18    double            m_price; 
19    double            m_stop_loss; 
20    double            m_take_profit; 
21 
 
22 public: 
23                      CPositionInfo(void); 
24                     ~CPositionInfo(void); 
25    //--- fast access methods to the integer position propertyes 
26    datetime          Time(void) const; 
27    ulong             TimeMsc(void) const; 
28    datetime          TimeUpdate(void) const; 
29    ulong             TimeUpdateMsc(void) const; 
30    ENUM_POSITION_TYPE PositionType(void) const; 
31    string            TypeDescription(void) const; 
32    long              Magic(void) const; 
33    long              Identifier(void) const; 
34    //--- fast access methods to the double position propertyes 
35    double            Volume(void) const; 
36    double            PriceOpen(void) const; 
37    double            StopLoss(void) const; 
38    double            TakeProfit(void) const; 
39    double            PriceCurrent(void) const; 
40    double            Commission(void) const; 
41    double            Swap(void) const; 
42    double            Profit(void) const; 
43    //--- fast access methods to the string position propertyes 
44    string            Symbol(void) const; 
45    string            Comment(void) const; 
46    //--- access methods to the API MQL5 functions 
47    bool              InfoInteger(const ENUM_POSITION_PROPERTY_INTEGER prop_id,long &var) const; 
48    bool              InfoDouble(const ENUM_POSITION_PROPERTY_DOUBLE prop_id,double &var) const; 
49    bool              InfoString(const ENUM_POSITION_PROPERTY_STRING prop_id,string &var) const; 
50    //--- info methods 
51    string            FormatType(string &str,const uint type) const; 
52    string            FormatPosition(string &str) const; 
53    //--- methods for select position 
54    bool              Select(const string symbol); 
55    bool              SelectByIndex(const int index); 
56    //--- 
57    void              StoreState(void); 
58    bool              CheckState(void); 
59   }; 
60 //+------------------------------------------------------------------+ 
61 //| Constructor                                                      | 
62 //+------------------------------------------------------------------+ 
63 CPositionInfo::CPositionInfo(void) : m_type(WRONG_VALUE), 
64                                      m_volume(0.0), 
65                                      m_price(0.0), 
66                                      m_stop_loss(0.0), 
67                                      m_take_profit(0.0) 
68   { 
69   } 
70 //+------------------------------------------------------------------+ 
71 //| Destructor                                                       | 
72 //+------------------------------------------------------------------+ 
73 CPositionInfo::~CPositionInfo(void) 
74   { 
75   } 
76 //+------------------------------------------------------------------+ 
77 //| Get the property value "POSITION_TIME"                           | 
78 //+------------------------------------------------------------------+ 
79 datetime CPositionInfo::Time(void) const 
80   { 
81    return((datetime)PositionGetInteger(POSITION_TIME)); 
82   } 
83 //+------------------------------------------------------------------+ 
84 //| Get the property value "POSITION_TIME_MSC"                       | 
85 //+------------------------------------------------------------------+ 
86 ulong CPositionInfo::TimeMsc(void) const 
87   { 
88    return((datetime)PositionGetInteger(POSITION_TIME_MSC)); 
89   } 
90 //+------------------------------------------------------------------+ 
91 //| Get the property value "POSITION_TIME_UPDATE"                    | 
92 //+------------------------------------------------------------------+ 
93 datetime CPositionInfo::TimeUpdate(void) const 
94   { 
95    return((datetime)PositionGetInteger(POSITION_TIME_UPDATE)); 
96   } 
97 //+------------------------------------------------------------------+ 
98 //| Get the property value "POSITION_TIME_UPDATE_MSC"                | 
99 //+------------------------------------------------------------------+ 
100 ulong CPositionInfo::TimeUpdateMsc(void) const 
101   { 
102    return((datetime)PositionGetInteger(POSITION_TIME_UPDATE_MSC)); 
103   } 
104 //+------------------------------------------------------------------+ 
105 //| Get the property value "POSITION_TYPE"                           | 
106 //+------------------------------------------------------------------+ 
107 ENUM_POSITION_TYPE CPositionInfo::PositionType(void) const 
108   { 
109    return((ENUM_POSITION_TYPE)PositionGetInteger(POSITION_TYPE)); 
110   } 
111 //+------------------------------------------------------------------+ 
112 //| Get the property value "POSITION_TYPE" as string                 | 
113 //+------------------------------------------------------------------+ 
114 string CPositionInfo::TypeDescription(void) const 
115   { 
116    string str; 
117 //--- 
118    return(FormatType(str,PositionType())); 
119   } 
120 //+------------------------------------------------------------------+ 
121 //| Get the property value "POSITION_MAGIC"                          | 
122 //+------------------------------------------------------------------+ 
123 long CPositionInfo::Magic(void) const 
124   { 
125    return(PositionGetInteger(POSITION_MAGIC)); 
126   } 
127 //+------------------------------------------------------------------+ 
128 //| Get the property value "POSITION_IDENTIFIER"                     | 
129 //+------------------------------------------------------------------+ 
130 long CPositionInfo::Identifier(void) const 
131   { 
132    return(PositionGetInteger(POSITION_IDENTIFIER)); 
133   } 
134 //+------------------------------------------------------------------+ 
135 //| Get the property value "POSITION_VOLUME"                         | 
136 //+------------------------------------------------------------------+ 
137 double CPositionInfo::Volume(void) const 
138   { 
139    return(PositionGetDouble(POSITION_VOLUME)); 
140   } 
141 //+------------------------------------------------------------------+ 
142 //| Get the property value "POSITION_PRICE_OPEN"                     | 
143 //+------------------------------------------------------------------+ 
144 double CPositionInfo::PriceOpen(void) const 
145   { 
146    return(PositionGetDouble(POSITION_PRICE_OPEN)); 
147   } 
148 //+------------------------------------------------------------------+ 
149 //| Get the property value "POSITION_SL"                             | 
150 //+------------------------------------------------------------------+ 
151 double CPositionInfo::StopLoss(void) const 
152   { 
153    return(PositionGetDouble(POSITION_SL)); 
154   } 
155 //+------------------------------------------------------------------+ 
156 //| Get the property value "POSITION_TP"                             | 
157 //+------------------------------------------------------------------+ 
158 double CPositionInfo::TakeProfit(void) const 
159   { 
160    return(PositionGetDouble(POSITION_TP)); 
161   } 
162 //+------------------------------------------------------------------+ 
163 //| Get the property value "POSITION_PRICE_CURRENT"                  | 
164 //+------------------------------------------------------------------+ 
165 double CPositionInfo::PriceCurrent(void) const 
166   { 
167    return(PositionGetDouble(POSITION_PRICE_CURRENT)); 
168   } 
169 //+------------------------------------------------------------------+ 
170 //| Get the property value "POSITION_COMMISSION"                     | 
171 //+------------------------------------------------------------------+ 
172 double CPositionInfo::Commission(void) const 
173   { 
174    return(PositionGetDouble(POSITION_COMMISSION)); 
175   } 
176 //+------------------------------------------------------------------+ 
177 //| Get the property value "POSITION_SWAP"                           | 
178 //+------------------------------------------------------------------+ 
179 double CPositionInfo::Swap(void) const 
180   { 
181    return(PositionGetDouble(POSITION_SWAP)); 
182   } 
183 //+------------------------------------------------------------------+ 
184 //| Get the property value "POSITION_PROFIT"                         | 
185 //+------------------------------------------------------------------+ 
186 double CPositionInfo::Profit(void) const 
187   { 
188    return(PositionGetDouble(POSITION_PROFIT)); 
189   } 
190 //+------------------------------------------------------------------+ 
191 //| Get the property value "POSITION_SYMBOL"                         | 
192 //+------------------------------------------------------------------+ 
193 string CPositionInfo::Symbol(void) const 
194   { 
195    return(PositionGetString(POSITION_SYMBOL)); 
196   } 
197 //+------------------------------------------------------------------+ 
198 //| Get the property value "POSITION_COMMENT"                        | 
199 //+------------------------------------------------------------------+ 
200 string CPositionInfo::Comment(void) const 
201   { 
202    return(PositionGetString(POSITION_COMMENT)); 
203   } 
204 //+------------------------------------------------------------------+ 
205 //| Access functions PositionGetInteger(...)                         | 
206 //+------------------------------------------------------------------+ 
207 bool CPositionInfo::InfoInteger(const ENUM_POSITION_PROPERTY_INTEGER prop_id,long &var) const 
208   { 
209    return(PositionGetInteger(prop_id,var)); 
210   } 
211 //+------------------------------------------------------------------+ 
212 //| Access functions PositionGetDouble(...)                          | 
213 //+------------------------------------------------------------------+ 
214 bool CPositionInfo::InfoDouble(const ENUM_POSITION_PROPERTY_DOUBLE prop_id,double &var) const 
215   { 
216    return(PositionGetDouble(prop_id,var)); 
217   } 
218 //+------------------------------------------------------------------+ 
219 //| Access functions PositionGetString(...)                          | 
220 //+------------------------------------------------------------------+ 
221 bool CPositionInfo::InfoString(const ENUM_POSITION_PROPERTY_STRING prop_id,string &var) const 
222   { 
223    return(PositionGetString(prop_id,var)); 
224   } 
225 //+------------------------------------------------------------------+ 
226 //| Converts the position type to text                               | 
227 //+------------------------------------------------------------------+ 
228 string CPositionInfo::FormatType(string &str,const uint type) const 
229   { 
230 //--- clean 
231    str=""; 
232 //--- see the type 
233    switch(type) 
234      { 
235       case POSITION_TYPE_BUY : str="buy";  break; 
236       case POSITION_TYPE_SELL: str="sell"; break; 
237       default                : str="unknown position type "+(string)type; 
238      } 
239 //--- return the result 
240    return(str); 
241   } 
242 //+------------------------------------------------------------------+ 
243 //| Converts the position parameters to text                         | 
244 //+------------------------------------------------------------------+ 
245 string CPositionInfo::FormatPosition(string &str) const 
246   { 
247    string      tmp,type; 
248    CSymbolInfo symbol; 
249 //--- set up 
250    symbol.Name(Symbol()); 
251    int digits=symbol.Digits(); 
252 //--- form the position description 
253    str=StringFormat("%s %s %s %s", 
254                     FormatType(type,PositionType()), 
255                     DoubleToString(Volume(),2), 
256                     Symbol(), 
257                     DoubleToString(PriceOpen(),digits+3)); 
258 //--- add stops if there are any 
259    double sl=StopLoss(); 
260    double tp=TakeProfit(); 
261    if(sl!=0.0) 
262      { 
263       tmp=StringFormat(" sl: %s",DoubleToString(sl,digits)); 
264       str+=tmp; 
265      } 
266    if(tp!=0.0) 
267      { 
268       tmp=StringFormat(" tp: %s",DoubleToString(tp,digits)); 
269       str+=tmp; 
270      } 
271 //--- return the result 
272    return(str); 
273   } 
274 //+------------------------------------------------------------------+ 
275 //| Access functions PositionSelect(...)                             | 
276 //+------------------------------------------------------------------+ 
277 bool CPositionInfo::Select(const string symbol) 
278   { 
279    return(PositionSelect(symbol)); 
280   } 
281 //+------------------------------------------------------------------+ 
282 //| Select a position on the index                                   | 
283 //+------------------------------------------------------------------+ 
284 bool CPositionInfo::SelectByIndex(const int index) 
285   { 
286    string name=PositionGetSymbol(index); 
287    if(name=="") 
288       return(false); 
289 //--- 
290    return(PositionSelect(name)); 
291   } 
292 //+------------------------------------------------------------------+ 
293 //| Stored position's current state                                  | 
294 //+------------------------------------------------------------------+ 
295 void CPositionInfo::StoreState(void) 
296   { 
297    m_type       =PositionType(); 
298    m_volume     =Volume(); 
299    m_price      =PriceOpen(); 
300    m_stop_loss  =StopLoss(); 
301    m_take_profit=TakeProfit(); 
302   } 
303 //+------------------------------------------------------------------+ 
304 //| Check position change                                            | 
305 //+------------------------------------------------------------------+ 
306 bool CPositionInfo::CheckState(void) 
307   { 
308    if(m_type==PositionType() &&  
309       m_volume==Volume() &&  
310       m_price==PriceOpen() &&  
311       m_stop_loss==StopLoss() &&  
312       m_take_profit==TakeProfit()) 
313       return(false); 
314 //--- 
315    return(true); 
316   } 
317 //+------------------------------------------------------------------+ 
