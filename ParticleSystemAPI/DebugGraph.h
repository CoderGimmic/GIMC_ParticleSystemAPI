// DebugGraph.h

#pragma once

#include <vector>
#include "SFML\System\Vector2.hpp"

namespace sf
{
	class RenderWindow;
	class Font;
};

class DebugGraph
{
	friend class System;
public:

	DebugGraph(std::string _label);

	void update(float _value);
	void draw(sf::RenderWindow* _window, sf::Font* _font, int _width, int _height);
	
	void setEntryCap(unsigned int _cap);
	void setWarningThreshold(float _value);
	void setPosition(float _x, float _y);
	void toggle();
	void toggleScale();
	void toggleOverlay();
	void toggleGraph();

private:

	bool m_visible;
	bool m_scale;
	bool m_custom;
	bool m_overlayMode;
	bool m_graph;
	unsigned int m_defaultCap;
	unsigned int m_cacheCap;

	std::string m_label;

	sf::Vector2f m_pos;

	std::vector<float> m_values;
	float m_warning;
	float m_valueSum;
	float m_valueMax;
	float m_valueDif;
	float m_valueMin;
	float m_valueAverage;
};