#ifndef M3DG_MODEL_M3DG_ANIMATION_H
#define M3DG_MODEL_M3DG_ANIMATION_H

namespace m3dg{
namespace model{

enum {
	KEYFRAME_CONSTANT = 0,
	KEYFRAME_LINEAR,
	KEYFRAME_BEZIER,
	KEYFRAME_HERMITE
};

// Keyframe 32byte
struct Keyframe {
Keyframe() {
	type = -1;
	outCount = 1;
	reserved = 0;
}
// 0x00
int		type, outCount;
int		reserved;
float	in;
// 0x10
float	inTanX, inTanY;
float	outTanX, outTanY;
// 0x20
float	out[16];
// 0x60
};

// Curve
struct Curve {
Curve() { keys = 0; managed = 0; count = 0; size = 0; /*in = 0;*/ }
~Curve() { if(keys) { if(managed) {delete [] keys;} keys = 0; } }
void Create(int count) { keys = new Keyframe[count]; this->count = count; managed = 1; size = sizeof(Keyframe)*count; }
inline float Interpolate(float in);
inline float *Interpolate(float in, float *out);

int			count;
int			size;
int			managed;
//float		in;
Keyframe	*keys;
};

#define ANIMATION_CURVE_COUNT	10

// Animation
struct Animation {
Animation() { target[0] = 0; }
#ifdef CONVERT
Curve curves[ANIMATION_CURVE_COUNT];
#else
Curve *curves[16];
#endif
char	target[256];
};



inline float linear(float p0, float p1, float s) {
	return p0+s*(p1-p0);
}

inline float bezier(float p0, float p1, float p2, float p3, float s) {
	return p0*(1-s)*(1-s)*(1-s) + 3*p1*s*(s-1)*(s-1) + 3*p2*s*s*(1-s) + p3*s*s*s;
}

inline float hermite(float p0, float t0, float p1, float t1, float s) {
	float s2 = s*s;
	float s3 = s2*s;
	return p0*(2*s3-3*s2+1) + t0*(s3-2*s2+s) + p1*(-2*s3+3*s2) + t1*(s3-s2);
}


inline float constantKey(Keyframe *key1, Keyframe *key2, float in) {
	return (in < key2->in) ? key1->out[0] : key2->out[0];
}

inline float *constantKey(Keyframe *key1, Keyframe *key2, float in, float *out) {
	for(int i=0; i<16; i++) {
		out[i] = (in < key2->in) ? key1->out[i] : key2->out[i];
	}
	return out;
}

inline float linearKey(Keyframe *key1, Keyframe *key2, float in) {
	float s = (in - key1->in) / (key2->in - key1->in);
	return linear(key1->out[0], key2->out[0], s);
}

inline float *linearKey(Keyframe *key1, Keyframe *key2, float in, float *out) {
	for(int i=0; i<16; i++) {
		float s = (in - key1->in) / (key2->in - key1->in);
		out[i] = linear(key1->out[i], key2->out[i], s);
	}
	return out;
}

inline float bezierKey(Keyframe *key1, Keyframe *key2, float in) {
	float s = (in - key1->in) / (key2->in - key1->in);
	s = bezier(key1->in, key1->outTanX, key2->inTanX, key2->in, s);
	s = (s - key1->in) / (key2->in - key1->in);
	return bezier(key1->out[0], key1->outTanY, key2->inTanY, key2->out[0], s);
}

inline float hermiteKey(Keyframe *key1, Keyframe *key2, float in) {
	float s = (in - key1->in) / (key2->in - key1->in);
	s = hermite(key1->in, key1->outTanX, key2->in, key2->inTanX, s);
	s = (s - key1->in) / (key2->in - key1->in);
	return hermite(key1->out[0], key1->outTanY, key2->out[0], key2->inTanY, s);
}


inline float Curve::Interpolate(float in) {

	Keyframe *key1 = 0;
	Keyframe *key2 = 0;

	if(this->count <= 0) return 0;
	else if(this->count == 1) return this->keys[0].out[0];

	// Check Under
	if(in < this->keys->in) {
		return this->keys->out[0];
	}
	// Check Over
	else if(this->keys[this->count-1].in < in) {
		return this->keys[this->count-1].out[0];
	}

	for(int i=1; i<this->count; i++) {
		if(in <= this->keys[i].in) {
			key2 = &this->keys[i];
			key1 = key2-1;
			break;
		}
	}

	switch(key1->type) {
	case KEYFRAME_CONSTANT:	return constantKey(key1, key2, in);
	case KEYFRAME_LINEAR:	return linearKey(key1, key2, in);
	case KEYFRAME_BEZIER:	return bezierKey(key1, key2, in);
	case KEYFRAME_HERMITE:	return hermiteKey(key1, key2, in);
	}

	return 0.f;

}

inline float *Curve::Interpolate(float in, float *out) {

	Keyframe *key1 = 0;
	Keyframe *key2 = 0;

	if(this->count <= 0) return 0;
	else if(this->count == 1) return this->keys[0].out;

	// Check Under
	if(in < this->keys->in) {
		return this->keys->out;
	}
	// Check Over
	else if(this->keys[this->count-1].in < in) {
		return this->keys[this->count-1].out;
	}

	for(int i=1; i<this->count; i++) {
		if(in <= this->keys[i].in) {
			key2 = &this->keys[i];
			key1 = key2-1;
			break;
		}
	}

	// マトリックスのBezier/Hermiteは未対応(Linearで置き換え)
	switch(key1->type) {
	case KEYFRAME_CONSTANT:	return constantKey(key1, key2, in, out);
	case KEYFRAME_LINEAR:	return linearKey(key1, key2, in, out);
	case KEYFRAME_BEZIER:	return linearKey(key1, key2, in, out);
	case KEYFRAME_HERMITE:	return linearKey(key1, key2, in, out);
	}

	out[ 0] = 1; out[ 1] = 0; out[ 2] = 0; out[ 3] = 0;
	out[ 4] = 0; out[ 5] = 1; out[ 6] = 0; out[ 7] = 0;
	out[ 8] = 0; out[ 9] = 0; out[10] = 1; out[11] = 0;
	out[12] = 0; out[13] = 0; out[14] = 0; out[15] = 1;
	return out;

}


}
}

#endif
