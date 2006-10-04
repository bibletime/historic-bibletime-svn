/***************************************************************************
 *   Copyright (C) 2006 by Martin Gruner   *
 *   mg.pub@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/* Small speed test program for the versification schema database of BibleTime2.*/

//QT includes
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QDir>
#include <QTime>

#include <vector>
#include <iostream>

class db_error{};

QString map_osisID(QSqlDatabase& dbh, QString& from, QString& to, QString& osisID){
	QSqlQuery select = dbh.exec( QString("SELECT target from ").append(from).append("_to_").append(to).append(" WHERE source='").append(osisID).append("'"));
	if (select.lastError().isValid()){
		qDebug() << select.lastError();
		throw db_error();
	}
	if (select.first()){ 
		return select.value(0).toString();
	}
	else 
		return osisID;
}

void testQuerySpeed(void){
	QSqlDatabase mapper_db = QSqlDatabase::addDatabase("QSQLITE", "v11n-mapper");
	mapper_db.setDatabaseName("db/v11n_mapper.db");
	QSqlDatabase schema_db = QSqlDatabase::addDatabase("QSQLITE", "v11n-schema");
	schema_db.setDatabaseName("db/v11n_schema.db");

	try{	
		//open db connections
		if (!mapper_db.open()){
			qDebug() << "Could not connect to mapper database!\n";
			throw db_error();
		}
		if (!schema_db.open()){
			qDebug() << "Could not connect to schema database!\n";
			throw db_error();
		}

		QTime clock;

		//First get all osisIDs from Bible schema
		clock.start();
		QStringList osisIDs;
		QSqlQuery getOsisIDs = schema_db.exec("SELECT osisID from bible");
		if (getOsisIDs.lastError().isValid()){
			qDebug() << getOsisIDs.lastError();
			throw db_error();
		}
		while (getOsisIDs.next()){
			osisIDs.append( getOsisIDs.value(0).toString() );
		}
		unsigned int osisIDcount = osisIDs.count();
		qDebug() << "Finished getting all osisIDs from schema bible (" << osisIDcount << ") in " << clock.elapsed() << " ms." ;

		clock.restart();
		qDebug() << QTime::currentTime() << "mapping all osisIDs from bible to bible_vul";
		QString mapped_id;
		QString from = "bible";
		QString to = "bible_vul";
		for (unsigned int i=0; i<osisIDcount; ++i){
			mapped_id = map_osisID(mapper_db, from, to, osisIDs[i] );
		}
		qDebug() << "Finished mapping all osisIDs from bible to bible_vul in " << clock.elapsed() << " ms.";

	}
	catch (...){
		qDebug() << "An error occurred. Exiting.";
		schema_db.close();
		mapper_db.close();
		QFile::remove("db/v11n_mapper.db");
		QFile::remove("db/v11n_schema.db");
		throw;
	}

	schema_db.close();
	mapper_db.close();
}

void testDBSpeed(void){
	std::vector <QSqlDatabase> db_vector;
	QString input;
	int dummy;
	qDebug() << "1";
	std::cin >> dummy;
	QTime clock;
	qDebug() << QTime::currentTime() << "trying to open 1000 db connections";
	clock.start();
	for (int i=0; i<1000; ++i){
		db_vector.push_back( QSqlDatabase::addDatabase("QSQLITE", QString("v11n-schema-").append(QString::number(i))) );
		db_vector[i].setDatabaseName(QString("db/%1.db").arg(i));
		if (!db_vector[i].open()){
			qDebug() << "Could not connect to database " << QString("db/%1.db").arg(i);
			throw db_error();
		}
	}
	qDebug() << "Finished opening all connections in " << clock.elapsed() << " ms.";

	qDebug() << "2";
	std::cin >> dummy;

	//qDebug() << "These databases are open:" << QSqlDatabase::connectionNames().join(", ");

	QSqlDatabase schema_db = QSqlDatabase::addDatabase("QSQLITE", "v11n-schema");
	schema_db.setDatabaseName("db/v11n_schema.db");
	schema_db.open();

	QStringList osisIDs;
	QSqlQuery getOsisIDs = schema_db.exec("SELECT osisID from bible");
	if (getOsisIDs.lastError().isValid()){
		qDebug() << getOsisIDs.lastError();
		throw db_error();
	}
	while (getOsisIDs.next()){
		osisIDs.append( getOsisIDs.value(0).toString() );
	}
	unsigned int osisIDcount = osisIDs.count();

	qDebug() << QTime::currentTime() << "Measuring query performance";
	clock.restart();
	QString osisID;
	for (int i=0; i<osisIDcount; ++i){
		qDebug() << "Database :"<< i;
//		QSqlQuery getOsisID = db_vector[1].exec( QString("SELECT osisID from bible WHERE osisID='").append(osisIDs[i]).append("'") );
		QSqlQuery getOsisID = db_vector[i % 1000].exec( QString("SELECT osisID from bible") );
//		QSqlQuery getOsisID = db_vector[i % 1000].exec( QString("SELECT osisID from bible WHERE osisID='").append(osisIDs[i]).append("'") );
		while (getOsisID.next()){
			osisID = getOsisID.value(0).toString();
		}
//		qDebug() << QString("SELECT osisID from bible WHERE osisID='").append(osisIDs.at(i)).append("'");
	}
	qDebug() << "Finished 38K SELECTs on 1000 DBs in " << clock.elapsed() << " ms.";

	qDebug() << "3";
	std::cin >> dummy;


}

int main(int argc, char *argv[])
{
	try{
		testDBSpeed();
	}
	catch (...){
		qDebug() << "An error occurred. Exiting.";
		throw;
	}
	
	return EXIT_SUCCESS;
}
