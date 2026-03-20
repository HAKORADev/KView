#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QStringList>

class FileHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath CONSTANT)
    Q_PROPERTY(QString fileType READ fileType NOTIFY fileTypeChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString content READ content NOTIFY contentChanged)
    Q_PROPERTY(QStringList archiveContents READ archiveContents NOTIFY archiveContentsChanged)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int totalFiles READ totalFiles NOTIFY totalFilesChanged)
    Q_PROPERTY(QString fileSize READ fileSize NOTIFY fileSizeChanged)

public:
    explicit FileHandler(QObject *parent = nullptr);
    void setFilePath(const QString &path);

    Q_INVOKABLE void openFileExternally();
    Q_INVOKABLE void nextFile();
    Q_INVOKABLE void previousFile();

    QString filePath() const { return m_filePath; }
    QString fileName() const { return m_fileName; }
    QString fileType() const { return m_fileType; }
    QUrl fileUrl() const { return QUrl::fromLocalFile(m_filePath); }
    QString content() const { return m_content; }
    QStringList archiveContents() const { return m_archiveContents; }
    int currentIndex() const { return m_currentIndex; }
    int totalFiles() const { return m_dirFiles.size(); }
    QString fileSize() const { return m_fileSize; }

signals:
    void fileTypeChanged();
    void fileUrlChanged();
    void contentChanged();
    void archiveContentsChanged();
    void fileNameChanged();
    void currentIndexChanged();
    void totalFilesChanged();
    void fileSizeChanged();
    void messageRequested(const QString &message);

private:
    QString m_filePath;
    QString m_fileName;
    QString m_fileType;
    QString m_fileSize;
    QString m_content;
    QStringList m_archiveContents;
    QStringList m_dirFiles;
    int m_currentIndex = -1;
    bool m_atEndWarned = false;
    bool m_atStartWarned = false;

    void detectFileType();
    void loadTextContent();
    void loadArchiveContent();
    void updateDirList();
};

#endif
