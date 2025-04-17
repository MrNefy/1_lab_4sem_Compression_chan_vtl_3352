#include "header.h"

vector<unsigned char> LZ78(const vector<unsigned char>& data) {
    unsigned short DICT_SIZE = 65535;
    vector<unsigned char> output;

    unordered_map<string, unsigned short> dictionary;  // �������: ����� -> �����
    unordered_map<unsigned short, string> reverse_dict; // �������: ����� -> ����� (��� �������� �������)
    unsigned short numD = 1, code; // ����� � �������
    string current;  // ������� �����

    for (unsigned char c : data)
    {
        if (numD == DICT_SIZE) {
            dictionary.clear();
            reverse_dict.clear();
            numD = 1;
        }

        string temp = current + (char)c;
        auto it = dictionary.find(temp);

        if (it != dictionary.end()) {
            current = temp;  // ����� ��� ���� � �������
        }
        else {
            code = 0;
            if (!current.empty()) {
                code = dictionary[current]; // ������� ����� ������ ��� �������
            }

            output.push_back(static_cast<unsigned char>(code & 0b11111111));  // ������� ����
            output.push_back(static_cast<unsigned char>(code >> 8));    // ������� ����
            output.push_back(c);

            dictionary[temp] = numD; // ���������� ������� ��� ������� ��������
            reverse_dict[numD] = temp; // ���������� ����� ������� ��� ������� ��������
            numD++;

            current.clear();
        }
    }

    // ��������� ���������� ������
    if (!current.empty()) {
        code = dictionary[current];
        output.push_back(static_cast<unsigned char>(code & 0b11111111));  // ������� ����
        output.push_back(static_cast<unsigned char>(code >> 8));    // ������� ����
    }

    return output;
}

vector<unsigned char> decodeLZ78(const vector<unsigned char>& data) {
    unsigned short DICT_SIZE = 65535;
    vector<unsigned char> output;
    unsigned size = data.size();
    output.reserve(size);
    unordered_map<unsigned short, string> dict; // dictionary - �������
    unsigned short numD = 1; // ����� � �������

    // �������������
    for (unsigned i = 0; i + 2 < size; i += 3) {
        LZ78code code;
        code.num = static_cast<unsigned short>(data[i] | data[i + 1] << 8);
        code.symbol = data[i + 2];

        if (code.num == 0) {
            // ������, �������� ��� � �������
            dict[numD] = string(1, code.symbol);
        }
        else {
            // ������ �� ������� + ����� ������
            dict[numD] = dict[code.num] + string(1, code.symbol);
        }

        // ��������� � �������� ������
        output.insert(output.end(), dict[numD].begin(), dict[numD].end());

        numD++;

        // ����� �������
        if (numD == DICT_SIZE)
        {
            dict.clear();
            numD = 1;
        }
    }
    if (size % 3 != 0) {
        // ��������� �����: 2 ����� ��� �������
        unsigned idxLastBytes = size - 2;
        unsigned short lastNum = static_cast<unsigned short>(data[idxLastBytes] | data[idxLastBytes + 1] << 8);
        output.insert(output.end(), dict[lastNum].begin(), dict[lastNum].end());
    }

    return output;
}