#include "header.h"

// по убыванию n log n
void quickSort(vector<Node*>& top, unsigned start, unsigned end) {
	if (end <= start) return;

	unsigned l = start, r = end;

	while (l != r)
	{
		while (l != r
			&& top[l]->count >= top[r]->count) r--;
		swap(top[l], top[r]);

		if (l == r) break;

		while (l != r
			&& top[l]->count > top[r]->count) l++;
		swap(top[l], top[r]);
	}

	if (r != start) quickSort(top, start, r - 1);
	if (l != end) quickSort(top, l + 1, end);
}
// O(n)
void buildCodeTable(Node* node, vector<bool> currentCode, unordered_map<unsigned char, vector<bool>>& codeTable) {
	if (!node) return;

	if (!node->left && !node->right) {
		codeTable[node->symbol] = currentCode;
		return;
	}

	currentCode.push_back(0);
	buildCodeTable(node->left, currentCode, codeTable);
	currentCode.back() = 1;
	buildCodeTable(node->right, currentCode, codeTable);
}

void tree(vector<Node*>& top) {
	short t_size = top.size();

	for (short i = static_cast<short>(t_size) - 1; t_size != 1; i--)
	{
		Node* node = new Node(top[i-1], top[i]);
		top.pop_back();
		top.pop_back();

		short idx = i - 2;
		while (idx > -1 && top[idx]->count < node->count)
		{
			idx--;
		}

		top.insert(top.begin() + ++idx, node);
		t_size--;
	}
}

vector<unsigned char> HA(const vector<unsigned char>& data) {
	vector<unsigned char> output;
	// проходимс€ по data считаем count
	vector<unsigned> dictionary(256, 0);

	for (unsigned char i : data)
	{
		dictionary[i]++;
	}

	// инциаци€ списка (количество символов)
	vector<Node*> top;
	size_t d_size = dictionary.size();

	for (unsigned short symbol = 0; symbol < d_size; symbol++)
	{
		if (dictionary[symbol] != 0)
		{
			Node* node = new Node(static_cast<unsigned char>(symbol), dictionary[symbol]);
			top.push_back(node);
		}
	}

	// удалили вектор
	vector<unsigned>().swap(dictionary);
	size_t t_size = top.size();

	quickSort(top, 0, static_cast<unsigned>(t_size) - 1);
	//registSort()

	// запись размера таблицы частот
	output.push_back(static_cast<unsigned char>(t_size) - 1);

	// запись таблицы частот
	for (short i = 0; i < static_cast<short>(t_size); i++)
	{
		output.push_back(top[i]->symbol);
		output.push_back(static_cast<unsigned char>(top[i]->count & 0b11111111));  // ћладший байт
		output.push_back(static_cast<unsigned char>(top[i]->count >> 8 & 0b11111111));  // —редний байт
		output.push_back(static_cast<unsigned char>(top[i]->count >> 16));  // —тарший байт
	}

	tree(top);

	// ѕредварительно строим таблицу кодов дл€ всех символов
	unordered_map<unsigned char, vector<bool>> codeTable;
	buildCodeTable(top[0], vector<bool>(), codeTable);
	delete top[0];

	// ѕодготовка к побитовой записи
	size_t size = data.size();
	uint8_t block = 0;
	short index = 7;

	for (size_t i = 0; i < size; i++) {
		const auto& code = codeTable[data[i]];

		for (bool bit : code) {
			block |= bit << index;
			if (--index == -1) {
				output.push_back(block);
				block = 0;
				index = 7;
			}
		}
	}

	// 1 символ принимает значение от 0-7, что означает что последние биты в количестве от 0 до 7 будут не проочитаны,так как не несут никакой информации.
	if (++index == 8)
	{
		output.emplace(output.begin(), 0);
	}
	else
	{
		output.push_back(block);
		output.emplace(output.begin(), index);
	}

	cout << "Done\n";
	return output;
}