#ifndef STUDENT_H
#define STUDENT_H
#include <QString>


class Student
{
public:
    Student();
    Student(QString fio, QString year, QString track);
    void setYear(QString year);
    void setFio(QString fio);
    void setTrack(QString track);

    QString getYear();
    QString getFio();
    QString getTrack();

private:
    QString fio;
    QString year;
    QString track;
};

#endif // STUDENT_H
