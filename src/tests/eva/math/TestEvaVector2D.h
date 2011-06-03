/*
 *  Copyright (c) 2011, EvaEngine Project
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE EVAENGINE PROEJCT BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TEST_EVA_VECTOR2D_H_
#define TEST_EVA_VECTOR2D_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "eva/math/evaVector.h"

class TestEvaVector2D : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestEvaVector2D);
    CPPUNIT_TEST(constructorTest);
    CPPUNIT_TEST(normalizeTest);
    CPPUNIT_TEST(applyMatrixTest);
    CPPUNIT_TEST(additionCompoundAssignmentTest);
    CPPUNIT_TEST(subtractionCompundAssignmentTest);
    CPPUNIT_TEST(multiplicationCompoundAssignmentTest);
    CPPUNIT_TEST(isZeroTest);
    CPPUNIT_TEST(magnitudeTest);
    CPPUNIT_TEST(dotProductTest);
    CPPUNIT_TEST(angleBetweenTest);
    CPPUNIT_TEST(toMatrixTest);
    CPPUNIT_TEST(additionTest);
    CPPUNIT_TEST(subtractionTest);
    CPPUNIT_TEST(multiplicationTest);
    CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

	protected:
		// Abstract Vector Methods
		void constructorTest();
		void normalizeTest();
		void applyMatrixTest();
		void additionCompoundAssignmentTest();
		void subtractionCompundAssignmentTest();
		void multiplicationCompoundAssignmentTest();
		void isZeroTest();
		void magnitudeTest();
		void dotProductTest();
		void angleBetweenTest();
		void toMatrixTest();
		void additionTest();
		void subtractionTest();
		void multiplicationTest();

		// Vector 2D Methods
		void angleTest();
		void rotateTest();

	private:
		eva::Vector2Df *zeroVector;
		eva::Vector2Df *a, *b, *c;
};

#endif
