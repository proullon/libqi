/*
** AUTOGENERATED CODE, DO NOT EDIT
**
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/

#ifndef I_WANT_HXX
#error("do not include hxx in your code")
#endif

#ifndef  AL_MESSAGING_FUNCTOR_HXX_
# define AL_MESSAGING_FUNCTOR_HXX_

# include <alcommon-ng/functor/functor.hpp>

namespace AL
{

  template < typename R>
  class Functor_0 : public Functor
  {
  public:
    typedef R(*FunctionType) ();

    Functor_0(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()() {
      return (R)(*fFunction)();
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 0);
      result.value((*fFunction)());
    };

  private:
    FunctionType  fFunction;
  };


  template <typename P0,  typename R>
  class Functor_1 : public Functor
  {
  public:
    typedef R(*FunctionType) (const P0 &p0);

    Functor_1(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()(const P0 &p0) {
      return (R)(*fFunction)(p0);
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 1);
      result.value((*fFunction)(params[0].as<P0>()));
    };

  private:
    FunctionType  fFunction;
  };


  template <typename P0, typename P1,  typename R>
  class Functor_2 : public Functor
  {
  public:
    typedef R(*FunctionType) (const P0 &p0, const P1 &p1);

    Functor_2(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()(const P0 &p0, const P1 &p1) {
      return (R)(*fFunction)(p0, p1);
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 2);
      result.value((*fFunction)(params[0].as<P0>(), params[1].as<P1>()));
    };

  private:
    FunctionType  fFunction;
  };


  template <typename P0, typename P1, typename P2,  typename R>
  class Functor_3 : public Functor
  {
  public:
    typedef R(*FunctionType) (const P0 &p0, const P1 &p1, const P2 &p2);

    Functor_3(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()(const P0 &p0, const P1 &p1, const P2 &p2) {
      return (R)(*fFunction)(p0, p1, p2);
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 3);
      result.value((*fFunction)(params[0].as<P0>(), params[1].as<P1>(), params[2].as<P2>()));
    };

  private:
    FunctionType  fFunction;
  };


  template <typename P0, typename P1, typename P2, typename P3,  typename R>
  class Functor_4 : public Functor
  {
  public:
    typedef R(*FunctionType) (const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3);

    Functor_4(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()(const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3) {
      return (R)(*fFunction)(p0, p1, p2, p3);
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 4);
      result.value((*fFunction)(params[0].as<P0>(), params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>()));
    };

  private:
    FunctionType  fFunction;
  };


  template <typename P0, typename P1, typename P2, typename P3, typename P4,  typename R>
  class Functor_5 : public Functor
  {
  public:
    typedef R(*FunctionType) (const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4);

    Functor_5(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()(const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) {
      return (R)(*fFunction)(p0, p1, p2, p3, p4);
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 5);
      result.value((*fFunction)(params[0].as<P0>(), params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>(), params[4].as<P4>()));
    };

  private:
    FunctionType  fFunction;
  };


  template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5,  typename R>
  class Functor_6 : public Functor
  {
  public:
    typedef R(*FunctionType) (const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5);

    Functor_6(FunctionType pFunction)
      : fFunction(pFunction)
    {}

    R operator()(const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5) {
      return (R)(*fFunction)(p0, p1, p2, p3, p4, p5);
    }

    void call(const AL::Messaging::ArgumentList &params, AL::Messaging::ReturnValue& result) {
      AL_FUNCTOR_ASSUME_NBR_PARAMS(params, 6);
      result.value((*fFunction)(params[0].as<P0>(), params[1].as<P1>(), params[2].as<P2>(), params[3].as<P3>(), params[4].as<P4>(), params[5].as<P5>()));
    };

  private:
    FunctionType  fFunction;
  };

}
#endif
