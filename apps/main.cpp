#include "ASMParser.h"
#include "ConfigParser.h"
#include "MemoryFileParser.h"

#include "Processor.h"

#include <bitset>
#include <iostream>
#include <string>


int main(int argc, char const *argv[])
{
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " [config_file]";
    return 0;
  }

  //----------------------------------------------------------------------------
  // Parsing input files

  // Read config input file
  ConfigParser config_parser;
  struct Configs configs = config_parser(argv[1]);

  // Translate input program to binary code
  ASMParser assembler;
  std::vector<Instruction>   instructions = assembler(configs.program_input);
  std::vector<unsigned long> binary_code;
  binary_code.reserve(instructions.size());
  for (auto& i : instructions)
    binary_code.push_back( std::bitset<32>(i.getEncoding()).to_ulong() );

  // read memory input file
  MemoryFileParser mem_parser;
  struct MemoryContent mem_content = mem_parser(configs.memory_contents_input);

  // read register input file
  struct MemoryContent reg_content = mem_parser(configs.register_file_input);

  //----------------------------------------------------------------------------
  // Init processor
  Processor processor(binary_code, 
                      reg_content.data,
                      mem_content.data, 
                      mem_content.start_address);

  std::ostream out_stream;
  if (configs.write_to_file)
    out_stream = std::ofstream(configs.output_file);
  else 
    out_stream = std::cout;

  if (!out_stream.good()) {
    std::cerr << "Failed to setup logging stream" << std::endl;
    exit(1);
  }

  Logger logger(out_stream);
  processor.setLogger(&logger);

  //----------------------------------------------------------------------------
  // Run
  int i = 1;
  while (!processor.isFinished()) {
    logger.log("====================================================================");
    logger.log(string("Cycle ") + to_string(i));
    processor.step();
    i++;
  }

  return 0;
}
