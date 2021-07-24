#pragma once

#define PI 3.14159265358979323846f

#define DEG_TO_RAD PI/180.0f
#define RAD_TO_DEG 180.0f/PI

const sf::Color RECT_UNSELECTED(40, 40, 40, 255);
const sf::Color RECT_HIGHLIGHTED(80, 80, 80, 255);
const sf::Color RECT_SELECTED(110, 110, 110, 255);
const sf::Color RECT_OUTLINE(190, 190, 190, 255);
#define RECT_OUTLINE_THICKNESS 2

#define TEXT_PADDING 6