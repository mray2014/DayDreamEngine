#pragma once
#include "DreamMath.h"
#include <iostream>

class DreamUnitTest {
public:
	template<typename T>
	static void ClassUnitTest(const char* testName, T answer, T result, bool shouldSucceed = true) {
		printf("=========================\n");
		printf(testName);
		printf("\n=========================\n");

		if (!shouldSucceed) {
			printf("[SHOULD FAIL!!!]\n");
		}

		printf("ANSWER:\n");
		printf(answer.ToString().c_str()); printf("\n\n");

		printf("RESULT:\n");
		printf(result.ToString().c_str()); printf("\n\n");

		if ((answer == result) == shouldSucceed) {
			printf("---------------------------[SUCCEEDED!]");
		}
		else {
			printf("---------------------------[FAILED!]");
		}

		printf("\n\n");
	}

	static void FloatUnitTest(const char* testName, float answer, float result, bool shouldSucceed = true) {
		printf("=========================\n");
		printf(testName);
		printf("\n=========================\n");

		if (!shouldSucceed) {
			printf("[SHOULD FAIL!!!]\n");
		}

		printf("ANSWER:\n");
		printf("%f", answer); printf("\n\n");

		printf("RESULT:\n");
		printf("%f", result); printf("\n");

		if ((DreamMath::abs(answer - result) <= EPSILON) == shouldSucceed) {
			printf("---------------------------[SUCCEEDED!]");
		}
		else {
			printf("---------------------------[FAILED!]");
		}

		printf("\n\n");

		//assert(answer == result);
	}

	static void PrintUnitTestRunName(const char* testRunName) {
		printf("\n\n=======================================================================\n");
		printf("///////////////////////////////////////////////////////////////////////\n");
		printf(testRunName);
		printf("\n///////////////////////////////////////////////////////////////////////\n");
		printf("=======================================================================\n\n");
	}
};