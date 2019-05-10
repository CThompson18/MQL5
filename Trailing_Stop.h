//TrailOrder
//***************************************************************************************************************************************************
void TrailOrder(double Trailingstart,double Trailingstop)
	{
		int ticket = 0;
		double tStopLoss = NormalizeDouble(OrderStopLoss(), Digits); // Stop Loss
		int cnt,vPoint,vSlippage;
		double sl     = OrderStopLoss(); // Stop Loss

		if (Digits == 3 || Digits == 5){vPoint = Point * 10; vSlippage = Slippage * 10;} else {vPoint = Point; vSlippage = Slippage;}
		
		RefreshRates();
		if(OrdersTotal()>0)
			{
				for(cnt=OrdersTotal();cnt>=0;cnt--)
					{
						OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES);
						if(OrderType()<=OP_SELL && OrderSymbol()==Symbol() && OrderMagicNumber()==MagicNumber)
							{
								if(OrderType()==OP_BUY)
									{
										if(Ask> NormalizeDouble(OrderOpenPrice()+TrailingStart* vPoint,Digits) && tStopLoss < NormalizeDouble(Bid-(TrailingStop+TrailingStep)*vPoint,Digits))
											{
												tStopLoss = NormalizeDouble(Bid-TrailingStop*vPoint,Digits);
												ticket = OrderModify(OrderTicket(),OrderOpenPrice(),tStopLoss,OrderTakeProfit(),0,Blue);
												if (ticket > 0)
													{
														Print ("TrailingStop #2 Activated: ", OrderSymbol(), ": SL", tStopLoss, ": Bid", Bid);
														return(0);
													}
											}
									}

								if (OrderType()==OP_SELL) 
									{
										if (Bid < NormalizeDouble(OrderOpenPrice()-TrailingStart*vPoint,Digits) && (sl >(NormalizeDouble(Ask+(TrailingStop+TrailingStep)*vPoint,Digits))) || (OrderStopLoss()==0))
											{
												tStopLoss = NormalizeDouble(Ask+TrailingStop*vPoint,Digits);
												ticket = OrderModify(OrderTicket(),OrderOpenPrice(),tStopLoss,OrderTakeProfit(),0,Red);
												if (ticket > 0)
													{
														Print ("Trailing #2 Activated: ", OrderSymbol(), ": SL ",tStopLoss, ": Ask ", Ask);
														return(0);
													}
											}
									}
							}
					}
			}
	}
	