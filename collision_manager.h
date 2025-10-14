#pragma once

#include "collision_box.h"

#include <vector>

class CollisionManager
{
public:
	static CollisionManager* instance();

	// NOTE: 工厂方法,对外屏蔽对象创建和销毁的细节
	CollisionBox* create_collision_box();
	void destroy_collision_box(CollisionBox* collision_box);

	void process_collide();
	void on_debug_render();

private:
	static CollisionManager* manager;

	std::vector<CollisionBox*> collision_box_list;

private:
	CollisionManager();
	~CollisionManager();

};
