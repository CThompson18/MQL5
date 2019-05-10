//+------------------------------------------------------------------+
//|                    Converted by: MqlConvert.com                  |
//|                       Site of CyberFX.org                        |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//|                                           SHI_SilverTrendSig.mq4 |
//|          Copyright © 2003, VIAC.RU, OlegVS, GOODMAN, 2005 Shurka |
//|                                                 shforex@narod.ru |
//|                                                                  |
//|                                                                  |
//| Ïèøó ïðîãðàììû íà çàêàç                                          |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2005, Shurka"
#property link      "http://shforex.narod.ru"

#property indicator_chart_window
#property indicator_buffers 2
#property indicator_color1 Red
#property indicator_color2 Blue
#define  SH_BUY   1
#define  SH_SELL  -1

//---- Âõîäíûå ïàðàìåòðû
input int     AllBars=0;//Äëÿ ñêîëüêè áàðîâ ñ÷èòàòü. 0 - äëÿ âñåõ.
input int     Otstup=30;//Îòñòóï.
input double  Per=9;//Ïåðèîä.
int            SH,NB,i,UD;
double         R,SHMax,SHMin;
double         BufD[];
double         BufU[];
//+------------------------------------------------------------------+
//| Ôóíêöèÿ èíèöèàëèçàöèè                                            |
//+------------------------------------------------------------------+
int init()
{
   //Â NB çàïèñûâàåì êîëè÷åñòâî áàðîâ äëÿ êîòîðûõ ñ÷èòàåì èíäèêàòîð
   if (Bars<AllBars+Per || AllBars==0) NB=Bars-Per; else NB=AllBars;
   //IndicatorBuffers(2);
   IndicatorShortName("SHI_SilverTrendSig");
   SetIndexStyle(0,DRAW_ARROW,0,1);
   SetIndexStyle(1,DRAW_ARROW,0,1);
   SetIndexArrow(0,159);
   SetIndexArrow(1,159);
   SetIndexBuffer(0,BufU);
   SetIndexBuffer(1,BufD);
   SetIndexDrawBegin(0,Bars-NB);//Èíäèêàòîð áóäåòîòîáðàæàòüñÿ òîëüêî äëÿ NB áàðîâ
   SetIndexDrawBegin(1,Bars-NB);
   ArrayInitialize(BufD,0.0);//Çàáü¸ì îáà áóôåðà íîëèêàìè. Èíà÷å áóäåò ìóñîð ïðè ñìåíå òàéìôðåéìà.
   ArrayInitialize(BufU,0.0);
   return(0);
}
//+------------------------------------------------------------------+
//| Ôóíêöèÿ äåèíèöèàëèçàöèè                                          |
//+------------------------------------------------------------------+
int deinit()
{
   return(0);
}
//+------------------------------------------------------------------+
//| Ñîáñíà èíäèêàòîð                                                 |
//+------------------------------------------------------------------+
int start()
{
   int CB=IndicatorCounted();
   /* Òóò âîò òà ñàìàÿ îïòèìèçàöèîííàÿ ôèøêà. Â ÿçûê ââåäåíà ôóíêöèÿ, êîòîðàÿ âîçâðàùàåò êîëè÷åñòâî
   ïîñ÷èòàííûõ áàðîâ, ïðè÷¸ì î÷åíü õèòðî. Ïðè ïåðâîì âûçîâå èíäèêàòîðà ýòî 0, âñ¸ ïîíÿòíî, åù¸ íè÷åãî
   íå ñ÷èòàëîñü, à çàòåì âûäà¸ò êîëè÷åñòâî îáñ÷èòàííûõ áàðîâ ìèíóñ îäèí. Ò.å. åñëè âñåãî áàðîâ 100,
   òî ôóíêöèÿ âåðí¸ò 99. ß ââ¸ë òàêîé êîä, âûøå ó ìåíÿ îïðåäåëÿëàñü NB - êîë-âî áàðîâ ïîäëåæàùèõ
   îáñ÷¸òó. Â ïðèíöèïå ýòîò ïàðàìåòð ìîæíî è âûêèíóòü, îäíàêî äëÿ òåõ êòî â òàíêå (I80286) ìîæíî
   è îñòàâèòü. Òàê âîò, çäåñü, ïðè ïåðâîì âûçîâå NB îñòà¸òñÿ ïðåæíåé, à ïðè ïîñëåäóþùèõ óìåíüøàåòñÿ
   äî ïîñëåäíåãî áàðà, ò.å. 1 èëè 2, íó èëè ñêîëüêî òàì îñòàëîñü ïîñ÷èòàòü*/
   if(CB<0) return(-1); else if(NB>Bars-CB) NB=Bars-CB;
   for (SH=1;SH<NB;SH++)//Ïðî÷¸ñûâàåì ãðàôèê îò 1 äî NB
   {
      for (R=0,i=SH;i<SH+10;i++) {R+=(10+SH-i)*(High[i]-Low[i]);}      R/=55;

      SHMax = High[Highest(NULL,0,2,Per,SH)];
      SHMin = Low[Lowest(NULL,0,1,Per,SH)];
      if (Close[SH]<SHMin+(SHMax-SHMin)*Otstup/100 && UD!=SH_SELL) { BufU[SH]=Low[SH]-R*0.5; UD=SH_SELL; }
      if (Close[SH]>SHMax-(SHMax-SHMin)*Otstup/100 && UD!=SH_BUY) { BufD[SH]=High[SH]+R*0.5; UD=SH_BUY; }
   }
   return(0);
}
//********************************************  Emulation *******************************************
double Ask;
double Bid;
double Bars;
double Open[];
double Close[];
double High[];
double Low[];
datetime Time[];
long Volume[];

void Set_Values_to_variables()
{
   MqlTick last_tick;
   SymbolInfoTick(_Symbol,last_tick);
   Ask=last_tick.ask;
   Bid=last_tick.bid;
  
   ArraySetAsSeries(Close,true);
   CopyClose(_Symbol,_Period,0,Bars(_Symbol,_Period),Close);
   ArraySetAsSeries(Open,true);
   CopyOpen(_Symbol,_Period,0,Bars(_Symbol,_Period),Open);
   ArraySetAsSeries(Low,true);
   CopyLow(_Symbol,_Period,0,Bars(_Symbol,_Period),Low);
   ArraySetAsSeries(High,true);
   CopyHigh(_Symbol,_Period,0,Bars(_Symbol,_Period),High);
   ArraySetAsSeries(Time,true);
   CopyTime(_Symbol,_Period,0,Bars(_Symbol,_Period),Time);
   ArraySetAsSeries(Volume,true);
   CopyTickVolume(_Symbol,_Period,0,Bars(_Symbol,_Period),Volume);
} 
void OnTick()
{
	Set_Values_to_variables();
}
int OnCalculate(const int rates_total, const int prev_calculated, const datetime &time[], const double &open[], const double &high[], 
                const double &low[], const double &close[], const long &tick_volume[], const long &volume[], const int &spread[])     
{
	Set_Values_to_variables();
}
void OnInit()
{
	Set_Values_to_variables();
	init();
}
void OnDeinit()
{
	deinit();
}
ENUM_TIMEFRAMES TFMigrate(int tf)
{
   switch(tf)
   {
      case 0: return(PERIOD_CURRENT);
      case 1: return(PERIOD_M1);
      case 5: return(PERIOD_M5);
      case 15: return(PERIOD_M15);
      case 30: return(PERIOD_M30);
      case 60: return(PERIOD_H1);
      case 240: return(PERIOD_H4);
      case 1440: return(PERIOD_D1);
      case 10080: return(PERIOD_W1);
      case 43200: return(PERIOD_MN1);
      
      case 2: return(PERIOD_M2);
      case 3: return(PERIOD_M3);
      case 4: return(PERIOD_M4);      
      case 6: return(PERIOD_M6);
      case 10: return(PERIOD_M10);
      case 12: return(PERIOD_M12);
      case 16385: return(PERIOD_H1);
      case 16386: return(PERIOD_H2);
      case 16387: return(PERIOD_H3);
      case 16388: return(PERIOD_H4);
      case 16390: return(PERIOD_H6);
      case 16392: return(PERIOD_H8);
      case 16396: return(PERIOD_H12);
      case 16408: return(PERIOD_D1);
      case 32769: return(PERIOD_W1);
      case 49153: return(PERIOD_MN1);      

      default: return(PERIOD_CURRENT);
   }
}