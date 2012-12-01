/* Libvisual - The audio visualisation framework cli tool
 *
 * Copyright (C) 2012 Libvisual team
 *
 * Authors: Chong Kai Xiong <kaixiong@codeleft.sg>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "config.h"
#include "display_driver_factory.hpp"
#include "stdout_driver.hpp"

#if HAVE_SDL
#include "sdl_driver.hpp"
#endif

#if HAVE_GLX
#include "glx_driver.hpp"
#endif

#include <unordered_map>

typedef std::unordered_map<std::string, DisplayDriverCreator> CreatorMap;

class DisplayDriverFactory::Impl
{
public:

    CreatorMap creators;
};

DisplayDriverFactory::DisplayDriverFactory ()
    : m_impl (new Impl)
{
    add_driver ("stdout", stdout_driver_new);
#if defined(HAVE_SDL)
    add_driver ("sdl", sdl_driver_new);
#endif
}

DisplayDriverFactory::~DisplayDriverFactory ()
{
    // nothing to do
}

void DisplayDriverFactory::add_driver (std::string const& name, Creator const& creator)
{
    m_impl->creators[name] = creator;
}

DisplayDriver* DisplayDriverFactory::make (std::string const& name, Display& display)
{
    auto entry = m_impl->creators.find (name);

    if (entry == m_impl->creators.end ()) {
        return nullptr;
    }

    return entry->second (display);
}

bool DisplayDriverFactory::has_driver (std::string const& name) const
{
    return (m_impl->creators.find (name) != m_impl->creators.end ());
}

DisplayDriverList DisplayDriverFactory::get_driver_list () const
{
    DisplayDriverList list;

    list.reserve (m_impl->creators.size ());

    for (auto creator : m_impl->creators)
        list.push_back (creator.first);

    return list;
}
