/***************************************************************************
 *   Copyright (C) 2006 by Martin Gruner   *
 *   mg.pub@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//libstdc++ includes
#include <string>
#include <stdio.h>
#include <stdlib.h>

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

void test(){

/*	printf("available drivers: %s\n", QSqlDatabase::drivers().join(", ").toStdString().c_str() );

	printf("db tables %s\n", schema_db.tables().join(", ").toStdString().c_str() );
	if (!schema_db.isValid()) printf("db invalid\n");

//	for (int i=1; i++; i>1000){
		QString QueryString = QString("SELECT * FROM bible"); 
		QSqlQuery query( QueryString, schema_db );
		query.exec();
		if (!query.isActive()) {
			printf("db error %s\n",schema_db.lastError().text().toStdString().c_str());
			printf("qery error %s\n",query.lastError().text().toStdString().c_str());
		}
		//printf("%i", i);
		printf("%s", QueryString.toStdString().c_str() );
		while (query.next()) {
			printf("%s", query.value(0).toString().toStdString().c_str() );
		}
//	}*/
}

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
		printf("COULD NOT OPEN FILE!\n");
		return QStringList();
	}
}

void feedSchema(QSqlDatabase& dbh, QString table, QStringList& data){

	QHash<QString, bool> hash;
	printf("Feeding data for schema %s into db.\n", table.toStdString().c_str() );
	
	dbh.transaction();
	//create database schema
	dbh.exec(QString("CREATE TABLE %1 (osisID TEXT NOT NULL UNIQUE)").arg(table));
	data = data.filter(QRegExp("<osisID code")).replaceInStrings(QRegExp("^.*\"(.+)\".*$"), QString("\\1"));
	QStringList::iterator i;
	for (i = data.begin(); i != data.end(); ++i){
		if (!hash.contains(*i)){ //check for uniqueness
			dbh.exec(QString("INSERT INTO %1 VALUES (\'%2\')").arg(table).arg(*i));
			hash[*i]=true;
		}
	}
	dbh.commit();
}

void feedMapping(QSqlDatabase& dbh, QString source, QString target, QStringList& source_data, QStringList& target_data){

	QHash<QString, bool> hash;
	printf("Feeding mapping from %s to %s into db.\n", source.toStdString().c_str(), target.toStdString().c_str() );

	dbh.transaction();
	dbh.exec(QString("CREATE TABLE %1_to_%2 (source TEXT NOT NULL UNIQUE, target TEXT NOT NULL)").arg(source).arg(target));
	QStringList::iterator i_source = source_data.begin();
	QStringList::iterator i_target = target_data.begin();
	while (i_source != source_data.end()){
		if (((*i_source) != (*i_target)) && !hash.contains( (*i_source) ) ){ //check for uniqueness, only store if source and target differ
			dbh.exec(QString("INSERT INTO %1_to_%2 VALUES (\'%3\', \'%4')").arg(source).arg(target).arg(*i_source).arg(*i_target));
			hash[*i_source]=true;
		}
	++i_source; 
	++i_target;
	}
	dbh.commit();
}

int main(int argc, char *argv[])
{
	//remove old dbs
	QFile::remove("db/v11n_mapper.db");
	QFile::remove("db/v11n_schema.db");
	//open db connections
	QSqlDatabase mapper_db = QSqlDatabase::addDatabase("QSQLITE", "v11n-mapper");
	mapper_db.setDatabaseName("db/v11n_mapper.db");
	if (!mapper_db.open()) printf("Could not connect to mapper database!\n");
	QSqlDatabase schema_db = QSqlDatabase::addDatabase("QSQLITE", "v11n-schema");
	schema_db.setDatabaseName("db/v11n_schema.db");
	if (!schema_db.open()) printf("Could not connect to schema database!\n");

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
	
	return EXIT_SUCCESS;
}
