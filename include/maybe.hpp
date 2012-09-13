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


#ifndef HASKELLLIKE_CLASS_MAYBE_MONAD
#define HASKELLLIKE_CLASS_MAYBE_MONAD


namespace lambdalike {

template<typename DataT>
class maybe;
template<typename DataT>
maybe<DataT> just(DataT obj);
struct nothing_object{nothing_object(){}};
const nothing_object nothing;

    // template class objects of which may or may not contain some given
   //  data object. Inspired by Haskell's Maybe monad.
     //Applications are often very similar to those of std::unique_ptr,
    // the main difference being that a maybe is copyable but can't hold
   //  derived instances.
template<typename DataT>
class maybe {
  DataT *obj;

 public:

  class iterator {
    DataT *mobj;
    explicit iterator(DataT *init):mobj(init){}
   public:
    iterator():mobj(nullptr){}
    iterator(const iterator &cp):mobj(cp.mobj){}
    bool operator!=(const iterator &other)const{return mobj!=other.mobj;}
    DataT &operator*() const{return *mobj;}
    iterator &operator++(){ mobj=nullptr; return *this; }
    friend class maybe;
  };
  class const_iterator {
    const DataT *mobj;
    explicit const_iterator(const DataT *init):mobj(init){}
   public:
    const_iterator():mobj(nullptr){}
    const_iterator(const const_iterator &cp):mobj(cp.mobj){}
    bool operator!=(const const_iterator &other)const{return mobj!=other.mobj;}
    const DataT &operator*() const{return *mobj;}
    const_iterator &operator++(){ mobj=nullptr; return *this; }
    friend class maybe;
  };
  iterator begin(){return iterator(obj);}
  iterator end(){return iterator();}
  const_iterator begin()const{return const_iterator(obj);}
  const_iterator end()const{return const_iterator();}
  const_iterator c_begin()const{return const_iterator(obj);}
  const_iterator c_end()const{return const_iterator();}

  bool is_nothing()const{return obj==nullptr;}
  void make_nothing(){delete obj; obj=nullptr;}
  bool is_just()const{return obj!=nullptr;}
  template<typename CpDataT>
  void with_just_assign(CpDataT &mdftg)const{if(obj) mdftg=*obj;}
  DataT &from_just(){return *obj;}
  DataT &operator*(){return *obj;}
  const DataT &from_just()const{return *obj;}
  const DataT &operator*()const{return *obj;}
  
  template<typename CmpDataT>
  bool operator==(const maybe<CmpDataT> &cmp)const{
    return is_just()==cmp.is_just() && (is_nothing() || *obj==*cmp.obj); }
  template<typename CmpDataT>
  bool operator!=(const maybe<CmpDataT> &cmp)const{
    return is_just()!=cmp.is_just() || (is_just() && *obj!=*cmp.obj); }
  bool operator==(const nothing_object &n)const{return obj==nullptr;}
  bool operator!=(const nothing_object &n)const{return obj!=nullptr;}
  
  template<typename MpFnT>
  auto fmapped(MpFnT f) const -> maybe<decltype(f(*obj))> {
    return obj? just(f(*obj)) : nothing;                  }
  template<typename MonadicFn>
  auto operator>>=(MonadicFn f) const -> decltype(f(*obj)) {
    return obj? f(*obj) : nothing;                         }
  template<typename MonadicFn>
  auto monadicbind_destructive(MonadicFn f) -> decltype(f(*obj)) {
    return obj? f(std::move(*obj)) : nothing;                    }
  template<typename ReplaceDT>
  auto operator>>(const maybe<ReplaceDT> &r) const -> maybe<ReplaceDT> {
    return obj? r : nothing;                                           }
  template<typename ReplaceDT>
  auto operator>>(maybe<ReplaceDT>&& r) const -> maybe<ReplaceDT> {
    return obj? std::move(r) : nothing;                           }
  auto operator>>(const nothing_object &n) const -> maybe<DataT> {
    return nothing;                                              }

  
// old constructor set, flawed
#ifdef USE_ALTERNATIVE_CONSTRUCTORS_FOR_MAYBE_DATATYPE
  maybe(const nothing_object &n):obj(nullptr){}
  template<typename CpDataT>
  explicit maybe(const CpDataT &cobj):obj(new DataT(cobj)){}
  template<typename CpDataT>
  maybe &operator=(const CpDataT &cobj){delete obj; obj=new DataT(cobj); return *this;}
  template<typename CpDataT>
  maybe(const maybe<CpDataT> &cp):obj(cp.is_just()?new DataT(cp.from_just()):nullptr){}
  template<typename CpDataT>
  maybe &operator=(const maybe<CpDataT> &cp){
    delete obj;  obj = cp.is_just()? new DataT(cp.from_just()) : nullptr; return *this;}
  maybe(maybe<DataT> &&mv):obj(mv.obj){mv.obj=nullptr;}
  maybe &operator=(maybe<DataT> &&mv) {
    delete obj; obj=mv.obj; mv.obj=nullptr; return *this; }
#else
  maybe(const nothing_object &n):obj(nullptr){}
  explicit maybe(DataT cobj):obj(new DataT(std::move(cobj))){}
  maybe &operator=(DataT cobj){delete obj; obj=new DataT(std::move(cobj)); return *this;}
  template<typename CpDataT>
  maybe(const maybe<CpDataT> &cp):obj(cp.is_just()?new DataT(cp.from_just()):nullptr){}
  template<typename CpDataT>
  maybe &operator=(const maybe<CpDataT> &cp){
    delete obj;  obj = cp.is_just()? new DataT(cp.from_just()) : nullptr; return *this;}
  maybe(maybe<DataT> &&mv)noexcept :obj(mv.obj) {mv.obj=nullptr;}
  maybe &operator=(maybe<DataT> &&mv) noexcept            {
    delete obj; obj=mv.obj; mv.obj=nullptr; return *this; }
#endif

  ~maybe(){delete obj;}
};

template<typename DataT>
auto just(DataT obj) -> maybe<DataT> { return maybe<DataT>(std::move(obj)); }

template<typename MpFnT, typename DataT>              // represents Haskell's <$> infix
auto operator^(MpFnT f, const maybe<DataT> &m) -> decltype(m.fmapped(f)) {
  return m.fmapped(f);
}

template<typename DataT>
auto joined(const maybe<maybe<DataT>> &m) -> maybe<DataT> {
  return m.is_just()? m.from_just() : nothing;
}


template<typename DataT>
auto maybe_yes(const std::pair<DataT,bool>& mbcst) -> maybe<DataT> {
  return mbcst.second ? just(mbcst.first) : nothing;
}
template<typename DataT>
auto maybe_not(const std::pair<DataT,bool>& mbcst) -> maybe<DataT> {
  return !mbcst.second ? just(mbcst.first) : nothing;
}


template<typename DataT, typename MonadicFn>auto
operator>>=(maybe<DataT>&& dsrc, MonadicFn f) -> decltype(f(std::declval<DataT>())) {
  return dsrc.monadicbind_destructive(f);                                           }



}


#endif