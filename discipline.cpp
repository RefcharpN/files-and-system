#include "discipline.h"

Discipline::Discipline()
{

}

Discipline::Discipline(QString title, QString year, QString teacher)
{
    this->title = title;
    this->year = year;
    this->teacher = teacher;
}

void Discipline::setTitle(QString title)
{
    this->title = title;
}

void Discipline::setYear(QString year)
{
    this->year = year;
}

void Discipline::setTeacher(QString teacher)
{
    this->teacher = teacher;
}

QString Discipline::getTitle()
{
    return this->title;
}

QString Discipline::getYear()
{
    return this->year;
}

QString Discipline::getTeacher()
{
    return this->teacher;
}
