#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete sprite_;
	delete model_;
}

void GameScene::Initialize(
	DirectXCommon*dxCommon,Input*input,Audio*audio,DebugText*debugText) {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, { 100,50 });
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize(dxCommon_->GetDevice());
	//ビュープロジェクションの初期化
	viewProjection_.Initialize(dxCommon_->GetDevice());
	//サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	//音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() 
{
	//スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();
	//座標を｛2,0｝移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE))
	{
		//音声停止
		audio_->StopWave(voiceHandle_);
	}
	//変数の値をインクリメント
	value_++;
	//値を含んだ文字列
	std::string strDebug = std::string("Value:") + std::to_string(value_);
	//デバッグテキストの表示
	debugText_->Print(strDebug, 50, 50, 1.0f);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
