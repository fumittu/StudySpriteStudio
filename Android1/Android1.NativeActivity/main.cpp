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

/// SS5プレイヤー
ss::Player *ssplayer;
ss::ResourceManager *resman;

/**s
* メイン関数
*/
int android_main(void)
{
	//DXライブラリの初期化
	SetGraphMode(720, 1280, 32);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);

	memset(&touchPos, 0, sizeof(touchPos));

	//メインループ
	mGameExec = 1;
	previousTime = GetNowCount();

	// プレイヤー初期化
	init();

	while(mGameExec == 1) 
	{
		ClearDrawScreen();
		update((float)waitTime / 1000.0f);		//ゲームの更新
		draw();									//ゲームの描画
		ScreenFlip();							//描画結果を画面に反映

												//次のフレームまでの時間待ち
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

	/// プレイヤー終了処理
	relese();

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

void init(void)
{
	/**********************************************************************************

	SSアニメ表示のサンプルコード
	Visual Studio Community、DXライブラリで動作を確認しています。
	ssbpとpngがあれば再生する事ができますが、Resourcesフォルダにsspjも含まれています。

	**********************************************************************************/

	//リソースマネージャの作成
	resman = ss::ResourceManager::getInstance();
	//プレイヤーの作成
	ssplayer = ss::Player::create();

	//アニメデータをリソースに追加

	//それぞれのプラットフォームに合わせたパスへ変更してください。
	resman->addData("character_template_comipo/character_template1.ssbp");
	//プレイヤーにリソースを割り当て
	ssplayer->setData("character_template1");        // ssbpファイル名（拡張子不要）
													 //再生するモーションを設定
	ssplayer->play("character_template_3head/stance");				 // アニメーション名を指定(ssae名/アニメーション名も可能、詳しくは後述)

																	 //表示位置を設定
	ssplayer->setPosition(1280 / 2, 300);
	//スケール設定
	ssplayer->setScale(0.5f, 0.5f);
	//回転を設定
	ssplayer->setRotation(0.0f, 0.0f, 0.0f);
	//透明度を設定
	ssplayer->setAlpha(255);
	//反転を設定
	ssplayer->setFlip(false, false);

}

//メインループ
//Zボタンでアニメをポーズ、再開を切り替えできます。
//ポーズ中は左右キーで再生するフレームを変更できます。
static bool sstest_push = false;
static int sstest_count = 0;
static bool sstest_pause = false;
void update(float dt)
{
	char str[128];
	char str2[128];

	//パーツ名から座標等のステートの取得を行う場合はgetPartStateを使用します。
	ss::ResluteState result;
	ssplayer->getPartState(result, "body");

	//取得座用の表示
	sprintf(str, "body = x:%f y:%f", result.x, result.y);
	DrawString(100, 120, str, GetColor(255, 255, 255));

	int touchNum = GetTouchInputNum();

	// タッチされている箇所の数だけ繰り返し
	if(GetTouchInputNum() > 0)
	{
		// タッチされている箇所の座標を取得
		GetTouchInput(0, &touchPos.x, &touchPos.y, NULL, NULL);
	}
	sprintf(str2, "touch = x:%d y:%d", touchPos.x, touchPos.y);
	DrawString(100, 150, str2, GetColor(255, 255, 255));

	sprintf(str2, "touchNum = %d", touchNum);
	DrawString(100, 170, str2, GetColor(255, 255, 255));

	//キー入力操作
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

	//アニメーションのフレームを表示
	sprintf(str, "play:%d frame:%d drawCount:%d", (int)sstest_pause, sstest_count, ssplayer->getDrawSpriteCount());
	DrawString(100, 100, str, GetColor(255, 255, 255));

	//プレイヤーの更新、引数は前回の更新処理から経過した時間
	ssplayer->update(dt);

}

//描画
void draw(void)
{
	//プレイヤーの描画
	ssplayer->draw();
}

/**
* プレイヤー終了処理
*/
void relese(void)
{

	//SS5Playerの削除
	delete (ssplayer);
	delete (resman);
}