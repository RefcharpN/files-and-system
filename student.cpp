#include "student.h"

Student::Student()
{

}

Student::Student(QString fio, QString year, QString track)
{
    this->year = year;
    this->fio = fio;
    this->track = track;

}

void Student::setYear(QString year)
{
    this->year = year;
}

void Student::setFio(QString fio)
{
    this->fio = fio;

}

void Student::setTrack(QString track)
{
    this->track = track;

}

QString Student::getYear()
{
    return this->year;
}

QString Student::getFio()
{
    return this->fio;
}

QString Student::getTrack()
{
    return this->track;
}
