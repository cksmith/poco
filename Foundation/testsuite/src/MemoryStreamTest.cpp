//
// MemoryStreamTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/MemoryStreamTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "MemoryStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MemoryStream.h"


using Poco::MemoryInputStream;
using Poco::MemoryOutputStream;


MemoryStreamTest::MemoryStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


MemoryStreamTest::~MemoryStreamTest()
{
}


void MemoryStreamTest::testInput()
{
	const char* data = "This is a test";
	MemoryInputStream istr1(data, 14);
	
	char c = istr1.get();
	assert (c == 'T');
	c = istr1.get();
	assert (c == 'h');
	
	std::string str;
	istr1 >> str;
	assert (str == "is");
	
	char buffer[32];
	istr1.read(buffer, sizeof(buffer));
	assert (istr1.gcount() == 10);
	buffer[istr1.gcount()] = 0;
	assert (std::string(" is a test") == buffer);
	
	const char* data2 = "123";
	MemoryInputStream istr2(data2, 3);
	c = istr2.get();
	assert (c == '1');
	assert (istr2.good());
	c = istr2.get();
	assert (c == '2');
	istr2.unget();
	c = istr2.get();
	assert (c == '2');
	assert (istr2.good());
	c = istr2.get();
	assert (c == '3');
	assert (istr2.good());
	c = istr2.get();
	assert (c == -1);
	assert (istr2.eof());
}


void MemoryStreamTest::testOutput()
{
	char output[64];
	MemoryOutputStream ostr1(output, 64);
	ostr1 << "This is a test " << 42 << std::ends;
	assert (ostr1.charsWritten() == 18);
	assert (std::string("This is a test 42") == output);
	
	char output2[4];
	MemoryOutputStream ostr2(output2, 4);
	ostr2 << "test";
	assert (ostr2.good());
	ostr2 << 'x';
	assert (ostr2.fail());
}


void MemoryStreamTest::setUp()
{
}


void MemoryStreamTest::tearDown()
{
}


CppUnit::Test* MemoryStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MemoryStreamTest");

	CppUnit_addTest(pSuite, MemoryStreamTest, testInput);
	CppUnit_addTest(pSuite, MemoryStreamTest, testOutput);

	return pSuite;
}