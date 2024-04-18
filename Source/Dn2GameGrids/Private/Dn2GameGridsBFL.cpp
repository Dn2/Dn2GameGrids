// Fill out your copyright notice in the Description page of Project Settings.


#include "Dn2GameGridsBFL.h"

/**/FString UDn2GameGridsBFL::Conv_FCellAddressToString(FCellAddress Struct)
{
	//return  "Row:" + FString::FromInt(Struct.Row) + " | Col:" + FString::FromInt(Struct.Col);
	return Struct.ToString();
}

bool UDn2GameGridsBFL::Equal_CellAddress(FCellAddress A, FCellAddress B)
{
	return A == B;
}



/*void UDn2GameGridsBFL::Set_CellInfoBlocked(FCellAddress Cell, bool Blocked)
{
	Cell.SetBlocked(Blocked);
}*/
