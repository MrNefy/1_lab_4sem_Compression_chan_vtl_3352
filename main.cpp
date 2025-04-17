#include "header.h"

int main() {
    ifstream ifT;
    ofstream ofT;
    ofstream console;
    ofstream compression;

    SetConsoleOutputCP(CP_UTF8);

    vector<unsigned char> data;
    vector<unsigned char> output;
    vector<int> index;
    unsigned char byte;
    unsigned size;

    //№ Combo ^-^
    //100 BWT + MTF
    // HA
    // RLE
    // BWT + RLE
    // BWT + MTF + HA
    // BWT + MTF + RLE + HA
    // LZ77
    // LZ77 + HA
    // LZ78
    // LZ78 + HA

    string combo = "BWT + MTF + HA";
    int mode = 2;
    string name = "black_white.raw";

    const int bwt = 10000;// размер блока
    const short lz77 = 10000;// размер буффера

    // enwik7.txt
    // Bible.txt
    // notepad++.exe
    // black_white.raw
    // grey.raw
    // fone.raw

    size_t dot_pos = name.rfind('.');
    string file = name.substr(0, dot_pos);
    string ext = name.substr(dot_pos);

    // if mode == 1 this off                        // .txt .exe .raw
    string save_exp = ext == ".exe" ? ".bin" : ext; // .txt .bin .raw

    string com = (mode == 1) ? "compression.txt" : "C:/Users/lin/Desktop/4 семестр/АиСД 4сем/Лаба 1 Сжатие/compress_" + file + '/' + combo + "+Encoded" + save_exp;
    string Tof = (mode == 1) ? "e7out.txt" : "C:/Users/lin/Desktop/4 семестр/АиСД 4сем/Лаба 1 Сжатие/compress_" + file + '/' + combo + "+Decoded" + save_exp;

    if (true)
    {
        ifT.open(name, ios::binary);
        if (!ifT) {
            cerr << "Не удалось открыть файл для чтения: " << name << endl;
            return 1;
        }
        while (ifT.read(reinterpret_cast<char*>(&byte), 1)) {
            data.push_back(byte);
        }
        ifT.close();

        compression.open(com, ios::binary);
        if (!compression) {
            cerr << "Не удалось открыть файл для чтения: " << name << endl;
            return 1;
        }
        ofT.open(Tof, ios::binary);
        if (!ofT) {
            cerr << "Не удалось открыть файл для чтения: " << name << endl;
            return 1;
        }
        console.open("console.txt");
        if (!console) {
            cerr << "Не удалось открыть файл для чтения: " << name << endl;
            return 1;
        }
    }

    ////// code

    //Combo ^-^
    {
        if (combo == "BWT + MTF")
        {
            output = BWT(data, index, bwt);
            output = MTF(output);
        }
        if (combo == "HA") output = HA(data);
        if (combo == "RLE") output = RLE(data);
        if (combo == "BWT + RLE") {
            output = BWT(data, index, bwt);
            output = RLE(output);
        }
        if (combo == "BWT + MTF + HA")
        {
            output = BWT(data, index, bwt);
            output = MTF(output);
            output = HA(output);
        }
        if (combo == "BWT + MTF + RLE + HA")
        {
            output = BWT(data, index, bwt);
            output = MTF(output);
            output = RLE(output);
            output = HA(output);
        }
        if (combo == "LZ77") output = LZ77(data, lz77);
        if (combo == "LZ77 + HA")
        {
            output = LZ77(data, lz77);
            output = HA(output);
        }
        if (combo == "LZ78") output = LZ78(data);
        if (combo == "LZ78 + HA")
        {
            output = LZ78(data);
            output = HA(output);
        }
    }

    if (true) {
        size = 0;
        for (unsigned char c : output) {
            compression.put(c);
            size++; // размер файла
        }
        console << "Original data: " << data.size() << " bytes\n\n";
        console << "Encoded file: " << size << " bytes\n\n";

        compression.close();
        vector <unsigned char>().swap(data);
        vector <unsigned char>().swap(output);
    }

    ////// decode

    ifstream encodeFile;
    if (true) {
        encodeFile.open(com, ios::binary);
        if (!encodeFile) {
            cerr << "Не удалось открыть файл для чтения: " << name << endl;
            return 1;
        }
        while (encodeFile.read(reinterpret_cast<char*>(&byte), 1)) {
            data.push_back(byte);
        }
    }

    // Combo ^-^
    {
        if (combo == "BWT + MTF")
        {
            output = decodeMTF(data);
            output = decodeBWT(output, index, bwt);
            //output = decodeBWT(data, index, bwt);
        }
        if (combo == "HA") output = decodeHA(data);
        if (combo == "RLE") output = decodeRLE(data);
        if (combo == "BWT + RLE") {
            output = decodeRLE(data);
            output = decodeBWT(output, index, bwt);
        }
        if (combo == "BWT + MTF + HA")
        {
            output = decodeHA(data);
            output = decodeMTF(output);
            output = decodeBWT(output, index, bwt);
        }
        if (combo == "BWT + MTF + RLE + HA")
        {
            output = decodeHA(data);
            output = decodeRLE(output);
            output = decodeMTF(output);
            output = decodeBWT(output, index, bwt);
        }
        if (combo == "LZ77") output = decodeLZ77(data, lz77);
        if (combo == "LZ77 + HA")
        {
            output = decodeHA(data);
            output = decodeLZ77(output, lz77);
        }
        if (combo == "LZ78") output = decodeLZ78(data);
        if (combo == "LZ78 + HA")
        {
            output = decodeHA(data);
            output = decodeLZ78(output);
        }
        if (true) {
            size = 0;
            for (unsigned char c : output) {
                ofT.put(c);
                size++; // размер файла
            }
            console << "Decoded file: " << size << " bytes";
        }
    }

    ofT.close();
    encodeFile.close();
    return 0;
}