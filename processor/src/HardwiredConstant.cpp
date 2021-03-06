#include "HardwiredConstant.h"


HardwiredConstant::
HardwiredConstant(unsigned long _constant, int _n_bits)
  : ProcessorComponent(NUM_INPUTS, _n_bits)
{
  m_outputs = new bool[_n_bits];
  for (int i = 0; i < _n_bits; i++) {
    m_outputs[i] = _constant & 1UL; // Last thing in constant
    _constant >>= 1;                // Update last thing in constant
  }
}


HardwiredConstant::
HardwiredConstant(bool* _constant, int _n_bits)
  : ProcessorComponent(NUM_INPUTS, _n_bits)
{
  m_outputs = new bool[_n_bits];
  for (int i = 0; i < _n_bits; i++)
    m_outputs[i] = _constant[i];
}


HardwiredConstant::
~HardwiredConstant()
{
  delete[] m_outputs;
}


void
HardwiredConstant::
setInput(int _line_id, bool _clock)
{
  if (_clock)
    updateOutput();
}


bool
HardwiredConstant::
getOutput(int _line_id)
{
  return m_outputs[_line_id];
}


void
HardwiredConstant::
updateOutput()
{
  fireAllOutputs();
}
