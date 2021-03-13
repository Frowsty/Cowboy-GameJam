#pragma once

#include "olcPixelGameEngine.h"

#include <string>
#include <vector>
#include <functional>

enum class element_state {
	IDLE = 0,
	HOVER = 1,
	ACTIVE = 2
};

struct element {
	olc::PixelGameEngine* m_pge;
	olc::vf2d m_pos;
	olc::vf2d m_size;
	std::string m_text;
	element_state m_state;
	std::function<void()> m_callback;

	element() = default;
	virtual ~element() = default;

	virtual void on_input() = 0;
	virtual void on_render() = 0;
};

struct element_text : public element {
	element_text(olc::PixelGameEngine* pge, olc::vf2d pos, std::string text) {
		m_pge = pge;
		m_pos = pos;
		m_text = text;
	}

	~element_text() = default;

	void on_input() override { }

	void on_render() override {
		m_pge->DrawString(m_pos, m_text);
	}
};

struct element_button : public element {
	element_button(olc::PixelGameEngine* pge, olc::vf2d pos, olc::vf2d size, std::string text, std::function<void()> callback) {
		m_pge = pge;
		m_pos = pos;
		m_size = size;
		m_text = text;
		m_state = element_state::IDLE;
		m_callback = callback;
	}

	~element_button() = default;

	void on_input() override {
		auto minput = m_pge->GetMouse(0);
		
		if (m_pge->GetMouseX() >= m_pos.x && m_pge->GetMouseX() <= m_pos.x + m_size.x && m_pge->GetMouseY() >= m_pos.y && m_pge->GetMouseY() <= m_pos.y + m_size.y)
			m_state = element_state::HOVER;
		else
			m_state = element_state::IDLE;

		if (m_state == element_state::HOVER && minput.bPressed)
			m_state = element_state::ACTIVE;

		if (m_state == element_state::ACTIVE)
			m_callback();
	}

	void on_render() override {
		if (m_state == element_state::IDLE)
			m_pge->FillRect(m_pos, m_size, olc::DARK_GREY);
		else if (m_state == element_state::HOVER)
			m_pge->FillRect(m_pos, m_size, olc::GREY);
		else if (m_state == element_state::ACTIVE)
			m_pge->FillRect(m_pos, m_size, olc::VERY_DARK_GREY);
		
		olc::vf2d text_pos = (m_pos + (m_size / 2) - (m_pge->GetTextSize(m_text) / 2));
		m_pge->DrawString(text_pos, m_text);
	}
};

class Menu {
private:
	olc::PixelGameEngine* m_pge;
	std::vector<std::shared_ptr<element>> m_elms;

public:
	void add_text(olc::PixelGameEngine* pge, const olc::vf2d& pos, const std::string& text) {
		m_elms.push_back(std::make_shared<element_text>(pge, pos, text));
	}

	void add_button(olc::PixelGameEngine* pge, const olc::vf2d& pos, const olc::vf2d& size, const std::string& text, std::function<void()> callback = nullptr) {
		m_elms.push_back(std::make_shared<element_button>(pge, pos, size, text, callback));
	}

	void on_input() {
		for (auto& e : m_elms)
			e->on_input();
	}

	void on_render() {
		for (auto& e : m_elms)
			e->on_render();
	}

	void reset() {
		m_elms.clear();
	}
};