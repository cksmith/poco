//
// Transaction.cpp
//
// $Id: //poco/Main/Data/src/Transaction.cpp#1 $
//
// Library: Data
// Package: DataCore
// Module:  Transaction
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Data/Transaction.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Data {


Transaction::Transaction(Poco::Data::Session& rSession, Poco::Logger* pLogger):
	_rSession(rSession),
	_pLogger(pLogger)
{
	begin();
}


Transaction::Transaction(Poco::Data::Session& rSession, bool start):
	_rSession(rSession),
	_pLogger(0)
{
	if (start) begin();
}

	
Transaction::~Transaction()
{
	if (_rSession.isTransaction())
	{
		try
		{
			if (_pLogger) 
				_pLogger->debug("Rolling back transaction.");

			_rSession.rollback();
		}
		catch (...)
		{
			if (_pLogger) 
				_pLogger->error("Error while rolling back database transaction.");
		}
	}
}


void Transaction::begin()
{
	if (!_rSession.isTransaction())
		_rSession.begin();
	else
		throw InvalidAccessException("Transaction in progress.");
}


void Transaction::execute(const std::string& sql, bool doCommit)
{
	if (!_rSession.isTransaction()) _rSession.begin();
	_rSession << sql, Keywords::now;
	if (doCommit) commit();
}


void Transaction::execute(const std::vector<std::string>& sql)
{
	try
	{
		std::vector<std::string>::const_iterator it = sql.begin();
		std::vector<std::string>::const_iterator end = sql.end();
		for (; it != end; ++it)	execute(*it, it + 1 == end ? true : false);
		return;
	}
	catch (Exception& ex)
	{
		if (_pLogger) _pLogger->error(ex.displayText());
	}

	rollback();
}


void Transaction::commit()
{
	if (_pLogger) 
		_pLogger->debug("Committing transaction.");

	_rSession.commit();
}

	
void Transaction::rollback()
{
	if (_pLogger) 
		_pLogger->debug("Rolling back transaction.");

	_rSession.rollback();
}


} } // namespace Poco::Data
