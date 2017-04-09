#pragma once

#include "smart_ptr.h"
#include "ViewerCohort.h"
#include "Enemy.h"
#include <array>
#include <vector>

PTR( ViewerCohort );

class ViewerCohort {
public:
	ViewerCohort();
	virtual ~ViewerCohort();
public:
	void draw( );
private:
	struct FORM {
		std::array< int, Enemy::MAX_MOTION > id;
	};
	struct GROUP {
		struct ENEMY {
			struct ID {
				int body_id;
				int anim_id;
			};
			std::array< ID, Enemy::MAX_MOTION > id;
		};
		std::vector< ENEMY > enemy;
		int num;
	};
private:
	void init( );
	void createModel( );
	void create( Enemy::TYPE type, Enemy::MOTION motion, const char * filename );
	void drawEnemy( EnemyConstPtr enemy );
private:
	std::array< GROUP, Enemy::MAX_TYPE > _group;
};

