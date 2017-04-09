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

///	@brief	メモリーストリームクラス
class memory_stream{

public:
	typedef unsigned int uint32_t;
	typedef std::basic_string<uint32_t, std::char_traits<uint32_t>,std::allocator<uint32_t> > u32string;
	
	
	///	@brief				コンストラクタ
	memory_stream();

	///	@brief				デストラクタ
	virtual ~memory_stream();

	///	@brief				メモリーを得る
	///	@return				メモリーへのポインタ
	template<typename T>
	T		memory() const{	return T((size() == 0) ? NULL : &buffer_[0]);	}

	///	@brief				サイズを得る
	///	@return				サイズ
	int		size() const{		return buffer_.size();		}

	///	@brief				サイズを設定する
	///	@param[in]	size	サイズ
	void	set_size(int size){	buffer_.resize(size);		}

	///	@brief				同じがどうかを比較する
	///	@return				true / false
	bool	is_same(const memory_stream &src) const;

	///	@brief				サイズをアライメントする
	///	@param[in]	align	アライメント単位
	void	align(int align);

	///	@brief				値を書き込む
	///	@param[in]	t		値
	template<typename T>
	void	write(const T &t){	write(&t, sizeof(t));	}

	///	@brief					バッファを書き込む
	///	@param[in]	buffer		バッファー
	///	@param[in]	byte_size	バッファーのバイトサイズ
	void	write(const void *buffer, int byte_size);

	///	@brief					メモリーストリームを書き込む
	///	@param[in]	src			メモリーストリーム
	void	write(const memory_stream &src);

	///	@brief					文字列を書き込む
	///	@param[in]	src			文字列
	void	write(const std::string &src);

	///	@brief					文字列を書き込む
	///	@param[in]	src			文字列
	void	write(const std::wstring &src);

	///	@brief					文字列を書き込む
	///	@param[in]	src			文字列
	void	write(const u32string &src);
	
	///	@brief					ファイルに書き込む
	///	@param[in]	filename	出力先ファイル名
	virtual void	save_to_file(const std::string &filename) const;

	///	@brief					ファイルに書き込む
	///	@param[in]	filename	出力先ファイル名
	virtual void	save_to_file(const std::wstring &filename) const;

	///	@brief					ファイルから読み込む
	///	@param[in]	filename	入力元ファイル名
	///	@return					読み込みの成否
	virtual bool	load_from_file(const std::string &filename);

	///	@brief					ファイルから読み込む
	///	@param[in]	filename	入力元ファイル名
	///	@return					読み込みの成否
	virtual bool	load_from_file(const std::wstring &filename);

	///	@brief					比較する
	///	@param[in]	m			比較対象
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
