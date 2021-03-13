#pragma once

#include "olcPixelGameEngine.h"

#include <string>
#include <vector>
#include <functional>

enum class ElementState {
	IDLE = 0,
	HOVER = 1,
	ACTIVE = 2
};

struct Element {
	olc::PixelGameEngine* m_Pge;
	olc::vf2d m_Pos;
	olc::vf2d m_Size;
	std::string m_Name;
	ElementState m_State;
	std::function<void()> m_Callback;

	Element() = default;
	virtual ~Element() = default;

	virtual void OnInput() = 0;
	virtual void OnRender() = 0;
};

struct ElementButton : public Element {
	ElementButton(olc::PixelGameEngine* pge, olc::vf2d pos, olc::vf2d size, std::string name) {
		m_Pge = pge;
		m_Pos = pos;
		m_Size = size;
		m_Name = name;
		m_State = ElementState::IDLE;
	}

	~ElementButton() = default;

	void OnInput() override {
		auto minput = m_Pge->GetMouse(0);
		
		if (m_Pge->GetMouseX() >= m_Pos.x && m_Pge->GetMouseX() <= m_Pos.x + m_Size.x && m_Pge->GetMouseY() >= m_Pos.y && m_Pge->GetMouseY() <= m_Pos.y + m_Size.y)
			m_State = ElementState::HOVER;
		else
			m_State = ElementState::IDLE;

		if (m_State == ElementState::HOVER && minput.bHeld)
			m_State = ElementState::ACTIVE;

		if (m_State == ElementState::ACTIVE)
			m_Callback();
	}

	void OnRender() override {
		if (m_State == ElementState::IDLE)
			m_Pge->FillRect(m_Pos, m_Size, olc::DARK_GREY);
		else if (m_State == ElementState::HOVER)
			m_Pge->FillRect(m_Pos, m_Size, olc::GREY);
		else if (m_State == ElementState::ACTIVE)
			m_Pge->FillRect(m_Pos, m_Size, olc::VERY_DARK_GREY);
		
		olc::vf2d text_pos = (m_Pos + (m_Size / 2) - (m_Pge->GetTextSize(m_Name) / 2));
		m_Pge->DrawString(text_pos, m_Name);
	}
};

class Menu {
private:
	olc::PixelGameEngine* m_pge;
	std::vector<std::shared_ptr<Element>> m_Elms;

public:
	void AddButton(olc::PixelGameEngine* pge, const olc::vf2d& pos, const olc::vf2d& size, const std::string& text) {
		m_Elms.push_back(std::make_shared<ElementButton>(pge, pos, size, text));
	}

	void OnInput() {
		for (auto& e : m_Elms)
			e->OnInput();
	}

	void OnRender() {
		for (auto& e : m_Elms)
			e->OnRender();
	}

	void Reset() {
		m_Elms.clear();
	}

	void SetCallback(const std::function<void()> callback) {
		auto e = m_Elms.back();
		e->m_Callback = callback;
	}
};