#include "Structures.h"

float getMagn(Vec3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vec3 normalize(Vec3 vec) {
	float magn = getMagn(vec);
	return { vec.x / magn, vec.y / magn,vec.z / magn };
}