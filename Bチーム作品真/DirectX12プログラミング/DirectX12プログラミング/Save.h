#pragma once

class PlayData
{
public:
	// �p�X��ݒ�
	void SetPath(const char* path);

	// �Z�[�u
	void Save(int score);

	// ���[�h
	int Load();
private:
	
	// �������������ꕨ
	std::string m_path;

};