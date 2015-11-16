// PerformanceGraph.cpp

#include "DebugGraph.h"

#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\RectangleShape.hpp"
#include "SFML\Graphics\VertexArray.hpp"

#include <sstream>
#include <iomanip>
#include <iostream>

DebugGraph::DebugGraph(std::string _label)
{
	m_custom = false;

	m_overlayMode = false;

	m_visible = false;
	m_scale = true;
	m_cacheCap = 200;
	m_defaultCap = 200;

	m_warning = FLT_MIN;
	m_preferSmallerValues = false;
	m_valueSum = 0.0f;

	m_valueMax = FLT_MIN;
	m_valueMin = FLT_MAX;

	m_valueDif = 0.0f;
	m_valueAverage = 0.0f;

	m_pos = sf::Vector2f(1.0f,1.0f);

	m_label = _label;
}

void DebugGraph::update(float _value)
{
	if (!m_visible)
		return;

	m_valueSum += _value;

	if (m_scale)
	{
		m_valueMax = std::max(m_valueMax, _value);
		m_valueMin = std::min(m_valueMin, _value);
		m_valueDif = m_valueMax - m_valueMin;
	}

	m_values.insert(m_values.begin(), _value);

	if (m_values.size() > m_cacheCap)
	{
		if ( m_scale && m_values.back() == m_valueMin )
		{
			m_valueMin = FLT_MAX;
			for(unsigned int i = 0; i < m_cacheCap - 1; i++)
				m_valueMin = std::min(m_valueMin, m_values[i]);
		}

		if ( m_scale && m_values.back() == m_valueMax )
		{
			m_valueMax = FLT_MIN;
			for(unsigned int i = 0; i < m_cacheCap - 1; i++)
				m_valueMax = std::max(m_valueMax, m_values[i]);
		}

		m_valueSum -= m_values.back();
		m_values.pop_back();
	}

	m_valueAverage = m_valueSum / m_values.size();
}

void DebugGraph::draw(sf::RenderWindow* _window, sf::Font* _font, int _width, int _height)
{
	if (!m_visible || m_values.empty())
		return;

	float width = 320;
	float height = 180;

	float textOffset = 32.0f;

	float step = (height - textOffset) / m_valueDif;

	float x_anchor =  m_pos.x > 0.5f ? _width*m_pos.x - width : _width*m_pos.x;
	float y_anchor =  m_pos.y > 0.5f ? _height*m_pos.y - height : _height*m_pos.y;

	float frontValue = m_values.front();
	unsigned sizeValues = m_values.size();

	sf::Text txt;

	if (!m_overlayMode)
	{
		// BACKGROUND
		sf::RectangleShape background(sf::Vector2f(width,height));
		background.setPosition(x_anchor,y_anchor);
		background.setFillColor(sf::Color(55,60,65,128));
		background.setOutlineColor(sf::Color(55,60,65,255));
		background.setOutlineThickness(2);
		_window->draw(background);

		background.setPosition(x_anchor,y_anchor + height - textOffset);
		background.setSize(sf::Vector2f(width,textOffset));
		background.setFillColor(sf::Color(0,0,0,0));
		_window->draw(background);

		// TEXT
		txt.setFont(*_font);
		txt.setCharacterSize(12);
		txt.setColor(sf::Color::White);
	}

	sf::Vector2f prev_pos(x_anchor + width,y_anchor + height - (frontValue - m_valueMin)*step - textOffset);

	float x_offset_add = width / std::min(sizeValues, m_cacheCap);
	float x_offset = x_anchor + width;

	sf::Color color = sf::Color(0,0,255,192);

	if (m_valueMin < 0.0f && m_valueMax > 0.0f)
	{
		// ZERO
		sf::Vertex zeroline[2] =
		{
			sf::Vertex(sf::Vector2f(x_anchor,y_anchor + height - (-m_valueMin)*step - textOffset),color),
			sf::Vertex(sf::Vector2f(x_anchor + width,y_anchor + height - (-m_valueMin)*step - textOffset),color)
		};
		_window->draw(zeroline, 2, sf::Lines);
	}

	color =
		m_preferSmallerValues == false ?
		(frontValue >= m_warning ? sf::Color::Green : sf::Color::Red) :
		(frontValue < m_warning ? sf::Color::Green : sf::Color::Red);

	if (m_warning == FLT_MIN)
		color = sf::Color::Yellow;

	for(unsigned int i = 0; i < m_cacheCap; i++)
	{
		if (i < sizeValues)
		{
			x_offset -= x_offset_add;
			sf::Vector2f pos(x_offset, y_anchor + height - (m_values[i] - m_valueMin)*step - textOffset);
			sf::Vertex line[2] =
			{
				sf::Vertex(prev_pos,color),
				sf::Vertex(pos,color)
			};

			prev_pos = pos;

			_window->draw(line, 2, sf::Lines);
		}
	}

	if (m_overlayMode)
		return;

	color = sf::Color(255,255,255,192);

	// AVERAGE
	sf::Vertex averageline[2] =
	{
		sf::Vertex(sf::Vector2f(x_anchor,y_anchor + height - (m_valueAverage - m_valueMin)*step - textOffset),color),
		sf::Vertex(sf::Vector2f(x_anchor + width,y_anchor + height - (m_valueAverage - m_valueMin)*step - textOffset),color)
	};
	_window->draw(averageline, 2, sf::Lines);

	std::ostringstream output1;
	output1 << m_label << std::setprecision(4) << ": " << m_values[0];
	txt.setString(output1.str());
	txt.setPosition(x_anchor + 8.0f, y_anchor + 8.0f);
	_window->draw(txt);

	std::ostringstream output;
	output << std::setprecision(4) 
		<< "min: " << m_valueMin
		<< "  max: " << m_valueMax
		<< "  avg: " << m_valueAverage;

	// TEXT
	txt.setString(output.str());
	txt.setPosition(x_anchor + 8.0f, y_anchor + height - 24.0f);
	_window->draw(txt);
}

void DebugGraph::setEntryCap(unsigned int _cap)
{
	if (_cap > 0)
		m_cacheCap = _cap;
	else _cap = m_defaultCap;
}

void DebugGraph::setWarningThreshold(float _value, bool _preferSmallerValues)
{
	m_warning = _value;
	m_preferSmallerValues = _preferSmallerValues;
}

void DebugGraph::setPosition(float _x, float _y)
{
	if ( _x > 1.0f || _x < 0.0f)
		_x = m_pos.x;
	if ( _y > 1.0f || _y < 0.0f)
		_y = m_pos.y;

	m_pos = sf::Vector2f(_x, _y);
}

void DebugGraph::toggle()
{
	m_visible = !m_visible;
}

void DebugGraph::toggleScale()
{
	m_scale = !m_scale;
}

void DebugGraph::toggleOverlay()
{
	m_overlayMode = !m_overlayMode;
}

void DebugGraph::toggleGraph()
{
	m_graph = !m_graph;
}