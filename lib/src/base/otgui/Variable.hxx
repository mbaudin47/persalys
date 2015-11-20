// Variable.hxx

#ifndef VARIABLE_H
#define VARIABLE_H

#include <PersistentObject.hxx>

namespace OTGUI {
class Variable : public OT::PersistentObject
{
  CLASSNAME
public:
  Variable();
  Variable(const std::string & name);
  Variable(const std::string & name, const double & value, const std::string & description="");
  Variable(const Variable & other);
  virtual Variable * clone() const = 0;

    /* String converter */
  virtual OT::String __repr__() const;

  virtual ~Variable();

  std::string getName() const;
  void setName(const std::string & name);

  double getValue() const;
  void setValue(const double & value);

  std::string getDescription() const;
  void setDescription(const std::string & description);

  virtual std::string dump() const = 0;

private:
  std::string name_;
  double value_;
  std::string description_;
};
}
#endif