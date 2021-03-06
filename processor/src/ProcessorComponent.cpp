#include "ProcessorComponent.h"

ProcessorComponent::
ProcessorComponent(int _n_inputs, int _n_outputs)
        : m_num_inputs(_n_inputs), m_num_outputs(_n_outputs)
{
  m_output_lines = new std::vector<OutputComponent> [m_num_outputs];
}


ProcessorComponent::
~ProcessorComponent()
{
  delete[] m_output_lines;
}


void
ProcessorComponent::
addOutputComponent(int _output_id,
        ProcessorComponent& _output_component, int _input_id)
{
  OutputComponent out_line = {_output_component, _input_id};
  m_output_lines[_output_id].push_back(out_line);
}


void
ProcessorComponent::
fireAllOutputs()
{
  // Get the bit of each output line of current component
  for (int i = 0; i < numOutputs(); i++)
  {
    bool output_bit = getOutput(i);

    // Set the bits for the output components' input lines
    for (OutputComponent& line : m_output_lines[i])
    {
      line.output_component.setInput(line.input_id, output_bit);
    }
  }
}


void
ProcessorComponent::
setLogger(Logger* _logger)
{
  m_logger = _logger;
}
