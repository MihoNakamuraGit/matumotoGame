#include "SpriteRenderer.hlsli"

// �f�J�[���e�N�X�`�� (�V�F�[�_�[���\�[�X���W�X�^0�ԂɃo�C���h)
Texture2D<float4> _MainTex : register(t0);

// �T���v���[�X�e�[�g (�T���v���[���W�X�^0�ԂɃo�C���h)
SamplerState _MainTexSampler : register(s0);

// �o�̓s�N�Z��
struct PSOutput
{
	float4 target0   : SV_TARGET0;		// �����_�[�^�[�Q�b�g[0]�ւ̏o�͒l
};


//----------------------------------------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�֐�
//----------------------------------------------------------------------------------------------------------------------
PSOutput main(in VSOutput input)
{
	// �e�N�X�`�����̎w�肵���e�N�X�`�����W�̐F�𒊏o(�T���v�����O)����
	const float4 texelColor = _MainTex.Sample(_MainTexSampler, input.texcoord);

	// �o�͗p�ϐ�
	PSOutput output = (PSOutput)0;
	output.target0 = texelColor * input.color;
	return output;
}


