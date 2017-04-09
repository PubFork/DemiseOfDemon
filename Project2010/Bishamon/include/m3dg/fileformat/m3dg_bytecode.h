#ifndef M3DG_FILEFORMAT_M3DG_BYTECODE_H
#define M3DG_FILEFORMAT_M3DG_BYTECODE_H

namespace m3dg{
namespace fileformat{

enum VertexFormatConst{
	//
	VertexFormatConst_BitP	= 1<<0,
	VertexFormatConst_BitN	= 1<<1,
	VertexFormatConst_BitC	= 1<<2,
	VertexFormatConst_BitT	= 1<<3,
	//
	VertexFormatConst_PN	= VertexFormatConst_BitP | VertexFormatConst_BitN,
	VertexFormatConst_PNT	= VertexFormatConst_BitP | VertexFormatConst_BitN                          | VertexFormatConst_BitT,
	VertexFormatConst_PNC	= VertexFormatConst_BitP | VertexFormatConst_BitN | VertexFormatConst_BitC,
	VertexFormatConst_PNCT	= VertexFormatConst_BitP | VertexFormatConst_BitN | VertexFormatConst_BitC | VertexFormatConst_BitT,

	//
	VertexFormatConst_FORCE32	= 0xFFFFFFFF
};

enum ByteCodeOpecodeConst{
	//																サイズ	要素の型
	//																		F:float
	//																		I:Int
	//																		B:Binary
	//																		S:String
	//																		W:Work

	// 事務的な
	ByteCodeOpecodeConst_End,									//	0

	// マテリアル関係
	ByteCodeOpecodeConst_SetMaterial,							//	17		F,F,F,F	AmbientColor
																//			F,F,F,F	DiffuseColor
																//			F,F,F,F	SpecularColor
																//			F,F,F,F	EmissionColor
																//			F		Shininess

	// 頂点配列の描画
	ByteCodeOpecodeConst_DrawArray_PN,							//	7		I,B,B,B,F,B,B
																//				0:頂点数
																//				1:頂点インデックス
																//				2:位置データ
																//				3:法線データ
																//				4:すべての三角形面積
																//				5:三角形加算面積
																//				6:バウンディングスフィア
	ByteCodeOpecodeConst_DrawArray_PNT,							//	9		I,B,B,B,B,S,F,B,B
																//				0:頂点数
																//				1:頂点インデックス
																//				2:位置データ
																//				3:法線データ
																//				4:テクスチャデータ
																//				5:テクスチャ名
																//				6:すべての三角形面積
																//				7:三角形加算面積
																//				8:バウンディングスフィア
	ByteCodeOpecodeConst_DrawArray_PNC,							//	8		I,B,B,B,B,F,B,B
																//				0:頂点数
																//				1:頂点インデックス
																//				2:位置データ
																//				3:法線データ
																//				4:カラー
																//				5:すべての三角形面積
																//				6:三角形加算面積
																//				7:バウンディングスフィア
	ByteCodeOpecodeConst_DrawArray_PNCT,						//	10		I,B,B,B,B,S,B,F,B,B
																//				0:頂点数
																//				1:頂点インデックス
																//				2:位置データ
																//				3:法線データ
																//				4:カラー
																//				5:テクスチャデータ
																//				6:テクスチャ名
																//				7:すべての三角形面積
																//				8:三角形加算面積
																//				9:バウンディングスフィア

	// インターリーブされた頂点配列の描画(DirectX9)
	ByteCodeOpecodeConst_DrawInterleavedArray_DirectX9,			//	9		I,I,B,B,I,S,F,B,B
																//				0:VertexFormatConst
																//				1:頂点数
																//				2:頂点インデックス
																//				3:頂点データ
																//				4:頂点ストライド
																//				5:テクスチャ名
																//				6:すべての三角形面積
																//				7:三角形加算面積
																//				8:バウンディングスフィア

	// インターリーブされた頂点配列バッファでの描画(DirectX9)
	ByteCodeOpecodeConst_DrawInterleavedArrayBuffer_DirectX9,	//	11		I,I,B,B,I,S,W,W,F,B,B
																//				0:VertexFormatConst
																//				1:頂点数
																//				2:頂点インデックス
																//				3:頂点データ
																//				4:頂点ストライド
																//				5:テクスチャ名
																//				6:頂点バッファ
																//				7:インデックスバッファ
																//				8:すべての三角形面積
																//				9:三角形加算面積
																//				10:バウンディングスフィア
	//
	ByteCodeOpecodeConst_Max,
	//
	ByteCodeOpecodeConst_FORCE32		= 0xFFFFFFFF
};

enum ByteCodeOperandSizeConst{
	ByteCodeOperandSizeConst_End									= 0,
	ByteCodeOperandSizeConst_SetMaterial							= 17,
	ByteCodeOperandSizeConst_DrawArray_PN							= 7,
	ByteCodeOperandSizeConst_DrawArray_PNT							= 9,
	ByteCodeOperandSizeConst_DrawArray_PNC							= 8,
	ByteCodeOperandSizeConst_DrawArray_PNCT							= 10,
	ByteCodeOperandSizeConst_DrawInterleavedArray_DirectX9			= 9,
	ByteCodeOperandSizeConst_DrawInterleavedArrayBuffer_DirectX9	= 11
};

}	// namespace fileformat
}	// namespace m3dg

#endif	// #ifndef M3DG_FILEFORMAT_M3DG_BYTECODE_H
