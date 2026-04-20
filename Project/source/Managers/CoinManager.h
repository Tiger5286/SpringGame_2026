#pragma once
#include <array>
#include <memory>
#include "../Utility/Vector3.h"

class CollisionManager;

class Coin;

class CoinManager
{
public:
	CoinManager(CollisionManager& collisionManager);
	~CoinManager();

	void Init();
	void End();
	void Update();
	void Draw();

	// コインを出現させる
	void Spawn(const Vector3& pos);

	// プレイヤーの位置を設定する(Updateの前に毎フレーム呼ぶ)
	void SetPlayerPos(const Vector3& pos) { m_playerPos = pos; }

	// 有効になっている全てのコインに引き寄せを発動させる
	void ActivateAtract(const Vector3& playerPos);

	// 取得したコインの数を返す
	int GetCoinNum() const { return m_getCoinNum; }

	// 音を鳴らした後、次に音が鳴るまでのフレームをリセットする
	void ResetAfterPlaySoundFrame() { m_afterPlaySoundFrame = 0; }
	int GetAfterPlaySoundFrame() const { return m_afterPlaySoundFrame; }

private:
	// コインの存在上限数
	static constexpr int kCoinMaxNum = 150;

private:
	CollisionManager& m_collisionManager;

	Vector3 m_playerPos; // プレイヤーの位置

	// コインを拾う音を出した後、何フレーム経過しているか
	int m_afterPlaySoundFrame = 0;

	std::array<std::shared_ptr<Coin>, kCoinMaxNum> m_coins;
	int m_getCoinNum = 0;
};