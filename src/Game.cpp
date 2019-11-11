#include "Game.h"
#include <thread>
#include "SDL.h"
#include "Exception.h"
#include "StringUtils.h"
#include "MathUtils.h"

NEG_BEGIN

static constexpr auto screen_w = 1024;
static constexpr auto screen_h = 768;
static constexpr auto paddle_h = 100;
static constexpr auto thickness = 15;

SDL_Window* create_window()
{	
	const auto window = SDL_CreateWindow("No Engine Game", 100, 100, screen_w, screen_h, 0);
	if (!window) throw sdl_error{fmt("Failed to create window: ", SDL_GetError())};
	return window;
}

SDL_Renderer* create_renderer(SDL_Window* const window)
{
	const auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) throw sdl_error{fmt("Failed to create renderer: ", SDL_GetError())};
	return renderer;
}

game::game()
	:window_{create_window(), SDL_DestroyWindow},
	renderer_{create_renderer(window_.get()), SDL_DestroyRenderer},
	ticks_count_{0}, paddle_dir_{0},
	is_running_{true}
{
	init();
}

game::~game() = default;

void game::run_loop()
{
	while (is_running_)
	{
		process_input();
		update_game();
		generate_output();
	}
}

void game::shutdown()
{
	is_running_ = false;
}

void game::init()
{
	paddle_pos_ = {20, screen_h / 2.f};
	ball_pos_ = {screen_w / 2.f, screen_h / 2.f};
	ball_velocity_ = {-200*1.5f, 235*1.5f};
}

void game::process_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			shutdown();
			break;
		default:
			break;
		}
	}

	const auto keyboard = SDL_GetKeyboardState(nullptr);
	if (keyboard[SDL_SCANCODE_ESCAPE]) shutdown();

	paddle_dir_ = 0;
	if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) paddle_dir_ -= 1;
	if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) paddle_dir_ += 1;
}

void game::update_game()
{
	constexpr auto max_fps = 60, min_fps = 10;
	constexpr auto time_scale = 1.f;
	
	std::this_thread::sleep_for(std::chrono::milliseconds{ticks_count_ + 1000ll/max_fps - SDL_GetTicks()});
	const auto delta_time = math::min((SDL_GetTicks() - ticks_count_) / 1000.f, 1.f/min_fps) * time_scale;
	ticks_count_ = SDL_GetTicks();

	paddle_pos_.y += paddle_dir_ * 300 * delta_time;
	paddle_pos_.y = math::clamp(paddle_pos_.y, paddle_h/2.f + thickness, screen_h - paddle_h/2.f - thickness);

	ball_pos_ += ball_velocity_ * delta_time;

	if (ball_pos_.y <= thickness*1.5f && ball_velocity_.y < 0.f ||
		ball_pos_.y >= screen_h - thickness*1.5f && ball_velocity_.y > 0.f)
	{
		ball_velocity_.y = -ball_velocity_.y;
	}

	else if (ball_pos_.x >= screen_w - thickness*1.5f && ball_velocity_.x > 0.f ||
		abs(ball_pos_.y - paddle_pos_.y) <= paddle_h/2.f &&
		math::is_nearly_equal<float>(ball_pos_.x, paddle_pos_.x+thickness/2, thickness/2) &&
		ball_velocity_.x < 0.f)
	{
		ball_velocity_.x = -ball_velocity_.x;
	}

	else if (ball_pos_.x < 0)
	{
		init();
	}
}

void game::generate_output()
{
	SDL_SetRenderDrawColor(renderer_.get(), 100, 100, 255, 255);
	SDL_RenderClear(renderer_.get());

	SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
	const SDL_Rect rects[]
	{
		{0, 0, screen_w, thickness},
		{0, screen_h - thickness, screen_w, thickness},
		{screen_w - thickness, 0, thickness, screen_w},
		{static_cast<int>(ball_pos_.x - thickness/2.f), static_cast<int>(ball_pos_.y - thickness/2.f), thickness, thickness},
		{static_cast<int>(paddle_pos_.x - thickness/2.f), static_cast<int>(paddle_pos_.y - paddle_h/2.f), thickness, paddle_h}
	};
	for (auto&& rect : rects)
	{
		SDL_RenderFillRect(renderer_.get(), &rect);
	}
	
	SDL_RenderPresent(renderer_.get());
}

game::sdl_raii::sdl_raii()
{
	const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) throw sdl_error{fmt("Unable to initialize SDL: ", SDL_GetError())};
}

game::sdl_raii::~sdl_raii()
{
	SDL_Quit();
}

NEG_END
