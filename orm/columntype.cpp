//
// Created by drin on 1/11/25.
//

#include "columntype.h"

std::ostream& operator<<(std::ostream& os, const FieldGetter& variant) {
  struct {
    std::ostream& os;

    void operator()(int value) const { os << value; }
    void operator()(double value) const { os << value; }
    void operator()(const std::string_view value) const {
      os << "'" << value << "'";
    }
    void operator()(const ForeignKeyRef& ref) const { os << ref.get().first; }

    void operator()(const IntOpt& opt) const { output_optional(os, opt); }
    void operator()(const DoubleOpt& opt) const { output_optional(os, opt); }
    void operator()(const StrViewOpt& opt) const { output_optional(os, opt); }

  } visitor{os};

  std::visit(visitor, variant);
  return os;
}
