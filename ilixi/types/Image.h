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

#ifndef ILIXI_IMAGE_H_
#define ILIXI_IMAGE_H_

#include "directfb.h"
#include "types/Size.h"
#include <string>

namespace ilixi
{
  //! Loads an image using DirectFB.
  /*!
   * Surface data is permanently stored in system memory. There's no video memory allocation/storage.
   */
  class Image
  {
  public:
    /*!
     * Constructor.
     * Does nothing.
     */
    Image();

    /*!
     *
     * @param path
     * @param width
     * @param height
     */
    Image(const std::string& path, int width, int height);

    /*!
     *
     * @param path
     * @param size
     */
    Image(const std::string& path, const Size& size);

    /*!
     * Copy constructor.
     */
    Image(const Image& img);

    /*!
     * Destructor.
     */
    ~Image();

    /*!
     * Returns height of image.
     */
    int
    height() const;

    /*!
     * Returns width of image.
     */
    int
    width() const;

    /*!
     * Returns size of image.
     */
    Size
    size() const;

    /*!
     * Returns a pointer to DirectFB surface.
     */
    IDirectFBSurface*
    getDFBSurface();

    /*!
     * Returns the image path.
     */
    std::string
    getImagePath() const;

    /*!
     * Loads image. Returns true if successful.
     * @param path image path.
     */
    bool
    loadImage(const std::string& path);

  private:
    //! This property stores the size for image.
    Size _size;
    //! This property stores the image path.
    std::string _imagePath;
    //! This property stores the pointer to DirectFB surface.
    IDirectFBSurface* _dfbSurface;
    //! This property stores the pointer to DirectFB provider.
    static IDirectFBImageProvider* _provider;

    /*!
     * Renders image to DirectFB surface using provider.
     * @param surface
     * @param path
     * @param width
     * @param height
     */
    static bool
    renderToSurface(IDirectFBSurface* surface, const char* path, int width = 0,
        int height = 0);
  };
}

#endif /* ILIXI_IMAGE_H_ */
