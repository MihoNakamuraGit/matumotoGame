#include "Precompiled.h"
#include "GameScene.h"

void MatumotoGame::GameScene::LoadAssets()
{
	// �V�[�����[�g�̃Q�[���I�u�W�F�N�g���쐬
	m_sceneRoot = new GameObject("�V�[�����[�g");

    // ���C���J�������쐬
    CreateMainCamera(m_sceneRoot->GetTransform());

    // �w�i���쐬
    CreateBackground(m_sceneRoot->GetTransform());

   
}

void MatumotoGame::GameScene::CreateMainCamera(Transform* parent)
{
    // ���C���J�����p�̃Q�[���I�u�W�F�N�g���쐬
    GameObject* mainCamera = new GameObject("���C���J����");

    // �J�����R���|�[�l���g��ǉ�����
    m_mainCameraComponent = mainCamera->AddComponent<Camera>();
    m_mainCameraComponent->SetOrthographic(true);
    m_mainCameraComponent->SetOrthographicSize(1080 / 2);
    m_mainCameraComponent->SetAspect(1920 / 1080.0f);
    m_mainCameraComponent->SetNearClipPlane(0.3f);
    m_mainCameraComponent->SetFarClipPlane(1000.0f);
    m_mainCameraComponent->SetClearFlags(CameraClearFlags::SolidColor);
    m_mainCameraComponent->SetBackgroundColor(Color::CornflowerBlue);
    m_mainCameraComponent->SetNormalizedViewportRect(Rect(0.0f, 0.0f, 1.0f, 1.0f));

    // �ʒu�̕ύX
    Transform* transform = mainCamera->GetTransform();
    transform->SetParent(parent);
    transform->SetLocalPosition(1920 / 2, 1080 / 2, -10);
}

void MatumotoGame::GameScene::CreateBackground(Transform* parent)
{
    // �w�i1����
    Texture2D* background01Texture = Texture2D::FromFile(L"Assets/Stage(B)/WindowsXP.jpg");
    GameObject::CreateWithSprite("�w�i�㕔", background01Texture, Rect(0, 0, 1920, 1080), Vector2(0.0f, 0.0f), 1.0f, Vector3(0, 0, 100), parent);
    // �w�i2����
    Texture2D* background02Texture = Texture2D::FromFile(L"Assets/Stage(B)/WindowsXP - �R�s�[.jpg");
    GameObject::CreateWithSprite("�w�i�㕔", background02Texture, Rect(0, 0, 1920, 1080), Vector2(0.0f, 0.0f), 1.0f, Vector3(1920, 0, 100), parent);

}


void MatumotoGame::GameScene::Update()
{
    // �ʒu�̕ύX
    Transform* transform = m_mainCameraComponent->GetTransform();
    DirectX::XMFLOAT3 cameraPos = transform->GetLocalPosition();
    cameraPos.x += 5;

    // 
    transform->SetLocalPosition(cameraPos);

    // ���J�������ǂ����ڂ��Ă��邩�����߁A�摜���ړ�������
    int a = (int)(cameraPos.x / 1920);

    

    Scene::Update();
}

void MatumotoGame::GameScene::Render()
{
    Scene::Render();
}

void MatumotoGame::GameScene::UnloadAssets()
{

}

