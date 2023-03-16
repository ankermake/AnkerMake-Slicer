#ifndef CCGLOBAL_SERIAL_H
#define CCGLOBAL_SERIAL_H
#include <iostream>
#include <fstream>
#include <string>
#include <functional>

typedef std::function<void(std::fstream& stream)> serialFunc;
void serialLoad(const std::string& file, serialFunc func)
{
	std::fstream in(file, std::ios::in | std::ios::binary);
	if (in.is_open())
	{
		func(in);
	}

	in.close();
}

void serialSave(const std::string& file, serialFunc func)
{
	std::fstream out(file, std::ios::out | std::ios::binary);
	if (func && out.is_open())
	{
		func(out);
	}

	out.close();
}

template<class T>
void loadT(std::fstream& in, T& t)
{
	in.read((char*)&t, sizeof(T));
}

template<class T>
void saveT(std::fstream& out, T& t)
{
	out.write((const char*)&t, sizeof(T));
}

template<class T>
void loadVectorT(std::fstream& in, std::vector<T>& vecs)
{
	int num = 0;
	loadT(in, num);
	if (num > 0)
	{
		vecs.resize(num);
		in.read((char*)&vecs.at(0), num * sizeof(T));
	}
}

template<class T>
void saveVectorT(std::fstream& out, std::vector<T>& vecs)
{
	int num = (int)vecs.size();
	saveT(out, num);
	if (num > 0)
		out.write((const char*)&vecs.at(0), num * sizeof(T));
}

#endif // CCGLOBAL_SERIAL_H
