#pragma once

#include "TopoNodeInput.h"
#include "TopoNodeOutput.h"

#include "utility/print_utils.h"

#include "base/vector.h"

namespace uit {

class TopoNode {

  using inputs_t = emp::vector<uit::TopoNodeInput>;
  using outputs_t = emp::vector<uit::TopoNodeOutput>;

  inputs_t inputs;
  outputs_t outputs;

public:

  TopoNode() = default;
  TopoNode(const inputs_t& inputs_, const outputs_t& outputs_)
  : inputs(inputs_), outputs(outputs_)
  { ; }

  const inputs_t& GetInputs() const { return inputs; }

  const outputs_t& GetOutputs() const { return outputs; }

  void AddInput(const uit::TopoNodeInput& input_) {
    inputs.push_back(input_);
  }

  void AddOutput(const uit::TopoNodeOutput& output_) {
    outputs.push_back(output_);
  }

  size_t GetNumInputs() const { return inputs.size(); }

  size_t GetNumOutputs() const { return outputs.size(); }

  bool HasInputs() const { return inputs.size(); }

  bool HasOutputs() const { return outputs.size(); }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member(
      "emp::vector<uit::TopoNodeInput> inputs",
      uit::to_string(inputs)
    ) << std::endl;
    ss << format_member(
      "emp::vector<uit::TopoNodeInput> inputs",
      uit::to_string(outputs)
    );
    return ss.str();
  }

  friend std::ostream& operator<<(std::ostream&, const TopoNode&);
  friend std::istream& operator>>(std::istream&, TopoNode&);

  }
};

std::ostream& operator<<(std::ostream& os, const TopoNode& node) {
  const auto& outputs = node.GetOutputs();
  for (const auto& node : std::span<const node::input_t>(
    outputs.cbegin(),
    outputs.cend() - 1
  )) {
    os << node.GetEdgeID() << " ";
  }
  os << outputs.back().GetEdgeID();
  return os;
}

std::istream& operator>>(std::istream& is, TopoNode& node) {
  size_t input;
  for (is >> input) {
    node.AddInput(input);
  }
  return is;
}

}
