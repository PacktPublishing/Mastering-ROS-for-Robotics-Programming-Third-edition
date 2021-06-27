#ifndef PLUGINLIB_CALCULATOR_CALCULTOR_BASE_H_
#define PLUGINLIB_CALCULATOR_CALCULTOR_BASE_H_

namespace calculator_base 
{
  class CalcFunctions
  {
    public:
      virtual void getNumbers(double number1, double number2) = 0;
      virtual double operation() = 0;
      virtual ~CalcFunctions(){}

    protected:
      CalcFunctions(){}
  };
};
#endif
