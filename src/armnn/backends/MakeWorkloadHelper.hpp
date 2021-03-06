//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#pragma once

namespace armnn
{
namespace
{

// Make a workload of the specified WorkloadType
template<typename WorkloadType>
struct MakeWorkloadForType
{
    template<typename QueueDescriptorType, typename... Args>
    static std::unique_ptr<WorkloadType> Func(const QueueDescriptorType& descriptor,
                                              const WorkloadInfo& info,
                                              Args&&... args)
    {
        return std::make_unique<WorkloadType>(descriptor, info, std::forward<Args>(args)...);
    }
};

// Specialization for void workload type used for unsupported workloads.
template<>
struct MakeWorkloadForType<NullWorkload>
{
    template<typename QueueDescriptorType, typename... Args>
    static std::unique_ptr<NullWorkload> Func(const QueueDescriptorType& descriptor,
                                              const WorkloadInfo& info,
                                              Args&&... args)
    {
        return nullptr;
    }
};

// Makes a workload for one the specified types based on the data type requirements of the tensorinfo.
// Specify type void as the WorkloadType for unsupported DataType/WorkloadType combos.
template <typename Float32Workload, typename Uint8Workload, typename QueueDescriptorType, typename... Args>
std::unique_ptr<IWorkload> MakeWorkload(const QueueDescriptorType& descriptor, const WorkloadInfo& info, Args&&... args)
{
    const DataType dataType = !info.m_InputTensorInfos.empty() ?
        info.m_InputTensorInfos[0].GetDataType()
        : info.m_OutputTensorInfos[0].GetDataType();

    BOOST_ASSERT(info.m_InputTensorInfos.empty() || info.m_OutputTensorInfos.empty()
        || info.m_InputTensorInfos[0].GetDataType() == info.m_OutputTensorInfos[0].GetDataType());

    switch (dataType)
    {
        case DataType::Float32:
            return MakeWorkloadForType<Float32Workload>::Func(descriptor, info, std::forward<Args>(args)...);
        case DataType::QuantisedAsymm8:
            return MakeWorkloadForType<Uint8Workload>::Func(descriptor, info, std::forward<Args>(args)...);
        default:
            BOOST_ASSERT_MSG(false, "Unknown DataType.");
            return nullptr;
    }
}

} //namespace
} //namespace armnn
