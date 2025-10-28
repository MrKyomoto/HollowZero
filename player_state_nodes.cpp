#include "player.h"
#include "character_manager.h"
#include "player_state_nodes.h"

#include <iostream>

PlayerAttackState::PlayerAttackState() {
	timer.set_wait_time(0.3f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]() {
		Player* player = (Player*)CharacterManager::instance()->get_player();
		player->set_attacking(false);
		});
}

void PlayerAttackState::on_enter() {
	std::cout << "attack state\n";
	CharacterManager::instance()->get_player()->set_animation("attack");

	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(true);
	player->set_attacking(true);
	update_hit_box_position();
	player->on_attack();
	timer.restart();

	switch (range_random(1,3))
	{
	case 1:
		play_audio(_T("player_attack_1"), false);
		break;
	case 2:
		play_audio(_T("player_attack_2"), false);
		break;
	case 3:
		play_audio(_T("player_attack_3"), false);
		break;
	default:
		break;
	}
}

void PlayerAttackState::on_update(float delta) {
	timer.on_update(delta);
	update_hit_box_position();

	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (player->get_hp() <= 0) {
		player->switch_state("dead");
	}
	else if (!player->get_attacking()) {
		if (player->get_velocity().y > 0) {
			player->switch_state("fall");
		}
		else if (player->get_move_axis() == 0) {
			player->switch_state("idle");
		}
		else if (player->is_on_floor() && player->get_move_axis() != 0) {
			player->switch_state("run");
		}
	}
}

void PlayerAttackState::on_exit() {
	std::cout << "quit attack state\n";
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(false);
	player->set_attacking(false);
}

void PlayerAttackState::update_hit_box_position() {
	Player* player = (Player*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	// NOTE: 根据当前攻击方向将攻击碰撞箱朝着攻击方向偏移了一定距离,更符合玩家期望
	switch (player->get_attack_dir())
	{
	case Player::AttackDir::Up:
		pos_hit_box = { pos_center.x, pos_center.y - size_hit_box.y / 2 };
		break;
	case Player::AttackDir::Down:
		pos_hit_box = { pos_center.x, pos_center.y + size_hit_box.y / 2 };
		break;
	case Player::AttackDir::Left:
		pos_hit_box = { pos_center.x - size_hit_box.x / 2, pos_center.y };
		break;
	case Player::AttackDir::Right:
		pos_hit_box = { pos_center.x + size_hit_box.x / 2, pos_center.y };
		break;
	default:
		break;
	}
	hit_box->set_position(pos_hit_box);
}

PlayerDeadState::PlayerDeadState() {
	timer.set_wait_time(2.0f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]() {
		MessageBox(GetHWnd(), _T("Opps...\nSomething seems to be wrong."), _T("Challenge Failed!"), MB_OK);
		exit(0);
		});
}

void PlayerDeadState::on_enter() {
	CharacterManager::instance()->get_player()->set_animation("dead");

	play_audio(_T("player_dead"), false);
}

void PlayerDeadState::on_update(float delta) {
	timer.on_update(delta);
}

void PlayerFallState::on_enter() {
	std::cout << "fall state\n";
	CharacterManager::instance()->get_player()->set_animation("fall");
}

void PlayerFallState::on_update(float delta) {
	Player* player = (Player*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0) {
		player->switch_state("dead");
	}
	else if (player->is_on_floor()) {
		player->switch_state("idle");
		player->on_land();

		play_audio(_T("player_land"), false);
	}
	else if (player->can_attack()) {
		player->switch_state("attack");
	}

}

void PlayerIdleState::on_enter() {
	std::cout << "idle state\n";
	CharacterManager::instance()->get_player()->set_animation("idle");
}

void PlayerIdleState::on_update(float delta) {
	Player* player = (Player*)CharacterManager::instance()->get_player();
	
	if (player->get_hp() <= 0) {
		player->switch_state("dead");
	}
	else if (player->can_attack()) {
		player->switch_state("attack");
	}
	else if (player->get_velocity().y > 0) {
		player->switch_state("fall");
	}
	else if (player->can_jump()) {
		player->switch_state("jump");
	}
	else if (player->can_roll()) {
		player->switch_state("roll");
	}
	else if (player->is_on_floor() && player->get_move_axis() != 0 ){
		player->switch_state("run");
	}
}

void PlayerJumpState::on_enter() {
	CharacterManager::instance()->get_player()->set_animation("jump");

	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->on_jump();

	play_audio(_T("player_jump"), false);
}

void PlayerJumpState::on_update(float delta) {
	Player* player = (Player*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0) {
		player->switch_state("dead");
	}
	else if (player->get_velocity().y > 0) {
		// NOTE: 竖直方向速度向下后切换至下落状态
		player->switch_state("fall");
	}
	else if (player->can_attack()) {
		player->switch_state("attack");
	}
}

PlayerRollState::PlayerRollState() {
	timer.set_wait_time(0.35f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]() {

		Player* player = (Player*)CharacterManager::instance()->get_player();
		player->set_rolling(false);
		});
}

void PlayerRollState::on_enter() {
	CharacterManager::instance()->get_player()->set_animation("roll");

	Player* player = (Player*)CharacterManager::instance()->get_player();
	// NOTE: 翻滚时是无敌状态,所以设置了受击hitbox为disable
	player->get_hurt_box()->set_enabled(false);
	player->set_rolling(true);
	player->on_roll();
	timer.restart();

	play_audio(_T("player_roll"), false);
}

void PlayerRollState::on_update(float delta) {
	timer.on_update(delta);

	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (!player->get_rolling()) {
		// NOTE: 由于翻滚状态是无敌状态因此这是唯一一个不需要跳转到死亡状态的节点
		if (player->get_move_axis() != 0) {
			player->switch_state("run");
		}
		else if (player->can_jump()) {
			player->switch_state("jump");
		}
		else {
			// NOTE: 我思考了一下为什么voidmatrix没有添加if can_attack的判断
			// 感觉应该是因为idle作为一个过渡态已经囊括了attack的转换,所以
			// 这里就仅仅是用一个else直接来跳转到idle了, 不过这么一说为什么上边要判断这个run 和 jump呢? 可能是跑和跳的优先级与攻击的优先级问题?
			player->switch_state("idle");
		}
	}
}

void PlayerRollState::on_exit() {
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hurt_box()->set_enabled(true);
}

void PlayerRunState::on_enter(){
	std::cout << "run state\n";
	CharacterManager::instance()->get_player()->set_animation("run");

	play_audio(_T("player_run"), true);
}

void PlayerRunState::on_update(float delta){
	Player* player = (Player*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0) {
		player->switch_state("dead");
	}
	else if (player->get_move_axis() == 0) {
		player->switch_state("idle");
	}
	else if (player->can_jump()) {
		player->switch_state("jump");
	}
	else if (player->can_attack()) {
		player->switch_state("attack");
	}
	else if (player->can_roll()) {
		player->switch_state("roll");
	}
}

void PlayerRunState::on_exit() {
	std::cout << "quit run state\n";
	stop_audio(_T("player_run"));
}
