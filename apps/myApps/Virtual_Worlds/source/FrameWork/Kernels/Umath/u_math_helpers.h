#pragma once

inline 	float unit_signf(float value) {
		if (value == 0.0f) return 0;
		if (value < 0.0f)  return -1.0f;
		if (value > 0.0f)  return 1.0f;
}

inline 	int unit_signi(int value) {
		if (value == 0) return 0;
		if (value < 0)  return 1;
		if (value > 0)  return 1;
}