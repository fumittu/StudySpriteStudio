#include "DxLib.h"
#include "SS5Player.h"

static int previousTime;
static int waitTime;
int mGameExec;
struct Touch
{
	int x;
	int y;
};
Touch touchPos;
#define WAIT_FRAME (16)

void init(void);
void update(float dt);
void draw(void);
void relese(void);

/// SS5�v���C���[
ss::Player *ssplayer;
ss::ResourceManager *resman;

/**s
* ���C���֐�
*/
int android_main(void)
{
	//DX���C�u�����̏�����
	SetGraphMode(720, 1280, 32);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);

	memset(&touchPos, 0, sizeof(touchPos));

	//���C�����[�v
	mGameExec = 1;
	previousTime = GetNowCount();

	// �v���C���[������
	init();

	while(mGameExec == 1) 
	{
		ClearDrawScreen();
		update((float)waitTime / 1000.0f);		//�Q�[���̍X�V
		draw();									//�Q�[���̕`��
		ScreenFlip();							//�`�挋�ʂ���ʂɔ��f

												//���̃t���[���܂ł̎��ԑ҂�
		waitTime = GetNowCount() - previousTime;
		previousTime = GetNowCount();

		if (waitTime < WAIT_FRAME) 
		{
			WaitTimer((WAIT_FRAME - waitTime));
		}
		else
		{
			if (ProcessMessage() == -1)
			{
				mGameExec = 0;
			}
		}
	}

	/// �v���C���[�I������
	relese();

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

void init(void)
{
	/**********************************************************************************

	SS�A�j���\���̃T���v���R�[�h
	Visual Studio Community�ADX���C�u�����œ�����m�F���Ă��܂��B
	ssbp��png������΍Đ����鎖���ł��܂����AResources�t�H���_��sspj���܂܂�Ă��܂��B

	**********************************************************************************/

	//���\�[�X�}�l�[�W���̍쐬
	resman = ss::ResourceManager::getInstance();
	//�v���C���[�̍쐬
	ssplayer = ss::Player::create();

	//�A�j���f�[�^�����\�[�X�ɒǉ�

	//���ꂼ��̃v���b�g�t�H�[���ɍ��킹���p�X�֕ύX���Ă��������B
	resman->addData("character_template_comipo/character_template1.ssbp");
	//�v���C���[�Ƀ��\�[�X�����蓖��
	ssplayer->setData("character_template1");        // ssbp�t�@�C�����i�g���q�s�v�j
													 //�Đ����郂�[�V������ݒ�
	ssplayer->play("character_template_3head/stance");				 // �A�j���[�V���������w��(ssae��/�A�j���[�V���������\�A�ڂ����͌�q)

																	 //�\���ʒu��ݒ�
	ssplayer->setPosition(1280 / 2, 300);
	//�X�P�[���ݒ�
	ssplayer->setScale(0.5f, 0.5f);
	//��]��ݒ�
	ssplayer->setRotation(0.0f, 0.0f, 0.0f);
	//�����x��ݒ�
	ssplayer->setAlpha(255);
	//���]��ݒ�
	ssplayer->setFlip(false, false);

}

//���C�����[�v
//Z�{�^���ŃA�j�����|�[�Y�A�ĊJ��؂�ւ��ł��܂��B
//�|�[�Y���͍��E�L�[�ōĐ�����t���[����ύX�ł��܂��B
static bool sstest_push = false;
static int sstest_count = 0;
static bool sstest_pause = false;
void update(float dt)
{
	char str[128];
	char str2[128];

	//�p�[�c��������W���̃X�e�[�g�̎擾���s���ꍇ��getPartState���g�p���܂��B
	ss::ResluteState result;
	ssplayer->getPartState(result, "body");

	//�擾���p�̕\��
	sprintf(str, "body = x:%f y:%f", result.x, result.y);
	DrawString(100, 120, str, GetColor(255, 255, 255));

	int touchNum = GetTouchInputNum();

	// �^�b�`����Ă���ӏ��̐������J��Ԃ�
	if(GetTouchInputNum() > 0)
	{
		// �^�b�`����Ă���ӏ��̍��W���擾
		GetTouchInput(0, &touchPos.x, &touchPos.y, NULL, NULL);
	}
	sprintf(str2, "touch = x:%d y:%d", touchPos.x, touchPos.y);
	DrawString(100, 150, str2, GetColor(255, 255, 255));

	sprintf(str2, "touchNum = %d", touchNum);
	DrawString(100, 170, str2, GetColor(255, 255, 255));

	//�L�[���͑���
	int animax = ssplayer->getMaxFrame();
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		mGameExec = 0;
	}

	//if (CheckHitKey(KEY_INPUT_Z))
	if(touchNum > 1)
	{
		if (sstest_push == false)
		{
			if (sstest_pause == false)
			{
				sstest_pause = true;
				sstest_count = ssplayer->getFrameNo();
				ssplayer->animePause();
			}
			else
			{
				sstest_pause = false;
				ssplayer->animeResume();
			}
		}
		sstest_push = true;

	}
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		if (sstest_push == false)
		{
			sstest_count += 20;
			if (sstest_count >= animax)
			{
				sstest_count = 0;
			}
		}
		sstest_push = true;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		if (sstest_push == false)
		{
			sstest_count -= 20;
			if (sstest_count < 0)
			{
				sstest_count = animax - 1;
			}
		}
		sstest_push = true;
	}
	//else if (CheckHitKey(KEY_INPUT_LEFT))
	else if((int)result.x < touchPos.x)
	{
		if (sstest_push == false)
		{
			sstest_count--;
			if (sstest_count < 0)
			{
				sstest_count = animax - 1;
			}
		}
		sstest_push = true;
	}
	//else if (CheckHitKey(KEY_INPUT_RIGHT))
	else if ((int)result.x > touchPos.x)
	{
		if (sstest_push == false)
		{
			sstest_count++;
			if (sstest_count >= animax)
			{
				sstest_count = 0;
			}
		}
		sstest_push = true;
	}
	else
	{
		sstest_push = false;
	}

	if (sstest_pause == true)
	{
		ssplayer->setFrameNo(sstest_count % animax);
	}
	else
	{
		sstest_count = ssplayer->getFrameNo();
	}

	//�A�j���[�V�����̃t���[����\��
	sprintf(str, "play:%d frame:%d drawCount:%d", (int)sstest_pause, sstest_count, ssplayer->getDrawSpriteCount());
	DrawString(100, 100, str, GetColor(255, 255, 255));

	//�v���C���[�̍X�V�A�����͑O��̍X�V��������o�߂�������
	ssplayer->update(dt);

}

//�`��
void draw(void)
{
	//�v���C���[�̕`��
	ssplayer->draw();
}

/**
* �v���C���[�I������
*/
void relese(void)
{

	//SS5Player�̍폜
	delete (ssplayer);
	delete (resman);
}