// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

#ifndef WINVER					// �Œ���K�v�ȃv���b�g�t�H�[���� Windows Vista �ł��邱�Ƃ��w�肵�܂��B
#define WINVER	0x0600			// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINNT			// �Œ���K�v�ȃv���b�g�t�H�[���� Windows Vista �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_WINNT 0x0600		// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_WINDOWS          // �Œ���K�v�ȃv���b�g�t�H�[���� Windows 98 �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_WINDOWS 0x0410	// ����� Windows Me �܂��͂���ȍ~�̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#ifndef _WIN32_IE				// �Œ���K�v�ȃv���b�g�t�H�[���� Internet Explorer 7.0 �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_IE 0x0700		// ����� IE �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B

// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>

// C �����^�C�� �w�b�_�[ �t�@�C��
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
