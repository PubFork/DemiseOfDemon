/*
 BMObject.h
 BISHAMON sample object
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef __BM_OBJECT_H__
#define __BM_OBJECT_H__

class BMObject {
public:
	BMObject() { ref=1; }
	virtual ~BMObject() {}

	virtual int Retain() { return ++ref; }
	virtual void Release() { if(--ref < 1) delete this; }
	virtual int RetainCount() { return ref; }

private:
	int ref;
};

#endif /* __BM_OBJECT_H__ */
