//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include "SoftmaxLayer.hpp"

#include "LayerCloneBase.hpp"

#include <armnn/TypesUtils.hpp>
#include <backends/WorkloadData.hpp>
#include <backends/WorkloadFactory.hpp>

namespace armnn
{

SoftmaxLayer::SoftmaxLayer(const SoftmaxDescriptor &param, const char* name)
    : LayerWithParameters(1, 1, LayerType::Softmax, param, name)
{
}

std::unique_ptr<IWorkload> SoftmaxLayer::CreateWorkload(const Graph& graph, const IWorkloadFactory& factory) const
{
    SoftmaxQueueDescriptor descriptor;
    return factory.CreateSoftmax(descriptor, PrepInfoAndDesc(descriptor, graph));
}

SoftmaxLayer* SoftmaxLayer::Clone(Graph& graph) const
{
    return CloneBase<SoftmaxLayer>(graph, m_Param, GetName());
}

void SoftmaxLayer::ValidateTensorShapesFromInputs()
{
    ConditionalThrow<LayerValidationException>(GetInputSlot(0).GetConnection() != nullptr,
                                               "SoftmaxLayer: Input slot must be connected.");

    const TensorShape& outShape = GetInputSlot(0).GetConnection()->GetTensorInfo().GetShape();
    ConditionalThrowIfNotEqual<LayerValidationException>(
        "SoftmaxLayer: TensorShape set on OutputSlot[0] does not match the inferred shape.",
        GetOutputSlot(0).GetTensorInfo().GetShape(),
        outShape);
}

} // namespace armnn
