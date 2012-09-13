  // Copyright 2012 Justus Sagemüller.
  // This file is part of the Lambdalike library.
   //This library is free software: you can redistribute it and/or modify
  // it under the terms of the GNU General Public License as published by
 //  the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
   //This library is distributed in the hope that it will be useful,
  // but WITHOUT ANY WARRANTY; without even the implied warranty of
 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
  // You should have received a copy of the GNU General Public License
 //  along with this library.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HASKELLLIKE_FUNCTION_PRIMITIVES
#define HASKELLLIKE_FUNCTION_PRIMITIVES

#include<utility>

namespace lambdalike {

template<class ReturnT>
class constFunc {
  ReturnT ret;
 public:
  constFunc(const ReturnT& ret) : ret(ret) {}
  constFunc(ReturnT&& ret) : ret(std::move(ret)) {}
  
  template<class ArgT>
  auto operator()(const ArgT& _)const -> ReturnT {
    return ret;
  }
};

template<class RT>
auto const_func(RT&& ret)
    -> constFunc<typename std::remove_reference<RT>::type> {
  return constFunc<typename std::remove_reference<RT>::type>
               (std::forward<RT>(ret));
}

}

#endif