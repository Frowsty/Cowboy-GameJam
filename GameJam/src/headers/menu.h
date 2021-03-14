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
		bool m_center;

		element() = default;
		virtual ~element() = default;

		virtual void on_input(olc::PixelGameEngine* pge) = 0;
		virtual void on_render(olc::PixelGameEngine* pge) = 0;
};

struct element_text : public element 
{
		element_text(olc::vf2d pos, std::string text, bool center) 
		{
				m_pos = pos;
				m_text = text;
				m_center = center;
		}

		~element_text() = default;

		void on_input(olc::PixelGameEngine* pge) override { }

		void on_render(olc::PixelGameEngine* pge) override 
		{
				auto text_pos = m_pos;

				if (m_center)
						text_pos.x -= (pge->GetTextSize(m_text).x / 2);

				pge->DrawStringDecal(text_pos, m_text);
		}
};

struct element_button : public element 
{
		element_button(olc::vf2d pos, olc::vf2d size, std::string text, bool center, std::function<void()> callback)
		{
				m_pos = pos;
				m_size = size;
				m_text = text;
				m_center = center;
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
						pge->FillRectDecal(m_pos, m_size, olc::DARK_GREY);
				else if (m_state == element_state::HOVER)
						pge->FillRectDecal(m_pos, m_size, olc::GREY);
				else if (m_state == element_state::ACTIVE)
						pge->FillRectDecal(m_pos, m_size, olc::VERY_DARK_GREY);
		
				olc::vf2d text_pos = {
						m_center ? (m_pos.x + (m_size.x / 2) - (pge->GetTextSize(m_text).x / 2)) : m_pos.x + 5,
						(m_pos.y + (m_size.y / 2) - (pge->GetTextSize(m_text).y / 2))
				};

				pge->DrawStringDecal(text_pos, m_text);
		}
};

class Menu 
{
private:
		olc::PixelGameEngine* m_pge;
		std::vector<std::shared_ptr<element>> m_elms;

public:
		Menu() = default;

		void setup(olc::PixelGameEngine* pge) 
		{
				m_pge = pge;
		}

		void add_text(const olc::vf2d& pos, const std::string& text, bool centered) 
		{
				m_elms.push_back(std::make_shared<element_text>(pos, text, centered));
		}

		void add_button(const olc::vf2d& pos, const olc::vf2d& size, const std::string& text, bool centered, std::function<void()> callback = nullptr)
		{			
				m_elms.push_back(std::make_shared<element_button>(pos, size, text, centered, callback));
		}

		bool has_elements() const {
			return m_elms.empty() == false;
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