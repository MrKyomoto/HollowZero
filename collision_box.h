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
	// ���ĵ�λ��
	Vector2 position;
	// �Ƿ�������ײ
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
	// NOTE: ���췽������Ϊprivate��ζ�Ų����������ⴴ����ײ�����
	// cuz ��Ϸ�е�������ײ�����Ӧ������ײ����������ͳһ����
	CollisionBox() = default;
	~CollisionBox() = default;

};

