 if (TotalOrders(glbMagic_Number)>0)
    {
		for (int t = 0;t < OrdersTotal();t++)
			{
				OrderSelect(t,SELECT_BY_POS);
				if (OrderType() == glbMagic_Number)
					{
						break;
					}
			}
		if (OrderType() == OP_BUY && dn)
			{
				OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Bid,Digits),glbSlippage,Pink);
			}
		if (OrderType() == OP_SELL && up)
			{
				OrderClose(OrderTicket(),OrderLots(),NormalizeDouble(Ask,Digits),glbSlippage,Pink);
			}
    }