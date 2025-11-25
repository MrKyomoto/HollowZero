#pragma once

#include "barb.h"
#include "sword.h"
#include "character.h"

class Enemy : public Character
{
public:
	Enemy();
	~Enemy();

	void on_update(float delta) override;
	void on_render() override;

	void on_hurt() override;

	void set_facing_left(bool flag) {
		this->is_facing_left = flag;
	}

	bool get_facing_left() const {
		return this->is_facing_left;
	}

	void set_dashing_in_air(bool flag) {
		this->is_dashing_in_air = flag;
	}

	bool get_dashing_in_air() const {
		return this->is_dashing_in_air;
	}

	void set_dashing_on_floor(bool flag) {
		this->is_dashing_on_floor = flag;
	}

	bool get_dashing_on_floor() const {
		return this->is_dashing_on_floor;
	}

	void set_throwing_silk(bool flag) {
		this->is_throwing_silk = flag;
	}

	bool get_throwing_silk() const {
		return this->is_throwing_silk;
	}

	void throw_barbs();
	void throw_sword();

	void on_dash();
	void on_throw_silk();
private:
	bool is_dashing_in_air;
	bool is_dashing_on_floor;
	bool is_throwing_silk;

	Animation animation_silk;
	AnimationGroup animation_dash_in_air_vfx;
	AnimationGroup animation_dash_on_floor_vfx;
	Animation* current_dash_animation = nullptr;

	std::vector<Barb*> barb_list;
	std::vector<Sword*> sword_list;
	CollisionBox* collision_box_silk = nullptr;
};
