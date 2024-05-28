// Fill out your copyright notice in the Description page of Project Settings.


#include "Dn2GameGridsBFL.h"

/**/FString UDn2GameGridsBFL::Conv_FCellAddressToString(FCellAddress Struct)
{
	//return  "Row:" + FString::FromInt(Struct.X) + " | Col:" + FString::FromInt(Struct.Y);
	return Struct.ToString();
}

bool UDn2GameGridsBFL::Equal_CellAddress(FCellAddress A, FCellAddress B)
{
	return A == B;
}


FCellAddress UDn2GameGridsBFL::Conv_GetCellNorth(FCellAddress Address)
{
	return FCellAddress(Address.X, Address.Y+1);
}

FCellAddress UDn2GameGridsBFL::Conv_GetCellSouth(FCellAddress Address)
{
	return FCellAddress(Address.X, Address.Y - 1);
}



FCellAddress UDn2GameGridsBFL::Conv_GetCellEast(FCellAddress Address)
{
	return FCellAddress(Address.X+1, Address.Y);
}

FCellAddress UDn2GameGridsBFL::Conv_GetCellWest(FCellAddress Address)
{
	return FCellAddress(Address.X-1, Address.Y);
}

/*void UDn2GameGridsBFL::Set_CellInfoBlocked(FCellAddress Cell, bool Blocked)
{
	Cell.SetBlocked(Blocked);
}*/
