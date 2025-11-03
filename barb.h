#pragma once

#include "animation.h";
#include "collision_box.h"

class Barb
{
public:
	Barb();
	~Barb();

	void on_update(float delta);
	void on_render();

	void set_position(const Vector2& position) {
		this->base_position = position;
		this->current_position = position;
	}

	bool check_valid() const {
		return is_valid;
	}
private:
	enum class Stage {
		Idle,
		Aim,
		Dash,
		Break
	};

private:
	const float SPEED_DASH = 1500.0f;

private:
	Timer timer_idle;
	Timer timer_aim;
	// NOTE: 随机数, 控制浮动的运动周期偏移
	int diff_period = 0;
	bool is_valid = true;
	// NOTE: 记录刺球生成以来度过的时间, 对其取三角函数来实现周期性运动
	float total_delta_time = 0;

	Vector2 velocity;
	Vector2 base_position;
	Vector2 current_position;

	Animation animation_loose;
	Animation animation_break;
	Animation* current_animation = nullptr;

	Stage stage = Stage::Idle;
	CollisionBox* collision_box = nullptr;

private:
	void on_break();
};
