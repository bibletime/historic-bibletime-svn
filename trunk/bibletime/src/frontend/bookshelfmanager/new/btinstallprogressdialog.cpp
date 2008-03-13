/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "btinstallprogressdialog.h"
#include "btinstallprogressdialog.moc"

#include "btinstallthread.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QHeaderView>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>

#include <QDebug>


BtInstallProgressDialog::BtInstallProgressDialog(QTreeWidget* selectedModulesTreeWidget, QString destination)
	: QDialog()
{
	//create the dialog which shows the status and lets the user stop installation
	m_statusWidget = new QTreeWidget();
	m_statusWidget->setRootIsDecorated(false);
	m_statusWidget->setHeaderLabels(QStringList(tr("Work")) << tr("Progress") << QString::null);
	m_statusWidget->header()->setStretchLastSection(false);

	m_statusWidget->header()->setResizeMode(1, QHeaderView::Stretch);

	foreach (QTreeWidgetItem* sourceItem, selectedModulesTreeWidget->invisibleRootItem()->takeChildren()) {
		foreach (QTreeWidgetItem* moduleItem, sourceItem->takeChildren()) {
			if (moduleItem->checkState(0) == Qt::Checked) {
				qDebug("");
				// create a thread for this module
				BtInstallThread* thread = new BtInstallThread(moduleItem->text(0), sourceItem->text(0), destination);
				m_threads.append(thread);
				// progress widget/item
				QProgressBar* bar = new QProgressBar(m_statusWidget);
				bar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
				QPushButton* stopButton = new QPushButton(tr("Stop"), m_statusWidget);
				stopButton->setFixedSize(stopButton->sizeHint());

				// the item
				QTreeWidgetItem* progressItem = new QTreeWidgetItem(m_statusWidget);

				progressItem->setSizeHint(2, stopButton->sizeHint());
				progressItem->setText(0, moduleItem->text(0));
				progressItem->setFlags(Qt::ItemIsEnabled);

				m_statusWidget->setItemWidget(progressItem, 1, bar);
				m_statusWidget->setItemWidget(progressItem, 2, stopButton);
				bar->setValue(0);
				bar->setVisible(false);
				progressItem->setText(1, tr("Connecting..."));
				//connect the signals between the dialog, items and threads
				QObject::connect(stopButton, SIGNAL(clicked()), thread, SLOT(slotStopInstall()) );
				QObject::connect(thread, SIGNAL(installStopped(QString)), this, SLOT(slotOneItemStopped(QString)));
				//is this needed or is statusUpdated enough?
				QObject::connect(thread, SIGNAL(installCompleted(QString, int)), this, SLOT(slotOneItemCompleted(QString)));
				QObject::connect(thread, SIGNAL(statusUpdated(QString, int)), this, SLOT(slotStatusUpdated(QString, int)));
				
				qDebug("");
			}
		}
	}

	QPushButton* stopAllButton = new QPushButton(tr("Stop All"), this);
	
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_statusWidget);
	layout->addWidget(stopAllButton);

	connect(stopAllButton, SIGNAL(clicked()), SLOT(slotStopInstall()) );

	qApp->processEvents();

	// start threads
	qDebug() << "start all threads...";
	foreach (QThread* t, m_threads) {
		t->start();
	}
}


BtInstallProgressDialog::~BtInstallProgressDialog()
{
}


void BtInstallProgressDialog::slotOneItemCompleted(QString module)
{
	qDebug("BtInstallProgressDialog::slotOneItemCompleted");
	// mark the item completed (change/remove it),
	// if all items are stopped/completed close the dialog
	m_statusWidget->setItemWidget(getItem(module), 1, 0);
	getItem(module)->setText(1, tr("Completed"));
	m_statusWidget->itemWidget(getItem(module), 2)->setEnabled(false);
	int runningThreads = 0;
	foreach(QThread* thread, m_threads){
		if (thread->isRunning()) {
			qDebug() << "a thread is running";
			++runningThreads;
		}
	}
	if (runningThreads == 0) {
		qDebug() << "todo: close the dialog, update the module list";
		//close();
	}
}

void BtInstallProgressDialog::slotOneItemStopped(QString module)
{
	qDebug("BtInstallProgressDialog::slotOneItemStopped");
	// mark the item completed (change/remove it),
	// if all items are stopped/completed close the dialog

	//find the item for this module name
	//remove the progress dialog
	// write "stopped"
	//remove the stop button

	if (false) {
		close();
	}
}

void BtInstallProgressDialog::slotStopInstall()
{
	qDebug("BtInstallProgressDialog::slotStopInstall");
}

void BtInstallProgressDialog::slotStatusUpdated(QString module, int status)
{
	qDebug("BtInstallProgressDialog::slotStatusUpdated");
	qDebug() << "module:" << module << "status:" << status;
	// find the progress bar for this module and update the value
	QWidget* itemWidget = m_statusWidget->itemWidget(getItem(module) , 1);
	QProgressBar* bar = dynamic_cast<QProgressBar*>(itemWidget);
	if (bar) bar->setValue(status);
}

void BtInstallProgressDialog::slotDownloadStarted(QString module)
{
	qDebug("BtInstallProgressDialog::slotDownloadStarted");
	getItem(module)->setText(1, QString::null);
	m_statusWidget->itemWidget(getItem(module), 1)->setVisible(true);
}

QTreeWidgetItem* BtInstallProgressDialog::getItem(QString moduleName)
{
	return m_statusWidget->findItems(moduleName, Qt::MatchExactly).at(0);
}