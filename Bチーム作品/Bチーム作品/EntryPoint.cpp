#include<Windows.h>
#include<process.h>
#include<tchar.h>
#include<d3d12.h>
#include <dxgi1_6.h>					// DXGI (Microsoft DirectX Graphics Infrastructure�̗�) �o�[�W����1.6
#include <d3dcompiler.h>				// �V�F�[�_�[�t�@�C���̃R���p�C���@�\
#include <DirectXMath.h>				// DirectX�n�̐��w���C�u����

#include<assert.h>
#include<stdio.h>

#pragma comment(lib, "winmm.lib")           // timeGetTime()�ׂ̈ɕK�v


//---------------------------------------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ� (�萔)
// 
// �萔�̏ꍇ�͕K���uconst�v�܂��́uconstexpr�v��t���悤�B
//---------------------------------------------------------------------------------------------------------------------------------------------

static const unsigned int GameScreenResolutionWidth = 1920;
static const unsigned int GameScreenResolutionHeight = 1080;



LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // ���b�Z�[�W�̎�ނŕ���
    switch (message)
    {
        // �E�B���h�E�������悤�Ƃ��Ă��鎞�̏���
    case WM_DESTROY:
    {
        // ���̃A�v�����I��������
        PostQuitMessage(0);
    }
    break;

    // �����̂Ȃ����b�Z�[�W�� DefWindowProc�֐��Ɋۓ�������B
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow) {
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // �E�B���h�E�N���X�̓o�^
    // 
    //  �E�E�B���h�E���쐬����O�ɃE�B���h�E�N���X��OS(Windows)�ɓo�^���Ă����K�v������B
    //  �E�E�B���h�E�N���X�́u�E�B���h�E�̐����v�̂��ƁB
    //  �ERegisterClassEx�֐����Ăяo�����ƂŃE�B���h�E�N���X��o�^���邱�Ƃ��ł���B
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));                             // ���ߖY�ꂪ�|���̂ōŏ��ɑS��0�Ŗ��߂�
    wcex.cbSize = sizeof(wcex);                                 // ���̕ϐ����g�̃T�C�Y(�P�ʂ̓o�C�g)
    wcex.cbClsExtra = 0;                                        // �E�B���h�E�N���X�p�̊g���������T�C�Y
    wcex.cbWndExtra = 0;                                        // �E�B���h�E�p�̊g���������T�C�Y
    wcex.style = CS_HREDRAW | CS_VREDRAW;                       // �ĕ`��t���O
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // �E�B���h�E�̔w�i��h��Ԃ��ׂ̃u���V
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);              // �E�B���h�E��ŕ\�������J�[�\��
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);            // �A�C�R��
    wcex.hIconSm = nullptr;                                     // ���A�C�R��
    wcex.lpszMenuName = nullptr;                                // ���j���[��
    wcex.lpszClassName = _T("B�`�[��");             // �E�B���h�E�N���X�� (���ł��悢)
    wcex.lpfnWndProc = WndProc;                                 // �E�B���h�E�v���V�[�W���֐��̃A�h���X
    wcex.hInstance = hInstance;                                 // �A�v���P�[�V�����C���X�^���X�����ʂ��鐮���l

    if (!RegisterClassEx(&wcex))
    {
        printf("[���s] �E�B���h�E�N���X�̓o�^\n");
        assert(0);
    }
    printf("[����] �E�B���h�E�N���X�̓o�^ (�E�B���h�E�N���X��: %s)\n", wcex.lpszClassName);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // �E�B���h�E�X�^�C��
    // 
    //   �E�E�B���h�E�̊O�ς�@�\�����߂�ׂ̒l�B
    //   �E�r�b�g�t���O�Ȃ̂ŕ����w�肷�邱�Ƃ��ł���B
    // 
    //          +------------------------+-----------------+------------------------------------------+
    //          |     �X�^�C����         |     16�i�l      |        �Ӗ�                              |
    //          +------------------------+-----------------+------------------------------------------+
    //          |   WS_OVERLAPPED        |   0x00000000    |    �^�C�g���o�[�Ƌ��E��������            |
    //          |   WS_CAPTION           |   0x00C00000    |    �^�C�g���o�[������                    |
    //          |   WS_SYSMENU           |   0x00080000    |    �^�C�g���o�[��Ƀ��j���[������        |
    //          |   WS_THICKFRAME        |   0x00040000    |    �T�C�Y�ύX�g������                    |
    //          |   WS_MINIMIZEBOX       |   0x00020000    |    �ŏ����{�^��������                    |
    //          |   WS_MAXIMIZEBOX       |   0x00010000    |    �ő剻�{�^��������                    |
    //          |   WS_OVERLAPPEDWINDOW  |   0x00CF0000    |    ��L�S�Ă̕����X�^�C��                |
    //          +------------------------+-----------------+------------------------------------------+
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    const unsigned int windowStyle = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // �w�肵���u�N���C���g�̈�T�C�Y�v����Ɂu�E�B���h�E�T�C�Y�v���v�Z����B
    //
    //    �E�N���C�A���g�̈�͎��ۂɃQ�[����ʂ��\�������ꏊ�Ȃ̂ŃQ�[����ʉ𑜓x�ƈ�v�����悤�B
    //    �E�E�B���h�E�T�C�Y = �N���C�A���g�̈�T�C�Y + �T�C�Y�ύX�g��
    //    �EAdjustWindowRect�֐��Ŏ����I�Ɍv�Z���Ă���� (�֗��I)
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    RECT rect;
    rect.left = 0;
    rect.right = GameScreenResolutionWidth;
    rect.top = 0;
    rect.bottom = GameScreenResolutionHeight;
    AdjustWindowRect(&rect, windowStyle, FALSE);

    // �E�B���h�E�̉��� = �E�ӂ�X���W - ���ӂ�X���W
    const int windowWidth = rect.right - rect.left;

    // �E�B���h�E�̍��� = ���ӂ�Y���W - ��ӂ�Y���W
    const int windowHeight = rect.bottom - rect.top;


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // �E�B���h�E���f�X�N�g�b�v��ʂ̒����ɔz�u�������B
    // 
    // �u�X�N���[���T�C�Y�v�Ɓu�E�B���h�E�T�C�Y�v��������Β����ɔz�u(�Z���^�����O)���邱�Ƃ��ł���B
    //   �X�N���[���T�C�Y�̓f�X�N�g�b�v��ʂ̉𑜓x�̂��Ƃ�GetSystemMetrics�֐����g���Ď擾���邱�Ƃ��ł���B
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------

    // �X�N���[���T�C�Y���擾
    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // �Z���^�����O���ꂽ�E�B���h�E�̈ʒu = (�X�N���[���T�C�Y - �E�B���h�E�T�C�Y) / 2
    //const int windowPosX = (screenWidth - windowWidth) / 2;
    //const int windowPosY = (screenHeight - windowHeight) / 2;
    const int windowPosX = 0;
    const int windowPosY = 0;

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // �E�B���h�E�̍쐬
    // 
    //      ��1���� : [in] �g���E�B���h�E�X�^�C�� 
    //      ��2���� : [in] �E�B���h�E�N���X��
    //      ��3���� : [in] �E�B���h�E��
    //      ��4���� : [in] �E�B���h�E�X�^�C��
    //      ��5���� : [in] �E�B���h�E������̏����ʒu(X���W)
    //      ��6���� : [in] �E�B���h�E������̏����ʒu(Y���W)
    //      ��7���� : [in] �E�B���h�E�̉��� (�P�ʂ̓s�N�Z��)
    //      ��8���� : [in] �E�B���h�E�̍��� (�P�ʂ̓s�N�Z��)
    //      ��9���� : [in] �e�E�B���h�E�n���h�� (�e�E�B���h�E�������ꍇ�� nullptr)
    //      ��1���� : [in] ���j���[�n���h�� (���j���[�������ꍇ�� nullptr)
    //     ��10���� : [in] ���̃A�v���P�[�V�����C���X�^���X�����ʂ���ׂ̃��j�[�N�Ȑ����l
    //     ��11���� : [in] WM_CREATE���b�Z�[�W����������ۂɓn����郆�[�U�[��`�̒l
    //       �߂�l : �E�B���h�E�n���h�� (�E�B���h�E�����ʂ���ׂ̃��j�[�N�Ȑ����l)
    //                �������A�쐬�Ɏ��s�����ꍇ�� nullptr�B
    //---------------------------------------------------------------------------------------------------------------------------------------------
    HWND hWnd = CreateWindowEx(0, wcex.lpszClassName, _T("B�`�[����i"), windowStyle, windowPosX, windowPosY, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        printf("[���s] �E�B���h�E�̍쐬\n");
        return false;
    }
    //printf("[����] �E�B���h�E�̍쐬 (�E�B���h�E�n���h��:%x)\n", (unsigned int)hWnd);


        // �E�B���h�E������ԂɕύX����
    ::ShowWindow(hWnd, SW_SHOW);

    // �^�C�}�[����\���ŏ��ɂ���
    timeBeginPeriod(1);

    // �Q�[���J�n���O�̎��Ԃ��擾����
    DWORD previousFrameTime = timeGetTime();


    MSG msg;
    while (1)
    {
        //---------------------------------------------------------------------------------------------------------------------------------------------
        // ���b�Z�[�W�|���v
        //      Windows�A�v���͂�������Ȃ��ƃE�B���h�E���ł܂�B
        //---------------------------------------------------------------------------------------------------------------------------------------------
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // ���b�Z�[�W���������ꍇ�̓E�B���h�E�v���V�[�W���ɔz������B
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // �A�v���I�����b�Z�[�W�������ꍇ�� while ���甲����
            if (msg.message == WM_QUIT)
                break;
        }


        //---------------------------------------------------------------------------------------------------------------------------------------------
        // ���Ԃ��v������FPS���Œ肷��
        //---------------------------------------------------------------------------------------------------------------------------------------------

        // 1�b�Ԃ�����̃t���[���� (�ڕW�l)
        static const DWORD TargetFPS = 60;

        // 1�t���[��������̎��� (�P�ʂ̓~���b)
        static const DWORD MillisecondsPerFrame = 1000 / TargetFPS;

        // ���݂̎��Ԃ��擾���� (�P�ʂ̓~���b)
        const DWORD currentTime = timeGetTime();

        // �o�ߎ��� = ���݂̎��� - �O��̎���
        const DWORD elapsedTime = currentTime - previousFrameTime;

        // (   �o�ߎ��� >= 1�t���[��������̎���)
        if (elapsedTime >= MillisecondsPerFrame)
        {
            // ���t���[���ׂ̈Ƀt���[���J�n���Ԃ��X�V���Ă���
            previousFrameTime = currentTime;

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // �t���[�������̊J�n
            //     �Q�[�����̎��Ԃ�(1/TargetFPS)�b�������i�߂�B
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // �L�[�{�[�h�̑S�ẴL�[���͏�Ԃ��Č��o
            //Keyboard::Update();

            // �i�߂�ׂ��������ԇ�t
            const float deltaTime = 1.0f / TargetFPS;

            // �V�[���X�V
           //SceneManager::GetActiveScene()->Update();

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // �Q�[����ʂ������_�����O����B
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // �t���[�����\�[�X�Z�b�g�̎擾
            //FrameResources* currentFrameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

            // �R�}���h���X�g�̎擾
           // ID3D12GraphicsCommandList* currentCommandList = currentFrameResources->GetCommandList();

            // �u�p�C�v���C���X�e�[�g�I�u�W�F�N�g��ݒ�v�R�}���h���R�}���h���X�g�ɒǉ�����B
          //  currentCommandList->SetPipelineState(d3d12PipelineState);

            // �u�O���t�B�b�N�X�p�C�v���C���p���[�g�V�O�l�`����ݒ�v�R�}���h���R�}���h���X�g�ɒǉ�����B
          //  currentCommandList->SetGraphicsRootSignature(d3d12RootSignature);

            // ���\�[�X�o���A�ڍ׏��
           //D3D12_RESOURCE_BARRIER resourceBarrier;
           //memset(&resourceBarrier, 0, sizeof(resourceBarrier));
           //
           // �u���\�[�X�o���A�v�R�}���h���R�}���h���X�g�ɒǉ�����B
           // 
           //  �����_�[�^�[�Q�b�g(�o�b�N�o�b�t�@)�̃��\�[�X��Ԃ�J�ڂ�����B
           // �y�v���[���g�\��ԁz�ˁy�����_�[�^�[�Q�b�g��ԁz
           //resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
           //resourceBarrier.Transition.pResource = currentFrameResources->GetBackBuffer();
           //resourceBarrier.Transition.Subresource = 0;
           //resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
           //resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
           //resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
           // currentCommandList->ResourceBarrier(1, &resourceBarrier);

            // �V�[���̃����_�����O
            //SceneManager::GetActiveScene()->Render();

            // �u���\�[�X�o���A�v�R�}���h���R�}���h���X�g�ɒǉ�����B
            // 
            //  �����_�[�^�[�Q�b�g(�o�b�N�o�b�t�@)�̃��\�[�X��Ԃ�J�ڂ�����B
            // �y�����_�[�^�[�Q�b�g��ԁz�ˁy�v���[���g�\��ԁz
            //resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            //resourceBarrier.Transition.pResource = currentFrameResources->GetBackBuffer();
            //resourceBarrier.Transition.Subresource = 0;
            //resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
            //resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
            //resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
           // currentCommandList->ResourceBarrier(1, &resourceBarrier);

            // �R�}���h���X�g�����B
            //if (FAILED(currentCommandList->Close()))
            //{
            //    assert(0);
            //}

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // �R�}���h���X�g���̃R�}���h�����߂���GPU�R�}���h�𔭍s����B
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // �R�}���h�L���[���擾
            //ID3D12CommandQueue* commandQueue = GraphicsEngine::Instance().GetDefaultCommandQueue();

            // �R�}���h���X�g�z����R�}���h�L���[�ɓn����GPU�ɃR�}���h�𑗐M����
          //ID3D12CommandList* const commandLists[] =
          //{
          //    currentCommandList,
          //};
          //commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

            // �t�F���X���g����GPU���̊�����҂B
            //currentFrameResources->WaitForCompletion(commandQueue);

            // �R�}���h�A���P�[�^�[�̃��Z�b�g
            //ID3D12CommandAllocator* currentCommandAllocator = currentFrameResources->GetCommandAllocator();
            //if (FAILED(currentCommandAllocator->Reset()))
            //{
            //    assert(0);
            //}
            //
            // �R�}���h���X�g�̃��Z�b�g
            //if (FAILED(currentCommandList->Reset(currentCommandAllocator, nullptr)))
            //{
            //    assert(0);
            //}

            // ���݂̃o�b�N�o�b�t�@���t�����g�o�b�t�@�Ƃ��A�f�B�X�v���C�ւ̓]�����J�n����B
           // GraphicsEngine::Instance().Present();

            //�Ă���

        }
    }
}

