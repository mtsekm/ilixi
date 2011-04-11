/*
 Copyright 2010, 2011 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "types/Image.h"
#include "core/Window.h"
#include "core/Logger.h"

using namespace ilixi;

IDirectFBImageProvider* Image::_provider = NULL;

Image::Image() :
  _dfbSurface(0), _imagePath("")
{
}

Image::Image(const std::string& path, int width, int height) :
  _size(width, height), _dfbSurface(0)
{
  loadImage(path);
}

Image::Image(const std::string& path, const Size& size) :
  _size(size), _dfbSurface(0)
{
  loadImage(path);
}

Image::Image(const Image& img) :
  _size(img._size), _dfbSurface(0)
{
  loadImage(img._imagePath);
}

Image::~Image()
{
  if (_dfbSurface)
    _dfbSurface->Release(_dfbSurface);
}

int
Image::height() const
{
  return _size.height();
}

int
Image::width() const
{
  return _size.width();
}

Size
Image::size() const
{
  return _size;
}

IDirectFBSurface*
Image::getDFBSurface()
{
  return _dfbSurface;
}

std::string
Image::getImagePath() const
{
  return _imagePath;
}

bool
Image::loadImage(const std::string& path)
{
  int result = access(path.c_str(), F_OK);
  if (result != 0)
    {
      ILOG_ERROR("File (%s) is not accessible!", path.c_str());
      return false;
    }

  _imagePath = path;

  if (_dfbSurface)
    _dfbSurface->Release(_dfbSurface);

  DFBSurfaceDescription desc;
  desc.flags = (DFBSurfaceDescriptionFlags) (DSDESC_WIDTH | DSDESC_HEIGHT
      | DSDESC_CAPS);
  desc.caps = (DFBSurfaceCapabilities) (DSCAPS_PREMULTIPLIED | DSCAPS_SHARED
      | DSCAPS_SYSTEMONLY);
  desc.width = _size.width();
  desc.height = _size.height();

  if (Window::DFBInterface()->CreateSurface(Window::DFBInterface(), &desc,
      &_dfbSurface) == DFB_OK)
    return renderToSurface(_dfbSurface, path.c_str(), _size.width(),
        _size.height());
  return false;
}

bool
Image::renderToSurface(IDirectFBSurface* surface, const char* path, int width,
    int height)
{
  if (Window::DFBInterface()->CreateImageProvider(Window::DFBInterface(), path,
      &_provider) != DFB_OK)
    return false;
  _provider->RenderTo(_provider, surface, NULL);
  _provider->Release(_provider);
  return true;
}
