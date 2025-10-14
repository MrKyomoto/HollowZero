#pragma once

#include "vector2.h"
#include "collision_layer.h"

#include <functional>

class CollisionManager;

class CollisionBox
{
	friend class CollisionManager;
private:
	Vector2 size;
	// 中心点位置
	Vector2 position;
	// 是否启用碰撞
	bool enabled = true;
	std::function<void()> on_collide;
	CollisionLayer layer_src = CollisionLayer::None;
	CollisionLayer layer_dst = CollisionLayer::None;

public:
	void set_enabled(bool flag) {
		enabled = flag;
	}

	void set_layer_src(CollisionLayer layer) {
		layer_src = layer;
	}

	void set_layer_dst(CollisionLayer layer) {
		layer_dst = layer;
	}

	void set_on_collide(std::function<void()> on_collide) {
		this->on_collide = on_collide;
	}

	void set_size(const Vector2& size) {
		this->size = size;
	}

	const Vector2& get_size() const{
		return size;
	}

	void set_position(const Vector2& position) {
		this->position = position;
	}

private:
	// NOTE: 构造方法声明为private意味着不能自行随意创建碰撞箱对象
	// cuz 游戏中的所有碰撞箱对象都应该由碰撞管理器进行统一管理
	CollisionBox() = default;
	~CollisionBox() = default;

};

