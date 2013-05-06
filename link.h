#ifndef LINK_H_
#define LINK_H_

#include <QString>

class Link {
public:
    Link();
    Link(const QString &link_name, const QString &link_dir,
         const QString &dir_path, const QString &file_name);
    Link(const QString &link_name, const QString &link_dir,
         const QString &url);

    QString filePath() const;
    QString fileName() const;

    void fileName(const QString &file_name);
    QString dirPath() const;

    QString linkName() const;
    void linkName(const QString &link_name);

    QString url() const;
    void url(const QString &url);

    bool isUrl() const;
    void isUrl(const bool &value);

    void linkDir(const QString &link_dir);
    QString linkDir() const;

    void dirPath(const QString &dir_path);

    QString link() const;
    void launch();
    bool openFileLocation();
    QString linkExtension() const;
    void renameLink(const QString &new_link_name);

private:
    QString m_fileName;     // Filename  of the linked file
    QString m_dirPath;      // Directory of the linked file
    QString m_linkName;     // Filename  of the link
    QString m_linkDir;      // Directory of the link
    QString m_url;          // URL
    bool m_isUrl;
};

#endif // LINK_H_
