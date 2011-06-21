/*
 Copyright 2010, 2011 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 This file is part of ilixi.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ilixi.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SQLITE3DRIVER_H_
#define SQLITE3DRIVER_H_

#include <string>
#include <vector>

namespace ilixi
{

  class SQLITE3Driver
  {
  public:
    SQLITE3Driver(std::string filename);

    virtual
    ~SQLITE3Driver();

    void
    exec(std::string query);

  private:
    std::string _error;
    std::vector<std::string> _columns;
    std::vector<std::string> _data;
  };

}

#endif /* SQLITE3DRIVER_H_ */
