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


#ifndef HASKELLLIKE_POLYMORPHIC_LITERALS
#define HASKELLLIKE_POLYMORPHIC_LITERALS

#include<complex>


namespace lambdalike {

using std::complex;

class polymorphicNumLiteral {
  float fl; double db; complex<float> cf; complex<double> cd;
 public:
  explicit polymorphicNumLiteral(double init)
    : fl(init), db(init), cf(init,0), cd(init,0) {}
  operator float()const{return fl;}
  operator double()const{return db;}
  operator complex<float>()const{return cf;}
  operator complex<double>()const{return cd;}
};
const polymorphicNumLiteral minustwo(-2), minusone(-1), zero(0), one(1), two(2);

template<typename NumT>auto polymorphic_1() -> NumT {return 1;}
template<>auto polymorphic_1<complex<double>>() -> complex<double> {
  return complex<double>(1., 0.);                                  }


}


#endif