#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include "tarsnapcli.h"

#include <QObject>
#include <QThread>
#include <QUrl>
#include <QUuid>
#include <QMap>
#include <QSharedPointer>
#include <QDateTime>
#include <QThreadPool>

class JobManager;

enum JobStatus { Started, Running, Completed, Failed, Paused, Unknown };

class BackupJob
{
public:
    QUuid                 uuid;
    QList<QUrl>           urls;
    QString               name;
    JobStatus             status;
    int                   exitCode;
    QString               reason;
    QString               output;

    BackupJob():uuid(QUuid::createUuid()),status(JobStatus::Unknown){}
};

typedef QSharedPointer<BackupJob> BackupJobPtr;

class Archive: public QObject
{
    Q_OBJECT

public:
    QUuid       uuid;
    QString     name;
    QDateTime   timestamp;
    qint64      sizeTotal;
    qint64      sizeCompressed;
    qint64      sizeUniqueTotal;
    qint64      sizeUniqueCompressed;
    QString     command;
    QStringList contents;

    Archive():uuid(QUuid::createUuid()),sizeTotal(0),sizeCompressed(0),sizeUniqueTotal(0),sizeUniqueCompressed(0){}

    void notifyChanged() { emit changed(); }
signals:
    void changed();
};

typedef QSharedPointer<Archive> ArchivePtr;

class JobManager : public QObject
{
    Q_OBJECT

public:
    explicit JobManager(QObject *parent = 0);
    ~JobManager();

signals:
    void registerMachineStatus(JobStatus status, QString reason);
    void jobUpdate(BackupJobPtr job);
    void archivesList(QList<ArchivePtr> archives);

public slots:
    void registerMachine(QString user, QString password, QString machine, QString key);
    void backupNow(BackupJobPtr job);
    void getArchivesList();
    void getArchiveDetails(ArchivePtr archive);

private slots:
    void jobFinished(QUuid uuid, int exitCode, QString output);
    void jobStarted(QUuid uuid);
    void registerMachineFinished(QUuid uuid, int exitCode, QString output);
    void getArchivesFinished(QUuid uuid, int exitCode, QString output);
    void getArchiveStatsFinished(QUuid uuid, int exitCode, QString output);
    void getArchiveContentsFinished(QUuid uuid, int exitCode, QString output);

private:
    QThread                                   _managerThread; // manager runs on a separate thread
    QMap<QUuid, BackupJobPtr>    _jobMap;
    QMap<QUuid, ArchivePtr>      _archiveMap;
    QThreadPool                              *_threadPool;
};

#endif // JOBMANAGER_H