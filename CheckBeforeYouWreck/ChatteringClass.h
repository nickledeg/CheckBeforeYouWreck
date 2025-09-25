// ReSharper disable CppIncorrectBlankLinesNearBraces
#pragma once

#include <iostream>
using std::cout;

class ChatteringClass {

  inline static size_t count_{};
  size_t n_{};

public:
  ChatteringClass() :
    n_{count_++} {
    cout
      << "Instance " << n_ <<
      " created from scratch.\n";
  }

  ChatteringClass(ChatteringClass const& x) :
    n_{count_++} {

    cout
      << "Instance " << n_ <<
      " created as copy of instance "
      << x.n_ << ".\n";
  }

  ChatteringClass& operator=(ChatteringClass const& x) {

    if (&x != this)
      cout
        << "Instance " << x.n_ <<
        " copied into instance " << n_ << ".\n";

    return *this;
  }

  ChatteringClass(ChatteringClass&& x) noexcept :
    n_{count_++} {

    try {
      cout
        << "Instance " << n_ <<
        " created by stealing from instance "
        << x.n_ << ".\n";
    }
    catch (...) {
      std::abort();
    }
  }

  ChatteringClass& operator=(ChatteringClass&& x) noexcept {

    try {
      if (&x != this)
        cout
          << "Instance " << n_ <<
          " steals contents from instance "
          << x.n_ << ".\n";
    }
    catch (...) {
      std::abort();
    }

    return *this;
  }

  ~ChatteringClass() noexcept {

    try {
      cout << "Instance " << n_ << " destroyed.\n";
      --count_;
    }
    catch (...) {
      std::abort();
    }
  }
};