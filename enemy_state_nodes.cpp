#include "enemy.h"
#include "character_manager.h"
#include "enemy_state_nodes.h"

// NOTE: aim is pre state of dash
EnemyAimState::EnemyAimState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->set_gravity_enabled(true);
		enemy->switch_state("dash_in_air");
		});
}

void EnemyAimState::on_enter() {
	CharacterManager::instance()->get_enemy()->set_animation("aim");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	// NOTE: 在空中悬停瞄准, 所以先禁用重力
	enemy->set_gravity_enabled(false);
	enemy->set_velocity({0,0});
	timer.restart();
}

void EnemyAimState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp() <= 0) {
		enemy->switch_state("dead");
	}
}

void EnemyDashInAirState::on_enter() {
	CharacterManager::instance()->get_enemy()->set_animation("dash_in_air");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	const Character* player = CharacterManager::instance()->get_player();
	Vector2 pos_target = { player->get_position().x, player->get_floor_y() };
	enemy->set_velocity((pos_target - enemy->get_position()).normalize() * SPEED_DASH);
	enemy->set_dashing_in_air(true);
	enemy->set_gravity_enabled(false);
	enemy->on_dash();

	play_audio(_T("enemy_dash"), false);
}

void EnemyDashInAirState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	if (enemy->get_hp() <= 0) {
		enemy->switch_state("dead");
	}
	else if (enemy->is_on_floor()) {
		enemy->switch_state("idle");
	}
}

void EnemyDashInAirState::on_exit() {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	enemy->set_gravity_enabled(true);
	enemy->set_dashing_in_air(false);
}

EnemyDashOnFloorState::EnemyDashOnFloorState() {
	timer.set_one_shot(true);
	timer.set_wait_time(0.5f);
	timer.set_on_timeout([&]() {
		Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
		enemy->set_dashing_on_floor(false);
		});
}

void EnemyDashOnFloorState::on_enter() {
	CharacterManager::instance()->get_enemy()->set_animation("dash_on_floor");

	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
	enemy->set_velocity({enemy->get_facing_left() ? -SPEED_DASH : SPEED_DASH, 0});
	enemy->set_dashing_on_floor(true);
	enemy->on_dash();
	timer.restart();

	play_audio(_T("enemy_dash"), false);
}

void EnemyDashOnFloorState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	timer.on_update(delta);

	if (enemy->get_hp() <= 0) {
		enemy->switch_state("dead");
	}
	else if (!enemy->get_dashing_on_floor()) {
		enemy->switch_state("idle");
	}
}

void EnemyDeadState::on_enter() {
	MessageBox(GetHWnd(), _T("Well, it works."), _T("Vector!"), MB_OK);
	exit(0);
}

void EnemyFallState::on_enter() {
	CharacterManager::instance()->get_enemy()->set_animation("fall");
}

void EnemyFallState::on_update(float delta) {
	Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

	if (enemy->get_hp() <= 0) {
		enemy->switch_state("dead");
	}
	else if (enemy->is_on_floor()) {
		enemy->switch_state("idle");
	}
}
