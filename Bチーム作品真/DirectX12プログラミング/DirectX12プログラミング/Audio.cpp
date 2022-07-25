#include "Precompiled.h"
#include "Audio.h"

WORD OpenAudio(const char* path)
{
    MCI_OPEN_PARMS mciop;
    memset(&mciop, 0, sizeof(mciop));
    mciop.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
    mciop.lpstrElementName = path;

    MCIERROR mciError;
    mciError = mciSendCommand(
        0,
        MCI_OPEN,
        MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT,
        (DWORD_PTR)&mciop);

    if (mciError)
    {
        assert(0); // ���s
    }

    return mciop.wDeviceID;
}

void PlayAudio(WORD id)
{
    MCI_PLAY_PARMS mcipp;
    memset(&mcipp, 0, sizeof(mcipp));
    mcipp.dwFrom = 0;  // �Đ��J�n�ʒu
    mcipp.dwTo = 0;    // �Đ��I���ʒu (0�̏ꍇ�͑S��)
    mciSendCommand(id, MCI_PLAY, MCI_FROM, (DWORD_PTR)&mcipp);
}

void StopAudio(WORD id)
{
    mciSendCommand(id, MCI_STOP, 0, 0);
}

void CloseAudio(WORD id)
{
    mciSendCommand(id, MCI_CLOSE, 0, 0);
}
