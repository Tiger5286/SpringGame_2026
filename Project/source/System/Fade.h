#pragma once

enum class FadeState
{
	NoFade,
	FadeIn,
	FadeOut
};

class Fade
{
public:
	Fade() = default;
	virtual ~Fade() = default;

	void Update();
	void Draw();

	void StartFadeIn();
	void StartFadeOut();

	/// <summary>
	/// フェードが終わった瞬間を取得する
	/// </summary>
	/// <returns>フェードが終わった瞬間:true</returns>
	bool IsFadeEnd();

	/// <summary>
	/// フェード中かどうかを取得する
	/// </summary>
	/// <returns>true:フェード中 / false:フェードしていない</returns>
	bool IsFading() const { return m_state != FadeState::NoFade; }

private:
	void DrawFade(float alpha);

private:
	FadeState m_state = FadeState::NoFade;
	FadeState m_prevState = FadeState::NoFade;
	int m_fadeFrame = 0;
};

