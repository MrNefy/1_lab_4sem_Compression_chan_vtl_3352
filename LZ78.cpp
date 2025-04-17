#include "header.h"

vector<unsigned char> LZ78(const vector<unsigned char>& data) {
    unsigned short DICT_SIZE = 65535;
    vector<unsigned char> output;

    unordered_map<string, unsigned short> dictionary;  // словарь: слово -> номер
    unordered_map<unsigned short, string> reverse_dict; // словарь: номер -> слово (для быстрого доступа)
    unsigned short numD = 1, code; // номер в словаря
    string current;  // текущее слово

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
            current = temp;  // слово уже есть в словаре
        }
        else {
            code = 0;
            if (!current.empty()) {
                code = dictionary[current]; // быстрый поиск номера для вставки
            }

            output.push_back(static_cast<unsigned char>(code & 0b11111111));  // Младший байт
            output.push_back(static_cast<unsigned char>(code >> 8));    // Старший байт
            output.push_back(c);

            dictionary[temp] = numD; // сохранение символа или цепочки символов
            reverse_dict[numD] = temp; // сохранение номер символа или цепочки символов
            numD++;

            current.clear();
        }
    }

    // Обработка оставшихся данных
    if (!current.empty()) {
        code = dictionary[current];
        output.push_back(static_cast<unsigned char>(code & 0b11111111));  // Младший байт
        output.push_back(static_cast<unsigned char>(code >> 8));    // Старший байт
    }

    return output;
}

vector<unsigned char> decodeLZ78(const vector<unsigned char>& data) {
    unsigned short DICT_SIZE = 65535;
    vector<unsigned char> output;
    unsigned size = data.size();
    output.reserve(size);
    unordered_map<unsigned short, string> dict; // dictionary - словарь
    unsigned short numD = 1; // номер в словаре

    // декодирование
    for (unsigned i = 0; i + 2 < size; i += 3) {
        LZ78code code;
        code.num = static_cast<unsigned short>(data[i] | data[i + 1] << 8);
        code.symbol = data[i + 2];

        if (code.num == 0) {
            // Символ, которого нет в словаре
            dict[numD] = string(1, code.symbol);
        }
        else {
            // Строка из словаря + новый символ
            dict[numD] = dict[code.num] + string(1, code.symbol);
        }

        // Добавляем в выходные данные
        output.insert(output.end(), dict[numD].begin(), dict[numD].end());

        numD++;

        // Сброс словаря
        if (numD == DICT_SIZE)
        {
            dict.clear();
            numD = 1;
        }
    }
    if (size % 3 != 0) {
        // последние байты: 2 байта без символа
        unsigned idxLastBytes = size - 2;
        unsigned short lastNum = static_cast<unsigned short>(data[idxLastBytes] | data[idxLastBytes + 1] << 8);
        output.insert(output.end(), dict[lastNum].begin(), dict[lastNum].end());
    }

    return output;
}