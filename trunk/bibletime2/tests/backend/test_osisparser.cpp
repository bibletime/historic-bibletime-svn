#include "test_osisparser.h"
#include "backend/osisparser.h"

#include <QCoreApplication>

using namespace backend;
using namespace QTest;

void TestOsisParser::test_parse_data() {
	addColumn<QString>("key");
	addColumn<QString>("result");

	newRow("1") << "Gen 1:2" << "Gen.1.2";
}

void TestOsisParser::test_parse() {
	QFETCH(QString, key);
	QFETCH(QString, result);

	QCOMPARE(parse_reflist(key), result);
}

QTEST_MAIN(TestOsisParser)

#include "test_osisparser.moc"
