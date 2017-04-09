#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_PACKET_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_PACKET_H

#include <ml/type/ml_basic.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace prim{

/// @brief Packet�\����
struct Packet{
/*
	ml::si32			count_;			// �v���~�e�B�u��
	ml::si32			byte_size_;		// �v���~�e�B�u�������̃o�C�g�T�C�Y
	ml::si32			active_count_;	// �A�N�e�B�u�ȃv���~�e�B�u�̐�
	ml::ui32			updated_frame_;	// �X�V���s�����t���[����
*/
	ml::ni::matrix44	generate_time_hierarchy_parent_matrix_;	// �������̐e�̍s��
	ml::ni::vector3d	generate_time_hierarchy_parent_matrix_row_length_;	// �������̐e�̍s��

	// �ȉ��A�ό�

	// �A�N�e�B�u�ȃv���~�e�B�u��
	//	active_primitive[0] ���� active_primitive[active_count_ - 1] �܂ł��L��
//	PrimitiveType	*active_primitive[count_];

	// �v���~�e�B�u��
//	PrimitiveType	primitive_[count_];
};

}	// namespace prim
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_PACKET_H
