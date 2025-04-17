#include "header.h"

vector<unsigned char> MTF(const vector<unsigned char>& data) {
    vector<unsigned char> output;
    output.reserve(data.size());
    size_t size = data.size();

    // ������������� ��������
    vector<unsigned char> alphabet;
    for (int i = 0; i < 256; i++) {
        alphabet.push_back(static_cast<unsigned char>(i));
    }

    for (unsigned i = 0; i < size; i++) {
        // ����� ������� � ��������
        auto it = find(alphabet.begin(), alphabet.end(), data[i]);
        unsigned index = distance(alphabet.begin(), it);

        // ������ �������
        output.push_back(index);

        // ����������� ������� � ������ ��������
        if (it != alphabet.begin()) {
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), data[i]);
        }
    }

    return output;
}

vector<unsigned char> decodeMTF(const vector<unsigned char>& data) {
    vector<unsigned char> output;

    // �������������� �������� ����� ������� �����
    vector<unsigned char> alphabet;
    for (int i = 0; i < 256; i++) {
        alphabet.push_back(static_cast<unsigned char>(i));
    }

    for (unsigned i = 0; i < data.size(); i++){
        // ����� ������� � �������� (�������� �����)
        unsigned index = static_cast<unsigned short>(data[i]);
        auto it = alphabet.begin() + index;
        unsigned char sym = *it;
        // ������ ������

        // ����������� ������� � ������ ��������
        if (it != alphabet.begin()) {
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), sym);
        }

        output.push_back(alphabet[0]);
    }

    return output;
}