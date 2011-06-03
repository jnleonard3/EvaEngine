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

#include "TestEvaVector2D.h"

#include "eva/math/evaMathDefines.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestEvaVector2D);

static const e_float32 ACCURACY = 0.01f;

void TestEvaVector2D::setUp()
{
	zeroVector = new eva::Vector2Df(0.0f,0.0f);
	a = new eva::Vector2Df(3.0f,2.0f);
	b = new eva::Vector2Df(-7.0f,1.0f);
	c = new eva::Vector2Df(2.0f,0.0f);
}

void TestEvaVector2D::tearDown()
{
	delete zeroVector;
	delete a;
	delete b;
	delete c;
}

void TestEvaVector2D::constructorTest()
{
	eva::Vector2Df tmp(2.0,4.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tmp[0], ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, tmp[1], ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tmp.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, tmp.j(), ACCURACY);
}

void TestEvaVector2D::normalizeTest()
{
	eva::Vector2Df normZero = *zeroVector;
	normZero.normalize();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, normZero.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, normZero.j(), ACCURACY);
	c->normalize();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, c->i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, c->j(), ACCURACY);
}

void TestEvaVector2D::applyMatrixTest()
{
	eva::Matrix<e_float32> badMatrix(0,0);
	a->applyMatrix(badMatrix);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, a->i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a->j(), ACCURACY);
	e_float32 matrixVals[] = {0.0, -1.0, 1.0, 0.0};
	eva::Matrix<e_float32> rotationMatrix(2,2,matrixVals);
	c->applyMatrix(rotationMatrix);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, c->i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, c->j(), ACCURACY);
	eva::Vector2Df rotC = c->rotate(eva::E_PI/2.0f);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, rotC.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rotC.j(), ACCURACY);
}

void TestEvaVector2D::additionCompoundAssignmentTest()
{
	eva::Vector2Df addA = *a;
	addA += *b;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, addA.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, addA.j(), ACCURACY);
}

void TestEvaVector2D::subtractionCompundAssignmentTest()
{
	eva::Vector2Df addA = *a;
	addA -= *b;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, addA.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, addA.j(), ACCURACY);
}

void TestEvaVector2D::multiplicationCompoundAssignmentTest()
{
	eva::Vector2Df mult = *a;
	mult *= 2.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, mult.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mult.j(), ACCURACY);
}

void TestEvaVector2D::isZeroTest()
{
	CPPUNIT_ASSERT(zeroVector->isZero());
	CPPUNIT_ASSERT(!a->isZero());
}

void TestEvaVector2D::magnitudeTest()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, zeroVector->magnitude(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, c->magnitude(), ACCURACY);
}

void TestEvaVector2D::dotProductTest()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL(a->dotProduct(*c), 6.0, ACCURACY);
}

void TestEvaVector2D::angleBetweenTest()
{
	eva::Vector2Df up(0.0,10.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(up.angleBetween(*c), eva::E_PI/2, ACCURACY);
}

void TestEvaVector2D::toMatrixTest()
{
	eva::Matrix<e_float32> matrix = b->toMatrix();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.0, matrix.at(0), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, matrix.at(1), ACCURACY);
}

void TestEvaVector2D::additionTest()
{
	eva::Vector2Df add = *a + *b;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, add.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, add.j(), ACCURACY);
}

void TestEvaVector2D::subtractionTest()
{
	eva::Vector2Df sub = *a - *b;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, sub.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, sub.j(), ACCURACY);
}

void TestEvaVector2D::multiplicationTest()
{
	eva::Vector2Df mult = (*a) * 2.0;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, mult.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mult.j(), ACCURACY);
}

void TestEvaVector2D::angleTest()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, c->angle(), ACCURACY);
}

void TestEvaVector2D::rotateTest()
{
	eva::Vector2Df rotC = c->rotate(eva::E_PI/2.0f);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rotC.i(), ACCURACY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, rotC.j(), ACCURACY);
}
