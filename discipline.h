#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QString>


class Discipline
{
public:
    Discipline();
    Discipline(QString title, QString year, QString teacher);
    void setTitle(QString title);
    void setYear(QString year);
    void setTeacher(QString teacher);

    QString getTitle();
    QString getYear();
    QString getTeacher();

private:
    QString title;
    QString year;
    QString teacher;
};

#endif // DISCIPLINE_H
