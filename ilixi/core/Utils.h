#ifndef ILIXI_UTILS_H_
#define ILIXI_UTILS_H_

namespace ilixi
{

  template<class T>
    const T&
    max(const T& a, const T& b)
    {
      return (a > b) ? a : b;
    }

  template<class T>
    const T&
    min(const T& a, const T& b)
    {
      return (a < b) ? a : b;
    }

}

#endif /* UTILS_H_ */
