/* BEGIN_COMMON_COPYRIGHT_HEADER 
* END_COMMON_COPYRIGHT_HEADER */

#include "utils.h"

#include "toconnection.h"
#include "tosql.h"

#include <qapplication.h>
#include <qfile.h>
#include <qregexp.h>
#include <QString>

// A little magic to get lrefresh to work and get a check on qApp

#undef QT_TRANSLATE_NOOP
#define QT_TRANSLATE_NOOP(x,y) QTRANS(x,y)

toSQL::sqlMap *toSQL::Definitions;
const char * const toSQL::TOSQL_USERLIST = "Global:UserList";
const char * const toSQL::TOSQL_CREATEPLAN = "Global:CreatePlan";

toSQL::toSQL(const QString &name,
             const QString &sql,
             const QString &description,
             const QString &ver,
             const QString &provider)
        : Name(name)
{
    updateSQL(name, sql, description, ver, provider, false);
}

toSQL::toSQL(const QString &name)
        : Name(name)
{}

void toSQL::allocCheck(void)
{
    if (!Definitions)
        Definitions = new sqlMap;
}

bool toSQL::updateSQL(const QString &name,
                      const QString &sql,
                      const QString &description,
                      const QString &ver,
                      const QString &provider,
                      bool modified)
{
    version def(provider, ver, sql, modified);

    allocCheck();
    sqlMap::iterator i = Definitions->find(name);
    if (i == Definitions->end())
    {
        if (description.isEmpty())
        {
            fprintf(stderr, "ERROR:Tried add new version to unknown SQL (%s)\n", name.toAscii().constData());
            return false;
        }
        definition newDef;
        newDef.Modified = modified;
        newDef.Description = description;
        if (!sql.isNull())
        {
            std::list<version> &cl = newDef.Versions;
            cl.insert(cl.end(), def);
        }
        (*Definitions)[name] = newDef;
        return true;
    }
    else
    {
        if (!description.isEmpty())
        {
            if ((*i).second.Description != description)
            {
                (*i).second.Description = description;
                (*i).second.Modified = modified;
            }
            if (!modified)
                fprintf(stderr, "ERROR:Overwrite description of nonmodified (%s)\n", name.toAscii().constData());
        }
        std::list<version> &cl = (*i).second.Versions;
        for (std::list<version>::iterator j = cl.begin();j != cl.end();j++)
        {
            if ((*j).Provider == provider && (*j).Version == ver)
            {
                if (!sql.isNull())
                {
                    (*j) = def;
                    if (def.SQL != (*j).SQL)
                        (*j).Modified = modified;
                }
                return false;
            }
            else if ((*j).Provider > provider ||
                     ((*j).Provider == provider && (*j).Version > ver))
            {
                if (!sql.isNull())
                    cl.insert(j, def);
                return true;
            }
        }
        cl.insert(cl.end(), def);
        return true;
    }
}

bool toSQL::deleteSQL(const QString &name,
                      const QString &ver,
                      const QString &provider)
{
    allocCheck();
    sqlMap::iterator i = Definitions->find(name);
    if (i == Definitions->end())
    {
        return false;
    }
    else
    {
        std::list<version> &cl = (*i).second.Versions;
        for (std::list<version>::iterator j = cl.begin();j != cl.end();j++)
        {
            if ((*j).Version == ver && (*j).Provider == provider)
            {
                cl.erase(j);
                if ( cl.empty() )
                    Definitions->erase(i);
                return true;
            }
            else if ((*j).Provider > provider ||
                     ((*j).Provider == provider && !(*j).Version.isNull()))
            {
                return false;
            }
        }
        return false;
    }
}

toSQL toSQL::sql(const QString &name)
{
    allocCheck();
    sqlMap::iterator i = Definitions->find(name);
    if (i != Definitions->end())
        return name;
    throw qApp->translate("toSQL", "Tried to get unknown SQL (%1)").arg(QString(name));
}

QString toSQL::string(const QString &name,
                      const toConnection &conn)
{
    allocCheck();
    QString ver = conn.version();
    QString prov = conn.provider();

    bool quit = false;

    sqlMap::iterator i = Definitions->find(name);
    if (i != Definitions->end())
    {
        do
        {
            if (prov == "Any")
                quit = true;
            QString *sql = NULL;
            std::list<version> &cl = (*i).second.Versions;
            for (std::list<version>::iterator j = cl.begin();j != cl.end();j++)
            {
                if ((*j).Provider == prov)
                {
                    if ((*j).Version <= ver || !sql)
                    {
                        sql = &(*j).SQL;
                    }
                    if ((*j).Version >= ver)
                        return *sql;
                }
            }
            if (sql)
                return *sql;

            prov = "Any";
        }
        while (!quit);
    }

    throw qApp->translate("toSQL", "Tried to get unknown SQL (%1)").arg(QString(name));
}

bool toSQL::saveSQL(const QString &filename, bool all)
{
    allocCheck();
    QString data;

    QRegExp backslash(QString::fromLatin1("\\"));
    QRegExp newline(QString::fromLatin1("\n"));
    for (sqlMap::iterator i = Definitions->begin();i != Definitions->end();i++)
    {
        QString str;
        definition &def = (*i).second;
        QString name = (*i).first;
        if (def.Modified || all)
        {
            QString line = name;
            line += "=";
            QString t = def.Description;
            t.replace(backslash, QString::fromLatin1("\\\\"));
            t.replace(newline, QString::fromLatin1("\\n"));
            line += t;
            str = line;
            str += "\n";
        }
        for (std::list<version>::iterator j = def.Versions.begin();j != def.Versions.end();j++)
        {
            version &ver = (*j);
            if (ver.Modified || all)
            {
                QString line = name;
                line += "[";
                line += ver.Version;
                line += "][";
                line += ver.Provider;
                line += "]=";
                QString t = ver.SQL;
                t.replace(backslash, QString::fromLatin1("\\\\"));
                t.replace(newline, QString::fromLatin1("\\n"));
                line += t;
                str += line;
                str += "\n";
            }
        }
        data += str;
    }
    return toWriteFile(filename, data);
}

void toSQL::loadSQL(const QString &filename)
{
    allocCheck();
    QByteArray data = toReadFile(filename).toUtf8();

    int size = data.length();
    int pos = 0;
    int bol = 0;
    int endtag = -1;
    int provtag = -1;
    int vertag = -1;
    int wpos = 0;
    while (pos < size)
    {
        switch (data[pos])
        {
        case '\n':
            if (endtag == -1)
                throw QT_TRANSLATE_NOOP("toSQL", "Malformed tag in config file. Missing = on row.");
            data[wpos] = 0;
            {
                QString nam = ((const char *)data) + bol;
                QString val(QString::fromUtf8(((const char *)data) + endtag + 1));
                QString ver;
                QString prov;
                if (vertag >= 0)
                {
                    ver = ((const char *)data) + vertag + 1;
                    if (provtag >= 0)
                        prov = ((const char *)data) + provtag + 1;
                    updateSQL(nam, val, QString::null, ver, prov, true);
                }
                else
                    updateSQL(nam, QString::null, val, "", "", true);
            }
            bol = pos + 1;
            provtag = vertag = endtag = -1;
            wpos = pos;
            break;
        case '=':
            if (endtag == -1)
            {
                endtag = pos;
                data[wpos] = 0;
                wpos = pos;
            }
            else
                data[wpos] = data[pos];
            break;
        case '[':
            if (endtag == -1)
            {
                if (vertag >= 0)
                {
                    if (provtag >= 0)
                        throw QT_TRANSLATE_NOOP("toSQL", "Malformed line in SQL dictionary file. Two '[' before '='");
                    provtag = pos;
                }
                else
                    vertag = pos;
                data[wpos] = 0;
                wpos = pos;
            }
            else
                data[wpos] = data[pos];
            break;
        case ']':
            if (endtag == -1)
            {
                data[wpos] = 0;
                wpos = pos;
            }
            else
                data[wpos] = data[pos];
            break;
        case '\\':
            pos++;
            switch (data[pos])
            {
            case 'n':
                data[wpos] = '\n';
                break;
            case '\\':
                if (endtag >= 0)
                    data[wpos] = '\\';
                else
                    data[wpos] = ':';
                break;
            default:
                throw QT_TRANSLATE_NOOP("toSQL", "Unknown escape character in string (Only \\\\ and \\n recognised)");
            }
            break;
        default:
            data[wpos] = data[pos];
        }
        wpos++;
        pos++;
    }
}

std::list<QString> toSQL::range(const QString &startWith)
{
    std::list<QString> ret;
    for (sqlMap::iterator i = Definitions->begin();i != Definitions->end();i++)
    {
        if ((*i).first > startWith || startWith.isNull())
        {
            if ((*i).first.mid(0, startWith.length()) == startWith || startWith.isNull())
                ret.insert(ret.end(), (*i).first);
            else
                return ret;
        }
    }
    return ret;
}

QString toSQL::description(const QString &name)
{
    allocCheck();
    sqlMap::iterator i = Definitions->find(name);
    if (i != Definitions->end())
        return (*i).second.Description;
    throw qApp->translate("toSQL", "Tried to get unknown SQL (%1)").arg(QString(name));
}
