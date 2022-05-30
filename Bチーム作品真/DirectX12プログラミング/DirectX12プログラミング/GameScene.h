#pragma once

#include<vector>

namespace MatumotoGame
{
	

	// �v���C���[�R���g���[���[
	class GameScene : public Scene
	{
	public:
		static const uint32_t MaxNumPlayers = 1;

	private:
		GameObject* m_sceneRoot;
		Camera* m_mainCameraComponent;
		GameObject* m_object1;
		GameObject* m_object2;

		// �Q�[���̐i�s��\���񋓌^
		enum  class State
		{
			waitforClick,    // �N���b�N�҂�
			controllable,    // �v���C���[����\���

			
		};

	private:
		
		
		friend class Scene;	      		// �V�[���N���X�͗F�B
		friend class GameObject;		// �Q�[���I�u�W�F�N�g�N���X�͗F�B

	public:
		// �R���X�g���N�^
		GameScene() = default;

		// ���z�f�X�g���N�^
		virtual ~GameScene() = default;

		// Scene::LoadAssets()���I�[�o�[���C�h
		void LoadAssets() override;

		// Scene::Update()���I�[�o�[���C�h
		void Update() override;

		// Scene::Render()���I�[�o�[���C�h
		void Render() override;

		// Scene::UnloadAssets()���I�[�o�[���C�h
		void UnloadAssets() override;

	private:
		// �J�������쐬���܂��B
		void CreateMainCamera(Transform* parent);

		// �w�i���쐬���܂��B
		void CreateBackground(Transform* parent);



	};
}
