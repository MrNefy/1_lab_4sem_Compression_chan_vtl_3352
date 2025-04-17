#include "header.h"

vector<unsigned char> decodeHA(const vector<unsigned char>& data) {
	vector<unsigned char> output;
	vector<Node*> top(static_cast<size_t>(data[1]) + 1);

	// ������ ������
	unsigned t_size = static_cast<unsigned>(top.size());

	// ������ ������ ��������
	for (unsigned i = 0, p = 2; i < t_size; i++, p += 4)
	{
		top[i] = new Node(data[p], data[p + 1] | (data[p + 2] << 8) | (data[p + 3] << 16));
		cout << top[i]->count << ' ' << (int)top[i]->symbol << '\n';
	}// ������� ���� � ������� ����

	// decode
	tree(top);

	// ���������� � ��������� ������
	unsigned size = static_cast<unsigned>(data.size());
	Node* node = top[0];

	// ������
	short zeroBits;
	for (unsigned i = 2 + t_size * 4; i < size; i++)
	{
		if (i == size - 1)
			zeroBits = data[0];
		else
			zeroBits = 0;

		for (short bit = 7; bit >= zeroBits; bit--) {
			if (node)
			{
				if (node->left == nullptr && node->right == nullptr)
				{// �������!!!
					output.push_back(node->symbol);
					node = top[0];
				}

				node = (data[i] >> bit) & 1 ? node->right : node->left;
			}
		}
	}

	if (node && node->left == nullptr && node->right == nullptr)
	{// ��������� ������ (����� node �������� � ��������� ����, �� �� ������� ������, ������� ������� ���)
		output.push_back(node->symbol);
	}

	delete top[0];
	cout << "Done\n";
	return output;
}