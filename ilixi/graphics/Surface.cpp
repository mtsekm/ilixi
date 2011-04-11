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

#include "graphics/Surface.h"
#include "core/Window.h"
#include "core/Logger.h"

using namespace ilixi;

Surface::Surface() :
  _dfbSurface(NULL), _parentSurface(NULL), _cairoSurface(NULL), _cairoContext(
      NULL), _pangoContext(NULL), _pangoLayout(NULL)
{
  pthread_mutex_init(&_surfaceLock, NULL);
}

Surface::~Surface()
{
  release();
  pthread_mutex_destroy(&_surfaceLock);
}

bool
Surface::createDFBSurface(int width, int height)
{
  release();
  DFBSurfaceDescription desc;
  desc .flags = (DFBSurfaceDescriptionFlags) (DSDESC_WIDTH | DSDESC_HEIGHT
      | DSDESC_PIXELFORMAT | DSDESC_CAPS);
  desc.width = width;
  desc.height = height;
  desc.pixelformat = Window::pixelFormat();
  desc.caps = (DFBSurfaceCapabilities) (DSCAPS_PREMULTIPLIED);
  desc.hints = DSHF_FONT;
  DFBResult ret = Window::DFBInterface()->CreateSurface(Window::DFBInterface(),
      &desc, &_dfbSurface);
  if (ret)
    {
      ILOG_ERROR("DFB CreateSurface Error: %d", ret)
        ;
      return false;
    }
  _dfbSurface->SetBlittingFlags(_dfbSurface, DSBLIT_BLEND_ALPHACHANNEL);
  return true;
}

bool
Surface::createDFBSubSurface(const Rectangle& geometry,
    IDirectFBSurface* parent)
{
  release();
  DFBRectangle r =
    { geometry.x(), geometry.y(), geometry.width(), geometry.height() };
  _parentSurface = parent;
  DFBResult ret = _parentSurface->GetSubSurface(_parentSurface, &r,
      &_dfbSurface);
  if (ret)
    {
      ILOG_ERROR("DFB GetSubSurface Error: %d", ret)
        ;
      return false;
    }
  _dfbSurface->SetBlittingFlags(_dfbSurface, DSBLIT_BLEND_ALPHACHANNEL);
  return true;
}

cairo_surface_t*
Surface::cairoSurface()
{
  if (!_cairoSurface)
    _cairoSurface = cairo_directfb_surface_create(Window::DFBInterface(),
        _dfbSurface);
  return _cairoSurface;
}

cairo_t*
Surface::cairoContext()
{
  if (!_cairoContext)
    _cairoContext = cairo_create(cairoSurface());
  return _cairoContext;
}

PangoContext*
Surface::pangoContext()
{
  if (!_pangoContext)
    _pangoContext = pango_cairo_create_context(cairoContext());
  return _pangoContext;
}

PangoLayout*
Surface::pangoLayout()
{
  if (!_pangoLayout)
    _pangoLayout = pango_cairo_create_layout(cairoContext());
  return _pangoLayout;
}

IDirectFBSurface*
Surface::DFBSurface()
{
  return _dfbSurface;
}

void
Surface::setGeometry(const Rectangle& geometry)
{
  DFBRectangle r =
    { geometry.x(), geometry.y(), geometry.width(), geometry.height() };
  DFBResult ret = _dfbSurface->MakeSubSurface(_dfbSurface, _parentSurface, &r);
  if (ret)
    ILOG_ERROR("DFB MakeSubSurface Error: %d", ret)
      ;
}

void
Surface::flip(const Rectangle& rect)
{
  DFBRegion r =
    { rect.x(), rect.y(), rect.right() - 1, rect.bottom() - 1 };

  DFBResult ret = _dfbSurface->Flip(_dfbSurface, &r, DSFLIP_BLIT);
  if (ret)
    ILOG_ERROR("DFB Flip Error: %d", ret)
      ;
}

void
Surface::lock()
{
  int rc = pthread_mutex_lock(&_surfaceLock);
  if (rc != 0)
    ILOG_ERROR("Error while locking surface!")
      ;
}

void
Surface::unlock()
{
  int rc = pthread_mutex_unlock(&_surfaceLock);
  if (rc != 0)
    ILOG_ERROR("Error while unlocking surface!")
      ;
}

void
Surface::clear()
{
  DFBResult ret = _dfbSurface->Clear(_dfbSurface, 0, 0, 0, 0);
  if (ret)
    ILOG_ERROR("DFB Clear Error: %d", ret)
      ;
}

void
Surface::clear(const Rectangle& rect)
{
  _dfbSurface->SetColor(_dfbSurface, 0, 0, 0, 0);
  _dfbSurface->FillRectangle(_dfbSurface, rect.x(), rect.y(), rect.width(),
      rect.height());
}

void
Surface::clip(const Rectangle& rect)
{
  DFBRegion r =
    { rect.x(), rect.y(), rect.right(), rect.bottom() };
  _dfbSurface->SetClip(_dfbSurface, &r);
}

void
Surface::resetClip()
{
  _dfbSurface->SetClip(_dfbSurface, NULL);
}

void
Surface::blit(IDirectFBSurface* source, const Rectangle& crop, int x, int y)
{
  DFBRectangle r =
    { crop.x(), crop.y(), crop.width(), crop.height() };
  DFBResult ret = _dfbSurface->Blit(_dfbSurface, source, &r, x, y);
  if (ret)
    ILOG_ERROR("DFB Blit Error: %d", ret)
      ;
}

void
Surface::blit(IDirectFBSurface* source, int x, int y)
{
  DFBResult ret = _dfbSurface->Blit(_dfbSurface, source, NULL, x, y);
  if (ret)
    ILOG_ERROR("DFB Blit Error: %d", ret)
      ;
}

void
Surface::blit(Surface* source, const Rectangle& crop, int x, int y)
{
  DFBRectangle r =
    { crop.x(), crop.y(), crop.width(), crop.height() };
  DFBResult ret =
      _dfbSurface->Blit(_dfbSurface, source->DFBSurface(), &r, x, y);
  if (ret)
    ILOG_ERROR("DFB Blit Error: %d", ret)
      ;
}

void
Surface::blit(Surface* source, int x, int y)
{
  DFBResult ret = _dfbSurface->Blit(_dfbSurface, source->DFBSurface(), NULL, x,
      y);
  if (ret)
    ILOG_ERROR("DFB Blit Error: %d", ret)
      ;
}

void
Surface::release()
{
  lock();
  if (_pangoLayout)
    g_object_unref(_pangoLayout);
  if (_pangoContext)
    g_object_unref(_pangoContext);
  if (_cairoContext)
    cairo_destroy(_cairoContext);
  if (_cairoSurface)
    cairo_surface_destroy(_cairoSurface);
  if (_dfbSurface)
    _dfbSurface->Release(_dfbSurface);
  unlock();
}
