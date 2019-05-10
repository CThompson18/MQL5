//	ENUM_TIMEFRAMES
//	********************************************
//	PERIOD_CURRENT	0		Current timeframe
//	PERIOD_M1		1		1 minute
//	PERIOD_M2		2		2 minutes
//	PERIOD_M3		3		3 minutes
//	PERIOD_M4		4		4 minutes
//	PERIOD_M5		5		5 minutes
//	PERIOD_M6		6		6 minutes
//	PERIOD_M10		10		10 minutes
//	PERIOD_M12		12		12 minutes
//	PERIOD_M15		15		15 minutes
//	PERIOD_M20		20		20 minutes
//	PERIOD_M30		30		30 minutes
//	PERIOD_H1		60		1 hour
//	PERIOD_H2		120		2 hours
//	PERIOD_H3		180		3 hours
//	PERIOD_H4		240		4 hours
//	PERIOD_H6		360		6 hours
//	PERIOD_H8		480		8 hours
//	PERIOD_H12		720		12 hours
//	PERIOD_D1		1440	1 day
//	PERIOD_W1		10080	1 week
//	PERIOD_MN1		43200	1 month


using System;
using MT4API;

class Program

{

   static void Main(string[] args)

   {

      string symbol = "EURUSD";

      MT4 mt = new MT4(null, 0);

      double ask = mt.MarketInfo(symbol, InfoMode.ASK);

      int ticket = mt.OrderSend(symbol, Op.BUY, 1, ask, 0, 0, 0);

      if (ticket <= 0)

         throw new Exception(mt.GetLastErrorDescription());

   }

}

 
