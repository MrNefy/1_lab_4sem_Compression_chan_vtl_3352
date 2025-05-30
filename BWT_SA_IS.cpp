#include "Header.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> buildTypeMap(const vector<int>& data) {
    int size = data.size();
    vector<int> res(size + 1, 0);// init S type

    if (data.empty()) return res;

    res[size - 1] = 1;// L type - последний символ строки не считая терминал

    for (int i = size - 2; i >= 0; i--) {
        if (data[i] > data[i + 1]) {
            res[i] = 1;// L type
        }
        else if (data[i] == data[i + 1] && res[i + 1] == 1) {
            res[i] = 1;// L type
        }
        else {
            res[i] = 0;// S type
        }
    }

    return res;
}

bool char_is_LMS(int offset, const vector<int>& typemap) {
    if (offset == 0) return 0;
    return typemap[offset] == 0 && typemap[offset - 1] == 1;// previos L type, current S type
}

bool LMS_substringsAreEqual(const vector<int>& str
    , const vector<int>& typemap,
    int offsetA, int offsetB) {

    if (offsetA == str.size() || offsetB == str.size()) return false;

    int i = 0;

    while (true) {
        bool a_is_LMS = char_is_LMS(i + offsetA, typemap);
        bool b_is_LMS = char_is_LMS(i + offsetB, typemap);

        if (i > 0 && a_is_LMS && b_is_LMS) return true;
        if (a_is_LMS != b_is_LMS) return false;
        if (str[i + offsetA] != str[i + offsetB]) return false;

        i++;
    }
}

vector<int> get_bucketSizes(const vector<int>& str, int alphabetSize) {
    vector<int> res(alphabetSize, 0);

    for (int c : str) res[c]++;
    return res;
}

vector<int> get_headOffsets(const vector<int>& bucketSizes) {
    vector<int> res;
    int offset = 1;

    for (int size : bucketSizes)
    {// записываем индекс для индуцирования вправую сторону (induceSortL)
        res.push_back(offset);
        offset += size;
    }
    return res;
}

vector<int> get_tailOffsets(const vector<int>& bucketSizes) {
    vector<int> res;
    int offset = 0;

    for (int size : bucketSizes)
    {// записываем индекс для индуцирования влевую сторону (induceSortS)
        offset += size;
        res.push_back(offset);
    }
    return res;
}

vector<int> LMS_sort(const vector<int>& str
    , const vector<int>& bucketSizes, const vector<int>& typemap) {
    int size = str.size();
    vector<int> SA(size + 1, -1);

    auto tails = get_tailOffsets(bucketSizes);

    for (int i = 0; i < size; i++) {
        if (!char_is_LMS(i, typemap)) continue;

        int idx = str[i];
        SA[tails[idx]] = i;
        tails[idx]--;
    }

    SA[0] = size;
    return SA;
}

void L_induceSort(const vector<int>& str
    , vector<int>& guessed_SA,const vector<int>& bucketSizes, const vector<int>& typemap) {
    int size = guessed_SA.size();
    auto heads = get_headOffsets(bucketSizes);

    for (int i = 0; i < size; i++) {
        if (guessed_SA[i] == -1) continue;

        int j = guessed_SA[i] - 1;
        if (j < 0 || typemap[j] != 1) continue;// L type

        int sym = str[j];
        guessed_SA[heads[sym]] = j;
        heads[sym]++;
    }
}

void S_induceSort(const vector<int>& str
    , vector<int>& guessed_SA, const vector<int>& bucketSizes, const vector<int>& typemap) {

    auto Offsets = get_tailOffsets(bucketSizes);

    for (int i = guessed_SA.size() - 1; i >= 0; i--) {
        int j = guessed_SA[i] - 1;
        if (j < 0 || typemap[j] != 0) continue;// S type

        int sym = str[j];
        guessed_SA[Offsets[sym]] = j;
        Offsets[sym]--;
    }
}

struct Summary {
    int alphabetSize;
    vector<int> string;
    vector<int> suffixOffsets;
};

Summary summarise_SA(const vector<int>& str
    , const vector<int>& guessed_SA, const vector<int>& typemap) {

    int size = guessed_SA.size();
    vector<int> lmsNames(str.size() + 1, -1);
    int currentName = 0;
    int lastLMS_SuffixOffset = guessed_SA[0];
    lmsNames[lastLMS_SuffixOffset] = currentName;

    for (int i = 1; i < size; i++) {
        int suffixOffset = guessed_SA[i];
        if (!char_is_LMS(suffixOffset, typemap)) continue;

        if (!LMS_substringsAreEqual(str, typemap, lastLMS_SuffixOffset, suffixOffset)) {
            currentName++;
        }

        lastLMS_SuffixOffset = suffixOffset;
        lmsNames[suffixOffset] = currentName;
    }

    vector<int> summaryStr;
    vector<int> summarySuffixOffsets;
    int lmsSize = lmsNames.size();

    for (int i = 0; i < lmsSize; i++) {
        if (lmsNames[i] == -1) continue;
        summarySuffixOffsets.push_back(i);
        summaryStr.push_back(lmsNames[i]);
    }

    int summaryAlphabetSize = currentName + 1;
    return { summaryAlphabetSize, summaryStr, summarySuffixOffsets };
}

vector<int> make_Summary_SA(const vector<int>& summaryStr,
    int summaryAlphabetSize);

vector<int> accurateLMSSort(const vector<int>& str,
    const vector<int>& bucketSizes, const vector<int>& typemap,
    const vector<int>& summary_SA, const vector<int>& summarySuffixOffsets) {
    vector<int> suffixOffsets(str.size() + 1, -1);
    auto Offsets = get_tailOffsets(bucketSizes);

    for (int i = summary_SA.size() - 1; i >= 1; i--) {
        int idx = summarySuffixOffsets[summary_SA[i]] - 1;
        int sym = str[idx];
        suffixOffsets[Offsets[sym]] = idx;
        Offsets[sym]--;
    }

    suffixOffsets[0] = str.size();
    return suffixOffsets;
}

vector<int> InducedSorting_SA(const vector<int>& str
    , int alphabetSize);
vector<int> make_Summary_SA(const vector<int>& summaryStr
    , int summaryAlphabetSize) {
    int size = summaryStr.size();
    
    if (summaryAlphabetSize == size) {
        vector<int> summary_SA(size + 1, -1);
        summary_SA[0] = size;

        for (int x = 0; x < size; ++x) {
            int y = summaryStr[x];
            summary_SA[y + 1] = x;
        }
        return summary_SA;
    }
    else {
        return InducedSorting_SA(summaryStr, summaryAlphabetSize);
    }
}

vector<int> InducedSorting_SA(const vector<int>& str,
    int alphabetSize) {
    auto typemap = buildTypeMap(str);
    auto bucketSizes = get_bucketSizes(str, alphabetSize);

    auto guessed_SA = LMS_sort(str, bucketSizes, typemap);
    
    L_induceSort(str, guessed_SA, bucketSizes, typemap);
    S_induceSort(str, guessed_SA, bucketSizes, typemap);

    auto summary = summarise_SA(str, guessed_SA, typemap);
    auto summary_SA = make_Summary_SA(summary.string, summary.alphabetSize);

    auto result = accurateLMSSort(str, bucketSizes, typemap, summary_SA, summary.suffixOffsets);

    L_induceSort(str, result, bucketSizes, typemap);
    S_induceSort(str, result, bucketSizes, typemap);

    return guessed_SA;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int get_indexZero(const vector<int>& sa) {
    size_t size = sa.size();

    for (size_t i = 0; i < size; i++)
    {
        if (sa[i] == 0)
        {
            return i;
        }
    }
}

// Функция для преобразования строки с суффиксным массивом в BWT
vector<int> sa_to_bwt(const vector<int>& str
    , const vector<int>& sa, int& index) {
    vector<int> bwt;
    vector<int> temp(str.begin(), str.end());
    index = get_indexZero(sa);

    temp.push_back(-1);

    int size = temp.size();

    for (int i = 0; i < size; i++) {
        // Для каждого суффикса в суффиксном массиве берем символ перед ним
        int pos = sa[i];

        if (pos == 0) {
            // Если суффикс начинается с первого символа, берем последний символ строки
            {
                if (temp[size - 1] != -1)
                {
                    bwt.push_back(temp[size - 1]);
                    cout << char(temp[size - 1]) << " " << temp[size - 1] << " ";
                }
            }
        }
        else {
            // Иначе берем предыдущий символ
            {
                if (temp[pos - 1] != -1)
                {
                    bwt.push_back(temp[pos - 1]);
                    cout << char(temp[pos - 1]) << " " << temp[pos - 1] << " ";
                }
            }
        }
    }

    return bwt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<unsigned char> decodeBWT_SA_IS(vector<int>& bwt, vector<int>& index, int block_size)
{
    int size = bwt.size();
    vector<unsigned char> out(size);

    vector<int> P(block_size);
    int pos, start = -block_size, end;

    int i_size = index.size();
    ofstream file("out.txt");


    for (int s = 0; s < i_size; s++)
    {
        // номер оригинальной строки суффиксного массива
        pos = index[s];

        start += block_size;
        end = min(start + block_size, size);

        vector<int> extended_bwt(bwt.begin() + start, bwt.begin() + end);
        for (size_t i = 0; i < extended_bwt.size(); i++)
        {
            extended_bwt[i]++;
        }
        extended_bwt.insert(extended_bwt.begin() + index[s], 0);
        end++;

        int count[257]{};
        P.assign(end, 0);

        for (int p = 0, i = start; i < end; p++, i++)
        {// записываем где встретили текущий символ и записываем его количество
            int c = extended_bwt[i];
            P[p] = count[c];
            count[c]++;
        }

        for (int i = 0, sum = 0; i < 257; i++)
        {// Sort (buckets)
            sum += count[i];
            count[i] = sum - count[i];
        }

        for (int p = 0, i = start; i < end; p++, i++)
        {// номер в bucket
            P[p] += count[extended_bwt[i]];
        }

        cout << '\n';
        pos = P[pos];
        end--;
        for (int i = end - 1; i >= start; i--)
        {
            out[i] = extended_bwt[start + pos] - 1;
            pos = P[pos];
        }


        for (unsigned char num : out)
        {
            file << num;
        }
    }
    file.close();

    return out;
}

// encoder

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

vector<unsigned char> BWT_SA_IS(const vector<unsigned char>& data, vector<int>& index, int block_size)
{
    int size = data.size();

    vector<unsigned char> output;
    output.reserve(size);
    int countBlock = size / block_size + min(1, size % block_size);
    int start = -block_size, end;

    index.reserve(countBlock);

    for (int s = 0; s < countBlock; s++)
    {
        start += block_size;
        end = min(size, start + block_size);
        int b_size = end - start;

        if (Not_every_symbol_is_the_same(data, start, end))
        {// если все сиимволы не равны друг другу
            // скопируем блок
            vector<int> str(data.begin() + start, data.begin() + end);

            // сортировка SA-IS
            vector<int> sa = InducedSorting_SA(str, 256);

            // запись бвт строки и индексов оригиналов
            vector<int> bwt = sa_to_bwt(str, sa, index[s]);

            // вывод бвт строки
            for (unsigned char sym : bwt)
                output.push_back(sym);
        }
        else
        {
            index.push_back(0);
            for (int i = start; i < end; i++)
                output.push_back(data[i]);
        }
    }

    return output;
}


int main() {
    //string word = "banana$";
    //vector<int> sa_sorted = { 6,5,3,1,0,4,2 };
    // 
    //string word = "cabbage$";
    //vector<int> sa_sorted = { 7,1,4,3,2,0,6,5 };

    //vector<int> str = { 'c', 'a', 'b', 'b', 'a', 'g', 'e' };
    // sa будет содержать [7, 1, 4, 3, 2, 0, 6, 5]

    //vector<int> str = { 'b', 'a', 'n', 'a', 'n', 'a' };
    // sa будет содержать [6,5,3,1,0,4,2]
    //vector<int> sa = InducedSorting_SA(str, 256);
    //for (int num : sa)
    //    cout << num;
    //cout << '\n';

    unsigned char b;
    string str = "";
    ifstream encodeFile;
    if (true) {
        encodeFile.open("data.txt", ios::binary);
        if (!encodeFile) {
            cerr << "Не удалось открыть файл для чтения: compressed" << endl;
            return 1;
        }
        while (encodeFile.read(reinterpret_cast<char*>(&b), 1)) {
            str += b;
        }
    }
    encodeFile.close();

    int n = str.size();
    vector<int> sa(n);      // Суффиксный массив
    vector<int> c(n);       // Классы эквивалентности
    vector<int> tmp(n);     // Временный массив для сортировки
    vector<int> cnt(max(256, n));  // Для сортировки подсчётом

    // Инициализация: сортируем суффиксы по первому символу
    for (int i = 0; i < n; ++i)
        cnt[unsigned char(str[i])]++;
    for (int i = 1; i < 256; ++i)
        cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i)
        sa[--cnt[unsigned char(str[i])]] = i;

    // Строим классы эквивалентности
    c[sa[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
        if (str[sa[i]] != str[sa[i - 1]])
            classes++;
        c[sa[i]] = classes - 1;
    }

    // Итеративно сортируем по длине 2^k
    for (int k = 0; (1 << k) < n; ++k) {
        // Сдвигаем суффиксы на 2^k влево
        for (int i = 0; i < n; ++i)
            tmp[i] = (sa[i] - (1 << k) + n) % n;

        // Сортируем подсчётом по классам правой половины
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; ++i)
            cnt[c[tmp[i]]]++;
        for (int i = 1; i < classes; ++i)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i)
            sa[--cnt[c[tmp[i]]]] = tmp[i];

        // Обновляем классы эквивалентности
        tmp[sa[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; ++i) {
            // Сравниваем пары классов
            int cur1 = c[sa[i]], cur2 = c[(sa[i] + (1 << k)) % n];
            int prev1 = c[sa[i - 1]], prev2 = c[(sa[i - 1] + (1 << k)) % n];
            if (cur1 != prev1 || cur2 != prev2)
                classes++;
            tmp[sa[i]] = classes - 1;
        }
        c.swap(tmp);
    }
    sa.insert(sa.begin(), sa.size());

    cout << '\n';
    for (int num : sa)
    {
        cout << num << " ";
    }

    vector<int> data;
    data.reserve(str.length());
    for (unsigned char s: str)
    {
        data.push_back(s);
    }

    vector<int> index(1);
    ////
    vector<int> bwt_test = sa_to_bwt(data, sa, index[0]);// нужно как-то избавиться от '$' или добавить его при декодировании строки зная индекс оригинальной строки
    cout << "\n";

    auto original = decodeBWT_SA_IS(bwt_test, index, 2000000);

    cout << "\n\nDone!";
	return 0;
}