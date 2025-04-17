#include "header.h"

vector<unsigned char> LZ77(const vector<unsigned char>& data, const short BUFF_SIZE) {
	int pos = 0, size = data.size();

	vector<unsigned char> output;

	while (pos < size)
	{
		int start = max(0, pos - BUFF_SIZE);
		int end = min(BUFF_SIZE, min(size - pos, pos));
		int M_len = 0, back = 0;


		for (int i = start; i < pos; i++)
		{
			int len = 0;

			while (len < end && data[i + len] == data[pos + len])
			{
				len++;
			}

			if (M_len < len)
			{
				back = pos - i;
				M_len = len;
			}
		}

		if (BUFF_SIZE > 256)
		{
			output.push_back(back >> 8);
			output.push_back(back & 0b11111111);
			output.push_back(M_len >> 8);
			output.push_back(M_len & 0b11111111);
			pos += M_len;
			if (pos < size)
				output.push_back(data[pos++]);
		}
		else
		{
			output.push_back(back);
			output.push_back(M_len);
			pos += M_len;
			if (pos < size)
				output.push_back(data[pos++]);
		}
	}

	return output;
}

vector<unsigned char> decodeLZ77(const vector<unsigned char>& data, const short BUFF_SIZE)
{
	vector<unsigned char> output; 

	int size = data.size(), back, len, i = 0, pos;

	if (BUFF_SIZE > 256)
	{
		while (i < size)
		{
			back = data[i++] << 8;
			back |= data[i++];
			len = data[i++] << 8;
			len |= data[i++];
			pos = output.size();

			for (int j = 0; j < len; j++)
			{
				output.push_back(output[pos - back + j]);
			}

			if (i < size) output.push_back(data[i++]);
		}
	}
	else
	{
		while (i < size)
		{
			back = data[i++];
			len = data[i++];
			pos = output.size();

			for (int j = 0; j < len; j++)
			{
				output.push_back(output[pos - back + j]);
			}

			if (i < size) output.push_back(data[i++]);
		}
	}
	
	return output;
}