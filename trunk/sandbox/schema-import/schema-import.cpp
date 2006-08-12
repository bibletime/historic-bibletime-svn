/***************************************************************************
 *   Copyright (C) 2006 by Martin Gruner   *
 *   mg.pub@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/* The purpose of this program is to read in versification schema and mapping data
	provided by ccel and turn it into a sqlite3 database to be used by other programs
	such as BibleTime2. */


//libstdc++ includes
//#include <string>

//QT includes
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QHash>
#include <QtDebug>
#include <QDir>

class file_error{};
class db_error{};

QStringList readFile(QString name){
	QFile file(name);
	QStringList result;
	if (file.open(QFile::ReadOnly)){
		QTextStream in(&file);
		while (!in.atEnd()){
			result.append(in.readLine());
		}
		file.close();
		return result;
	}
	else{
		qWarning() << "COULD NOT OPEN FILE!\n";
		throw file_error();
	}
}

void feedSchema(QSqlDatabase& dbh, QString table, QStringList& data){

	QHash<QString, bool> hash;
	qDebug() << "Feeding data for schema " << table << "into db.";
	
	dbh.transaction();
	//create database schema
	dbh.exec(QString("CREATE TABLE %1 (osisID TEXT NOT NULL UNIQUE)").arg(table));
	data = data.filter(QString("<osisID code")).replaceInStrings(QRegExp("^.*\"(.+)\".*$"), QString("\\1"));
	QStringList::iterator i;
	for (i = data.begin(); i != data.end(); ++i){
		if (!hash.contains(*i)){ //check for uniqueness
			QSqlQuery insert = dbh.exec(QString("INSERT INTO %1 VALUES (\'%2\')").arg(table).arg(*i));
			if (insert.lastError().isValid()){
				qDebug() << "SQL Error: " << insert.lastError().text();
				throw db_error();
			}
			hash[*i]=true;
		}
	}
	dbh.commit();
}

void feedMapping(QSqlDatabase& dbh, QString source, QString target, QStringList& source_data, QStringList& target_data){

	QHash<QString, bool> hash;
	qDebug() << "Feeding mapping from " << source << " to " << target << " into db.";

	dbh.transaction();
	dbh.exec(QString("CREATE TABLE %1_to_%2 (source TEXT NOT NULL UNIQUE, target TEXT NOT NULL)").arg(source).arg(target));
	QStringList::iterator i_source = source_data.begin();
	QStringList::iterator i_target = target_data.begin();
	while (i_source != source_data.end()){
		if (((*i_source) != (*i_target)) && !hash.contains( (*i_source) ) ){ //check for uniqueness, only store if source and target differ
			QSqlQuery insert = dbh.exec(QString("INSERT INTO %1_to_%2 VALUES (\'%3\', \'%4')").arg(source).arg(target).arg(*i_source).arg(*i_target));
			if (insert.lastError().isValid()){
				qDebug() << "SQL Error: " << insert.lastError().text();
				throw db_error();
			}

			hash[*i_source]=true;
		}
	++i_source; 
	++i_target;
	}
	dbh.commit();
}

int main(int argc, char *argv[])
{
	{
		//set application's dir as current dir
		QDir program_dir( QDir::currentPath() );
		program_dir.mkdir("db");
		//remove old dbs
		QFile::remove("db/v11n_mapper.db");
		QFile::remove("db/v11n_schema.db");
	}

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
	
		QStringList data= readFile("data/Bible.xml");
		feedSchema(schema_db, "bible", data);
	
		data = readFile("data/Bible.NAB.xml");
		feedSchema(schema_db, "bible_nab", data);
	
		data = readFile("data/Bible.NJB.xml");
		feedSchema(schema_db, "bible_njb", data);
	
		data = readFile("data/Bible.ORG.xml");
		feedSchema(schema_db, "bible_org", data);
	
		data = readFile("data/Bible.Vul.xml");
		feedSchema(schema_db, "bible_vul", data);
	
		data = readFile("data/Bible.LXX.xml");
		feedSchema(schema_db, "bible_lxx", data);
	
		data = readFile("data/refsysmap.unl");
		QStringList schemata;
		schemata << "Bible" << "Bible.NAB" << "Bible.NJB" << "Bible.ORG" << "Bible.Vul" << "Bible.LXX";
		QStringList::iterator i1,i2;
		for (i1 = schemata.begin(); i1 != schemata.end(); ++i1){
			for (i2 = schemata.begin(); i2 != schemata.end(); ++i2){
				if ((*i1) == (*i2)) continue;
				if (((*i1) != "Bible") && ((*i2) != "Bible")) continue;
				QString source_table = (*i1).toLower().replace(QRegExp("\\."), "_");
				QString target_table = (*i2).toLower().replace(QRegExp("\\."), "_");
				QStringList mapping_data = data.filter( QRegExp( (*i1)+":.+:"+(*i2) ) );
				QStringList source_data, target_data, mapSplit;
				QStringList::iterator i_mapping;
				for (i_mapping = mapping_data.begin(); i_mapping != mapping_data.end(); i_mapping++){
					mapSplit = (*i_mapping).split(":");
					source_data.append(mapSplit[1]);
					target_data.append(mapSplit[3]);
				}
				feedMapping(mapper_db, source_table, target_table, source_data, target_data); 
			}
		}
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
	
	return EXIT_SUCCESS;
}
