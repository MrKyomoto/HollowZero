#include "util.h"
#include <graphics.h>
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "bullet_time_manager.h"
#include <chrono>
#include <thread>

// NOTE: 给函数声明static是修饰该函数成为局部函数,即使使用extern也无法被外部调用
// 这是为了防止命名冲突
static void draw_background() {
	// NOTE: 而内部的static修饰的变量则是仅在一开始声明一次,生命周期跟程序一样长,后续再调用该函数,这些变量无需再次声明
	static IMAGE* img_background = ResourcesManager::instance()->find_image("background");
	static Rect rect_dst = {
		// x,y,w,h, (x,y)是左上角的点
		(getwidth() - img_background->getwidth()) / 2,
		(getheight() - img_background->getheight()) / 2,
		img_background->getwidth(),
		img_background->getheight(),
	};
	putimage_ex(img_background, &rect_dst);
}

static void draw_ramain_hp() {
	static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");
	Rect rect_dst = { 0,10, img_ui_heart->getwidth(), img_ui_heart->getheight() };
	for (int i = 0; i < CharacterManager::instance()->get_player()->get_hp(); i++) {
		rect_dst.x = 10 + i * 40;
		putimage_ex(img_ui_heart, &rect_dst);
	}
}
int main(int argc, char** argv) {
	using namespace std::chrono;
	HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
	SetWindowText(hwnd, _T("Hollow Zero"));

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载: %s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	play_audio(_T("bgm"), true);
	const nanoseconds frame_duration(1000000000 / 144);
	steady_clock::time_point last_tick = steady_clock::now();

	ExMessage msg;
	bool is_quit = false;

	BeginBatchDraw();

	while (!is_quit) {
		while (peekmessage(&msg)) {
			// process msg
			CharacterManager::instance()->on_input(msg);
		}

		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);

		// process update
		float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());
		CharacterManager::instance()->on_update(scaled_delta);
		CollisionManager::instance()->process_collide();

		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		// process draw
		draw_background();
		CharacterManager::instance()->on_render();
		CollisionManager::instance()->on_debug_render();
		draw_ramain_hp();

		FlushBatchDraw();
		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0)) {
			// NOTE: 如果帧更新所消耗的时间小于我们所期望的帧率标准时间
			// 则休眠 
			std::this_thread::sleep_for(sleep_duration);
		}
	}

	EndBatchDraw();
	return 0;
}