#pragma once

class BulletTimeManager
{
public:
	enum class Status {
		Entering,
		Exiting
	};
public:
	static BulletTimeManager* instance();

	void post_process();
	void set_status(Status status);

	float on_update(float delta);

private:
	static BulletTimeManager* manager;

private:
	// NOTE: 表示子弹时间的状态,从0到1
	float progress = 0;
	Status status = Status::Exiting;
	/// <summary>
	/// @param: 子弹时间的更新速度, 缩放比例, 画面色彩的改变比例
	/// </summary>
	const float SPEED_PROGRESS = 2.0f;
	const float DST_DELTA_FACTOR = 0.35f;
	const float DST_COLOR_FACTOR = 0.35f;

private:
	BulletTimeManager();
	~BulletTimeManager();

	// NOTE: lerp函数实现插值, 计算进度从start到end的过程中到了进度为t的时候的值
	float lerp(float start, float end, float t) {
		return (1 - t) * start + t * end;
	}
};
