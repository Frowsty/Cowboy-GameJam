#pragma once

#include "olcPixelGameEngine.h"

#include <string>
#include <vector>
#include <functional>

enum class element_state 
{
		IDLE = 0,
		HOVER = 1,
		ACTIVE = 2
};

struct element 
{
		olc::vf2d m_pos;
		olc::vf2d m_size;
		std::string m_text;
		element_state m_state;
		std::function<void()> m_callback;

		element() = default;
		virtual ~element() = default;

		virtual void on_input(olc::PixelGameEngine* pge) = 0;
		virtual void on_render(olc::PixelGameEngine* pge) = 0;
};

struct element_text : public element 
{
		element_text(olc::vf2d pos, std::string text) 
		{
				m_pos = pos;
				m_text = text;
		}

		~element_text() = default;

		void on_input(olc::PixelGameEngine* pge) override { }

		void on_render(olc::PixelGameEngine* pge) override 
		{
				pge->DrawString(m_pos, m_text);
		}
};

struct element_button : public element 
{
		element_button(olc::vf2d pos, olc::vf2d size, std::string text, std::function<void()> callback) 
		{
				m_pos = pos;
				m_size = size;
				m_text = text;
				m_state = element_state::IDLE;
				m_callback = callback;
		}

		~element_button() = default;

		void on_input(olc::PixelGameEngine* pge) override 
		{
				auto minput = pge->GetMouse(0);
		
				if (pge->GetMouseX() >= m_pos.x && pge->GetMouseX() <= m_pos.x + m_size.x && pge->GetMouseY() >= m_pos.y && pge->GetMouseY() <= m_pos.y + m_size.y)
						m_state = element_state::HOVER;
				else
						m_state = element_state::IDLE;

				if (m_state == element_state::HOVER && minput.bPressed)
						m_state = element_state::ACTIVE;

				if (m_state == element_state::ACTIVE)
						m_callback();
		}

		void on_render(olc::PixelGameEngine* pge) override 
		{
				if (m_state == element_state::IDLE)
						pge->FillRect(m_pos, m_size, olc::DARK_GREY);
				else if (m_state == element_state::HOVER)
						pge->FillRect(m_pos, m_size, olc::GREY);
				else if (m_state == element_state::ACTIVE)
						pge->FillRect(m_pos, m_size, olc::VERY_DARK_GREY);
		
				olc::vf2d text_pos = (m_pos + (m_size / 2) - (pge->GetTextSize(m_text) / 2));
				pge->DrawString(text_pos, m_text);
		}
};

class Menu 
{
private:
		olc::PixelGameEngine* m_pge;
		std::vector<std::shared_ptr<element>> m_elms;

public:
		void setup(olc::PixelGameEngine* pge) 
		{
				m_pge = pge;
		}

		void add_text(const olc::vf2d& pos, const std::string& text) 
		{
				m_elms.push_back(std::make_shared<element_text>(pos, text));
		}

		void add_button(const olc::vf2d& pos, const olc::vf2d& size, const std::string& text, std::function<void()> callback = nullptr) 
		{
				m_elms.push_back(std::make_shared<element_button>(pos, size, text, callback));
		}

		void on_input() 
		{
				for (auto& e : m_elms)
						e->on_input(m_pge);
		}

		void on_render() 
		{
				for (auto& e : m_elms)
						e->on_render(m_pge);
		}

		void reset() 
		{
				m_elms.clear();
		}
};