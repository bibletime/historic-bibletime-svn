#include <QtTest/QTest>
#include <QObject>

#ifndef TEST_OSISPARSER
#define TEST_OSISPARSER

class TestOsisParser : public QObject {
  Q_OBJECT

private slots:
  void test_parse();
  void test_parse_data();
};

#endif
