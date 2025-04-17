#include "header.h"

vector<unsigned char> RLE(const vector<unsigned char>& data)
{
	int start = 0, i;
	vector<unsigned char> output;
	size_t size = data.size();

	while (start < size)
	{
		i = start + 1;

		if (i < size && data[start] != data[i])
		{
			for (i; i < size && data[i - 1] != data[i]; i++)
			{
				if (i - start == 129)
				{
					output.push_back(char(i - start + 126));
					while (start < i - 1) output.push_back(data[start++]);
				}
			}

			if (start < i - 1)
			{
				output.push_back(char(i - start + 126));
				while (start < i - 1) output.push_back(data[start++]);
			}
		}

		if (i < size)
		{
			while (i < size && data[i] == data[start])
			{
				if (i - start == 127)
				{
					output.push_back(char(i - start));output.push_back(data[start]); start = i + 1; i++;
				}
				i++;
			}

			if (start < size && start + 1 < i)
			{
				output.push_back(char(i - 1 - start)); output.push_back(data[start]);
				start = i;
			}
		}
		else if (start < size)
		{
			output.push_back(128); output.push_back(data[start++]);
		}
	}

	return output;
}

vector<unsigned char> decodeRLE(const vector<unsigned char>& data)
{
	int num, start = 0;
	vector<unsigned char> output;
	size_t size = data.size();

	while(start < size)
	{
		num = int(data[start++]);

		if (num < 128)
		{
			for (int end = 0; end <= num; end++)
			{
				output.push_back(data[start]);
			}
			start++;
		}
		else
		{
			for (int j = 128; j <= num; j++)
			{
				output.push_back(data[start++]);
			}
		}
	}

	return output;
}