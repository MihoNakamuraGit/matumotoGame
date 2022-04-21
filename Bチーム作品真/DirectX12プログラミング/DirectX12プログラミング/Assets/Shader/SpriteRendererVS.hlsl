#include "SpriteRenderer.hlsli"

// �t���[���萔�o�b�t�@
struct Frame
{
	matrix viewMatrix;	// �r���[�ϊ��s��
	matrix projMatrix;	// �v���W�F�N�V�����ϊ��s��
};
ConstantBuffer<Frame> cFrame : register(b0);


// �I�u�W�F�N�g�萔�o�b�t�@
struct Object
{
	matrix worldMatrix;		// ���[���h�ϊ��s��
	float4 tint;			// �F����
};
ConstantBuffer<Object> cObject : register(b1);


// ���͒��_
struct VSInput
{
	float2 positionOS   : POSITION;		// ���f����ԍ��W(x, y)
	float2 texcoord		: TEXCOORD;		// �e�N�X�`�����W(u, v)
};


//----------------------------------------------------------------------------------------------------------------------
// ���_�V�F�[�_�[�̃G���g���[�|�C���g�֐�
//----------------------------------------------------------------------------------------------------------------------
VSOutput main(in VSInput input)
{
	// 2�����̃I�u�W�F�N�g��ԍ��W��4�����Ɋg�� (������ z=0, w=1)
	const float4 positionOS = float4(input.positionOS, 0, 1);

	// ���W�ϊ�
	const float4 positionWS = mul(positionOS, cObject.worldMatrix);	// ���[���h��ԍ��W = ���f����ԍ��W �~ ���[���h�ϊ��s��
	const float4 positionVS = mul(positionWS, cFrame.viewMatrix);	// �r���[��ԍ��W = ���[���h��ԍ��W �~ �r���[�ϊ��s��
	const float4 positionCS = mul(positionVS, cFrame.projMatrix);	// �v���W�F�N�V������ԍ��W = �r���[��ԍ��W �~ �v���W�F�N�V�����ϊ��s��

	// �o�͗p�ϐ�
	VSOutput output = (VSOutput)0;
	output.positionCS = positionCS;
	output.color = cObject.tint;
	output.texcoord = input.texcoord;

	return output;
}

