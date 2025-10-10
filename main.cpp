#include <graphics.h>
#include "resources_manager.h"
#include <chrono>
#include <thread>

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
	const nanoseconds frame_duration(1000000000 / 144);
	steady_clock::time_point last_tick = steady_clock::now();

	ExMessage msg;
	bool is_quit = false;

	while (!is_quit) {
		while (peekmessage(&msg)) {
			// process msg
		}

		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);
		// process update

		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		// process draw

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