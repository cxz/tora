/* BEGIN_COMMON_COPYRIGHT_HEADER 
* END_COMMON_COPYRIGHT_HEADER */

#ifndef TORESULTPIE_H
#define TORESULTPIE_H

#include "config.h"
#include "tobackground.h"
#include "topiechart.h"
#include "toresult.h"

#include <list>

#include <qregexp.h>

class toNoBlockQuery;
class toSQL;

/** Display the result of a query in a piechart. The first column of the query should
 * contain the value and the second should contain an optional label.
 */

class toResultPie : public toPieChart, public toResult
{
    Q_OBJECT

    toNoBlockQuery *Query;
    std::list<QString> Labels;
    std::list<double> Values;
    toBackground Poll;
    int Columns;
    bool Started;
    bool LabelFirst;
    QRegExp Filter;
    QRegExp ValueFilter;
public:
    /** Create widget.
     * @param parent Parent of list.
     * @param name Name of widget.
     */
    toResultPie(QWidget *parent, const char *name = NULL);

    /** Reimplemented for internal reasons.
     */
    virtual void query(const QString &sql, const toQList &param);
    virtual bool canHandle(toConnection &)
    {
        return true;
    }

    /** Stop automatic updating from tool timer.
     */
    void stop();
    /** Start automatic updating from tool timer.
     */
    void start();

    /** Indicate that the first column should be the label.
     */
    void setLabelFirst(bool first)
    {
        LabelFirst = first;
    }
    /** Check label first status.
     */
    bool labelFirst(void)
    {
        return LabelFirst;
    }
    /** Set a filter on which columns to add based on label.
     * @param filter A regexp which the label must match.
     * @param valueFilter A regexp which the value must match.
     */
    void setFilter(const QRegExp &filter, const QRegExp &valueFilter)
    {
        Filter = filter;
        ValueFilter = valueFilter;
    }
    /** Get the current filter.
     */
    const QRegExp &filter(void)
    {
        return Filter;
    }
    /** Get the current value filter.
     */
    const QRegExp &valueFilter(void)
    {
        return ValueFilter;
    }

    // Why are these needed?
#if 1
    /** Set the SQL statement of this list
     * @param sql String containing statement.
     */
    void setSQL(const QString &sql)
    {
        toResult::setSQL(sql);
    }
    /** Set the SQL statement of this list. This will also affect @ref Name.
     * @param sql SQL containing statement.
     */
    void setSQL(const toSQL &sql)
    {
        toResult::setSQL(sql);
    }
    /** Set new SQL and run query.
     * @param sql New sql.
     * @see setSQL
     */
    void query(const QString &sql)
    {
        toResult::query(sql);
    }
    /** Set new SQL and run query.
     * @param sql New sql.
     * @see setSQL
     */
    void query(const toSQL &sql)
    {
        toResult::query(sql);
    }
    /** Set new SQL and run query.
     * @param sql New sql.
     * @see setSQL
     */
    void query(const toSQL &sql, toQList &par)
    {
        toResult::query(sql, par);
    }
#endif
public slots:
    /** Reimplemented for internal reasons.
     */
    virtual void refresh(void)
    {
        toResult::refresh();
    }
    /** Reimplemented for internal reasons.
     */
    virtual void changeParams(const QString &Param1)
    {
        toResult::changeParams(Param1);
    }
    /** Reimplemented For internal reasons.
     */
    virtual void changeParams(const QString &Param1, const QString &Param2)
    {
        toResult::changeParams(Param1, Param2);
    }
    /** Reimplemented for internal reasons.
     */
    virtual void changeParams(const QString &Param1, const QString &Param2, const QString &Param3)
    {
        toResult::changeParams(Param1, Param2, Param3);
    }
private slots:
    void poll(void);
};

#endif
