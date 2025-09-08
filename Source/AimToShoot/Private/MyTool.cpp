// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTool.h"

/***************************************************************************************************************
背包所用到的特殊排序算法

****************************************************************************************************************/


int32 UMyTool::GetBPItemCount(const TArray<FBackPackStruct>& BackPack, int32 ItemID) {
	//占位
	return ItemID;
}

//我在这里采用lambda便于后期灵活设定规则。
//考虑以后加泛型筛选，UI下拉框和搜索框还不会，暂时先搁一边，先实现核心逻辑
void UMyTool::SortBackPack(TArray<FBackPackStruct>& BackPack, int32 Low, int32 High)
{
	merge_sort(BackPack,Low,High, [](const FBackPackStruct& a, const FBackPackStruct& b)
		{
			return CompareItems_A(a, b);
		});
}
void UMyTool::SortBackPack_A(TArray<FBackPackStruct>& BackPack, int32 Low, int32 High)
{
	merge_sort(BackPack, Low, High, [](const FBackPackStruct& a, const FBackPackStruct& b)
		{
			return CompareItems_A(a, b);
		});
}
void UMyTool::SortBackPack_B(TArray<FBackPackStruct>& BackPack, int32 Low, int32 High)
{
	merge_sort(BackPack, Low, High, [](const FBackPackStruct& a, const FBackPackStruct& b)
		{
			return CompareItems_B(a, b);
		});
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
bool UMyTool::CompareItems_A(const FBackPackStruct& a, const FBackPackStruct& b) {

	int32 A_Weight = UAllDataDefine::GetItemTypeSortWeight(a.ItemType);
	int32 B_Weight = UAllDataDefine::GetItemTypeSortWeight(b.ItemType);
	if (A_Weight != B_Weight) {
		return A_Weight < B_Weight;
	}
	if (a.Quality != b.Quality) {
		return a.Quality > b.Quality;
	}
	return a.Quantity > b.Quantity;

}

bool UMyTool::CompareItems_B(const FBackPackStruct& a, const FBackPackStruct& b) {

	int32 A_Weight = UAllDataDefine::GetItemTypeSortWeight(a.ItemType);
	int32 B_Weight = UAllDataDefine::GetItemTypeSortWeight(b.ItemType);
	if (A_Weight != B_Weight) {
		return A_Weight < B_Weight;
	}
	if (a.Quantity != b.Quantity) {
		return a.Quantity > b.Quantity;
	}
	return a.Quality > b.Quality;

}
template<typename Compare>
void UMyTool::merge(TArray<FBackPackStruct>& num, int32 left,int32 mid, int32 right, Compare comp) {
	int32 n1 = mid - left + 1;
	int32 n2 = right - mid;

	TArray<FBackPackStruct> L, R;
	L.SetNum(n1);
	R.SetNum(n2);
	for (int32 i = 0; i < n1; i++)	L[i] = num[left + i];
	for (int32 j = 0; j < n2; j++)	R[j] = num[mid + 1 + j];

	int32 i = 0, j = 0, k = left;

	while (i < n1 && j < n2) {
		if (comp(L[i],R[j] ) || (!comp(R[j], L[i]) && !comp(L[i], R[j]))) {
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
template<typename Compare>
void UMyTool::merge_sort(TArray<FBackPackStruct>& num, int32 left, int32 right, Compare comp) {
	if (left < right) {
		int32 mid =left + (right - left) / 2;

		merge_sort(num, left, mid,comp);
		merge_sort(num, mid + 1, right, comp);

		merge(num, left, mid, right, comp);

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