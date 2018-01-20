/*
	This file is part of the Geometry library.
	Copyright (C) 2010-2013 Benjamin Eikel <benjamin@eikel.org>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <fstream>

int main(int argc, char ** argv) {
	const std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

	CppUnit::TestResult controller;

	CppUnit::TestResultCollector result;
	controller.addListener(&result);

	CppUnit::BriefTestProgressListener progress;
	controller.addListener(&progress);

	CppUnit::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());

	runner.run(controller, testPath);

	std::ofstream fileStream("cppunit_results.xml");
	CppUnit::XmlOutputter xmlOutput(&result, fileStream, "UTF-8");
	xmlOutput.write();

	CppUnit::TextOutputter textOutput(&result, std::cout);
	textOutput.write();

	return result.wasSuccessful() ? 0 : 1;
}
