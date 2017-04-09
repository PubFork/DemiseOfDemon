#pragma once

// ポート番号
static const int TCP_PORT_NUM = 9850;
static const int UDP_PORT_NUM = 9900;

// ネットワークの状態確認時間
static const int NETWORK_STATE_TIME = 1200;

// TCP送信データ
const unsigned char COMMAND_DUMMY		= 0xF0; // ダミー
const unsigned char COMMAND_CONDITION	= 0xE0; // 接続確認
const unsigned char COMMAND_STATE		= 0x10; // State変更
const unsigned char COMMAND_BGM			= 0x20; // BGMの設定	
const unsigned char COMMAND_SE			= 0x30; // SEの登録
const unsigned char COMMAND_LEVEL       = 0x41; // LEVEL変更
const unsigned char COMMAND_HEAL		= 0x42; // ライフ回復
const unsigned char COMMAND_LIFE        = 0x43; // ライフ回復
const unsigned char COMMAND_DAMAGE      = 0x44; // ダメージ処理
const unsigned char COMMAND_RETURNENTRY = 0x45; // ベースキャンプ帰還
const unsigned char COMMAND_CREATE      = 0x46; // 新規作成
const unsigned char COMMAND_CLEAR       = 0x47; //
const unsigned char COMMAND_EXP         = 0x48; //


/*
数値の意味
0 - 15 device番号
0x20   READY
0x21   WAIT
0x22   NONE
*/
const unsigned char DEVICE_READY        = 0x20; // 128
const unsigned char DEVICE_WAIT			= 0x21; //デバイス登録待ち
const unsigned char DEVICE_NONE			= 0x22; //デバイスを承認しない

const unsigned char SOUND_NONE					= 0x00;	// SEなし
const unsigned char SOUND_SE_JUMP				= 0x01;	// ジャンプ
const unsigned char SOUND_SE_PLAYER_APPEARANCE	= 0x02; // プレイヤー出現
const unsigned char SOUND_SE_PLAYER_LANDING		= 0x03; // 着地
const unsigned char SOUND_SE_PLAYER_SWORD		= 0x04; // ソード
const unsigned char SOUND_SE_PLAYER_NONA		= 0x05; // ノバ 
const unsigned char SOUND_SE_PLAYER_SHOT		= 0x06; // ショット
const unsigned char SOUND_SE_PLAYER_ARROW		= 0x07; // アロー
const unsigned char SOUND_SE_PLAYER_LAY			= 0x08; // レイ
const unsigned char SOUND_SE_PLAYER_DAMAGE		= 0x09; // ダメージ
const unsigned char SOUND_SE_PLAYER_HEAL		= 0x0a; // 回復
const unsigned char SOUND_SE_PLAYER_LIFE_COUNT	= 0x0b; // ライフカウント
const unsigned char SOUND_SE_PLAYER_LIFE_COUNT_ANOUNCE	= 0x0c; // ライフのアナウンス
const unsigned char SOUND_SE_PLAYER_LIFE_COUNT_ZERO	= 0x0d; // ライフゼロ
const unsigned char SOUND_SE_AREA_CLEAR = 0x0e;			//エリアクリア
const unsigned char SOUND_SE_WEAPON_CHANGE		= 0x0f; //武器チェンジ
const unsigned char SOUND_SE_WEAPON_LEVEL_UP	= 0x10; // 武器レベルアップ
const unsigned char SOUND_SE_DOOR_OPEN			= 0x11; // ドアオープン
const unsigned char SOUND_SE_DEEDBOX_OPEN		= 0x12; // 宝箱オープン
const unsigned char SOUND_SE_GAME_OVER			= 0x13; // ゲームオーバー
const unsigned char SOUND_SE_GAME_CLEAR			= 0x14; // ゲームクリア	
const unsigned char SOUND_SE_AREA_WARP			= 0x15; // エリアワープ
const unsigned char SOUND_SE_DEEDBOX_GET_ITEM	= 0x16; // 宝箱ゲット
const unsigned char SOUND_SE_BOSS_CLEAVE		= 0x17; // 薙ぎ払い
const unsigned char SOUND_SE_BOSS_APPEAR_RUMBLING	= 0x18; // 地響き
const unsigned char SOUND_SE_BOSS_APPEAR_ROAR	= 0x19; // 雄叫び
const unsigned char SOUND_SE_BOSS_LOWHIT		= 0x1a; // 突きさし
const unsigned char SOUND_SE_BOSS_CHANT			= 0x1b; // 詠唱
const unsigned char SOUND_SE_BOSS_MAGIC			= 0x1c; // 魔法

const int PLAYER_NUM = 6;
const int STATUS_NUM = 16;	//最大デバイス数
const unsigned char PLAYER_KNIGHT      = 0;
const unsigned char PLAYER_WARRIOR     = 1;
const unsigned char PLAYER_MAGE        = 2;
const unsigned char PLAYER_MONK        = 3;
const unsigned char PLAYER_SHOOTER     = 4;
const unsigned char PLAYER_SUMMONER	   = 5;

const unsigned char STATE_NONE         = 0x00; // なにもしない
const unsigned char STATE_READY        = 0x01; // エントリー待機
const unsigned char STATE_DEAD         = 0x02; // 死亡
const unsigned char STATE_CLEAR        = 0x03; // クリア
const unsigned char STATE_CAMP         = 0x10; // CAMPにいる
const unsigned char STATE_LABYRINTH	   = 0x20; // RABYRINTHにいる
const unsigned char STATE_DEATHPARADE  = 0x21; // DEATHPARADEにいる
const unsigned char STATE_BOSS         = 0x30; // Bossにいる
const unsigned char STATE_TEST         = 0xa0; // TESTにいる
const unsigned char STATE_ERROR        = 0xff; // よきしないエラー
const unsigned char WEAPON_SHOT        = 0x10; //ITEM種類は消す？
const unsigned char WEAPON_LAY         = 0x20;
const unsigned char WEAPON_NOVA        = 0x30;
const unsigned char WEAPON_ARROW       = 0x40;
const unsigned char WEAPON_SWORD       = 0x50;
const unsigned char STAGE_NONE         = 0x00;

const unsigned char BUTTON_A = 0x01; // INPUT_A ( 0b00000001 )
const unsigned char BUTTON_B = 0x02; // INPUT_B ( 0b00000010 )
const unsigned char BUTTON_C = 0x04; // INPUT_C ( 0b00000100 )
const unsigned char BUTTON_D = 0x08; // INPUT_X ( 0b00001000 )

const unsigned char STAGE_LABYRINTH = 0x00;
const unsigned char STAGE_DEATHPARADE = 0x01;
const unsigned char STAGE_BOSS = 0x02;
const unsigned char STAGE_MAX = 0x03;

const unsigned char LIFE_LEVEL_MAX = 0x09;

const int MAX_MACHINE = 15;

#pragma pack( 1 )
	struct SERVERDATA {
		unsigned char command; // コマンド
		unsigned char index;   // プレイヤー
		int value;   // データ
	};

	struct CLIENTDATA {
		struct USER {
			unsigned int  life;
			char x; // -100 +100
			char y; // -100 +100
			unsigned char device;
			unsigned char button; // 8ボタン
			unsigned char client; // 現在いるクライアント
			unsigned char state;
			unsigned char level_weapon;
			unsigned char level_life;
			unsigned int exp;
		};
		USER user[ STATUS_NUM ];
		unsigned char stage[ STAGE_MAX ];
	};
#pragma pack( )
	