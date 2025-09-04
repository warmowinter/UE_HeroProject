// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTool.h"


int32 UMyTool::GetBPItemCount(const TArray<FBackPackStruct>& BackPack, int32 ItemID) {
	//占位
	return ItemID;
}


void UMyTool::SortBackPack(TArray<FBackPackStruct>& BackPack, int32 Low, int32 High)
{
	merge_sort(BackPack,Low,High);
}
/**************************************************************************************************************
排序算法：快排(Lomuto style)

**************************************************************************************************************/
int UMyTool::Partition(TArray<FBackPackStruct>& Num, int32 low, int32 high) {
	int32 PivotIndex = (low + high) / 2;
	FBackPackStruct temp = Num[PivotIndex];
	Num.Swap(low, PivotIndex);

	int32 i = low;
	for (int j = low + 1; j <= high; j++) {
		if (Num[j].ItemID != 0 && (temp.ItemID == 0 || Num[j].ItemID <= temp.ItemID)) {
			i++;
			Num.Swap(i,j);
		}

	}

	Num.Swap(i,low);
	return i;
}

void UMyTool::FastSort_A(TArray<FBackPackStruct>& Num, int32 low, int32 high) {
	if (low < high) {
		int pi = Partition(Num, low, high);

		FastSort_A(Num, low, pi - 1);
		FastSort_A(Num, pi + 1,high);
	}
}
/***************************************************************************************************************
归并排序（merge）

*****************************************************************************************************************/
void UMyTool::merge(TArray<FBackPackStruct>& num, int32 left,int32 mid, int32 right) {
	int32 n1 = mid - left + 1;
	int32 n2 = right - mid;

	TArray<FBackPackStruct> L, R;
	L.SetNum(n1);
	R.SetNum(n2);
	for (int32 i = 0; i < n1; i++)	L[i] = num[left + i];
	for (int32 j = 0; j < n2; j++)	R[j] = num[mid + 1 + j];

	int32 i = 0, j = 0, k = left;

	while (i < n1 && j < n2) {
		if (L[i].ItemID <= R[j].ItemID && L[i].ItemID != 0 && R[j].ItemID != 0 || L[i].ItemID != 0 && R[j].ItemID == 0) {
			num[k] = L[i];
			i++;
		}
		else {
			num[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		num[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		num[k] = R[j];
		j++;
		k++;
	}
}

void UMyTool::merge_sort(TArray<FBackPackStruct>& num, int32 left, int32 right) {
	if (left < right) {
		int32 mid =left + (right - left) / 2;

		merge_sort(num, left, mid);
		merge_sort(num, mid + 1, right);

		merge(num, left, mid, right);

	}
}

/********************************************************************************************************************
基数排序LSD + MSD

*********************************************************************************************************************/
int32 UMyTool::GetMaxDigits(const TArray<FBackPackStruct>& num) {
	if (num.IsEmpty())	return 0;
	int32 MaxDigits = 0;
	for (int32 i = 0; i < num.Num(); i++) {
		if (MaxDigits <= num[i].ItemID) {
			MaxDigits = num[i].ItemID;
		}
	}
	int32 digits = 0;
	while (MaxDigits > 0) {
		MaxDigits /= 10;
		digits++;
	}
	return digits;
}


int32 UMyTool::GetDigit(int32 num, int32 d) {
	int32 divisor = 1;
	for (int32 i = 0; i < d; i++) {
		divisor *= 10;
	}
	return (num / divisor) % 10;
}

void UMyTool::radixSortLSD(TArray<FBackPackStruct>& num) {
	if (num.IsEmpty())	return;
	int32 maxDigits = GetMaxDigits(num);
	const int radix = 10;

	TArray<TArray<FBackPackStruct>> buckets;
	buckets.SetNum(radix);

	for (int32 d = 0; d < maxDigits; d++) {
		
		for (FBackPackStruct a : num) {
			int32 digit = GetDigit(a.ItemID, d);
			buckets[digit].Push(a);
		}
		int index = 0;
		for (auto& bucket : buckets) {
			for (FBackPackStruct b : bucket) {
				num[index++] = b;
			}
			bucket.Empty();
		}
	}
}