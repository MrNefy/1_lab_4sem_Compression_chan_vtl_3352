#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
using namespace std;

// BWT

vector<unsigned char> BWT(const vector<unsigned char>& data, vector<int>& index, int block_size);
vector<unsigned char> decodeBWT(const vector<unsigned char>& data, vector<int>& index, int block_size);

//Узел дерева Хаффмана
struct Node {
	unsigned char symbol;
	int count;
	Node* left;
	Node* right;

	Node(unsigned char symbol, unsigned count)
		: symbol(symbol), count(count), left(nullptr), right(nullptr) {}

	Node(Node* Left, Node* Right)
		: symbol(0), count(Left->count + Right->count), left(Left), right(Right) {}

	~Node() {
		delete left;   // Рекурсивное удаление левого поддерева
		delete right;  // Рекурсивное удаление правого поддерева
	}
};

// Алгоритм Хаффмана (HA)

void tree(vector<Node*>& top);
vector<unsigned char> HA(const vector<unsigned char>& data);
vector<unsigned char> decodeHA(const vector<unsigned char>& data);

// RLE

vector<unsigned char> RLE(const vector<unsigned char>& data);
vector<unsigned char> decodeRLE(const vector<unsigned char>& data);

// MTF

vector<unsigned char> MTF(const vector<unsigned char>& data);
vector<unsigned char> decodeMTF(const vector<unsigned char>& data);

// LZ family

struct LZ78code {
	unsigned short num;
	unsigned char symbol;
};

vector<unsigned char> LZ77(const vector<unsigned char>& data, const short BUFF_SIZE);
vector<unsigned char> decodeLZ77(const vector<unsigned char>& data, const short BUFF_SIZE);

vector<unsigned char> LZ78(const vector<unsigned char>& data);
vector<unsigned char> decodeLZ78(const vector<unsigned char>& data);
