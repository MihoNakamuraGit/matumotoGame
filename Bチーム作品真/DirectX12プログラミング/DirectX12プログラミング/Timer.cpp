#include<iostream>
#include<chrono>
#include"Timer.h"

int Timer()
{
	std::chrono::system_clock::time_point start, end;
	//�v�����ԊJ�n
	start = std::chrono::system_clock::now();
	//�v���I������
	end = std::chrono::system_clock::now();
	//�����ɗv��������
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}