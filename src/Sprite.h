/***************************************************************************

    Copyright 2010 Jakob Gruber

    This file is part of picmi.

    picmi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    picmi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with picmi.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************/

#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

#include "Error.h"

using boost::shared_ptr;

class Sprite
{
public:

    void load(std::string Filename);
    void blit(shared_ptr<sf::RenderWindow> target, sf::Vector2i p);
    void setAsIcon(shared_ptr<sf::RenderWindow> target) const;

    void setRotation(int angle);
    void setAlpha(int alpha);
    void setFlipX();

private:

    shared_ptr<sf::Sprite> Surface;
    sf::Image Image;

    int Rotation;
};
#endif // SPRITE_H
