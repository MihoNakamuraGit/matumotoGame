#pragma once
#include<Windows.h>

// ���y�t�@�C�����J��
// �p�X�Ŏ���
WORD OpenAudio(const char* path);

// ���y���J��
void PlayAudio(WORD deviceID);

// ���y���ꎞ��~����
void StopAudio(WORD deviceID);

// ���y�����
void closeAudio(WORD deviceID);