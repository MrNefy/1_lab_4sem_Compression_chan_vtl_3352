#include "header.h"

//слева направо
void sort2(const vector<unsigned char>& data, int* V, int start, int size, int m, int N, int I = 0)
{
	int J = start - m;
	int block = N - m;
	int end = min(start + block, size);
	int str_size = end - start;

	unordered_map<unsigned char, int> S;
	int C[256]{};
	vector<int> P(str_size), Vtemp(str_size);

	for (int i = 0; i < str_size; i++)
	{
		int Suf = V[i + J];
		unsigned char sym = data[(Suf + I) % block + m];
		P[i] = C[sym]++;
		Vtemp[i] = V[i + J];
	}

	for (int i = 0, sum = 0; i < 256; i++)
	{
		if (C[i] != 0)
		{
			S[i] = sum;
			sum += C[i];
		}
	}

	for (int i = 0; i < str_size; i++)
	{
		int Suf = V[i + J];
		unsigned char sym = data[(Suf + I) % block + m];
		P[i] += S[sym];
	}

	for (int i = 0; i < str_size; i++)
	{
		V[P[i] + J] = Vtemp[i];
	}

	// конец сортировки
	if (++I == N) return;

	int i = 0;

	while (start <= end)
	{
		while (i < 256 && C[i] == 0) i++;
		if (C[i] != 1)
			sort2(data, V, start, size, m, N, I);
		start += C[i++];
	}
}

//слева направо
void sort1(const vector<unsigned char>& data, int* V, int start, int end, int m, int N, int I = 0)
{
	int str_size = end - start + 1;
	int block = N - m;
	int J = start - m;
	vector<unsigned char> str(str_size);
	unordered_map<unsigned char, int> S;
	int C[256]{};
	vector<int> P(str_size), Vtemp(str_size);

	for (int i = 0; i < str_size; i++)
	{
		int Suf = V[i + J];
		unsigned char sym = data[(Suf + I) % block + m];
		P[i] = C[sym]++;
		str[i] = sym;
		Vtemp[i] = V[i + J];
	}

	for (int i = 0, sum = 0; i < 256; i++)
	{
		if (C[i] != 0)
		{
			S[i] = sum;
			sum += C[i];
		}
	}

	for (int i = 0; i < str_size; i++)
	{
		if (i == 823)
		{
			cout << "ok";
		}
		P[i] += S[str[i]];
		V[P[i] + J] = Vtemp[i];
	}

	// конец сортировки
	if (++I == N) return;

	int i = 0;

	while (start <= end)
	{
		while (i < 256 && C[i] == 0) i++;
		if (C[i] != 1)
			sort1(data, V, start, start + C[i] - 1, m, N, I);
		start += C[i++];
	}
}

// не работает
/*void quickSort1(const vector<unsigned char>& data, int* V, int left, int right, int start, int b_size, int I) {
	if (right <= left) return;

	int l = left, r = right;

	while (l != r)
	{
		while (l != r
			&& data[start + (V[l] + I) % b_size]
			<= data[start + (V[r] + I) % b_size])
			r--;
		swap(V[l], V[r]);

		if (l == r) break;

		while (l != r
			&& data[start + (V[l] + I) % b_size]
			<  data[start + (V[r] + I) % b_size])
			l++;
		swap(V[l], V[r]);
	}

	if (r != left)  quickSort1(data, V, left,  r - 1, start, b_size, I);
	if (l != right) quickSort1(data, V, l + 1, right, start, b_size, I);
}

void quickSort(const vector<unsigned char>& data, int* V, int start, int end, int m, int N, int I)
{
	if (end <= start) return;

	int middle = data[(V[(start + end) / 2 - m] + I) % (N - m) + m];
	int l = start, r = end;

	while (l <= r)
	{
		while (data[(V[l - m] + I) % (N - m) + m] < middle) l++;
		while (data[(V[r - m] + I) % (N - m) + m] > middle) r--;

		if (l <= r) swap(V[l++ - m], V[r-- - m]);
	}

	if (start < r) quickSort(data, V, start, r, m, N, I);
	if (l < end)   quickSort(data, V, l,   end, m, N, I);
}*/

void quickSort(const vector<unsigned char>& data, int* V, int left, int right, int start, int b_size, int I)
{
	if (right <= left) return;
	
	int l = left, r = right;
	unsigned char middle = data[start + (V[(l + r) / 2 - start] + I) % b_size];

	while (l <= r)
	{
		while (data[start + (V[l - start] + I) % b_size] < middle) l++;
		while (data[start + (V[r - start] + I) % b_size] > middle) r--;

		if (l <= r) swap(V[l++ - start], V[r-- - start]);
	}

	if (left < r)  quickSort(data, V, left,  r, start, b_size, I);
	if (l < right) quickSort(data, V, l, right, start, b_size, I);
}

// Функция для слияния двух отсортированных подмассивов
void mergeArr(const vector<unsigned char>& data, int* V, int left, int mid, int right, int start, int end, int I) {
	int b_size = end - start;
	int arr1 = mid - left + 1;    // Размер левого подмассива
	int arr2 = right - mid;        // Размер правого подмассива

	vector<int> L(arr1), R(arr2);
	//int* L = new int[arr1], * R = new int[arr2];

	// Копируем данные во временные массивы
	for (int i = 0; i < arr1; i++)
		L[i] = V[left - start + i];
	for (int j = 0; j < arr2; j++)
		R[j] = V[mid - start + 1 + j];

	// Слияние временных массивов
	int i = 0, j = 0, k = left - start;
	while (i < arr1 && j < arr2) {
		if (data[start + (L[i] + I) % b_size] <= data[start + (R[j] + I) % b_size]) {
			V[k] = L[i++];
		}
		else {
			V[k] = R[j++];
		}
		k++;
	}

	// Копируем оставшиеся элементы L[]
	while (i < arr1) {
		V[k++] = L[i++];
	}

	// Копируем оставшиеся элементы R[]
	while (j < arr2) {
		V[k++] = R[j++];
	}
}

void mergeSort(const vector<unsigned char>& data, int* V, int left, int right, int start, int end, int I) {
	if (left < right) {
		int mid = left + (right - left) / 2;  // Находим середину

		// Сортируем левую и правую части
		mergeSort(data, V, left, mid, start, end, I);
		mergeSort(data, V, mid + 1, right, start, end, I);

		// Сливаем отсортированные части
		mergeArr(data, V, left, mid, right, start, end, I);
	}
}

void sort(const vector<unsigned char>& data, int* V, int left, int right, int start, int end, int I)
{
	if (right <= left && I >= end - start) return;

	int b_size = end - start;

	/*bool flag = false;

	for (int i = left; i < right; i++)
	{
		if (data[start + (V[i] + I) % b_size] != data[start + (V[i + 1] + I) % b_size])
		{
			flag = true;
			break;
		}
	}

	if (flag)
	{
		cout << left << " " << right << " " << I << endl;
		quickSort(data, V, left, right, start, b_size, I);
	}*/

	//sort2(data, V, left, data.size(), start, end, I);
	//sort1(data, V, left, right, start, end, I);
	//mergeSort(data, V, left, right, start, end, I);
	quickSort(data, V, left, right, start, b_size, I);

	//не работает
	//quickSort1(data, V, left - start, right - start, start, b_size, I);

	int l = left, r = l;

	while (r <= right)
	{
		while (r <= right
			&& data[start + (V[l - start] + I) % b_size]
			== data[start + (V[r - start] + I) % b_size])
			r++;

		sort(data, V, l, r - 1, start, end, I + 1);

		l = r;
	}
}

bool Not_every_symbol_is_the_same(const vector<unsigned char>& data, int start, int end) {
	// предположим что все одинаковые
	bool result = false;

	for (int i = start + 1; i < end; i++) {
		if (data[i] != data[start]/*первому символу*/)
		{// не все одинаковые
			result = true;
			break;
		}
	}

	return result;
}

void sort_go(const vector<unsigned char>& data, int* V, int b_size, int start, int end, int I)
{
	uint8_t* range = new uint8_t[b_size];
	memset(range, 0, b_size);
	//vector<uint8_t> range(b_size);
	range[0] = 1;
	range[b_size - 1] = 1;

	for (int I = 0; I < b_size; I++)
	{// Suffix sort
		int i = 0, j = 0;
		bool At_least_one = false;

		while (j < b_size)
		{
			//обработка столбика в текущем диапозоне
			if (range[j] == 2)
			{//массив из одного эл
				j++;
				continue;
			}
			if (range[j] == 1)
			{//обычный случай
				At_least_one = true;
				i = j++;

				while (j < b_size && range[j] == 0)
					j++;
				//после while текущий j - индекс последнего элемента
			}

			if (j >= b_size) break;

			//нет смысла инициализировать i, но чтобы не жаловался сделаю
			int left  = start + i;
			int right = start + j;

			//проверка на одинаковые символы
			//предположим что все одинаковые
			/*bool result = false;
			unsigned char first = data[start + (V[i] + I) % b_size];

			for (int v = i; v <= j; v++)
			{
				if (data[start + (V[v] + I) % b_size] != first)
				{// не все одинаковые
					result = true;
					break;
				}
			}

			if (result)
			{
				quickSort(data, V, left, right, start, b_size, I);

				//запись следующего столбика
				int l, r = i;//left - start
				right -= start;

				while (r <= right)
				{
					l = r++;
					first = data[start + (V[l] + I) % b_size];

					while (r <= right)
					{
						if (first == data[start + (V[r] + I) % b_size]) r++;
						else break;
					}

					if (l == r - 1)
						range[r - 1] = 2;//массив из одного эл
					else {
						range[r - 1] = 1;//начало массива
						range[l] = 1;//конец
					}
				}
			}
			else
			{
				range[i] = 1;//начало массива
				range[j] = 1;//конец
			}*/
			// дольше чем без неё (при блоке 10 тыс теряем 3 сек)

			quickSort(data, V, left, right, start, b_size, I);

			//запись следующего столбика
			int l, r = i;//left - start
			right -= start;

			while (r <= right)
			{
				l = r++;
				unsigned char first = data[start + (V[l] + I) % b_size];

				while (r <= right)
				{
					if (first == data[start + (V[r] + I) % b_size]) r++;
					else break;
				}

				if (l == r - 1)
					range[r - 1] = 2;//массив из одного эл
				else {
					range[r - 1] = 1;//начало массива
					range[l] = 1;//конец
				}
			}

			j++;
		}

		if (At_least_one == false) break;
	}

	delete[] range;
}

vector<unsigned char> BWT(const vector<unsigned char>& data, vector<int>& index, int block_size)
{
	int size = static_cast<int>(data.size());
	//vector<int> V(block_size);
	int* V = new int[block_size];
	memset(V, 0, block_size);
	vector<unsigned char> output;
	output.reserve(size);
	int countBlock = size / block_size + min(1, size % block_size);
	int start = -block_size, end;

	for (int s = 0; s < countBlock; s++)
	{
		start += block_size;
		end = min(size, start + block_size);
		int b_size = end - start;

		if (Not_every_symbol_is_the_same(data, start, end))
		{
			for (int i = 0; i < b_size; i++) V[i] = i;

			sort_go(data, V, b_size, start, end, 0);
			//sort(data, V, start, end - 1, start, end, 0);

			for (int i = 0; i < b_size; i++)
			{
				if (V[i] == 0) index.push_back(i);
				output.push_back(data[start + (V[i] + b_size - 1) % b_size]);
			}
		}
		else
		{
			index.push_back(0);
			for (int i = start; i < end; i++)
				output.push_back(data[i]);
		}
	}

	delete[] V;

	return output;
}

// Обратное BWT-преобразование
vector<unsigned char> decodeBWT(const vector<unsigned char>& data, vector<int>& index, int block_size)
{
	int size = static_cast<int>(data.size());
	int i_size = static_cast<int>(index.size());
	vector<unsigned char> output(size);
	vector<int> P(block_size);
	int pos, start = -block_size, end;

	for (int s = 0; s < i_size; s++)
	{
		start += block_size;
		end = min(start + block_size, size);

		int C[256]{};
		P.assign(block_size, 0);

		for (int p = 0, i = start; i < end; p++, i++)
		{// записываем где встретили текущий символ и записываем его количество
			P[p] = C[data[i]];
			C[data[i]]++;
		}

		for (int i = 0, sum = 0; i < 256; i++)
		{// Sort
			sum += C[i];
			C[i] = sum - C[i];
		}

		for (int p = 0, i = start; i < end; p++, i++)
		{// номер перестановки
			P[p] += C[data[i]];
		}

		// номер оригинальной строки суффиксного массива
		pos = index[s];

		for (int i = end - 1; i >= start; i--)
		{
			output[i] = data[start + pos];
			pos = P[pos];
		}
	}

	return output;
}

/*
void qck_sort(const vector<unsigned char>& data, int* V, int start, int end, int m, int N, int I)
{
	if (end > start)
	{
		int middle = data[(V[(start + end) / 2 - m] + I) % (N - m) + m];

		int u = start, v = end, tmp;

		while (u <= v)
		{
			while (data[(V[u - m] + I) % (N - m) + m] < middle) u++;
			while (data[(V[v - m] + I) % (N - m) + m] > middle) v--;

			if (u <= v)
			{
				tmp = V[u - m];
				V[u++ - m] = V[v - m];
				V[v-- - m] = tmp;
			}
		}

		qck_sort(data, V, start, v, m, N, I);
		qck_sort(data, V, u, end, m, N, I);
	}
}

void sort(const vector<unsigned char>& data, int* V, int start, int end, int m, int N, int I)
{

	if (end > start && I < N - m)
	{
		qck_sort(data, V, start, end, m, N, I);

		int first = start, second = first;

		while (second <= end)
		{
			while (second <= end && data[(V[first - m] + I) % (N - m) + m]
				== data[(V[second - m] + I) % (N - m) + m]) {
				second++;
			}
			sort(data, V, first, second - 1, m, N, I + 1);

			first = second;
		}
	}
}

vector<unsigned char> BWT(const vector<unsigned char>& data, vector<int>& index, int block_size)
{
	int size = static_cast<int>(data.size());
	vector<int> V(block_size);
	vector<unsigned char> output;
	int countBlock = size / block_size + min(1, size % block_size);
	int start = -block_size, end;

	for (int s = 0; s < countBlock; s++)
	{
		start += block_size;
		end = min(size, start + block_size);
		int b_size = end - start;

		for (int i = 0; i < b_size; i++) V[i] = i;

		sort(data, V, start, end - 1, start, end, 0);

		for (int i = 0; i < b_size; i++)
		{
			if (V[i] == 0) index.push_back(i);
			output.push_back(start + data[(V[i] + b_size - 1) % b_size]);
		}
	}

	return output;
}


// Обратное BWT-преобразование
vector<unsigned char> decodeBWT(const vector<unsigned char>& data, vector<int>& index, int block_size)
{
	int size = static_cast<int>(data.size());
	int i_size = static_cast<int>(index.size());
	vector<unsigned char> output(size);
	vector<int> P(block_size);
	int sum, pos, start = -block_size, end;

	for (int s = 0; s < i_size; s++)
	{
		start += block_size;
		end = min(start + block_size, size);

		int C[256]{};
		P.assign(block_size, 0);

		for (int p = 0, i = start; i < end; p++, i++)
		{// записываем где встретили текущий символ и записываем его количество
			P[p] = C[data[i]];
			C[data[i]]++;
		}

		sum = 0;

		for (int i = 0; i < 256; i++)
		{// Sort
			sum += C[i];
			C[i] = sum - C[i];
		}

		for (int p = 0, i = start; i < end; p++, i++)
		{// номер перестановки
			P[p] += C[data[i]];
		}

		// номер оригинальной строки суффиксного массива
		pos = index[s];

		for (int i = end - 1; i >= start; i--)
		{
			output[i] = data[start + pos];
			pos = P[pos];
		}
	}

	return output;
}*/

/*bool suffix_compare(const vector<unsigned char>& data, int i, int j, int m, int N)
{
	for (int s = 0; s < N - m; s++)
	{
		if (data[(s + i) % (N - m) + m] != data[(s + j) % (N - m) + m])
			return data[(s + i) % (N - m) + m] < data[(s + j) % (N - m) + m];
	}

	return false;
}

void qck_sort(const vector<unsigned char>& data, int* V, int start, int end, int m, int N)
{
	if (end > start)
	{
		int middle = V[(start + end) / 2 - m];

		int u = start, v = end;

		while (u <= v)
		{
			while (suffix_compare(data, V[u - m], middle, m, N)) u++;
			while (suffix_compare(data, middle, V[v - m], m, N)) v--;

			if (u <= v) swap(V[u++ - m], V[v-- - m]);
		}
		qck_sort(data, V, start, v, m, N);
		qck_sort(data, V, u, end, m, N);
	}
}

//слева направо
void sort2(const vector<unsigned char>& data, int* V, int start, int size, int m, int N, int I = 0)
{
	int J = start - m;
	int block = N - m;
	int end = min(start + block, size);
	int str_size = end - start + 1;

	unordered_map<unsigned char, int> S;
	int C[256]{};
	vector<int> P(str_size), Vtemp(str_size);

	for (int i = 0; i < str_size; i++)
	{
		int Suf = V[i + J];
		unsigned char sym = data[(Suf + I) % block + m];
		P[i] = C[sym]++;
		Vtemp[i] = V[i + J];
	}

	for (int i = 0, sum = 0; i < 256; i++)
	{
		if (C[i] != 0)
		{
			S[i] = sum;
			sum += C[i];
		}
	}

	for (int i = 0; i < str_size; i++)
	{
		int Suf = V[i + J];
		unsigned char sym = data[(Suf + I) % block + m];
		P[i] += S[sym];
	}

	for (int i = 0; i < str_size; i++)
	{
		V[P[i] + J] = Vtemp[i];
	}

	// конец сортировки
	if (++I == N) return;

	int i = 0;

	while (start <= end)
	{
		while (i < 256 && C[i] == 0) i++;
		if (C[i] != 1)
			sort2(data, V, start, size, m, N, I);
		start += C[i++];
	}
}

//слева направо
void sort1(const vector<unsigned char>& data, int* V, int start, int end, int m, int N, int I = 0)
{
	int str_size = end - start + 1;
	int block = N - m;
	int J = start - m;
	vector<unsigned char> str(str_size);
	unordered_map<unsigned char, int> S;
	int C[256]{};
	vector<int> P(str_size), Vtemp(str_size);

	for (int i = 0; i < str_size; i++)
	{
		int Suf = V[i + J];
		unsigned char sym = data[(Suf + I) % block + m];
		P[i] = C[sym]++;
		str[i] = sym;
		Vtemp[i] = V[i + J];
	}

	for (int i = 0, sum = 0; i < 256; i++)
	{
		if (C[i] != 0)
		{
			S[i] = sum;
			sum += C[i];
		}
	}

	for (int i = 0; i < str_size; i++)
	{
		if (i == 823)
		{
			cout << "ok";
		}
		P[i] += S[str[i]];
		V[P[i] + J] = Vtemp[i];
	}

	// конец сортировки
	if (++I == N) return;

	int i = 0;

	while (start <= end)
	{
		while (i < 256 && C[i] == 0) i++;
		if (C[i] != 1)
			sort1(data, V, start, start + C[i] - 1, m, N, I);
		start += C[i++];
	}
}

//sort(data, V, size, b_size, start, end, max_idx);
//справа налево
void sort(const vector<unsigned char>& data, int* V, const int size, int b_size, int start, int end, int I)
{
	int C[256]{};
	vector<int> Pb_size, Vtemp(b_size);

	for (int p = 0, i = start; i < end; p++, i++)
	{// записываем где встретили текущий символ и записываем его количество
		unsigned char sym = data[(V[i] + I) % b_size];
		P[p] = C[sym];
		C[sym]++;
		Vtemp[i] = V[i];
	}

	for (int i = 0, sum = 0; i < 256; i++)
	{// Sort
		sum += C[i];
		C[i] = sum - C[i];
	}

	for (int p = 0, i = start; i < end; p++, i++)
	{// номер перестановки
		P[i] += C[data[(V[i] + I) % b_size + start]];
	}

	for (int p = 0, i = start; i < end; p++, i++)
	{
		V[P[i]] = Vtemp[i];
	}

	// конец сортировки
	if (--I < 0) return;
	else
		sort(data, V, size, b_size, start, end, I);
}

// Преобразование BWT
vector<unsigned char> BWT(const vector<unsigned char>& data, vector<int>& index, const short BLOCK_SIZE)
{
	int size = static_cast<int>(data.size());
	vector<int> V(BLOCK_SIZE);
	vector<unsigned char> output;
	int block_c = size / BLOCK_SIZE + min(1, size % BLOCK_SIZE);

	//for (int s = 0; s < block_c; s++)
	//{
	//	n = s * BLOCK_SIZE;
	//	N = min(size, n + BLOCK_SIZE);
	//	b_size = N - n;
	//
	//	for (int i = 0; i < b_size; i++) V[i] = i;
	//
	//	sort(data, V, -N, b_size, n, N, N - 1);
	//	//sort1(data, V, n, N - 1, n, N);
	//	//sort2(data, V, n, size, n, N);
	//	//qck_sort(data, V, n, N - 1, n, N);
	//
	//	for (int i = 0; i < b_size; i++)
	//	{
	//		if (V[i] == 0)
	//			index.push_back(i);
	//		output.push_back(data[(V[i] + b_size - 1) % b_size + n]);
	//	}
	//}

	int start = -BLOCK_SIZE, end, b_size;

	for (int s = 0; s < block_c; s++)
	{
		start += BLOCK_SIZE;
		end = min(size, start + BLOCK_SIZE);
		b_size = end - start;

		for (int i = 0; i < b_size; i++) V[i] = i;

		int* P = new int[b_size];
		int* Vtemp = new int[b_size];

		int max_idx = b_size - 1;

		for (int I = max_idx; I >= 0; I--)
		{// Suffix sort
			int C[256]{};

			for (int i = 0; i < b_size; i++)
			{// записываем где встретили текущий символ и записываем его количество
				P[i] = C[data[start + (V[i] + I) % b_size]]++;
				Vtemp[i] = V[i];
			}

			for (int i = 0, sum = 0; i < 256; i++)
			{// Sort char
				sum += C[i];
				C[i] = sum - C[i];
			}

			for (int i = 0; i < b_size; i++)
			{// номер перестановки
				P[i] += C[data[start + (V[i] + I) % b_size]];
			}

			for (int i = 0; i < b_size; i++)
			{
				//int pos = P[i];
				V[P[i]] = Vtemp[i];
			}
		}

		delete[] P;
		delete[] Vtemp;

		for (int i = 0; i < b_size; i++)
		{
			if (V[i] == 0)
				index.push_back(i);
			output.push_back(data[start + (V[i] + max_idx) % b_size]);
		}
	}

	return output;
}*/