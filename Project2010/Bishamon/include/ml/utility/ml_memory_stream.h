#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_MEMORY_STREAM_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_MEMORY_STREAM_H

#include <vector>
#include <string>
#include "../type/ml_basic.h"

namespace ml{


///	@addtogroup	utility
///	@{

///	@addtogroup	utility_memory_stream
///	@{

///	@brief	�������[�X�g���[���N���X
class memory_stream{

public:
	typedef unsigned int uint32_t;
	typedef std::basic_string<uint32_t, std::char_traits<uint32_t>,std::allocator<uint32_t> > u32string;
	
	
	///	@brief				�R���X�g���N�^
	memory_stream();

	///	@brief				�f�X�g���N�^
	virtual ~memory_stream();

	///	@brief				�������[�𓾂�
	///	@return				�������[�ւ̃|�C���^
	template<typename T>
	T		memory() const{	return T((size() == 0) ? NULL : &buffer_[0]);	}

	///	@brief				�T�C�Y�𓾂�
	///	@return				�T�C�Y
	int		size() const{		return buffer_.size();		}

	///	@brief				�T�C�Y��ݒ肷��
	///	@param[in]	size	�T�C�Y
	void	set_size(int size){	buffer_.resize(size);		}

	///	@brief				�������ǂ������r����
	///	@return				true / false
	bool	is_same(const memory_stream &src) const;

	///	@brief				�T�C�Y���A���C�����g����
	///	@param[in]	align	�A���C�����g�P��
	void	align(int align);

	///	@brief				�l����������
	///	@param[in]	t		�l
	template<typename T>
	void	write(const T &t){	write(&t, sizeof(t));	}

	///	@brief					�o�b�t�@����������
	///	@param[in]	buffer		�o�b�t�@�[
	///	@param[in]	byte_size	�o�b�t�@�[�̃o�C�g�T�C�Y
	void	write(const void *buffer, int byte_size);

	///	@brief					�������[�X�g���[������������
	///	@param[in]	src			�������[�X�g���[��
	void	write(const memory_stream &src);

	///	@brief					���������������
	///	@param[in]	src			������
	void	write(const std::string &src);

	///	@brief					���������������
	///	@param[in]	src			������
	void	write(const std::wstring &src);

	///	@brief					���������������
	///	@param[in]	src			������
	void	write(const u32string &src);
	
	///	@brief					�t�@�C���ɏ�������
	///	@param[in]	filename	�o�͐�t�@�C����
	virtual void	save_to_file(const std::string &filename) const;

	///	@brief					�t�@�C���ɏ�������
	///	@param[in]	filename	�o�͐�t�@�C����
	virtual void	save_to_file(const std::wstring &filename) const;

	///	@brief					�t�@�C������ǂݍ���
	///	@param[in]	filename	���͌��t�@�C����
	///	@return					�ǂݍ��݂̐���
	virtual bool	load_from_file(const std::string &filename);

	///	@brief					�t�@�C������ǂݍ���
	///	@param[in]	filename	���͌��t�@�C����
	///	@return					�ǂݍ��݂̐���
	virtual bool	load_from_file(const std::wstring &filename);

	///	@brief					��r����
	///	@param[in]	m			��r�Ώ�
	///	@return					true / false
	bool operator<(const memory_stream &m) const{
		return buffer_ < m.buffer_;
	}

protected:
	std::vector<ml::ui8>	buffer_;
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_STRING_H
